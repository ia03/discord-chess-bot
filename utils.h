#ifndef DISCORD_CHESS_BOT_UTILS_H
#define DISCORD_CHESS_BOT_UTILS_H

#include <vector>
#include <array>
#include "types.h"


const Bitboard white_squares = 0x55AA55AA55AA55AA;
const Bitboard black_squares = ~white_squares;

const Bitboard row_8 = 0xFF00000000000000;
const Bitboard row_7 = 0xFF000000000000;
const Bitboard row_6 = 0xFF0000000000;
const Bitboard row_5 = 0xFF00000000;
const Bitboard row_4 = 0xFF000000;
const Bitboard row_3 = 0xFF0000;
const Bitboard row_2 = 0xFF00;
const Bitboard row_1 = 0xFF;

const Bitboard col_a = 0x101010101010101;
const Bitboard col_b = 0x202020202020202;
const Bitboard col_c = 0x404040404040404;
const Bitboard col_d = 0x808080808080808;
const Bitboard col_e = 0x1010101010101010;
const Bitboard col_f = 0x2020202020202020;
const Bitboard col_g = 0x4040404040404040;
const Bitboard col_h = 0x8080808080808080;

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

// Creates a normal move (no castling, promotion, or en passant)
// from one square to another.
Move create_normal_move(Square origin_sq, Square dest_sq);

// Creates all the promotion moves from one square to another.
std::array<Move, 4> create_promo_moves(Square origin_sq, Square dest_sq);

// Creates an en passant move from one square to another.
Move create_en_passant_move(Square origin_sq, Square dest_sq);

// Creates a castling move from one square to another. The origin and
// destination squares are those of the king.
Move create_castling_move(Square origin_sq, Square dest_sq);

// Extracts the origin square.
Square extract_origin_sq(Move move);

// Extracts the destination square.
Square extract_dest_sq(Move move);

// Extracts the promotion piece type.
Promotion_piece extract_promo_piece(Move move);

// Extracts the move type.
Move_type extract_move_type(Move move);

// Gets the color of a piece type.
Color piece_color(Piece piece);

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

// Generates a vector of moves using a bitboard.
std::vector<Move> gen_moves_from_bitboard(
        Square origin_sq,
        Bitboard bitboard
);

// Counts the number of set bits in a bitboard.
int count_bits_set(Bitboard bitboard);

// Converts a square to a bitboard with a single bit turned on.
Bitboard square_to_bb(Square square);

// Returns the square north of the origin square.
Square north_of(Square origin_sq);

// Returns the square south of the origin square.
Square south_of(Square origin_sq);

// Determines if a square is on a bitboard.
bool on_bitboard(Square square, Bitboard bitboard);

// Determines if a bitboard has any set bits on another bitboard.
bool on_bitboard(Bitboard bitboard1, Bitboard bitboard2);

#endif  //DISCORD_CHESS_BOT_UTILS_H
