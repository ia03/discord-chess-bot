#include <vector>
#include <array>
#include <algorithm>
#include "game.h"
#include "utils.h"


std::vector<Move> Game::pseudo_legal_w_moves() const
{
	std::vector<Move> all_moves;
	for (auto square_index = 0; square_index < 64; square_index++)
	{
		std::vector<Move> piece_moves;
        Square square = static_cast<Square>(square_index);
        
		switch (piece_on(square))
		{
			case Piece::w_pawn:
				piece_moves = pseudo_legal_w_pawn_moves(square);
				break;
			case Piece::w_knight:
				piece_moves = pseudo_legal_knight_moves(square);
				break;
			case Piece::w_bishop:
				piece_moves = pseudo_legal_bishop_moves(square);
				break;
			case Piece::w_rook:
				piece_moves = pseudo_legal_rook_moves(square);
				break;
			case Piece::w_queen:
				piece_moves = pseudo_legal_queen_moves(square);
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
    std::vector<Move> all_moves;
	for (auto square_index = 0; square_index < 64; square_index++)
	{
		std::vector<Move> piece_moves;
        Square square = static_cast<Square>(square_index);
        
		switch (piece_on(square))
		{
			case Piece::b_pawn:
				piece_moves = pseudo_legal_b_pawn_moves(square);
				break;
			case Piece::b_knight:
				piece_moves = pseudo_legal_knight_moves(square);
				break;
			case Piece::b_bishop:
				piece_moves = pseudo_legal_bishop_moves(square);
				break;
			case Piece::b_rook:
				piece_moves = pseudo_legal_rook_moves(square);
				break;
			case Piece::b_queen:
				piece_moves = pseudo_legal_queen_moves(square);
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

Move Game::pawn_north_move(Square origin_sq) const
{
    auto dest_sq = north_of(origin_sq);
    
    // Generate the non-attack move of moving north by 1 square as long as
    // the pawn is not on the 7th row and that square is not occupied.
    if (!on_bitboard(origin_sq, row_7) && !is_occupied(dest_sq))
	{
		return create_normal_move(origin_sq, dest_sq);
	}
    else
    {
        return Move::none;
    }
}

Move Game::pawn_south_move(Square origin_sq) const
{
    auto dest_sq = south_of(origin_sq);
    
    // Generate the non-attack move of moving south by 1 square as long as
    // the pawn is not on the 2nd row and that square is not occupied.
    if (!on_bitboard(origin_sq, row_2) && !is_occupied(dest_sq))
	{
		return create_normal_move(origin_sq, dest_sq);
	}
    else
    {
        return Move::none;
    }
}

Move Game::pawn_north_north_move(Square origin_sq) const
{
    auto dest_sq = north_of(north_of(origin_sq));
    
    // If the pawn is on the 2nd row (meaning it has not moved yet), generate
	// the 2-squares-north move as long as both squares north of the pawn are
	// not occupied.
    if (on_bitboard(origin_sq, row_2) && !is_occupied(north_of(dest_sq)) &&
        !is_occupied(dest_sq))
    {
		return create_normal_move(origin_sq, dest_sq);
	}
    else
    {
        return Move::none;
    }
}

Move Game::pawn_south_south_move(Square origin_sq) const
{
    auto dest_sq = south_of(south_of(origin_sq));
    
    // If the pawn is on the 7th row (meaning it has not moved yet), generate
	// the 2-squares-south move as long as both squares south of the pawn are
	// not occupied.
    if (on_bitboard(origin_sq, row_7) && !is_occupied(south_of(dest_sq)) &&
        !is_occupied(dest_sq))
    {
		return create_normal_move(origin_sq, dest_sq);
	}
    else
    {
        return Move::none;
    }
}

Move Game::pawn_north_east_move(Square origin_sq) const
{
    auto dest_sq = north_of(east_of(origin_sq));
    
    // If the pawn is not on the 7th row and is not on the H column
	// and there is a black piece on the square north east of the pawn's
	// square, generate an attack move to that square.
    if (!on_bitboard(origin_sq, row_7 | col_h) &&
        is_occupied_by_black(dest_sq))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_south_east_move(Square origin_sq) const
{
    auto dest_sq = south_of(east_of(origin_sq));
    
    // If the pawn is not on the 2nd row and is not on the H column
	// and there is a white piece on the square south east of the pawn's
	// square, generate an attack move to that square.
    if (!on_bitboard(origin_sq, row_2 | col_h) &&
        is_occupied_by_white(dest_sq))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_north_west_move(Square origin_sq) const
{
    auto dest_sq = north_of(west_of(origin_sq));
    
    // If the pawn is not on the 7th row and is not on the A column
	// and there is a black piece on the square north west of the pawn's
	// square, generate an attack move to that square.
    if (!on_bitboard(origin_sq, row_7 | col_a) &&
        is_occupied_by_black(dest_sq))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_south_west_move(Square origin_sq) const
{
    auto dest_sq = south_of(west_of(origin_sq));
    
    // If the pawn is not on the 2nd row and is not on the A column
	// and there is a white piece on the square south west of the pawn's
	// square, generate an attack move to that square.
    if (!on_bitboard(origin_sq, row_2 | col_a) &&
        is_occupied_by_white(dest_sq))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_ep_north_east_move(Square origin_sq) const
{
    auto dest_sq = north_of(east_of(origin_sq));

    // If the pawn is not on the H column and the square north east of the
	// pawn is the en passant square, generate the en passant move.
    if (!on_bitboard(origin_sq, col_h) && dest_sq == en_passant_square)
    {
        return create_en_passant_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_ep_south_east_move(Square origin_sq) const
{
    auto dest_sq = south_of(east_of(origin_sq));

    // If the pawn is not on the H column and the square south east of the
	// pawn is the en passant square, generate the en passant move.
    if (!on_bitboard(origin_sq, col_h) && dest_sq == en_passant_square)
    {
        return create_en_passant_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_ep_north_west_move(Square origin_sq) const
{
    auto dest_sq = north_of(west_of(origin_sq));
    
    // If the pawn is not on the A column and the square north west of the
	// pawn is the en passant square, generate the en passant move.
    if (!on_bitboard(origin_sq, col_a) && dest_sq == en_passant_square)
    {
        return create_en_passant_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_ep_south_west_move(Square origin_sq) const
{
    auto dest_sq = south_of(west_of(origin_sq));
    
    // If the pawn is not on the A column and the square south west of the
	// pawn is the en passant square, generate the en passant move.
    if (!on_bitboard(origin_sq, col_a) && dest_sq == en_passant_square)
    {
        return create_en_passant_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

std::array<Move, 4> Game::pawn_promo_north_moves(Square origin_sq) const
{
    auto dest_sq = north_of(origin_sq);
    
    // If the pawn is on the 7th row and the square in front of it is not
	// occupied, generate all non-capture promotion moves.
    if (on_bitboard(origin_sq, row_7) && !is_occupied(dest_sq))
	{
        return create_promo_moves(origin_sq, dest_sq);
	}
    else
    {
        return {Move::none, Move::none, Move::none, Move::none};
    }
}

std::array<Move, 4> Game::pawn_promo_south_moves(Square origin_sq) const
{
    auto dest_sq = south_of(origin_sq);
    
    // If the pawn is on the 2nd row and the square in front of it is not
	// occupied, generate all non-capture promotion moves.
    if (on_bitboard(origin_sq, row_2) && !is_occupied(dest_sq))
	{
        return create_promo_moves(origin_sq, dest_sq);
	}
    else
    {
        return {Move::none, Move::none, Move::none, Move::none};
    }
}

std::array<Move, 4> Game::pawn_promo_north_east_moves(Square origin_sq) const
{
    auto dest_sq = north_of(east_of(origin_sq));
    
    // If the pawn is on the 7th row and is not on the H column and there
	// is a black piece on the square north east of the pawn's square,
	// generate all promotion attack moves to that square.
    if (on_bitboard(origin_sq, row_7 | col_h) &&
        is_occupied_by_black(dest_sq))
    {
        return create_promo_moves(origin_sq, dest_sq);
    }
    else
    {
        return {Move::none, Move::none, Move::none, Move::none};
    }
}

std::array<Move, 4> Game::pawn_promo_south_east_moves(Square origin_sq) const
{
    auto dest_sq = south_of(east_of(origin_sq));
    
    // If the pawn is on the 2nd row and is not on the H column and there
	// is a white piece on the square south east of the pawn's square,
	// generate all promotion attack moves to that square.
    if (on_bitboard(origin_sq, row_2 | col_h) &&
        is_occupied_by_white(dest_sq))
    {
        return create_promo_moves(origin_sq, dest_sq);
    }
    else
    {
        return {Move::none, Move::none, Move::none, Move::none};
    }
}

std::array<Move, 4> Game::pawn_promo_north_west_moves(Square origin_sq) const
{
    auto dest_sq = north_of(west_of(origin_sq));
    
    // If the pawn is on the 7th row and is not on the A column and there is a
	// black piece on the square north west of the pawn's square, generate all
	// promotion attack moves to that square.
    if (on_bitboard(origin_sq, row_7 | col_a) &&
        is_occupied_by_black(dest_sq))
    {
        return create_promo_moves(origin_sq, dest_sq);
    }
    else
    {
        return {Move::none, Move::none, Move::none, Move::none};
    }
}

std::array<Move, 4> Game::pawn_promo_south_west_moves(Square origin_sq) const
{
    auto dest_sq = south_of(west_of(origin_sq));
    
    // If the pawn is on the 2nd row and is not on the A column and there is a
	// white piece on the square north west of the pawn's square, generate all
	// promotion attack moves to that square.
    if (on_bitboard(origin_sq, row_2 | col_a) &&
        is_occupied_by_white(dest_sq))
    {
        return create_promo_moves(origin_sq, dest_sq);
    }
    else
    {
        return {Move::none, Move::none, Move::none, Move::none};
    }
}

std::vector<Move> Game::pseudo_legal_w_pawn_moves(Square square) const
{
	std::vector<Move> possible_moves;
    
    // Generate non-capture moves.
    possible_moves.push_back(pawn_north_move(square));
    possible_moves.push_back(pawn_north_north_move(square));
    
    // Generate capture moves.
    possible_moves.push_back(pawn_north_east_move(square));
    possible_moves.push_back(pawn_north_west_move(square));
    
    // Generate en passant moves.
    possible_moves.push_back(pawn_ep_north_east_move(square));
    possible_moves.push_back(pawn_ep_north_west_move(square));
    
    // Generate non-capture promotion moves.
    auto promo_north = pawn_promo_north_moves(square);
    
    possible_moves.insert(
            possible_moves.end(),
            promo_north.begin(),
            promo_north.end()
    );
    
    // Generate capture promotion moves.
    auto promo_north_east = pawn_promo_north_east_moves(square);
    
    possible_moves.insert(
            possible_moves.end(),
            promo_north_east.begin(),
            promo_north_east.end()
    );
    
    auto promo_north_west = pawn_promo_north_west_moves(square);
    
    possible_moves.insert(
            possible_moves.end(),
            promo_north_west.begin(),
            promo_north_west.end()
    );
    
    // Remove the moves that are not pseudo-legal.
    std::remove(possible_moves.begin(), possible_moves.end(), Move::none);
    
    return possible_moves;
}

std::vector<Move> Game::pseudo_legal_b_pawn_moves(Square square) const
{
    std::vector<Move> possible_moves;
    
    // Generate non-capture moves.
    possible_moves.push_back(pawn_south_move(square));
    possible_moves.push_back(pawn_south_south_move(square));
    
    // Generate capture moves.
    possible_moves.push_back(pawn_south_east_move(square));
    possible_moves.push_back(pawn_south_west_move(square));
    
    // Generate en passant moves.
    possible_moves.push_back(pawn_ep_south_east_move(square));
    possible_moves.push_back(pawn_ep_south_west_move(square));
    
    // Generate non-capture promotion moves.
    auto promo_south = pawn_promo_south_moves(square);
    
    possible_moves.insert(
            possible_moves.end(),
            promo_north.begin(),
            promo_north.end()
    );
    
    // Generate capture promotion moves.
    auto promo_south_east = pawn_promo_south_east_moves(square);
    
    possible_moves.insert(
            possible_moves.end(),
            south_north_east.begin(),
            south_north_east.end()
    );
    
    auto promo_south_west = pawn_promo_south_west_moves(square);
    
    possible_moves.insert(
            possible_moves.end(),
            south_north_west.begin(),
            south_north_west.end()
    );
    
    // Remove the moves that are not pseudo-legal.
    std::remove(possible_moves.begin(), possible_moves.end(), Move::none);
    
    return possible_moves;
}

std::vector<Move> Game::pseudo_legal_knight_moves(Square square) const
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
