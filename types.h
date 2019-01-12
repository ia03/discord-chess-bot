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
    A8, B8, C8, D8, E8, F8, G8, H8
};

// A move needs exactly 2 bytes (16 bits).
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
enum class Move : int
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

enum class Promotion_piece : int
{
    queen,
    rook,
    bishop,
    knight,
    
    // This value is never saved to a move but used as a placeholder if it
    // is unset when creating a move.
    none = -1
};

enum class Move_type : int
{
    normal,
    castling,
    promotion,
    en_passant
};

enum class Castle : int
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
    int castling_rights;
    Square en_passant_sq;
    int rule50;

    // Zobrist key - used to detect threefold repetition
    Bitstring key;
};

#endif //DISCORD_CHESS_BOT_TYPES_H
