/*
 * Ibrahim Ahmed
 * January 6, 2018
 * Utility function declarations
*/

#ifndef DISCORD_CHESS_BOT_UTILS_H
#define DISCORD_CHESS_BOT_UTILS_H

#include <vector>
#include "types.h"

Move create_move(
        Square from,
        Square to,
        Promotion_piece promo_piece,
        Move_type move_type
);

Square from_sq(Move move);

Square to_sq(Move move);

Promotion_piece promo_piece(Move move);

Move_type move_type(Move move);

Bitstring rand_hash();

std::vector<Move> gen_moves_from_bitboard(Bitboard bitboard);


#endif //DISCORD_CHESS_BOT_UTILS_H
