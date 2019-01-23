#include "types.h"
#include "utils.h"
#include "game.h"


Game::Game()
{
    // Initialize variables needed for hashing.
    init_zobrist();
    init_hash();
    
    // Initialize the evaluation variable.
    init_eval();
}

const Color Game::get_turn() const
{
    return turn;
}

void Game::end_turn()
{
    turn = reverse_color(turn);
}

bool Game::w_kingside_castling_invalidated() const
{
    // Check if the bit for white kingside castling is set.
    return (static_cast<unsigned>(castling_rights) &
            static_cast<unsigned>(Castling_right::w_kingside)) == 0;
}

bool Game::w_queenside_castling_invalidated() const
{
    // Check if the bit for white queenside castling is set.
    return (static_cast<unsigned>(castling_rights) &
            static_cast<unsigned>(Castling_right::w_queenside)) == 0;
}

bool Game::b_kingside_castling_invalidated() const
{
    // Check if the bit for black kingside castling is set.
    return (static_cast<unsigned>(castling_rights) &
            static_cast<unsigned>(Castling_right::b_kingside)) == 0;
}

bool Game::b_queenside_castling_invalidated() const
{
    // Check if the bit for black queenside castling is set.
    return (static_cast<unsigned>(castling_rights) &
            static_cast<unsigned>(Castling_right::b_queenside)) == 0;
}

void Game::invalidate_w_kingside_castling()
{
    // Unset the bit for white kingside castling.
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::w_kingside));
}

void Game::invalidate_w_queenside_castling()
{
    // Unset the bit for white queenside castling.
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::w_queenside));
}

void Game::invalidate_white_castling()
{
    // Unset the bits for white castling.
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::white));
}

void Game::invalidate_b_kingside_castling()
{
    // Unset the bit for black kingside castling.
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::b_kingside));
}

void Game::invalidate_b_queenside_castling()
{
    // Unset the bit for black queenside castling.
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::b_queenside));
}

void Game::invalidate_black_castling()
{
    // Unset the bits for black castling.
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::black));
}
