#include <vector>
#include <random>
#include "types.h"
#include "utils.h"


Color reverse_color(const Color color)
{
    if (color == Color::white)
    {
        return Color::black;
    }
    else if (color == Color::black)
    {
        return Color::white;
    }
    else
    {
        return Color::none;
    }
}

Move create_move(
        const Square origin_sq,
        const Square dest_sq,
        const Promotion_piece promo_piece,
        const Move_type move_type
)
{
    Move move = Move::none;

    // Set the values of the move.
    move = set_origin_sq(move, origin_sq);
    move = set_dest_sq(move, dest_sq);
    move = set_promo_piece(move, promo_piece);
    move = set_move_type(move, move_type);
    
    return move;
}

Move create_normal_move(const Square origin_sq, const Square dest_sq)
{
    return create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::none,
            Move_type::normal
    );
}

std::array<Move, 4> create_promo_moves(
        const Square origin_sq,
        const Square dest_sq
)
{
    std::array<Move, 4> moves;
    
    // Create the queen move.
    moves[0] = create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::queen,
            Move_type::normal
    );
    
    // Create the rook move.
    moves[1] = create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::rook,
            Move_type::normal
    );
    
    // Create the bishop move.
    moves[2] = create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::bishop,
            Move_type::normal
    );
    
    // Create the knight move.
    moves[3] = create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::knight,
            Move_type::normal
    );
    
    return moves;
}

Move create_en_passant_move(const Square origin_sq, const Square dest_sq)
{
    return create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::none,
            Move_type::en_passant
    );
}

Move create_castling_move(const Square origin_sq, const Square dest_sq)
{
    return create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::none,
            Move_type::castling
    );
}

Move set_origin_sq(const Move move, const Square origin_sq)
{
    // Origin square is bits 0-5.
    return static_cast<Move>(
            static_cast<int>(move) | static_cast<int>(origin_sq));
}

Move set_dest_sq(const Move move, const Square dest_sq)
{
    // Destination square is bits 6-11.
    return static_cast<Move>(
            static_cast<int>(move) | (static_cast<int>(dest_sq) << 6));
}

Move set_promo_piece(const Move move, const Promotion_piece promo_piece)
{
    // Promotion piece flag is bits 12-13.
    return static_cast<Move>(
            static_cast<int>(move) | static_cast<int>(promo_piece));
}

Move set_move_type(const Move move, const Move_type move_type)
{
    // Special move flag is bits 14-15.
    return static_cast<Move>(
            static_cast<int>(move) | static_cast<int>(move_type));
}

Square extract_origin_sq(const Move move)
{
    // Origin square is bits 0-5.
    return static_cast<Square>(
            static_cast<int>(move) & 0b0000000000111111);
}

Square extract_dest_sq(const Move move)
{
    // Destination square is bits 6-11.
    return static_cast<Square>(
            ((static_cast<int>(move) & 0b0000111111000000) >> 6));
}

Promotion_piece extract_promo_piece(const Move move)
{
    // Promotion piece flag is bits 12-13.
    return static_cast<Promotion_piece>(
            static_cast<int>(move) & 0b0011000000000000);
}

Move_type extract_move_type(const Move move)
{
    // Special move flag is bits 14-15.
    return (Move_type)(
            static_cast<int>(move) & 0b1100000000000000);
}

Color piece_color(const Piece piece)
{
    switch (piece)
    {
        case Piece::w_pawn:
        case Piece::w_knight:
        case Piece::w_bishop:
        case Piece::w_rook:
        case Piece::w_queen:
        case Piece::w_king:
            return Color::white;

        case Piece::b_pawn:
        case Piece::b_knight:
        case Piece::b_bishop:
        case Piece::b_rook:
        case Piece::b_queen:
        case Piece::b_king:
            return Color::black;

        default:
            return Color::none;
    }
}

void find_enemy_pawn_ep(
        Piece &enemy_pawn_type,
        Square &enemy_pawn_square,
        const Square dest_sq,
        const Color turn
)
{
    // If this is White's move, the captured pawn will be 1 square
    // south of the destination square. If this is Black's move, the
    // captured pawn will be 1 square north of the destination square.
    if (turn == Color::white)
    {
        enemy_pawn_type = Piece::b_pawn;
        enemy_pawn_square = south_of(dest_sq);
    }
    else
    {
        enemy_pawn_type = Piece::w_pawn;
        enemy_pawn_square = north_of(dest_sq);
    }
}

void castle_rook_squares(
        Piece &rook_type,
        Square &rook_origin_sq,
        Square &rook_dest_sq,
        const Square king_origin_sq,
        const Square king_dest_sq
)
{
    // White
    if (king_origin_sq == Square::E1)
    {
        rook_type = Piece::w_rook;
                
        // Kingside
        if (king_dest_sq == Square::G1)
        {
            rook_origin_sq = Square::H1;
            rook_dest_sq = Square::F1;
        }
        // Queenside
        else
        {
            rook_origin_sq = Square::A1;
            rook_dest_sq = Square::D1;
        }
    }
    // Black
    else
    {
        rook_type = Piece::b_rook;
        
        // Kingside
        if (king_dest_sq == Square::G8)
        {
            rook_origin_sq = Square::H8;
            rook_dest_sq = Square::F8;
        }
        // Queenside
        else
        {
            rook_origin_sq = Square::A8;
            rook_dest_sq = Square::D8;
        }
    }
}

