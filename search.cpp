#include <vector>
#include <algorithm>
#include "types.h"
#include "game.h"


const int depth = 6;
const int infinity = 9999999;

Move Game::best_move()
{
    std::vector<Move> possible_moves = pseudo_legal_moves();
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
	for (int i = 0; i < possible_moves.size(); i++)
	{
		Move move = possible_moves[i];
		
		// If the move is illegal, skip it.
		if (!make_move(move))
		{
			continue;
		}
		int eval = minimax(depth, -infinity, infinity, !is_maximizing);
		undo();
		
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

int Game::minimax(int depth, int alpha, int beta, bool is_maximizing)
{
	std::vector<Move> possible_moves = pseudo_legal_moves();

	Game_state state = game_state(possible_moves);

	// A checkmate should be counted as an evaluation of infinity for White or
	// an evaluation of -infinity for Black, as it is the best position for
	// the player making the checkmate and the worst for their opponent.
	// A tie should have an evaluation of 0, it should only be forced if the
	// bot is at a disadvantage.
	switch (state)
	{
		case Game_state::in_progress:
			break;
		case Game_state::white_checkmate:
			return infinity;
		case Game_state::black_checkmate:
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
		for (int i = 0; i < possible_moves.size(); i++)
		{
			// If the move is illegal, skip it.
			if (!make_move(possible_moves[i]))
			{
				continue;
			}
			int eval = minimax(depth - 1, alpha, beta, false);
			undo();
			
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
		for (int i = 0; i < possible_moves.size(); i++)
		{
			if (!make_move(possible_moves[i]))
			{
				continue;
			}
			int eval = minimax(depth - 1, alpha, beta, true);
			undo();
			
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
