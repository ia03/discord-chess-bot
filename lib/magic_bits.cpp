#include "common.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

inline unsigned Indx(unsigned row, unsigned col) { return row * 8 + col; }
inline unsigned Row(unsigned index) { return index / 8; }
inline unsigned Col(unsigned index) { return index % 8; }

class Direction {
 public:
  enum D {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NORTH_EAST,
    NORTH_WEST,
    SOUTH_EAST,
    SOUTH_WEST
  };

  Direction(D direction)
      : direction_(direction) {}

  // Index of the next square along this direction. Returns -1 if 
  // next index is outside the board.
  int NextIndex(int index) const {
    int row = Row(index);
    int col = Col(index);

    switch (direction_) {
      case NORTH:      ++row;        break;
      case SOUTH:      --row;        break;
      case EAST:       ++col;        break;
      case WEST:       --col;        break;
      case NORTH_EAST: ++row; ++col; break;
      case NORTH_WEST: ++row; --col; break;
      case SOUTH_EAST: --row; ++col; break;
      case SOUTH_WEST: --row; --col; break;
    }
    return (row > 7 || col > 7 || row < 0 || col < 0) ? -1 : Indx(row, col);
  }

  // Number of squares from given square to the edge of the board
  // along this direction.
  int EdgeDistance(int index) const {
    using std::min;
    int row = Row(index);
    int col = Col(index);

    auto inv = [](int x)->int { return 7 - x; };

    int d = -1;
    switch (direction_) {
      case NORTH:      d = inv(row);                break;
      case SOUTH:      d = row;                     break;
      case EAST:       d = inv(col);                break;
      case WEST:       d = col;                     break;
      case NORTH_EAST: d = min(inv(row), inv(col)); break;
      case NORTH_WEST: d = min(inv(row), col);      break;
      case SOUTH_EAST: d = min(row, inv(col));      break;
      case SOUTH_WEST: d = min(row, col);           break;
    }
    assert(d >= 0 && d <= 7);
    return d;
  }

 private:
  D direction_;
};

// Masks all the bits from the given index, and along the given direction to 1,
// excluding the square given by the index and the edge of the board along
// given direction.
U64 MaskBits(const Direction& direction, const int index) {
  U64 bitboard = 0ULL;
  int next_index = index;
  while ((next_index = direction.NextIndex(next_index)) >= 0 &&
         direction.NextIndex(next_index) >= 0) {
    bitboard |= (1ULL << next_index);
  }
  return bitboard;
}

// Generate all piece occupancies along a rank, file or diagonal, in the
// given direction, with index as the reference point. The square given
// by the index and the edge of the board in the given direction are not
// covered. For example, direction = NORTH_WEST, index = 29 (marked by X)
// will generate all combinations of occupancies for squares marked by #
// (there are 8 possible occupancies):
// 8 | 0 0 0 0 0 0 0 0
// 7 | 0 0 # 0 0 0 0 0
// 6 | 0 0 0 # 0 0 0 0
// 5 | 0 0 0 0 # 0 0 0
// 4 | 0 0 0 0 0 X 0 0
// 3 | 0 0 0 0 0 0 0 0
// 2 | 0 0 0 0 0 0 0 0
// 1 | 0 0 0 0 0 0 0 0
// -------------------
//   | A B C D E F G H
void GenerateOccupancies(const Direction& direction,
                         const int index,
                         std::vector<U64>* bbv) {
  // Number of squares in this direction excluding current square and
  // edge of the board.
  const int num_squares = direction.EdgeDistance(index) - 1;
  if (num_squares <= 0) {
    return;
  }

  // Number of possible piece occupancies in these squares along
  // the given direction.
  const unsigned num_occupancies = (1U << num_squares);

  // Create bitboard for each occupancy with the index next to given
  // index as starting point, along the given direction.
  for (unsigned occupancy = 0U; occupancy < num_occupancies; ++occupancy) {
    U64 bitboard = 0ULL;
    int next_index = index;
    for (unsigned bit_mask = 1U; bit_mask <= occupancy; bit_mask <<= 1) {
      next_index = direction.NextIndex(next_index);
      assert(next_index != -1);
      bitboard |= (U64(!!(occupancy & bit_mask)) << next_index);
    }
    bbv->push_back(bitboard);
  }
}

class OccupancyCombiner {
 public:
  OccupancyCombiner(int index)
      : index_(index) {}

  // Combines occupancy bitboards by bitwise ORing each stored
  // bitboard with bitboards generated by GenerateOccupancies
  // along given direction.
  void Combine(const Direction& direction);

  const std::vector<U64>& Occupancies() const {
    return occupancies_;
  }

 private:
  const int index_;
  std::vector<U64> occupancies_;
};

void OccupancyCombiner::Combine(const Direction& direction) {
  std::vector<U64> bbv;
  GenerateOccupancies(direction, index_, &bbv);
  if (bbv.empty()) {
    return;
  }
  if (occupancies_.empty()) {
    occupancies_.insert(occupancies_.end(), bbv.begin(), bbv.end());
    return;
  }
  std::vector<U64> tmp;
  for (const U64 bb : bbv) {
    for (const U64 occupancy : occupancies_) {
      tmp.push_back(bb | occupancy);
    }
  }
  occupancies_.swap(tmp);
}

