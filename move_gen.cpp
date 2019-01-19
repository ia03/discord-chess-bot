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
        const Square square = static_cast<Square>(square_index);
        
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
        const Square square = static_cast<Square>(square_index);
        
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
    
    return all_moves;
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

Move Game::pseudo_legal_normal_move(
        const Square origin_sq,
        const Square dest_sq
) const
{
    // Return Move::none if the destination square is invalid or is occupied
    // by a friendly piece.
    if (dest_sq != Square::none && !is_occupied(dest_sq, turn))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pseudo_legal_normal_move(
        const Square origin_sq,
        const std::vector<Direction> &directions
) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            directions
    );
    
    // Generate a move to the specified square as long as it is within the
    // boundaries of the board and is not occupied by any friendly pieces.
    return pseudo_legal_normal_move(origin_sq, dest_sq);
}

Move Game::pawn_north_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(origin_sq, {Direction::north});
    
    // Generate the non-attack move of moving north by 1 square as long as
    // that square is in the boundaries of the board and is not occupied.
    return pseudo_legal_normal_move(origin_sq, dest_sq);
}

Move Game::pawn_south_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(origin_sq, {Direction::south});
    
    // Generate the non-attack move of moving south by 1 square as long as
    // that square is in the boundaries of the board and is not occupied.
    return pseudo_legal_normal_move(origin_sq, dest_sq);
}

Move Game::pawn_north_north_move(const Square origin_sq) const
{
    const auto dest_sq = north_of(north_of(origin_sq));
    
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

Move Game::pawn_south_south_move(const Square origin_sq) const
{
    const auto dest_sq = south_of(south_of(origin_sq));
    
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

Move Game::pawn_north_east_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            {Direction::north, Direction::east}
    );
    
    // If the square north east of the pawn's square is within the boundaries
    // of the board and is occupied by a black piece, generate an attack move
    // to that square.
    if (dest_sq != Square::none &&
        is_occupied(dest_sq, Color::black))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_south_east_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            {Direction::south, Direction::east}
    );
    
    // If the square south east of the pawn's square is within the boundaries
    // of the board and is occupied by a white piece, generate an attack move
    // to that square.
    if (dest_sq != Square::none &&
        is_occupied(dest_sq, Color::white))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_north_west_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            {Direction::north, Direction::west}
    );
    
    // If the square north west of the pawn's square is within the boundaries
    // of the board and is occupied by a black piece, generate an attack move
    // to that square.
    if (dest_sq != Square::none &&
        is_occupied(dest_sq, Color::black))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_south_west_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            {Direction::south, Direction::west}
    );
    
    // If the square south west of the pawn's square is within the boundaries
    // of the board and is occupied by a white piece, generate an attack move
    // to that square.
    if (dest_sq != Square::none &&
        is_occupied(dest_sq, Color::white))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_ep_north_east_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            {Direction::north, Direction::east}
    );

    // If the square north east of the pawn is within the boundaries of the
    // board and is the en passant square, generate the en passant move.
    if (dest_sq == en_passant_square)
    {
        return create_en_passant_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_ep_south_east_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            {Direction::south, Direction::east}
    );

    // If the square south east of the pawn is within the boundaries of the
    // board and is the en passant square, generate the en passant move.
    if (dest_sq == en_passant_square)
    {
        return create_en_passant_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_ep_north_west_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            {Direction::north, Direction::west}
    );
    
    // If the square north west of the pawn is within the boundaries of the
    // board and is the en passant square, generate the en passant move.
    if (dest_sq == en_passant_square)
    {
        return create_en_passant_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

Move Game::pawn_ep_south_west_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            {Direction::south, Direction::west}
    );
    
    // If the square south west of the pawn is within the boundaries of the
    // board and is the en passant square, generate the en passant move.
    if (dest_sq == en_passant_square)
    {
        return create_en_passant_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}

