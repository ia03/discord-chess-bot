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
			switch (piece_on(static_cast<Square>(square)))
			{
				case Piece::w_pawn:
					piece_moves = pseudo_legal_w_pawn_moves(
                            static_cast<Square>(square));
					break;
				case Piece::w_knight:
                    piece_moves = pseudo_legal_knight_moves(
                            static_cast<Square>(square));
					break;
				case Piece::w_bishop:
                    piece_moves = pseudo_legal_bishop_moves(
                            static_cast<Square>(square));
					break;
				case Piece::w_rook:
                    piece_moves = pseudo_legal_rook_moves(
                            static_cast<Square>(square));
					break;
				case Piece::w_queen:
                    piece_moves = pseudo_legal_queen_moves(
                            static_cast<Square>(square));
					break;
				case Piece::w_king:
                    piece_moves = pseudo_legal_king_moves();
                    break;
                default:
                    continue;
			}
            
            all_moves.insert(
                    all_moves.end(),
                    piece_moves.begin(),
                    piece_moves.end());
		}
	}
	else
	{
		for (auto square = 0; square < 64; square++)
		{
			std::vector<Move> piece_moves;
			switch (piece_on(static_cast<Square>(square)))
			{
				case Piece::b_pawn:
					piece_moves = pseudo_legal_b_pawn_moves(
                            static_cast<Square>(square));
					break;
				case Piece::b_knight:
                    piece_moves = pseudo_legal_knight_moves(
                            static_cast<Square>(square));
					break;
				case Piece::b_bishop:
                    piece_moves = pseudo_legal_bishop_moves(
                            static_cast<Square>(square));
					break;
				case Piece::b_rook:
                    piece_moves = pseudo_legal_rook_moves(
                            static_cast<Square>(square));
					break;
				case Piece::b_queen:
                    piece_moves = pseudo_legal_queen_moves(
                            static_cast<Square>(square));
					break;
				case Piece::b_king:
                    piece_moves = pseudo_legal_king_moves();
                    break;
                default:
                    continue;
			}
            
            all_moves.insert(
                    all_moves.end(),
                    piece_moves.begin(),
                    piece_moves.end());
		}
	}
    
    return all_moves;
}

std::vector<Move> Game::pseudo_legal_w_pawn_moves(Square square) const
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
