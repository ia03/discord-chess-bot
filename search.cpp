#include <vector>
#include <algorithm>
#include "types.h"
#include "game.h"


const depth = 6;
const infinity = 9999999;

Move Game::best_move()
{
    std::vector<Move> possible_moves = pseudo_legal_moves();
	Move best_move;
	int best_eval = infinity;
	
	bool is_maximizing;
	
	if (turn == Color::white)
	{
		is_maximizing = true;
	}
	else
	{
		is_maximizing = false;
	}
	
	for (int i = 0; i < possible_moves.size(); i++)
	{
		Move move = possible_moves[i];
		make_move(move);
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
		
		return best_move;
	}
}

int Game::minimax(int depth, int alpha, int beta, bool is_maximizing)
{
	std::vector<Move> possible_moves = pseudo_legal_moves();
	
	Game_state state = game_state(possible_moves);
	switch (game_state)
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
	
	if (depth == 0)
	{
		return evaluate();
	}
	
	if (is_maximizing)
	{
		int best_eval = -infinity;
		for (int i = 0; i < possible_moves.size(); i++)
		{
			// Make the move. If it's illegal, skip it.
			if (!make_move(possible_moves[i]))
			{
				continue;
			}
			int eval = minimax(depth - 1, alpha, beta, false);
			undo();
			
			// Alpha-beta pruning
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
			
			// Alpha-beta pruning
			beta = std::min(beta, best_eval);
			if (beta <= alpha)
			{
				return best_eval;
			}
		}
		
		return best_eval;
	}
	
}
