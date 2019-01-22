#include "types.h"
#include "utils.h"
#include "game.h"


Game::Game()
{
    init_zobrist();
    init_hash();
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
    return (static_cast<unsigned>(castling_rights) &
            static_cast<unsigned>(Castling_right::w_kingside)) == 0;
}

bool Game::w_queenside_castling_invalidated() const
{
    return (static_cast<unsigned>(castling_rights) &
            static_cast<unsigned>(Castling_right::w_queenside)) == 0;
}

bool Game::b_kingside_castling_invalidated() const
{
    return (static_cast<unsigned>(castling_rights) &
            static_cast<unsigned>(Castling_right::b_kingside)) == 0;
}

bool Game::b_queenside_castling_invalidated() const
{
    return (static_cast<unsigned>(castling_rights) &
            static_cast<unsigned>(Castling_right::b_queenside)) == 0;
}

void Game::invalidate_w_kingside_castling()
{
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::w_kingside));
}

void Game::invalidate_w_queenside_castling()
{
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::w_queenside));
}

void Game::invalidate_white_castling()
{
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::white));
}

void Game::invalidate_b_kingside_castling()
{
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::b_kingside));
}

void Game::invalidate_b_queenside_castling()
{
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::b_queenside));
}

void Game::invalidate_black_castling()
{
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::black));
}
