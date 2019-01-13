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
    } else
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

    move = set_origin_sq(move, origin_sq);
    move = set_dest_sq(move, dest_sq);
    move = set_promo_piece(move, promo_piece);
    move = set_move_type(move, move_type);
    
    return move;
}

Move set_origin_sq(Move move, Square origin_sq)
{
    // Origin square is bits 0-5.
    return (Move)((int)move | (int)origin_sq);
}

Move set_dest_sq(Move move, Square dest_sq)
{
    // Destination square is bits 6-11.
    return (Move)((int)move | ((int)dest_sq << 6));
}

Move set_promo_piece(Move move, Promotion_piece promo_piece)
{
    // Promotion piece flag is bits 12-13.
    return (Move)((int)move | (int)promo_piece);
}

Move set_move_type(Move move, Move_type move_type)
{
    // Special move flag is bits 14-15.
    return (Move)((int)move | (int)move_type);
}


Square extract_origin_sq(Move move)
{
    // Origin square is bits 0-5.
    return (Square)((int)move & 0b0000000000111111);
}

Square extract_dest_sq(Move move)
{
    // Destination square is bits 6-11.
    return (Square)((int)move & 0b0000111111000000);
}

Promotion_piece extract_promo_piece(Move move)
{
    // Promotion piece flag is bits 12-13.
    return (Promotion_piece)((int)move & 0b0011000000000000);
}

Move_type extract_move_type(Move move)
{
    // Special move flag is bits 14-15.
    return (Move_type)((int)move & 0b1100000000000000);
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
            moves.push_back(set_dest_sq(template_move, (Square)position));
        }
        bitboard >>= 1;
    }
    
    return moves;
}
