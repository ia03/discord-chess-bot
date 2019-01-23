#include <vector>
#include <algorithm>
#include "types.h"
#include "game.h"


const int infinity = 9999999;


// Search function used for the root ply. It uses minimax and alpha-beta
// pruning to return the best legal move for the current position.
Move Game::best_move(const int depth)
{
    const std::vector<Move> possible_moves = pseudo_legal_moves();
    
    Move best_move;
    
    int best_eval;
    
    bool is_maximizing;

    // White wants to maximize the evaluation. Black does not. Start out at
    // the worst evaluation so that another move is picked as the best move.
    if (turn == Color::white)
    {
        is_maximizing = true;
        best_eval = -infinity;
    }
    else
    {
        is_maximizing = false;
        best_eval = infinity;
    }

    // Go through all the moves and pick the one with the best evaluation.
    for (const auto move : possible_moves)
    {
        // Make the move. If it is illegal, skip it.
        if (!make_move(move))
        {
            continue;
        }
        const int eval = minimax(depth, -infinity, infinity, !is_maximizing);
        undo();
        
        // Update the best evaluation if the evaluation of this move is the
        // new best.
        if (is_maximizing)
        {
            if (eval >= best_eval)
            {
                best_move = move;
                best_eval = eval;
            }
        }
        else
        {
            if (eval <= best_eval)
            {
                best_move = move;
                best_eval = eval;
            }
        }
        
    }
    return best_move;
}


// The recursive function that returns the best evaluation found for a
// ply. It utilizes minimax with alpha-beta pruning. This will not be
// used for the root ply.
int Game::minimax(int depth, int alpha, int beta, bool is_maximizing)
{
    const std::vector<Move> possible_moves = pseudo_legal_moves();

    const Game_state state = game_state(possible_moves);

    // A checkmate should be counted as an evaluation of infinity for White or
    // an evaluation of -infinity for Black, as it is the best position for
    // the player making the checkmate and the worst for their opponent.
    // A tie should have an evaluation of 0, it should only be forced if the
    // bot is at a disadvantage.
    switch (state)
    {
        case Game_state::in_progress:
            break;
        case Game_state::checkmate_by_white:
            return infinity;
        case Game_state::checkmate_by_black:
            return -infinity;
        case Game_state::stalemate:
        case Game_state::threefold_repetition:
        case Game_state::fifty_move:
        case Game_state::insufficient_material:
            return 0;
    }

    // Going deeper would be too time-consuming. Evaluate the board instead.
    if (depth == 0)
    {
        return evaluate();
    }
    
    if (is_maximizing)
    {
        int best_eval = -infinity;
        // Go through every move to pick the one with the best evaluation.
        for (auto const move : possible_moves)
        {
            // Make the move. If it is illegal, skip it.
            if (!make_move(move))
            {
                continue;
            }
            const int eval = minimax(depth - 1, alpha, beta, false);
            undo();
            
            best_eval = std::max(best_eval, eval);
            
            // Alpha-beta pruning - stops the search of this position when it
            // is certain that it is better than all the previously evaluated
            // sibling positions.
            alpha = std::max(alpha, best_eval);
            if (beta <= alpha)
            {
                return best_eval;
            }
            
        }
        
        return best_eval;
    }
    else
    {
        int best_eval = infinity;
        // Go through every move to pick the one with the best evaluation.
        for (auto const move : possible_moves)
        {
            // Make the move. If it is illegal, skip it.
            if (!make_move(move))
            {
                continue;
            }
            const int eval = minimax(depth - 1, alpha, beta, true);
            undo();
            
            best_eval = std::min(best_eval, eval);
            
            // Alpha-beta pruning - stops the search of this position when it
            // is certain that it is better than all the previously evaluated
            // sibling positions.
            beta = std::min(beta, best_eval);
            if (beta <= alpha)
            {
                return best_eval;
            }
        }
        
        return best_eval;
    }
    
}
