#include "types.h"
#include "utils.h"
#include "game.h"


// Initializes Zobrist hashing and the evaluation variable.
Game::Game()
{
    // Initialize variables needed for hashing.
    init_zobrist();
    init_hash();
    
    // Initialize the evaluation variable.
    init_eval();
}


// Gets the color of the player who is to play this turn.
const Color Game::get_turn() const
{
    return turn;
}


// Ends the current turn.
void Game::end_turn()
{
    turn = reverse_color(turn);
}


// Determines if white kingside castling has been invalidated. 
bool Game::w_kingside_castling_invalidated() const
{
    // Check if the bit for white kingside castling is set.
    return (static_cast<unsigned>(castling_rights) &
            static_cast<unsigned>(Castling_right::w_kingside)) == 0;
}


// Determines if white queenside castling has been invalidated.
bool Game::w_queenside_castling_invalidated() const
{
    // Check if the bit for white queenside castling is set.
    return (static_cast<unsigned>(castling_rights) &
            static_cast<unsigned>(Castling_right::w_queenside)) == 0;
}


// Determines if black kingside castling has been invalidated.
bool Game::b_kingside_castling_invalidated() const
{
    // Check if the bit for black kingside castling is set.
    return (static_cast<unsigned>(castling_rights) &
            static_cast<unsigned>(Castling_right::b_kingside)) == 0;
}


// Determines if white queenside castling has been invalidated.
bool Game::b_queenside_castling_invalidated() const
{
    // Check if the bit for black queenside castling is set.
    return (static_cast<unsigned>(castling_rights) &
            static_cast<unsigned>(Castling_right::b_queenside)) == 0;
}


// Invalidates white kingside castling.
void Game::invalidate_w_kingside_castling()
{
    // Unset the bit for white kingside castling.
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::w_kingside));
}


// Invalidates white queenside castling.
void Game::invalidate_w_queenside_castling()
{
    // Unset the bit for white queenside castling.
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::w_queenside));
}


// Invalidates white castling.
void Game::invalidate_white_castling()
{
    // Unset the bits for white castling.
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::white));
}


// Invalidates black kingside castling.
void Game::invalidate_b_kingside_castling()
{
    // Unset the bit for black kingside castling.
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::b_kingside));
}


// Invalidates black queenside castling.
void Game::invalidate_b_queenside_castling()
{
    // Unset the bit for black queenside castling.
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::b_queenside));
}


// Invalidates black castling.
void Game::invalidate_black_castling()
{
    // Unset the bits for black castling.
    castling_rights = static_cast<Castling_right>(
             static_cast<unsigned>(castling_rights) &
            ~static_cast<unsigned>(Castling_right::black));
}
