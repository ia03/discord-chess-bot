#include <vector>
#include <array>
#include <algorithm>
#include "game.h"
#include "utils.h"
#include "lib/magicmoves.h"


// Generates all pseudo-legal moves for White.
std::vector<Move> Game::pseudo_legal_w_moves() const
{
    std::vector<Move> all_moves;

    // Generate the pseudo-legal moves for each square.
    for (auto square_index = 0; square_index < 64; square_index++)
    {
        std::vector<Move> piece_moves;
        const auto square = static_cast<Square>(square_index);

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
                piece_moves = pseudo_legal_king_moves(square);
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


// Generates all pseudo-legal moves for Black.
std::vector<Move> Game::pseudo_legal_b_moves() const
{
    std::vector<Move> all_moves;

    // Generate the pseudo-legal moves for each square.
    for (auto square_index = 0; square_index < 64; square_index++)
    {
        std::vector<Move> piece_moves;
        const auto square = static_cast<Square>(square_index);

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
                piece_moves = pseudo_legal_king_moves(square);
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


// Generates all pseudo-legal moves for the current player.
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


// If the destination square is invalid or is occupied by a friendly
// piece, this returns Move::none. Otherwise, it returns a normal move
// with the origin and destination squares set. Used with
// find_dest_square() to generate simple moves.
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


// Generates a normal move from the origin square to wherever the
// directions lead to if that is a square within the boundaries of the
// board and is not occupied by any friendly pieces.
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


// Removes squares from an attack bitboard that are occupied by pieces
// belonging to the player who is to move this turn.
Bitboard Game::discard_self_captures(const Bitboard attack_bitboard) const
{
    // Discard self-captures.
    if (turn == Color::white)
    {
        return attack_bitboard & ~white_bitboard;
    }
    else
    {
        return attack_bitboard & ~black_bitboard;
    }
}


// Generates the pawn north-by-1 move using the origin square. Returns
// Move::none if the move would not be pseudo-legal.
Move Game::pawn_north_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(origin_sq, {Direction::north});

    // Generate the non-attack move of moving north by 1 square as long as
    // that square is in the boundaries of the board and is not occupied.
    if (dest_sq != Square::none && !is_occupied(dest_sq))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}


// Generates the pawn south-by-1 move using the origin square. Returns
// Move::none if the move would not be pseudo-legal.
Move Game::pawn_south_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(origin_sq, {Direction::south});

    // Generate the non-attack move of moving south by 1 square as long as
    // that square is in the boundaries of the board and is not occupied.
    if (dest_sq != Square::none && !is_occupied(dest_sq))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}


// Generates the pawn north-by-2 move using the origin square. Returns
// Move::none if the move would not be pseudo-legal.
Move Game::pawn_north_north_move(const Square origin_sq) const
{
    const auto dest_sq = north_of(north_of(origin_sq));

    // If the pawn is on the 2nd row (meaning it has not moved yet), generate
    // the 2-squares-north move as long as both squares north of the pawn are
    // not occupied.
    if (on_bitboard(origin_sq, row_2) && !is_occupied(north_of(origin_sq)) &&
        !is_occupied(dest_sq))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}


// Generates the pawn south-by-2 move using the origin square. Returns
// Move::none if the move would not be pseudo-legal.
Move Game::pawn_south_south_move(const Square origin_sq) const
{
    const auto dest_sq = south_of(south_of(origin_sq));

    // If the pawn is on the 7th row (meaning it has not moved yet), generate
    // the 2-squares-south move as long as both squares south of the pawn are
    // not occupied.
    if (on_bitboard(origin_sq, row_7) && !is_occupied(south_of(origin_sq)) &&
        !is_occupied(dest_sq))
    {
        return create_normal_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}


// Generates the pawn north-east capture move using the origin square.
// Returns Move::none if the move would not be pseudo-legal.
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


// Generates the pawn south-east capture move using the origin square.
// Returns Move::none if the move would not be pseudo-legal.
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


// Generates the pawn north-west capture move using the origin square.
// Returns Move::none if the move would not be pseudo-legal.
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


// Generates the pawn south-west capture move using the origin square.
// Returns Move::none if the move would not be pseudo-legal.
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


// Generates the pawn north-east en passant move using the origin square.
// Returns Move::none if the move would not be pseudo-legal.
Move Game::pawn_ep_north_east_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            {Direction::north, Direction::east}
    );

    // If the square north east of the pawn is within the boundaries of the
    // board and is the en passant square, generate the en passant move.
    if (dest_sq != Square::none && dest_sq == en_passant_square)
    {
        return create_en_passant_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}


// Generates the pawn south-east en passant move using the origin square.
// Returns Move::none if the move would not be pseudo-legal.
Move Game::pawn_ep_south_east_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            {Direction::south, Direction::east}
    );

    // If the square south east of the pawn is within the boundaries of the
    // board and is the en passant square, generate the en passant move.
    if (dest_sq != Square::none && dest_sq == en_passant_square)
    {
        return create_en_passant_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}


// Generates the pawn north-west en passant move using the origin square.
// Returns Move::none if the move would not be pseudo-legal.
Move Game::pawn_ep_north_west_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            {Direction::north, Direction::west}
    );

    // If the square north west of the pawn is within the boundaries of the
    // board and is the en passant square, generate the en passant move.
    if (dest_sq != Square::none && dest_sq == en_passant_square)
    {
        return create_en_passant_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}