// Generate an attack bitboard from a given square in the given direction
// for a specific occupancy of pieces.
U64 GenerateAttack(const Direction& direction,
                   const int index,
                   const U64 occupancy) {
  U64 attack_bb = 0ULL;
  for (int i = index; (i = direction.NextIndex(i)) != -1;) {
    attack_bb |= (1ULL << i);
    if (occupancy & (1ULL << i)) {
      break;
    }
  }
  return attack_bb;
}

// Returns a unsigned 64 bit random number.
U64 U64Rand() {
  return (U64(0xFFFF & rand()) << 48) |
      (U64(0xFFFF & rand()) << 32) |
      (U64(0xFFFF & rand()) << 16) |
      U64(0xFFFF & rand());
}

// Bias the random number to contain more 0 bits.
U64 ZeroBitBiasedRandom() {
  return U64Rand() & U64Rand() & U64Rand();
}

void GenerateMagic(const std::vector<Direction>& directions,
                   const int index,
                   const int shift_bits,
                   U64* magic,
                   std::vector<U64>* attack_table) {
  // Generate occupancies.
  OccupancyCombiner combiner(index);
  for (const Direction& direction : directions) {
    combiner.Combine(direction);
  }
  std::vector<U64> occupancies = combiner.Occupancies();

  // Generate attacks.
  std::vector<U64> attacks;
  for (const U64 occupancy : occupancies) {
    U64 attack = 0ULL;
    for (const Direction& direction : directions) {
      attack |= GenerateAttack(direction, index, occupancy);
    }
    attacks.push_back(attack);
  }

  // No bishop or rook attack can cover all squares of the board.
  static const U64 kInvalidAttack = ~0ULL;

  // Trial and error approach to generate magics.
  while (true) {
    std::vector<U64> table(1U << shift_bits, kInvalidAttack);
    U64 candidate_magic = ZeroBitBiasedRandom();
    bool collision = false;
    for (int k = 0; k < occupancies.size(); ++k) {
      const U64 occupancy = occupancies.at(k);
      const U64 attack = attacks.at(k);
      const int offset =
          (occupancy * candidate_magic) >> (64 - shift_bits);
      if (table.at(offset) == kInvalidAttack || table.at(offset) == attack) {
        table.at(offset) = attack;
      } else {
        collision = true;
        break;
      }
    }
    if (!collision) {
      *magic = candidate_magic;
      attack_table->swap(table);
      break;
    }
  }
}

void MagicBits(const std::vector<Direction>& directions,
               const int shifts[],
               U64 magics[],
               std::vector<U64>* attack_table,
               int offsets[]) {
  for (int i = 0; i < kSquares; ++i) {
    std::vector<U64> tmp_attack_table;
    GenerateMagic(directions, i, shifts[i], &magics[i], &tmp_attack_table);
    offsets[i] = attack_table->size();
    attack_table->insert(attack_table->end(),
                         tmp_attack_table.begin(),
                         tmp_attack_table.end());
  }
}

template <typename Container>
void WriteToFile(const std::string& filename, const Container& c) {
  std::fstream ofs(filename.c_str(), std::ios::out);
  for (auto iter = std::begin(c); iter != std::end(c); ++iter) {
    ofs << std::hex << *iter << '\n';
  }
  ofs.close();
}

int main(int argc, char** argv) {
  static const int rook_shifts[kSquares] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12,
  };
  WriteToFile(kRookShifts, rook_shifts);

  static const int bishop_shifts[kSquares] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6,
  };
  WriteToFile(kBishopShifts, bishop_shifts);

  const std::vector<Direction> rook_directions({
    Direction(Direction::NORTH),
    Direction(Direction::SOUTH),
    Direction(Direction::EAST),
    Direction(Direction::WEST)
  });
  const std::vector<Direction> bishop_directions({
    Direction(Direction::NORTH_EAST),
    Direction(Direction::NORTH_WEST),
    Direction(Direction::SOUTH_EAST),
    Direction(Direction::SOUTH_WEST)
  });

  U64 rook_masks[kSquares], bishop_masks[kSquares];

  for (int i = 0; i < kSquares; ++i) {
    rook_masks[i] = 0ULL;
    bishop_masks[i] = 0ULL;
    for (const Direction& d : rook_directions) {
      rook_masks[i] |= MaskBits(d, i);
    }
    for (const Direction& d : bishop_directions) {
      bishop_masks[i] |= MaskBits(d, i);
    }
  }
  WriteToFile(kRookMasks, rook_masks);
  WriteToFile(kBishopMasks, bishop_masks);

  U64 rook_magics[kSquares], bishop_magics[kSquares];
  std::vector<U64> rook_attack_table, bishop_attack_table;
  int rook_offsets[kSquares], bishop_offsets[kSquares];

  MagicBits(rook_directions,
            rook_shifts,
            rook_magics,
            &rook_attack_table,
            rook_offsets);
  WriteToFile(kRookMagics, rook_magics);
  WriteToFile(kRookOffsets, rook_offsets);
  WriteToFile(kRookAttackTable, rook_attack_table);

  MagicBits(bishop_directions,
            bishop_shifts,
            bishop_magics,
            &bishop_attack_table,
            bishop_offsets);
  WriteToFile(kBishopMagics, bishop_magics);
  WriteToFile(kBishopOffsets, bishop_offsets);
  WriteToFile(kBishopAttackTable, bishop_attack_table);

  return 0;
}