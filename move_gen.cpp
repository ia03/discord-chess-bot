#include <vector>
#include "game.h"
#include "utils.h"


std::vector<Move> Game::pseudo_legal_moves() const
{
	std::vector<Move> all_moves;
	if (turn == Color::white)
	{
		for (auto square = 0; square < 64; square++)
		{
			std::vector<Move> piece_moves;
			switch (piece)
			{
				case Piece::w_pawn:
					
					break;
				case Piece::w_knight:
				
					break;
				case Piece::w_bishop:
				
					break;
				case Piece::w_rook:
					break;
				case Piece::w_queen:
					break;
				case Piece::w_king:
				
			}
		}
	}
	else
	{
		for (auto piece : pieces_on_board)
		{
			
		}
	}
	
    std::vector<Move> pawn_moves = pseudo_legal_pawn_moves();
    std::vector<Move> knight_moves = pseudo_legal_knight_moves();
    std::vector<Move> bishop_moves = pseudo_legal_bishop_moves();
    std::vector<Move> rook_moves = pseudo_legal_rook_moves();
    std::vector<Move> queen_moves = pseudo_legal_queen_moves();
    std::vector<Move> king_moves = pseudo_legal_king_moves();
    
    
    all_moves.insert(all_moves.end(), pawn_moves.begin(), pawn_moves.end());
    all_moves.insert(all_moves.end(), knight_moves.begin(), knight_moves.end());
    all_moves.insert(all_moves.end(), bishop_moves.begin(), bishop_moves.end());
    all_moves.insert(all_moves.end(), rook_moves.begin(), rook_moves.end());
    all_moves.insert(all_moves.end(), queen_moves.begin(), queen_moves.end());
    all_moves.insert(all_moves.end(), king_moves.begin(), king_moves.end());
    
    return all_moves;
}

std::vector<Move> Game::pseudo_legal_pawn_moves(Square square) const
{
	// White pawn moves are different from black pawn moves.
    if (turn == Color::white)
	{
		// Generate the non-attack move of moving north by 1 square as long as
		// the pawn is not on the 7th row and that square is not occupied.
		
        // If the pawn is on the 7th row and the square in front of it is not occupied, generate all non-capture promotion moves.
        // If the pawn is not on the 7th or 8th rows and is not on the H column and there is a black piece on the square north east of the pawn's square, generate an attack move to that square.
        // If the pawn is not on the 7th or 8th rows and is not on the A column and there is a black piece on the square north west of the pawn's square, generate an attack move to that square.
        // If the pawn is on the 7th row and is not on the H column and there is a black piece on the square north east of the pawn's square, generate all promotion attack moves to that square.
        // If the pawn is on the 7th row and is not on the A column and there is a black piece on the square north west of the pawn's square, generate all promotion attack moves to that square.
        // If the pawn is on the 2nd row (meaning it has not moved yet), generate the 2-squares-north move as long as both squares north of the pawn are not occupied.
        // If the pawn is not on the H column and the square north east of the pawn is the en passant square, generate the en passant move.
        // If the pawn is not on the A column and the square north west of the pawn is the en passant square, generate the en passant move.
	}
	else
	{
		
	}
}