std::array<Move, 4> Game::pawn_promo_north_moves(
        const Square origin_sq
) const
{
    const auto dest_sq = north_of(origin_sq);
    
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

std::array<Move, 4> Game::pawn_promo_south_moves(
        const Square origin_sq
) const
{
    const auto dest_sq = south_of(origin_sq);
    
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

std::array<Move, 4> Game::pawn_promo_north_east_moves(
        const Square origin_sq
) const
{
    const auto dest_sq = north_of(east_of(origin_sq));
    
    // If the pawn is on the 7th row and is not on the H column and there
	// is a black piece on the square north east of the pawn's square,
	// generate all promotion attack moves to that square.
    if (on_bitboard(origin_sq, row_7 & ~col_h) &&
        is_occupied(dest_sq, Color::black))
    {
        return create_promo_moves(origin_sq, dest_sq);
    }
    else
    {
        return {Move::none, Move::none, Move::none, Move::none};
    }
}

std::array<Move, 4> Game::pawn_promo_south_east_moves(
        const Square origin_sq
) const
{
    const auto dest_sq = south_of(east_of(origin_sq));
    
    // If the pawn is on the 2nd row and is not on the H column and there
	// is a white piece on the square south east of the pawn's square,
	// generate all promotion attack moves to that square.
    if (on_bitboard(origin_sq, row_2 & ~col_h) &&
        is_occupied(dest_sq, Color::white))
    {
        return create_promo_moves(origin_sq, dest_sq);
    }
    else
    {
        return {Move::none, Move::none, Move::none, Move::none};
    }
}

std::array<Move, 4> Game::pawn_promo_north_west_moves(
        const Square origin_sq
) const
{
    const auto dest_sq = north_of(west_of(origin_sq));
    
    // If the pawn is on the 7th row and is not on the A column and there is a
	// black piece on the square north west of the pawn's square, generate all
	// promotion attack moves to that square.
    if (on_bitboard(origin_sq, row_7 & ~col_a) &&
        is_occupied(dest_sq, Color::black))
    {
        return create_promo_moves(origin_sq, dest_sq);
    }
    else
    {
        return {Move::none, Move::none, Move::none, Move::none};
    }
}

std::array<Move, 4> Game::pawn_promo_south_west_moves(
        const Square origin_sq
) const
{
    const auto dest_sq = south_of(west_of(origin_sq));
    
    // If the pawn is on the 2nd row and is not on the A column and there is a
	// white piece on the square south west of the pawn's square, generate all
	// promotion attack moves to that square.
    if (on_bitboard(origin_sq, row_2 & ~col_a) &&
        is_occupied(dest_sq, Color::white))
    {
        return create_promo_moves(origin_sq, dest_sq);
    }
    else
    {
        return {Move::none, Move::none, Move::none, Move::none};
    }
}

std::vector<Move> Game::pseudo_legal_w_pawn_moves(const Square square) const
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
    const auto promo_north = pawn_promo_north_moves(square);
    
    possible_moves.insert(
            possible_moves.end(),
            promo_north.begin(),
            promo_north.end()
    );
    
    // Generate capture promotion moves.
    const auto promo_north_east = pawn_promo_north_east_moves(square);
    
    possible_moves.insert(
            possible_moves.end(),
            promo_north_east.begin(),
            promo_north_east.end()
    );
    
    const auto promo_north_west = pawn_promo_north_west_moves(square);
    
    possible_moves.insert(
            possible_moves.end(),
            promo_north_west.begin(),
            promo_north_west.end()
    );
    
    // Remove invalid moves.
    std::remove(possible_moves.begin(), possible_moves.end(), Move::none);
    
    return possible_moves;
}

std::vector<Move> Game::pseudo_legal_b_pawn_moves(const Square square) const
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
    const auto promo_south = pawn_promo_south_moves(square);
    
    possible_moves.insert(
            possible_moves.end(),
            promo_south.begin(),
            promo_south.end()
    );
    
    // Generate capture promotion moves.
    const auto promo_south_east = pawn_promo_south_east_moves(square);
    
    possible_moves.insert(
            possible_moves.end(),
            promo_south_east.begin(),
            promo_south_east.end()
    );
    
    const auto promo_south_west = pawn_promo_south_west_moves(square);
    
    possible_moves.insert(
            possible_moves.end(),
            promo_south_west.begin(),
            promo_south_west.end()
    );
    
    // Remove invalid moves.
    std::remove(possible_moves.begin(), possible_moves.end(), Move::none);
    
    return possible_moves;
}

std::vector<Move> Game::pseudo_legal_knight_moves(const Square square) const
{
	std::vector<Move> possible_moves;
    
    // North North East
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::north, Direction::north, Direction::east}
    ));
    
    // North North West
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::north, Direction::north, Direction::west}
    ));
    
    // North East East
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::north, Direction::east, Direction::east}
    ));
    
    // North West West
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::north, Direction::west, Direction::west}
    ));
    
    // South South East
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::south, Direction::south, Direction::east}
    ));
    
    // South South West
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::south, Direction::south, Direction::west}
    ));
    
    // South East East
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::south, Direction::east, Direction::east}
    ));
    
    // South West West
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::south, Direction::west, Direction::west}
    ));
    
    // Remove invalid moves.
    std::remove(possible_moves.begin(), possible_moves.end(), Move::none);
    
    return possible_moves;
}