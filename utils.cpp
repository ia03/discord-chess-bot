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
    // Structure of a move
    // Bits 0-5: origin square
    // Bits 6-11: destination square
    // Bits 12-13 promotion piece flag
    // Bits 14-15 special move flag

    Move move = Move::none;

    move |= origin_sq;
    move |= (dest_sq << 6);
    move |= (promo_piece << 12);
    move |= (move_type << 14);

    return move;
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

Move_type move_type(Move move)
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

}
