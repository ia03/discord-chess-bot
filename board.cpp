#include <map>
#include <string>
#include <cctype>
#include "types.h"
#include "utils.h"
#include "game.h"


std::map<Piece, std::string> piece_fen =
{
    {Piece::w_pawn,   "P"},
    {Piece::w_knight, "N"},
    {Piece::w_bishop, "B"},
    {Piece::w_rook,   "R"},
    {Piece::w_queen,  "Q"},
    {Piece::w_king,   "K"},
    {Piece::b_pawn,   "p"},
    {Piece::b_knight, "n"},
    {Piece::b_bishop, "b"},
    {Piece::b_rook,   "r"},
    {Piece::b_queen,  "q"},
    {Piece::b_king,   "k"}
};


// Returns a reference to the specified piece type's bitboard.
Bitboard &Game::get_piece_bitboard(const Piece piece)
{
    switch (piece)
    {
        case Piece::w_pawn:
            return w_pawn_bitboard;
        case Piece::w_knight:
            return w_knight_bitboard;
        case Piece::w_bishop:
            return w_bishop_bitboard;
        case Piece::w_rook:
            return w_rook_bitboard;
        case Piece::w_queen:
            return w_queen_bitboard;
        case Piece::w_king:
            return w_king_bitboard;

        case Piece::b_pawn:
            return b_pawn_bitboard;
        case Piece::b_knight:
            return b_knight_bitboard;
        case Piece::b_bishop:
            return b_bishop_bitboard;
        case Piece::b_rook:
            return b_rook_bitboard;
        case Piece::b_queen:
            return b_queen_bitboard;
        case Piece::b_king:
            return b_king_bitboard;
    }
}


// Returns a reference to the specified color's bitboard.
Bitboard &Game::get_color_bitboard(const Color color)
{
    if (color == Color::white)
    {
        return white_bitboard;
    }
    else
    {
        return black_bitboard;
    }
}


// Adds a piece to the specified square.
void Game::add_piece(const Piece piece, const Square square)
{
    if (piece == Piece::none)
    {
        return;
    }

    const Bitboard piece_position = square_to_bb(square);
    auto &piece_bitboard = get_piece_bitboard(piece);
    auto &color_bitboard = get_color_bitboard(piece_color(piece));

    // Update bitboards.
    piece_bitboard |= piece_position;
    color_bitboard |= piece_position;
    all_bitboard |= piece_position;

    // Update the piece array.
    pieces_on_board[static_cast<unsigned>(square)] = piece;

    // Update the position hash.
    position_hash ^= hash_square(square);

    // Update the evaluation.
    evaluation += eval_square(square);
}


// Removes a piece from the specified square.
void Game::remove_piece(const Piece piece, const Square square)
{
    if (piece == Piece::none)
    {
        return;
    }
    // Update the evaluation.
    evaluation -= eval_square(square);

    // Update the position hash.
    position_hash ^= hash_square(square);

    const Bitboard piece_position = square_to_bb(square);
    Bitboard &piece_bitboard = get_piece_bitboard(piece);
    Bitboard &color_bitboard = get_color_bitboard(piece_color(piece));

    // Update bitboards.
    piece_bitboard &= ~piece_position;
    color_bitboard &= ~piece_position;
    all_bitboard &= ~piece_position;

    // Update the piece array.
    pieces_on_board[static_cast<unsigned>(square)] = Piece::none;
}


// Gets the type of piece on a certain square.
Piece Game::piece_on(const Square square) const
{
    return pieces_on_board[static_cast<unsigned>(square)];
}


// Checks if there are not enough pieces on the board for a checkmate
// to be possible.
bool Game::insufficient_material() const
{
    // If any pawns, rooks, or queens exist on the board, we know a
    // checkmate is possible.
    if (w_pawn_bitboard != 0 || b_pawn_bitboard != 0 ||
        w_rook_bitboard != 0 || b_rook_bitboard != 0 ||
        w_queen_bitboard != 0 || b_queen_bitboard != 0)
    {
        return false;
    }

    // If a player has 2 bishops of different square colors, a checkmate is
    // possible.
    if (((w_bishop_bitboard & white_squares) != 0 &&
         (w_bishop_bitboard & black_squares) != 0) ||
        ((b_bishop_bitboard & white_squares) != 0 &&
         (b_bishop_bitboard & black_squares) != 0))
    {
        return false;
    }

    // If a player has 2 knights, a checkmate is possible.
    if (count_bits_set(w_knight_bitboard) > 1 ||
        count_bits_set(b_knight_bitboard) > 1)
    {
        return false;
    }

    // If a player has a knight and a bishop, a checkmate is possible.
    if ((w_knight_bitboard != 0 && w_bishop_bitboard != 0) ||
        (b_knight_bitboard != 0 && b_bishop_bitboard != 0))
    {
        return false;
    }

    // If none of the above conditions are met, we can assume that a checkmate
    // would not be possible.
    return true;
}


