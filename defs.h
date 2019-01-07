/*
 * Ibrahim Ahmed
 * January 6, 2018
 * Definitions for the Python library.
*/

#include <map>
#include <string>

using namespace std;

// 64-bit bitboards will be used to represent the 64 squares of a chessboard
// as per little-endian rank-file mapping.
typedef uint64_t Bitboard;

// A position on a bitboard can be represented using only 6 bits, storing
// numbers from 0 to 63.
typedef uint8_t Position;

// A move needs exactly 2 bytes (16 bits).
// Bits 0-5: position of the origin square
// Bits 6-11: position of the target square
// Bits 12-13 promotion piece flag
// Bits 14-15 special move flag
//
// The promotion piece flag can be (in binary):
// 00 for the queen
// 01 for the rook
// 10 for the bishop
// 11 for the knight
//
// The special move flag can be (in binary):
// 00 for a normal move
// 01 for castling
// 10 for promotion
// 11 for en passant
typedef uint16_t Move;

// Used for Zobrist hashing. Hash collisions should occur rarely when using
// numbers that are 64 bits long.
typedef uint64_t Bitstring;

enum class Piece
{
    none,
    w_pawn,
    w_knight,
    w_bishop,
    w_rook,
    w_queen,
    w_king,
    b_pawn,
    b_knight,
    b_bishop,
    b_rook,
    b_queen,
    b_king
}

enum class Promotion_piece
{
    knight,
    bishop,
    rook,
    queen
}

enum class Move_type
{
    normal,
    castling,
    promotion,
    en_passant
}

enum class Castle_type
{
    w_kingside,
    w_queenside,
    b_kingside,
    b_queenside
}
enum class Color_type
{
    none,
    white,
    black
}

enum class Game_state
{
    in_progress,
    w_checkmate,
    b_checkmate,
    stalemate,
    threefold_repetition,
    fifty_move,
    insufficient_material
}

class Game
{
private:

public:
}

Bitstring rand_hash();

// String coordinates to/from positions

unordered_map<string, Position> coord_to_pos =
{
    {"a1", 0},
    {"b1", 1},
    {"c1", 2},
    {"d1", 3},
    {"e1", 4},
    {"f1", 5},
    {"g1", 6},
    {"h1", 7},
    {"a2", 8},
    {"b2", 9},
    {"c2", 10},
    {"d2", 11},
    {"e2", 12},
    {"f2", 13},
    {"g2", 14},
    {"h2", 15},
    {"a3", 16},
    {"b3", 17},
    {"c3", 18},
    {"d3", 19},
    {"e3", 20},
    {"f3", 21},
    {"g3", 22},
    {"h3", 23},
    {"a4", 24},
    {"b4", 25},
    {"c4", 26},
    {"d4", 27},
    {"e4", 28},
    {"f4", 29},
    {"g4", 30},
    {"h4", 31},
    {"a5", 32},
    {"b5", 33},
    {"c5", 34},
    {"d5", 35},
    {"e5", 36},
    {"f5", 37},
    {"g5", 38},
    {"h5", 39},
    {"a6", 40},
    {"b6", 41},
    {"c6", 42},
    {"d6", 43},
    {"e6", 44},
    {"f6", 45},
    {"g6", 46},
    {"h6", 47},
    {"a7", 48},
    {"b7", 49},
    {"c7", 50},
    {"d7", 51},
    {"e7", 52},
    {"f7", 53},
    {"g7", 54},
    {"h7", 55},
    {"a8", 56},
    {"b8", 57},
    {"c8", 58},
    {"d8", 59},
    {"e8", 60},
    {"f8", 61},
    {"g8", 62},
    {"h8", 63}
};

string pos_to_coord [64] =
{
    "a1",
    "b1",
    "c1",
    "d1",
    "e1",
    "f1",
    "g1",
    "h1",
    "a2",
    "b2",
    "c2",
    "d2",
    "e2",
    "f2",
    "g2",
    "h2",
    "a3",
    "b3",
    "c3",
    "d3",
    "e3",
    "f3",
    "g3",
    "h3",
    "a4",
    "b4",
    "c4",
    "d4",
    "e4",
    "f4",
    "g4",
    "h4",
    "a5",
    "b5",
    "c5",
    "d5",
    "e5",
    "f5",
    "g5",
    "h5",
    "a6",
    "b6",
    "c6",
    "d6",
    "e6",
    "f6",
    "g6",
    "h6",
    "a7",
    "b7",
    "c7",
    "d7",
    "e7",
    "f7",
    "g7",
    "h7",
    "a8",
    "b8",
    "c8",
    "d8",
    "e8",
    "f8",
    "g8",
    "h8"
};

// Promotion placeholders to/from binary flags

unordered_map<string, Move> promo_str_to_bin =
{
    {"q", 0b00},
    {"r", 0b01},
    {"b", 0b10},
    {"n", 0b11},
}

unordered_map<Move, string> promo_bin_to_str =
{
    {0b00, "q"},
    {0b01, "r"},
    {0b10, "b"},
    {0b11, "n"}
}

// Piece-square tables

int pawn_pst [64] =
{
/*
     0,  0,  0,  0,  0,  0,  0,  0,
    60, 60, 60, 60, 60, 60, 60, 60,
    20, 20, 30, 40, 40, 30, 20, 20,
    15, 15, 20, 37, 37, 20, 15, 15,
    10, 10, 10, 36, 36, 10, 10, 10,
    15,  5,  0, 10, 10,  0,  5, 15,
    15, 20, 20,-15,-15, 20, 20, 15,
     0,  0,  0,  0,  0,  0,  0,  0
*/
}

int knight_pst [64] =
{
    
}

int bishop_pst [64] =
{

}

int rook_pst [64] =
{

}

int queen_pst [64] =
{

}

int king_pst [64] = 
{

}
