#include "types.h"
#include "utils.h"
#include "game.h"


Game::Game()
{
    init_zobrist();
    init_hash();
}

const Color Game::get_turn()
{
    return turn;
}

void Game::end_turn()
{
    turn = reverse_color(turn);
}