// Generates the pawn south-west en passant move using the origin square.
// Returns Move::none if the move would not be pseudo-legal.
Move Game::pawn_ep_south_west_move(const Square origin_sq) const
{
    const auto dest_sq = find_dest_square(
            origin_sq,
            {Direction::south, Direction::west}
    );

    // If the square south west of the pawn is within the boundaries of the
    // board and is the en passant square, generate the en passant move.
    if (dest_sq != Square::none && dest_sq == en_passant_square)
    {
        return create_en_passant_move(origin_sq, dest_sq);
    }
    else
    {
        return Move::none;
    }
}


// Generates the 4 north-by-1 promotion moves using the origin square.
// Returns Move::none if the moves would not be pseudo-legal.
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


// Generates the 4 south-by-1 promotion moves using the origin square.
// Returns Move::none if the moves would not be pseudo-legal.
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


// Generates the 4 north-east promotion capture moves using the origin
// square. Returns Move::none if the moves would not be pseudo-legal.
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


// Generates the 4 south-east promotion capture moves using the origin
// square. Returns Move::none if the moves would not be pseudo-legal.
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


// Generates the 4 north-west promotion capture moves using the origin
// square. Returns Move::none if the moves would not be pseudo-legal.
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


// Generates the 4 south-west promotion capture moves using the origin
// square. Returns Move::none if the moves would not be pseudo-legal.
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


// Generates the white kingside castling move as long as it has not been
// invalidated and no pieces are blocking it.
Move Game::white_kingside_castle_move(const Square origin_sq) const
{
    // Make sure castling has not been invalidated and no pieces are blocking
    // it.
    if (!w_kingside_castling_invalidated() && !is_occupied(Square::F1) &&
        !is_occupied(Square::G1))
    {
        return create_castling_move(origin_sq, Square::G1);
    }
    else
    {
        return Move::none;
    }
}


// Generates the white queenside castling move as long as it has not been
// invalidated and no pieces are blocking it.
Move Game::white_queenside_castle_move(const Square origin_sq) const
{
    // Make sure castling has not been invalidated and no pieces are blocking
    // it.
    if (!w_queenside_castling_invalidated() && !is_occupied(Square::D1) &&
        !is_occupied(Square::C1) && !is_occupied(Square::B1))
    {
        return create_castling_move(origin_sq, Square::C1);
    }
    else
    {
        return Move::none;
    }
}


// Generates the black kingside castling move as long as it has not been
// invalidated and no pieces are blocking it.
Move Game::black_kingside_castle_move(const Square origin_sq) const
{
    // Make sure castling has not been invalidated and no pieces are blocking
    // it.
    if (!b_kingside_castling_invalidated() && !is_occupied(Square::F8) &&
        !is_occupied(Square::G8))
    {
        return create_castling_move(origin_sq, Square::G8);
    }
    else
    {
        return Move::none;
    }
}


