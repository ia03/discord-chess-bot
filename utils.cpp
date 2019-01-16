#include <vector>
#include <random>
#include "types.h"
#include "utils.h"


Color reverse_color(Color color)
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
        Square origin_sq,
        Square dest_sq,
        Promotion_piece promo_piece,
        Move_type move_type
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

Move set_origin_sq(Move move, Square origin_sq)
{
    // Origin square is bits 0-5.
    return static_cast<Move>(
			static_cast<int>(move) | static_cast<int>(origin_sq));
}

Move set_dest_sq(Move move, Square dest_sq)
{
    // Destination square is bits 6-11.
    return static_cast<Move>(
			static_cast<int>(move) | (static_cast<int>(dest_sq) << 6));
}

Move set_promo_piece(Move move, Promotion_piece promo_piece)
{
    // Promotion piece flag is bits 12-13.
    return static_cast<Move>(
			static_cast<int>(move) | static_cast<int>(promo_piece));
}

Move set_move_type(Move move, Move_type move_type)
{
    // Special move flag is bits 14-15.
    return static_cast<Move>(
			static_cast<int>(move) | static_cast<int>(move_type));
}

Square extract_origin_sq(Move move)
{
    // Origin square is bits 0-5.
    return static_cast<Square>(
			static_cast<int>(move) & 0b0000000000111111);
}

Square extract_dest_sq(Move move)
{
    // Destination square is bits 6-11.
    return static_cast<Square>(
			static_cast<int>(move) & 0b0000111111000000);
}

Promotion_piece extract_promo_piece(Move move)
{
    // Promotion piece flag is bits 12-13.
    return static_cast<Promotion_piece>(
			static_cast<int>(move) & 0b0011000000000000);
}

Move_type extract_move_type(Move move)
{
    // Special move flag is bits 14-15.
    return (Move_type)(
			static_cast<int>(move) & 0b1100000000000000);
}

Color piece_color(Piece piece)
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
        Square dest_sq,
        Color turn
)
{
    // If this is White's move, the captured pawn will be 1 square
    // south of the destination square. If this is Black's move, the
    // captured pawn will be 1 square north of the destination square.
    if (turn == Color::white)
    {
        enemy_pawn_type = Piece::b_pawn;
        enemy_pawn_square = static_cast<Square>(
                static_cast<int>(dest_sq) - 8);
    }
    else
    {
        enemy_pawn_type = Piece::w_pawn;
        enemy_pawn_square = static_cast<Square>(
                static_cast<int>(dest_sq) + 8);
    }
}

void castle_rook_squares(
        Piece &rook_type,
        Square &rook_origin_sq,
        Square &rook_dest_sq,
        Square king_origin_sq,
        Square king_dest_sq
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

Piece promo_piece_to_piece(Promotion_piece promo_piece, Color color)
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

std::vector<Move> gen_moves_from_bitboard(Square origin_sq, Bitboard bitboard)
{
    std::vector<Move> moves;
    Move template_move = Move::none;
    
    // Set the origin square.
    template_move = set_origin_sq(template_move, origin_sq);

    // Get the positions of the set bits in the bitboard and use them to
    // create moves.
    for(int position = 0; bitboard != 0; position++)
    {
        if ((bitboard & 2) == 1)
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

Bitboard square_to_bb(Square square)
{
    return 1 << static_cast<int>(square);
}
