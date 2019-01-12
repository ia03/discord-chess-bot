#include "types.h"
#include "utils.h"
#include "game.h"


void Game::init_zobrist()
{
    white_bitstring = rand_hash();
    black_bitstring = rand_hash();

    for (Square i = 0; i < 64; i++)
    {
        w_pawn_bitstrings[i] = rand_hash();
        w_knight_bitstrings[i] = rand_hash();
        w_bishop_bitstrings[i] = rand_hash();
        w_rook_bitstrings[i] = rand_hash();
        w_queen_bitstrings[i] = rand_hash();
        w_king_bitstrings[i] = rand_hash();

        en_passant_bitstrings[i] = rand_hash();
    }

    for (int i = 0; i < 16; i++)
        castling_bitstrings[i] = rand_hash();
}

void Game::init_hash()
{
    position_hash = 0;

    for (Square i = 0; i < 64; i++)
    {
        position_hash ^= hash_square(i);
    }
}

Bitstring Game::hash_square(Square square)
{
    switch (piece_on(square))
    {
        case Piece::none:
            return 0;

        case Piece::w_pawn:
            return w_pawn_bitstrings[square];
        case Piece::w_knight:
            return w_knight_bitstrings[square];
        case Piece::w_bishop:
            return w_bishop_bitstrings[square];
        case Piece::w_rook:
            return w_rook_bitstrings[square];
        case Piece::w_queen:
            return w_queen_bitstrings[square];
        case Piece::w_king:
            return w_king_bitstrings[square];

        case Piece::b_pawn:
            return b_pawn_bitstrings[square];
        case Piece::b_knight:
            return b_knight_bitstrings[square];
        case Piece::b_bishop:
            return b_bishop_bitstrings[square];
        case Piece::b_rook:
            return b_rook_bitstrings[square];
        case Piece::b_queen:
            return b_queen_bitstrings[square];
        case Piece::b_king:
            return b_king_bitstrings[square];
    }
}

Bitstring Game::hash()
{
    Bitstring hash_result;
    if (turn == Color::white)
        hash_result = white_bitstring;
    else
        hash_result = black_bitstring;

    hash_result ^= castling_bitstrings[castling_rights];

    if (en_passant_square != -1)
        hash_result ^= en_passant_bitstrings[en_passant_square];

    return hash_result;
}