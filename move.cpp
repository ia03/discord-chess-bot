#include "utils.h"
#include "types.h"
#include "game.h"


bool Game::make_move(Move move)
{
    Square origin_sq = extract_origin_sq(move);
    Square dest_sq = extract_dest_sq(move);
    Promotion_piece promo_piece = extract_promo_piece(move);
    Move_type move_type = extract_move_type(move);

    Piece moved_piece = piece_on(origin_sq);
    Piece captured_piece = piece_on(dest_sq);

    // Data needs to be saved to undo moves and check for threefold repetition
    // later.
    Ply_data ply_data;
    ply_data.last_move = move;
    ply_data.captured_piece = captured_piece;
    ply_data.castling_rights = castling_rights;
    ply_data.en_passant_sq = en_passant_square;
    ply_data.rule50 = rule50;
    ply_data.hash = hash();


    // Invalidate a specific type of castling if a rook moves or is captured.
    // White kingside castling
    if (origin_sq == Square::H1 || dest_sq == Square::H1)
    {
        castling_rights &= ~Castling_right::w_kingside;
    }
    // White queenside castling
    if (origin_sq == Square::A1 || dest_sq == Square::A1)
    {
        castling_rights &= ~Castling_right::w_queenside;
    }
    // Black kingside castling
    if (origin_sq == Square::H8 || dest_sq == Square::H8)
    {
        castling_rights &= ~Castling_right::b_kingside;
    }
    // Black queenside castling
    if (origin_sq == Square::A8 || dest_sq == Square::A8)
    {
        castling_rights &= ~Castling_right::b_queenside;
    }

    // Invalidate castling for a player if their king moves.
    // White
    if (origin_sq == Square::E1)
    {
        castling_rights &= ~Castling_right::w_kingside;
        castling_rights &= ~Castling_right::w_queenside;
    }
    // Black
    else if (origin_sq == Square::E8)
    {
        castling_rights &= ~Castling_right::b_kingside;
        castling_rights &= ~Castling_right::b_queenside;
    }

    rule50++;
    en_passant_sq = Square::none;

    switch (move_type)
    {
        case Move_type::normal:
            remove_piece(moved_piece, origin_sq);
            remove_piece(moved_piece, dest_sq);
            add_piece(moved_piece, dest_sq);

            // If the move was a two-square pawn move, set the en passant
            // square.
            if ((moved_piece == Piece::w_pawn) &&
                (dest_sq == origin_sq + 16))
            {
                en_passant_square = origin_sq + 8;
            }
            else if ((moved_piece == Piece::b_pawn) &&
                     (dest_sq == origin_sq - 16))
            {
                en_passant_square = origin_sq - 8;
            }

            // If a piece was captured or a pawn was moved, reset the 50-move
            // rule variable.
            if (captured_piece != Piece::none ||
               (moved_piece == Piece::w_pawn || moved_piece == Piece::b_pawn))
            {
                rule50 = 0;
            }
            break;
        case Move_type::castling:
            Square rook_origin_sq;

            // This is also the square the king moves through when castling.
            Square rook_dest_sq;

            Piece rook_type;

            // White
            if (turn == Color::White)
            {
                rook_type = Piece::w_rook;
                // Kingside
                if (dest_sq == Square::G1)
                {
                    rook_origin_sq = Square::H1;
                    rook_dest_sq = Square::F1;
                }
                // Queenside
                else
                {
                    rook_origin_sq = Square::A1;
                    rook_dest_sq = Square::D1;
                }
            }
            // Black
            else
            {
                rook_type = Piece::b_rook;
                // Kingside
                if (dest_sq == Square::G8)
                {
                    rook_origin_sq = Square::H8;
                    rook_dest_sq = Square::F8;
                }
                // Queenside
                else
                {
                    rook_origin_sq = Square::A8;
                    rook_dest_sq = Square::D8;
                }
            }

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
        case Move_type::promotion:
            remove_piece(moved_piece, origin_sq);
            remove_piece(captured_piece, origin_sq);
            add_piece(promo_piece_to_piece(promo_piece, turn), dest_sq);

            rule50 = 0;
            break;
        case Move_type::en_passant:
            remove_piece(moved_piece, origin_sq);
            Square enemy_pawn_sq;
            Piece enemy_pawn;

            // If this is White's move, the captured pawn will be 1 square
            // south of the destination square. If this is Black's move, the
            // captured pawn will be 1 square north of the destination square.
            if (turn == Color::white)
            {
                enemy_pawn = Piece::b_pawn;
                enemy_pawn_sq = dest_sq - 8;
            }
            else
            {
                enemy_pawn = Piece::w_pawn;
                enemy_pawn_sq = dest_sq + 8;
            }
            remove_piece(enemy_pawn, enemy_pawn_sq)

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

    Ply_data last_ply = history.back();
    history.pop_back();

    castling_rights = last_ply.castling_rights;
    en_passant_square = last_ply.en_passant_square;
    rule50 = last_ply.rule50;
    Piece captured_piece = last_ply.captured_piece;

    Move move = last_ply.last_move;
    Square origin_sq = extract_origin_sq(move);
    Square dest_sq = extract_dest_sq(move);
    Move_type move_type = extract_move_type(move);
    Piece moved_piece = piece_on(dest_sq);

    switch (move_type)
    {
        case Move_type::normal:
            remove_piece(moved_piece, dest_sq);
            add_piece(moved_piece, origin_sq);
            add_piece(captured_piece, dest_sq);
            break;
        case Move_type::castling:
            break;
    }
}