// Generates the black queenside castling move as long as it has not been
// invalidated and no pieces are blocking it.
Move Game::black_queenside_castle_move(const Square origin_sq) const
{
    // Make sure castling has not been invalidated and no pieces are blocking
    // it.
    if (!b_queenside_castling_invalidated() && !is_occupied(Square::D8) &&
        !is_occupied(Square::C8) && !is_occupied(Square::B8))
    {
        return create_castling_move(origin_sq, Square::C8);
    }
    else
    {
        return Move::none;
    }
}


// Generates all pseudo-legal moves for a white pawn that belongs to the
// player to move this turn.
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
    possible_moves.erase(
            std::remove(
                    possible_moves.begin(),
                    possible_moves.end(),
                    Move::none
            ),
            possible_moves.end()
    );

    return possible_moves;
}


// Generates all pseudo-legal moves for a black pawn that belongs to the
// player to move this turn.
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
    possible_moves.erase(
            std::remove(
                    possible_moves.begin(),
                    possible_moves.end(),
                    Move::none
            ),
            possible_moves.end()
    );

    return possible_moves;
}


// Generates all pseudo-legal knight moves for a knight that belongs to the
// player to move this turn.
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
    possible_moves.erase(
            std::remove(
                    possible_moves.begin(),
                    possible_moves.end(),
                    Move::none
            ),
            possible_moves.end()
    );

    return possible_moves;
}


// Generates all pseudo-legal bishop moves for a bishop that belongs to the
// player to move this turn.
std::vector<Move> Game::pseudo_legal_bishop_moves(const Square square) const
{
    // Use magic bitboards to generate the attack bitboard.
    Bitboard attack_bitboard = Bmagic(
            static_cast<unsigned>(square),
            all_bitboard
    );

    // Discard self-captures.
    attack_bitboard = discard_self_captures(attack_bitboard);

    return gen_moves_from_bitboard(square, attack_bitboard);
}


// Generates all pseudo-legal rook moves for a rook that belongs to the
// player to move this turn. Castling does not count as a rook move.
std::vector<Move> Game::pseudo_legal_rook_moves(const Square square) const
{
    // Use magic bitboards to generate the attack bitboard.
    Bitboard attack_bitboard = Rmagic(
            static_cast<unsigned>(square),
            all_bitboard
    );

    // Discard self-captures.
    attack_bitboard = discard_self_captures(attack_bitboard);

    return gen_moves_from_bitboard(square, attack_bitboard);
}


// Generates all pseudo-legal queen moves for a queen that belongs to the
// player to move this turn.
std::vector<Move> Game::pseudo_legal_queen_moves(const Square square) const
{
    // Use magic bitboards to generate the attack bitboard.
    Bitboard bishop_attack_bitboard = Bmagic(
            static_cast<unsigned>(square),
            all_bitboard
    );

    Bitboard rook_attack_bitboard = Rmagic(
            static_cast<unsigned>(square),
            all_bitboard
    );

    auto attack_bitboard = bishop_attack_bitboard | rook_attack_bitboard;

    // Discard self-captures.
    attack_bitboard = discard_self_captures(attack_bitboard);

    return gen_moves_from_bitboard(square, attack_bitboard);
}


// Generates all pseudo-legal king moves for the king that belongs to the
// player to move this turn.
std::vector<Move> Game::pseudo_legal_king_moves(const Square square) const
{
    std::vector<Move> possible_moves;

    // Normal moves

    // North
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::north}
    ));

    // North East
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::north, Direction::east}
    ));

    // East
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::east}
    ));

    // South East
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::south, Direction::east}
    ));

    // South
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::south}
    ));

    // South West
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::south, Direction::west}
    ));

    // West
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::west}
    ));

    // North West
    possible_moves.push_back(pseudo_legal_normal_move(
            square,
            {Direction::north, Direction::west}
    ));

    // Castling moves

    if (turn == Color::white)
    {
        possible_moves.push_back(white_kingside_castle_move(square));
        possible_moves.push_back(white_queenside_castle_move(square));
    }
    else
    {
        possible_moves.push_back(black_kingside_castle_move(square));
        possible_moves.push_back(black_queenside_castle_move(square));
    }

    // Remove invalid moves.
    possible_moves.erase(
            std::remove(
                    possible_moves.begin(),
                    possible_moves.end(),
                    Move::none
            ),
            possible_moves.end()
    );

    return possible_moves;
}
