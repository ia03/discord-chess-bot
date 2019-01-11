/*
 * Ibrahim Ahmed
 * January 6, 2019
 * Utility functions' declarations
*/

#ifndef DISCORD_CHESS_BOT_UTILS_H
#define DISCORD_CHESS_BOT_UTILS_H

#include <vector>
#include "types.h"

// Create a move.
Move create_move(
        Square origin_sq,
        Square dest_sq,
        Promotion_piece promo_piece,
        Move_type move_type
);

// Extract the origin square.
Square extract_origin_sq(Move move);

// Extract the destination square.
Square extract_dest_sq(Move move);

// Extract the promotion piece type.
Promotion_piece extract_promo_piece(Move move);

// Extract the move type.
Move_type move_type(Move move);

// Generate a random bitstring.
Bitstring rand_hash();

// Generate a vector of moves using an attack bitboard.
std::vector<Move> gen_moves_from_bitboard(Bitboard bitboard);


#endif //DISCORD_CHESS_BOT_UTILS_H
