#include <vector>
#include "game.h"
#include "utils.h"


std::vector<Move> Game::pseudo_legal_w_moves() const
{
	std::vector<Move> all_moves;
	for (auto square_index = 0; square_index < 64; square_index++)
	{
		std::vector<Move> piece_moves;
		switch (piece_on(static_cast<Square>(square_index)))
		{
			case Piece::w_pawn:
				piece_moves = pseudo_legal_w_pawn_moves(
						static_cast<Square>(square_index));
				break;
			case Piece::w_knight:
				piece_moves = pseudo_legal_knight_moves(
						static_cast<Square>(square_index));
				break;
			case Piece::w_bishop:
				piece_moves = pseudo_legal_bishop_moves(
						static_cast<Square>(square_index));
				break;
			case Piece::w_rook:
				piece_moves = pseudo_legal_rook_moves(
						static_cast<Square>(square_index));
				break;
			case Piece::w_queen:
				piece_moves = pseudo_legal_queen_moves(
						static_cast<Square>(square_index));
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
	
	return all_moves;
}

std::vector<Move> Game::pseudo_legal_b_moves() const
{
	for (auto square_index = 0; square_index < 64; square_index++)
	{
		std::vector<Move> piece_moves;
		switch (piece_on(static_cast<Square>(square_index)))
		{
			case Piece::b_pawn:
				piece_moves = pseudo_legal_b_pawn_moves(
						static_cast<Square>(square_index));
				break;
			case Piece::b_knight:
				piece_moves = pseudo_legal_knight_moves(
						static_cast<Square>(square_index));
				break;
			case Piece::b_bishop:
				piece_moves = pseudo_legal_bishop_moves(
						static_cast<Square>(square_index));
				break;
			case Piece::b_rook:
				piece_moves = pseudo_legal_rook_moves(
						static_cast<Square>(square_index));
				break;
			case Piece::b_queen:
				piece_moves = pseudo_legal_queen_moves(
						static_cast<Square>(square_index));
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

std::vector<Move> Game::pseudo_legal_moves() const
{
	
	if (turn == Color::white)
	{
		return pseudo_legal_w_moves();
	}
	else
	{
		return pseudo_legal_b_moves();
	}
}

std::vector<Move> Game::pseudo_legal_w_pawn_moves(Square square) const
{
	std::vector<Move> possible_moves;
	const auto pawn_bitboard = square_to_bb(square);
	
	const bool pawn_on_2nd_row = (pawn_bitboard & row_2) != 0;
	const bool pawn_on_7th_row = (pawn_bitboard & row_7) != 0;
	
	const bool pawn_on_a_col = (pawn_bitboard & col_a) != 0;
	const bool pawn_on_h_col = (pawn_bitboard & col_h) != 0;
	
    // Generate the non-attack move of moving north by 1 square as long as
    // the pawn is not on the 7th row and that square is not occupied.
    if (!pawn_on_7th_row && ((pawn_bitboard << 8) & all_bitboard == 0))
	{
		possible_moves.push_back(create_normal_move(
				square,
				static_cast<Square>(static_cast<int>(square) + 8),
		));
	}
	
    // If the pawn is on the 7th row and the square in front of it is not
	// occupied, generate all non-capture promotion moves.
	if (pawn_on_7th_row && ((pawn_bitboard << 8) & all_bitboard == 0))
	{
        auto promo_moves = create_promo_moves(
                square,
                static_cast<Square>(static_cast<int>(square) + 8)
        );
        
        possible_moves.insert(
                possible_moves.end(),
                promo_moves.begin(),
                promo_moves.end()
        );
	}
	
    // If the pawn is not on the 7th row and is not on the H column
	// and there is a black piece on the square north east of the pawn's
	// square, generate an attack move to that square.
	if (!pawn_on_7th_row && !pawn_on_h_col &&
		((pawn_bitboard << 9) & black_bitboard != 0))
	{
		possible_moves.push_back(create_normal_move(
				square,
				static_cast<Square>(static_cast<int>(square) + 9),
		));
	}
	
    // If the pawn is not on the 7th row and is not on the A column
	// and there is a black piece on the square north west of the pawn's
	// square, generate an attack move to that square.
	if (!pawn_on_7th_row && !pawn_on_h_col &&
		((pawn_bitboard << 7) & black_bitboard != 0))
	{
		possible_moves.push_back(create_normal_move(
				square,
				static_cast<Square>(static_cast<int>(square) + 7)
		));
	}
	// If the pawn is on the 7th row and is not on the H column and there
	// is a black piece on the square north east of the pawn's square,
	// generate all promotion attack moves to that square.
	if (pawn_on_7th_row && !pawn_on_h_col &&
		((pawn_bitboard << 9) & black_bitboard != 0))
	{
		auto promo_moves = create_promo_moves(
				square,
				static_cast<Square>(static_cast<int>(square) + 9)
		);
		
		possible_moves.insert(
				possible_moves.end(),
				promo_moves.begin(),
				promo_moves.end()
		);
	}
    
    // If the pawn is on the 7th row and is not on the A column and there is a
	// black piece on the square north west of the pawn's square, generate all
	// promotion attack moves to that square.
	if (pawn_on_7th_row && !pawn_on_a_col &&
		((pawn_bitboard << 7) & black_bitboard != 0))
	{
		auto promo_moves = create_promo_moves(
				square,
				static_cast<Square>(static_cast<int>(square) + 7)
		);
		
		possible_moves.insert(
				possible_moves.end(),
				promo_moves.begin(),
				promo_moves.end()
		);
	}
	
    // If the pawn is on the 2nd row (meaning it has not moved yet), generate
	// the 2-squares-north move as long as both squares north of the pawn are
	// not occupied.
	if (pawn_on_2nd_row && ((pawn_bitboard << 8) & all_bitboard == 0) &&
		((pawn_bitboard << 16) & all_bitboard = 0))
	{
		possible_moves.push_back(create_normal_move(
				square,
				static_cast<Square>(static_cast<int>(square) + 16)
		));
	}
	
    // If the pawn is not on the H column and the square north east of the
	// pawn is the en passant square, generate the en passant move.
	if (!pawn_on_h_col && ((static_cast<int>(square) + 9) == en_passant_square))
	{
		possible_moves.push_back(create_en_passant_move(
				square,
				en_passant_square
		));
	}
	
	
    // If the pawn is not on the A column and the square north west of the
	// pawn is the en passant square, generate the en passant move.
	if (!pawn_on_a_col && ((static_cast<int>(square) + 7) == en_passant_square))
	{
		possible_moves.push_back(create_en_passant_move(
				square,
				en_passant_square
		));
	}
}

std::vector<Move> pseudo_legal_knight_moves(Square square) const
{
	if (turn == Color::white)
	{
		const auto &friendly_bitboard = white_bitboard;
	}
	else
	{
		const auto &friendly_bitboard = black_bitboard;
	}
	
	std::vector<Move> possible_moves;
	const auto knight_bitboard = square_to_bb(square);
	
	const bool knight_on_1st_row = (knight_bitboard & row_1) != 0;
	const bool knight_on_2nd_row = (knight_bitboard & row_2) != 0;
	const bool knight_on_7th_row = (knight_bitboard & row_7) != 0;
	const bool knight_on_8th_row = (knight_bitboard & row_8) != 0;
	
	const bool knight_on_a_col = (knight_bitboard & col_a) != 0;
	const bool knight_on_b_col = (knight_bitboard & col_b) != 0;
	const bool knight_on_g_col = (knight_bitboard & col_g) != 0;
	const bool knight_on_h_col = (knight_bitboard & col_h) != 0;
	
	
	// If the knight is not on the H column and is not on the 7th or 8th rows,
	// generate a move to the north north east square as long as it is not
	// occupied by any friendly pieces.
	if (!knight_on_h_col && !knight_on_7th_row && !knight_on_8th_row &&
		((static_cast<int>(square) + 25) & friendly_bitboard == 0))
	{
		possible_moves.push_back(create_normal_move(
				square,
				static_cast<Square>(static_cast<int>(square) + 25)
		));
	}
	
	// If the knight is not on the A column and is not on the 7th or 8th rows,
	// generate a move to the north north west square as long as it is not
	// occupied by any friendly pieces.
	if (!knight_on_a_col && !knight_on_7th_row && !knight_on_8th_row &&
		(((static_cast<int>(square) + 23) & friendly_bitboard == 0)))
	{
		
	}
	
	// If the knight is not on the G or H columns and is not on the 8th row, generate a move to the north east east square as long as it is not occupied by any friendly pieces.
	// If the knight is not on the A or B columns and is not on the 8th row, generate a move to the north west west square as long as it is not occupied by any friendly pieces.
	// If the knight is not on the H column and is not on the 1st or 2nd rows, generate a move to the south south east square as long as it is not occupied by any friendly pieces.
	// If the knight is not on the A column and is not on the 1st or 2nd rows, generate a move to the south south west square as long as it is not occupied by any friendly pieces.
	// If the knight is not on the G or H columns and is not on the 1st row, generate a move to the south east east square as long as it is not occupied by any friendly pieces.
	// If the knight is not on the A or B columns and is not on the 1st row, generate a move to the south west west square as long as it is not occupied by any friendly pieces.
}
