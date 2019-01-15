#include "types.h"
#include "utils.h"
#include "game.h"


void Game::init_zobrist()
{
    // Initialize player to move bitstrings
    white_bitstring = rand_hash();
    black_bitstring = rand_hash();

    // Initialize bitstrings for each square
    for (int i = 0; i < 64; i++)
    {
        w_pawn_bitstrings[i] = rand_hash();
        w_knight_bitstrings[i] = rand_hash();
        w_bishop_bitstrings[i] = rand_hash();
        w_rook_bitstrings[i] = rand_hash();
        w_queen_bitstrings[i] = rand_hash();
        w_king_bitstrings[i] = rand_hash();

        en_passant_bitstrings[i] = rand_hash();
    }

    // Initialize castling rights bitstrings
    for (int& castling_bitstring : castling_bitstrings)
    {
        castling_bitstring = rand_hash();
    }
}

void Game::init_hash()
{
    position_hash = 0;

    for (int i = 0; i < 64; i++)
    {
        position_hash ^= hash_square((Square)i);
    }
}

Bitstring Game::hash_square(Square square)
{
    Bitstring* board_bitstrings = nullptr;

    switch (piece_on(square))
    {
        case Piece::none:
            return 0;

        case Piece::w_pawn:
            board_bitstrings = w_pawn_bitstrings;
            break;
        case Piece::w_knight:
            board_bitstrings = w_knight_bitstrings;
            break;
        case Piece::w_bishop:
            board_bitstrings = w_bishop_bitstrings;
            break;
        case Piece::w_rook:
            board_bitstrings = w_rook_bitstrings;
            break;
        case Piece::w_queen:
            board_bitstrings = w_queen_bitstrings;
            break;
        case Piece::w_king:
            board_bitstrings = w_king_bitstrings;
            break;

        case Piece::b_pawn:
            board_bitstrings = b_pawn_bitstrings;
            break;
        case Piece::b_knight:
            board_bitstrings = b_knight_bitstrings;
            break;
        case Piece::b_bishop:
            board_bitstrings = b_bishop_bitstrings;
            break;
        case Piece::b_rook:
            board_bitstrings = b_rook_bitstrings;
            break;
        case Piece::b_queen:
            board_bitstrings = b_queen_bitstrings;
            break;
        case Piece::b_king:
            board_bitstrings = b_king_bitstrings;
            break;
    }

    return board_bitstrings[(int)square];
}

Bitstring Game::hash()
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

    hash_result ^= castling_bitstrings[(int)castling_rights];


    if (en_passant_square != Square::none)
    {
        hash_result ^= en_passant_bitstrings[(int)en_passant_square];
    }

    return hash_result;
}
