#include "types.h"
#include "utils.h"
#include "game.h"


void Game::init_zobrist()
{
    // Initialize player to move bitstrings
    white_bitstring = rand_hash();
    black_bitstring = rand_hash();

    // Initialize bitstrings for each square
    for (auto square = 0; square < 64; square++)
    {
        w_pawn_bitstrings[square] = rand_hash();
        w_knight_bitstrings[square] = rand_hash();
        w_bishop_bitstrings[square] = rand_hash();
        w_rook_bitstrings[square] = rand_hash();
        w_queen_bitstrings[square] = rand_hash();
        w_king_bitstrings[square] = rand_hash();

        en_passant_bitstrings[square] = rand_hash();
    }

    // Initialize castling rights bitstrings
    for (auto& castling_bitstring : castling_bitstrings)
    {
        castling_bitstring = rand_hash();
    }
}

void Game::init_hash()
{
    position_hash = 0;

    // XOR the hash of each square.
    for (auto square = 0; square < 64; square++)
    {
        position_hash ^= hash_square(static_cast<Square>(square));
    }
}

Bitstring Game::hash_square(Square square) const
{

    switch (piece_on(square))
    {
        case Piece::none:
            return 0;

        case Piece::w_pawn:
            return w_pawn_bitstrings[static_cast<int>(square)];
        case Piece::w_knight:
            return w_knight_bitstrings[static_cast<int>(square)];
        case Piece::w_bishop:
            return w_bishop_bitstrings[static_cast<int>(square)];
        case Piece::w_rook:
            return w_rook_bitstrings[static_cast<int>(square)];
        case Piece::w_queen:
            return w_queen_bitstrings[static_cast<int>(square)];
        case Piece::w_king:
            return w_king_bitstrings[static_cast<int>(square)];

        case Piece::b_pawn:
            return b_pawn_bitstrings[static_cast<int>(square)];
        case Piece::b_knight:
            return b_knight_bitstrings[static_cast<int>(square)];
        case Piece::b_bishop:
            return b_bishop_bitstrings[static_cast<int>(square)];
        case Piece::b_rook:
            return b_rook_bitstrings[static_cast<int>(square)];
        case Piece::b_queen:
            return b_queen_bitstrings[static_cast<int>(square)];
        case Piece::b_king:
            return b_king_bitstrings[static_cast<int>(square)];
    }
}

Bitstring Game::hash() const
{
    // The position hash is already calculated.
    // XOR it with the turn, castling rights, and en passant square bitstrings
    // to get the final hash.
    Bitstring hash_result = position_hash;

    if (turn == Color::white)
    {
        hash_result ^= white_bitstring;
    }
    else
    {
        hash_result ^= black_bitstring;
    }

    hash_result ^= castling_bitstrings[static_cast<int>(castling_rights)];


    if (en_passant_square != Square::none)
    {
        hash_result ^= en_passant_bitstrings[
				static_cast<int>(en_passant_square)];
    }

    return hash_result;
}
