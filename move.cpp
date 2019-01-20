#include <algorithm>
#include "utils.h"
#include "types.h"
#include "game.h"


void Game::update_castling_rights(
        const Square origin_sq,
        const Square dest_sq
)
{
    // Invalidate a specific type of castling if a rook moves or is captured.
    // White kingside castling
    if (origin_sq == Square::H1 || dest_sq == Square::H1)
    {
        invalidate_w_kingside_castling();
    }
    // White queenside castling
    if (origin_sq == Square::A1 || dest_sq == Square::A1)
    {
        invalidate_w_queenside_castling();
    }
    // Black kingside castling
    if (origin_sq == Square::H8 || dest_sq == Square::H8)
    {
        invalidate_b_kingside_castling();
    }
    // Black queenside castling
    if (origin_sq == Square::A8 || dest_sq == Square::A8)
    {
        invalidate_b_queenside_castling();
    }

    // Invalidate castling for a player if their king moves.
    // White
    if (origin_sq == Square::E1)
    {
        invalidate_black_castling();
    }
    // Black
    else if (origin_sq == Square::E8)
    {
        invalidate_black_castling();
    }
}

bool Game::is_pseudo_legal(Move move) const
{
    std::vector<Move> moves = pseudo_legal_moves();
    
    // Check if the move was found in the list of generated pseudo-legal
    // moves.
    if (std::find(moves.begin(), moves.end(), move) != moves.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Game::make_move(const Move move)
{
    // Extract data from the move.
    const auto origin_sq = extract_origin_sq(move);
    const auto dest_sq = extract_dest_sq(move);
    const auto promo_piece = extract_promo_piece(move);
    const auto move_type = extract_move_type(move);

    const auto moved_piece = piece_on(origin_sq);
    const auto captured_piece = piece_on(dest_sq);

    // Data needs to be saved to undo moves later.
    Ply_data ply_data;
    ply_data.last_move = move;
    ply_data.captured_piece = captured_piece;
    ply_data.castling_rights = castling_rights;
    ply_data.en_passant_square = en_passant_square;
    ply_data.rule50 = rule50;
    
    // Keep track of this hash's occurrence to be able to detect threefold
    // repetition.
    const auto current_game_hash = hash();

    if (hash_count.find(current_game_hash) == hash_count.end())
    {
        hash_count[current_game_hash] = 1;
    }
    else
    {
        hash_count[current_game_hash]++;
        // Check if threefold repetition has occurred.
        if (hash_count[current_game_hash] >= 3)
        {
            threefold_repetition = true;
        }
    }

    // Update the castling rights.
    update_castling_rights(origin_sq, dest_sq);
    
    rule50++;
    en_passant_square = Square::none;

    switch (move_type)
    {
        // Make a normal move (no castling, promotion, or en passant).
        case Move_type::normal:
            remove_piece(moved_piece, origin_sq);
            remove_piece(captured_piece, dest_sq);
            add_piece(moved_piece, dest_sq);

            // If the move was a two-square pawn move, set the en passant
            // square.
            if ((moved_piece == Piece::w_pawn) &&
                (dest_sq == north_of(north_of(origin_sq))))
            {
                en_passant_square = north_of(origin_sq);
            }
            // Two-square black pawn move.
            else if ((moved_piece == Piece::b_pawn) &&
                     (dest_sq == south_of(south_of(origin_sq))))
            {
                en_passant_square = south_of(origin_sq);
            }

            // If a piece was captured or a pawn was moved, reset the 50-move
            // rule variable.
            if (captured_piece != Piece::none ||
               (moved_piece == Piece::w_pawn || moved_piece == Piece::b_pawn))
            {
                rule50 = 0;
            }
            break;
        // Make a castling move.
        case Move_type::castling:
            Square rook_origin_sq;

            // This is also the square the king moves through when castling.
            Square rook_dest_sq;

            Piece rook_type;

            // Set the rook piece type and origin and destination squares.
            castle_rook_squares(
                    rook_type,
                    rook_origin_sq,
                    rook_dest_sq,
                    origin_sq,
                    dest_sq);

            remove_piece(moved_piece, origin_sq);
            add_piece(moved_piece, dest_sq);
            remove_piece(rook_type, rook_origin_sq);
            add_piece(rook_type, rook_dest_sq);

            // If the origin square or the square the king moves through is
            // under attack, this move is illegal.
            if (square_attacked(origin_sq, reverse_color(turn)) ||
                square_attacked(rook_dest_sq, reverse_color((turn))))
            {
                history.push_back(ply_data);
                end_turn();
                undo();
                return false;
            }
            break;
        // Make a promotion move.
        case Move_type::promotion:
            remove_piece(moved_piece, origin_sq);
            remove_piece(captured_piece, origin_sq);
            add_piece(promo_piece_to_piece(promo_piece, turn), dest_sq);

            
            rule50 = 0;
            break;
        // Make an en passant move.
        case Move_type::en_passant:
            // Move the friendly pawn and remove the enemy pawn.
            remove_piece(moved_piece, origin_sq);
            add_piece(moved_piece, dest_sq);
            Square enemy_pawn_sq;
            Piece enemy_pawn;

            find_enemy_pawn_ep(
                    enemy_pawn,
                    enemy_pawn_sq,
                    dest_sq,
                    turn
            );
            
            remove_piece(enemy_pawn, enemy_pawn_sq);

            rule50 = 0;
            break;
    }

    history.push_back(ply_data);
    end_turn();

    // If the player who made the move is now in check, this is an illegal
    // move.
    if (king_in_check(reverse_color(turn)))
    {
        undo();
        return false;
    }

    return true;
}

void Game::undo()
{
    end_turn();

    const Ply_data last_ply = history.back();
    history.pop_back();

    castling_rights = last_ply.castling_rights;
    en_passant_square = last_ply.en_passant_square;
    rule50 = last_ply.rule50;
    Piece captured_piece = last_ply.captured_piece;

    const Move move = last_ply.last_move;
    const Square origin_sq = extract_origin_sq(move);
    const Square dest_sq = extract_dest_sq(move);
    const Move_type move_type = extract_move_type(move);
    const Piece moved_piece = piece_on(dest_sq);

    switch (move_type)
    {
        // Undo a normal move.
        case Move_type::normal:
            remove_piece(moved_piece, dest_sq);
            add_piece(moved_piece, origin_sq);
            add_piece(captured_piece, dest_sq);
            break;
        // Undo a castling move.
        case Move_type::castling:
            Piece rook_type;
            Square rook_origin_sq;
            Square rook_dest_sq;
            
            // Set the rook piece type and origin and destination squares.
            castle_rook_squares(
                    rook_type,
                    rook_origin_sq,
                    rook_dest_sq,
                    origin_sq,
                    dest_sq
            );
            
            remove_piece(moved_piece, dest_sq);
            add_piece(moved_piece, origin_sq);
            remove_piece(rook_type, rook_dest_sq);
            add_piece(rook_type, rook_origin_sq);
            
            break;
        // Undo a promotion move.
        case Move_type::promotion:
            remove_piece(moved_piece, dest_sq);
            add_piece(captured_piece, dest_sq);
            Piece pawn_type;
            // White
            if (turn == Color::white)
            {
                pawn_type = Piece::w_pawn;
            }
            // Black
            else
            {
                pawn_type = Piece::b_pawn;
            }
            
            add_piece(pawn_type, origin_sq);
            break;
        // Undo an en passant move.
        case Move_type::en_passant:
            remove_piece(moved_piece, dest_sq);
            add_piece(moved_piece, origin_sq);
            
            Piece enemy_pawn;
            Square enemy_pawn_sq;
            
            find_enemy_pawn_ep(
                    enemy_pawn,
                    enemy_pawn_sq,
                    dest_sq,
                    turn
            );
            
            add_piece(enemy_pawn, enemy_pawn_sq);
            break;
    }

    // Treat this position as if it never happened for the purposes of
    // threefold repetition.
    hash_count[hash()]--;
    threefold_repetition = false;
}