Piece promo_piece_to_piece(
        const Promotion_piece promo_piece,
        const Color color
)
{
    if (color == Color::white)
    {
        switch (promo_piece)
        {
            case Promotion_piece::queen:
                return Piece::w_queen;
            case Promotion_piece::rook:
                return Piece::w_rook;
            case Promotion_piece::bishop:
                return Piece::w_bishop;
            case Promotion_piece::knight:
                return Piece::w_knight;
        }
    }
    else
    {
        switch (promo_piece)
        {
            case Promotion_piece::queen:
                return Piece::b_queen;
            case Promotion_piece::rook:
                return Piece::b_rook;
            case Promotion_piece::bishop:
                return Piece::b_bishop;
            case Promotion_piece::knight:
                return Piece::b_knight;
        }
    }
    return Piece::none;
}

Bitstring rand_hash()
{
    // Use a Mersenne Twister pseudo-random generator to generate a random
    // 64-bit number.
    std::mt19937_64 generator (std::random_device{}());

    return generator();
}

std::vector<Move> gen_moves_from_bitboard(
        const Square origin_sq,
        Bitboard bitboard
)
{
    std::vector<Move> moves;
    
    // Set the origin square.
    const Move template_move = set_origin_sq(template_move, origin_sq);

    // Get the positions of the set bits in the bitboard and use them to
    // create moves.
    for(auto position = 0; bitboard != 0; position++)
    {
        if ((bitboard & 1) == 1)
        {
            moves.push_back(set_dest_sq(
                    template_move,
                    static_cast<Square>(position)));
        }
        bitboard >>= 1;
    }
    
    return moves;
}

int count_bits_set(Bitboard bitboard)
{
    int i;
    
    // This uses Brian Kernighan's algorithm.
    // Clears the least significant set bit until all bits have been cleared.
    for (i = 0; bitboard != 0; i++)
    {
        bitboard &= bitboard - 1;  // Clear the least significant set bit.
    }
    return i;
}

int set_bit_pos(Bitboard bitboard)
{
    int position = 0;
    
    // Keep right shifting the bitboard by 1 bit until the first bit is set.
    while ((bitboard & 1) == 0)
    {
        bitboard >>= 1;
        position++;
    }
    
    return position;
}

Bitboard square_to_bb(const Square square)
{
    return 1 << static_cast<int>(square);
}

Square north_of(const Square origin_sq)
{
    return static_cast<Square>(static_cast<int>(origin_sq) + 8);
}

Square south_of(const Square origin_sq)
{
    return static_cast<Square>(static_cast<int>(origin_sq) - 8);
}

Square east_of(const Square origin_sq)
{
    return static_cast<Square>(static_cast<int>(origin_sq) + 1);
}

Square west_of(const Square origin_sq)
{
    return static_cast<Square>(static_cast<int>(origin_sq) - 8);
}

bool on_bitboard(const Square square, const Bitboard bitboard)
{
    return on_bitboard(square_to_bb(square), bitboard);
}

bool on_bitboard(const Bitboard bitboard1, const Bitboard bitboard2)
{
    return bitboard1 & bitboard2 != 0;
}

Square find_dest_square(
        const Square origin_sq,
        const std::vector<Direction> &directions
)
{
    auto dest_sq = origin_sq;
    const Bitboard origin_bb = square_to_bb(origin_sq);
    int north_count = 0;
    int east_count = 0;
    int south_count = 0;
    int west_count = 0;
    
    // Keep track of each directional move of the square, making sure it does
    // not exit the boundaries of the board. If it does not, then continue
    // moving it until all the requested moves have been made.
    for (const auto &direction : directions)
    {
        switch (direction)
        {
            case Direction::north:
                if (on_bitboard(origin_bb, rows[7 - north_count]))
                {
                    return Square::none;
                }
                dest_sq = north_of(dest_sq);
                north_count++;
                break;
            case Direction::east:
                if (on_bitboard(origin_bb, cols[7 - east_count]))
                {
                    return Square::none;
                }
                dest_sq = east_of(dest_sq);
                east_count++;
                break;
            case Direction::south:
                if (on_bitboard(origin_bb, rows[south_count]))
                {
                    return Square::none;
                }
                dest_sq = south_of(dest_sq);
                south_count++;
                break;
            case Direction::west:
                if (on_bitboard(origin_bb, cols[west_count]))
                {
                    return Square::none;
                }
                dest_sq = west_of(dest_sq);
                west_count++;
                break;
        }
    }
    
    return dest_sq;
}