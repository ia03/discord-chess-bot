/*
 * Ibrahim Ahmed
 * January 10, 2019
 * Utility functions' definitions
*/

#include <vector>
#include <random>
#include "types.h"


Move create_move(
        Square origin_sq,
        Square dest_sq,
        Promotion_piece promo_piece,
        Move_type move_type
)
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
    return move | origin_sq;
}

Move set_dest_sq(Move move, Square dest_sq)
{
    // Destination square is bits 6-11.
    return move | (dest_sq << 6);
}

Move set_promo_piece(Move move, Promotion_piece promo_piece)
{
    // Promotion piece flag is bits 12-13.
    return move | (promo_piece << 12);
}

Move set_move_type(Move move, Move_type move_type)
{
    // Special move flag is bits 14-15.
    return move | (move_type << 14);
}


Square extract_origin_sq(Move move)
{
    // Origin square is bits 0-5.
    return move & 0x3F;
}

Square extract_dest_sq(Move move)
{
    // Destination square is bits 6-11.
    return move & 0xFC0;
}

Promotion_piece extract_promo_piece(Move move)
{
    // Promotion piece flag is bits 12-13.
    return move & 0x3000;
}

Move_type extract_move_type(Move move)
{
    // Special move flag is bits 14-15.
    return move & 0xC000;
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
    template_move |= origin_sq;
    
    for(Square position = 0; bitboard != 0; position++)
    {
        if (bitboard % 2 == 1)
        {
            moves.push_back(set_dest_sq(template_move, position));
        }
        bitboard >>= 1;
    }
    
    return moves;
}
