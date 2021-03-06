#ifndef DISCORD_CHESS_BOT_TYPES_H
#define DISCORD_CHESS_BOT_TYPES_H


// 64-bit bitboards will be used to represent the 64 squares of a chessboard
// as per little-endian rank-file mapping.
typedef unsigned long long Bitboard;

// A position on a bitboard can be represented using only 6 bits, storing
// numbers from 0 to 63.
enum class Square : int
{
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,

    none = -1
};

// A move needs exactly 2 bytes (16 bits), the minimum size an int can be.
// Bits 0-5: position of the origin square
// Bits 6-11: position of the destination square
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
//
// none is a special case because the origin square is always different from
// the destination square, so it is guaranteed to never be a possible move.
enum class Move : unsigned int
{
    none
};

// Used for Zobrist hashing. Hash collisions should occur rarely when using
// numbers that are 64 bits long.
typedef unsigned long long Bitstring;

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
};

// Bits 12-13 of a move, so shifted left by 12 bits.
enum class Promotion_piece : unsigned int
{
    none,  // None is the same as the value for queen.
    queen  = 0,
    rook   = 1 << 12,
    bishop = 2 << 12,
    knight = 3 << 12
};

// Bits 14-15 of a move, so shifted left by 14 bits.
enum class Move_type : unsigned int
{
    normal,
    castling   = 1 << 14,
    promotion  = 2 << 14,
    en_passant = 3 << 14
};

// Each castling right is one bit.
enum class Castling_right : unsigned int
{
    no_castling,
    w_kingside,
    w_queenside  = w_kingside << 1,
    white        = w_kingside | w_queenside,
    b_kingside   = w_kingside << 2,
    b_queenside  = w_kingside << 3,
    black        = b_kingside | b_queenside,
    all_castling = white | black
};

enum class Color
{
    none,
    white,
    black
};

enum class Direction
{
    north,
    east,
    south,
    west
};

enum class Game_state
{
    in_progress,
    checkmate_by_white,
    checkmate_by_black,
    stalemate,
    threefold_repetition,
    fifty_move,
    insufficient_material
};

// Stores information for a ply. Used to reverse moves.
struct Ply_data
{
    // The move that ended this ply
    Move last_move;
    Piece captured_piece;

    // After the move occurred
    bool threefold_repetition;

    // Before the move occurred
    Castling_right castling_rights;
    Square en_passant_square;
    int rule50;
};

#endif  //DISCORD_CHESS_BOT_TYPES_H
