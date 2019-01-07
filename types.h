/*
 * Ibrahim Ahmed
 * January 6, 2018
 * Type definitions
*/

#ifndef DISCORD_CHESS_BOT_TYPES_H
#define DISCORD_CHESS_BOT_TYPES_H

#include <map>
#include <vector>
#include <string>

// 64-bit bitboards will be used to represent the 64 squares of a chessboard
// as per little-endian rank-file mapping.
typedef uint64_t Bitboard;

// A position on a bitboard can be represented using only 6 bits, storing
// numbers from 0 to 63.
typedef uint8_t Square;

// A move needs exactly 2 bytes (16 bits).
// Bits 0-5: position of the origin square
// Bits 6-11: position of the target square
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
typedef uint16_t Move;

// Used for Zobrist hashing. Hash collisions should occur rarely when using
// numbers that are 64 bits long.
typedef uint64_t Bitstring;

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

enum class Promotion_piece
{
    knight,
    bishop,
    rook,
    queen
};

enum class Move_type
{
    normal,
    castling,
    promotion,
    en_passant
};

enum class Castle
{
    w_kingside,
    w_queenside,
    b_kingside,
    b_queenside
};

enum class Color
{
    none,
    white,
    black
};

enum class Game_state
{
    in_progress,
    white_checkmate,
    black_checkmate,
    stalemate,
    threefold_repetition,
    fifty_move,
    insufficient_material
};

// Stores information for a ply. Used to reverse moves and detect repetition.
struct Ply_data
{
    // The move that ended this ply
    Move last_move;

    Piece captured_piece;

    // Before the move occurred
    uint8_t castling_rights;
    Square en_passant_sq;
    uint8_t rule50;

    // Zobrist key - used to detect threefold repetition
    Bitstring key;
};

#endif //DISCORD_CHESS_BOT_TYPES_H