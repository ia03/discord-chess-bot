#ifndef DISCORD_CHESS_BOT_UTILS_H
#define DISCORD_CHESS_BOT_UTILS_H

#include <vector>
#include "types.h"


// Returns the opposite color. Returns none if the input is not white or
// black.
Color reverse_color(Color color);

// Sets the origin square.
Move set_origin_sq(Move move, Square origin_sq);

// Sets the destination square.
Move set_dest_sq(Move move, Square dest_sq);

// Sets the promotion piece type.
Move set_promo_piece(Move move, Promotion_piece promo_piece);

// Sets the move type.
Move set_move_type(Move move, Move_type move_type);

// Creates a move.
Move create_move(
        Square origin_sq,
        Square dest_sq,
        Promotion_piece promo_piece,
        Move_type move_type
);

// Extracts the origin square.
Square extract_origin_sq(Move move);

// Extracts the destination square.
Square extract_dest_sq(Move move);

// Extracts the promotion piece type.
Promotion_piece extract_promo_piece(Move move);

// Extracts the move type.
Move_type extract_move_type(Move move);

// Gets the type and position of the enemy pawn captured in an en passant
// move.
void find_enemy_pawn_ep(
        Piece &enemy_pawn_type,
        Square &enemy_pawn_square,
        Square dest_sq,
        Color turn
);

// Sets the rook type and origin and destination squares based on the king
// origin and destination squares.
void castle_rook_squares(
        Piece &rook_type,
        Square &rook_origin_sq,
        Square &rook_dest_sq,
        Square king_origin_sq,
        Square king_dest_sq
);

// Converts a promotion piece to a piece.
Piece promo_piece_to_piece(Promotion_piece promo_piece, Color color);

// Generates a random bitstring.
Bitstring rand_hash();

// Generates a vector of moves using an attack bitboard.
std::vector<Move> gen_moves_from_bitboard(
        Square origin_sq,
        Bitboard bitboard
);


#endif  //DISCORD_CHESS_BOT_UTILS_H