// Checks if the specified square is occupied.
bool Game::is_occupied(const Square square) const
{
    return (square_to_bb(square) & all_bitboard) != 0;
}


// Checks if the specified square is occupied by a piece of a certain
// color.
bool Game::is_occupied(const Square square, const Color color) const
{
    if (color == Color::white)
    {
        return (square_to_bb(square) & white_bitboard) != 0;
    }
    else
    {
        return (square_to_bb(square) & black_bitboard) != 0;
    }
}


// Using a list of possible moves, checks if the game has ended, and if
// so, why.
Game_state Game::game_state(const std::vector<Move> &possible_moves)
{
    bool legal_moves_exist = false;

    // Check if any legal moves exist.
    for (auto const move : possible_moves)
    {
        if(make_move(move))
        {
            undo();
            legal_moves_exist = true;
            break;
        }
    }

    // No legal moves for the current player means the game has ended in
    // either a checkmate or stalemate.
    if (!legal_moves_exist)
    {
        // If the current player's king is also in check, it is a checkmate.
        if (king_in_check(turn))
        {
            if (turn == Color::white)
            {
                return Game_state::checkmate_by_black;
            }
            else
            {
                return Game_state::checkmate_by_white;
            }
        }
        // If the current player's king is not in check, it is a stalemate.
        else
        {
            return Game_state::stalemate;
        }
    }

    // If the same position has occurred three times in the past, this is a
    // draw.
    if (history.back().threefold_repetition)
    {
        return Game_state::threefold_repetition;
    }

    // If 50 moves (100 plies) have been played with no pawn movements or
    // piece captures, this is a draw.
    if (rule50 >= 100)
    {
        return Game_state::fifty_move;
    }

    // If there is insufficient material to perform a checkmate using any
    // possible sequence of legal moves, this is a draw.
    if (insufficient_material())
    {
        return Game_state::insufficient_material;
    }

    // If none of the above criteria have been met, the game has not ended.
    return Game_state::in_progress;
}


// Checks if the game has ended, and if so, why.
Game_state Game::game_state()
{
    return game_state(pseudo_legal_moves());
}


// Checks if the specified square is under attack by a specified player.
bool Game::square_attacked(const Square square, const Color attacker)
{
    bool attacker_is_opponent = false;

    // If the attacker is not playing this turn, switch sides so that moves
    // can be generated for them.
    if (turn != attacker)
    {
        attacker_is_opponent = true;
        end_turn();
    }

    std::vector<Move> possible_moves = pseudo_legal_moves();

    // Go through each possible move for the attacker to check if the
    // specified square is under attack.
    for (auto const move : possible_moves)
    {
        if (extract_dest_sq(move) == square)
        {
            if (attacker_is_opponent)
            {
                end_turn();
            }
            return true;
        }
    }

    // Switch the sides back.
    if (attacker_is_opponent)
    {
        end_turn();
    }

    return false;
}


// Checks if the specified player's king is in check.
bool Game::king_in_check(const Color color)
{
    // Find the square the king is on.
    Square king_square;
    if (color == Color::white)
    {
        king_square = static_cast<Square>(set_bit_pos(w_king_bitboard));
    }
    else
    {
        king_square = static_cast<Square>(set_bit_pos(b_king_bitboard));
    }

    // Check if the square is under attack by the other player.
    return square_attacked(king_square, reverse_color(color));
}


// Returns the FEN representation of the board.
std::string Game::fen() const
{
    std::string fen_str;

    // Generate an FEN string for each row.
    for (auto row = 7; row >= 0; row--)
    {
        // Go through each square in this row.
        for (auto position = row * 8; position < ((row + 1) * 8); position++)
        {
            const Piece piece = piece_on(static_cast<Square>(position));
            // If a piece is on this square, append the FEN representation of
            // this piece to the string.
            if (piece != Piece::none)
            {
                fen_str += piece_fen[piece];
            }
            // If there is no piece on this square, a "1" should be added
            // instead, unless a number is already at the end of the string,
            // then that number should be incremented by 1.
            else
            {
                const char last_char = fen_str.back();
                // Add 1 to the last character if it is a number.
                if (isdigit(last_char))
                {
                    fen_str.back()++;
                }
                // Otherwise, add a "1" to the end of the string.
                else
                {
                    fen_str += "1";
                }
            }
        }
        // Add a slash to the end for each row.
        fen_str += "/";
    }
    // Remove the slash at the end.
    fen_str.pop_back();

    return fen_str;
}
