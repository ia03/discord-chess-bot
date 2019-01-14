#include "types.h"
#include "utils.h"
#include "game.h"
#include <map>
#include <string>
#include <cctype>


std::map<Piece, std::string> piece_fen =
{
    {Piece::w_pawn,   "W"},
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

Bitboard &Game::get_piece_bitboard(Piece piece)
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

Bitboard &Game::get_color_bitboard(Color color)
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

void Game::add_piece(Piece piece, Square square)
{
    if (piece == Piece::none)
    {
        return;
    }
    
    Bitboard piece_position = 1 << (int)square;
    Bitboard &piece_bitboard = get_piece_bitboard(piece);
    Bitboard &color_bitboard = get_color_bitboard(piece_color(piece));

    // Update bitboards.
    piece_bitboard |= piece_position;
    color_bitboard |= piece_position;
    all_bitboard |= piece_position;
    
    // Update the piece array.
    pieces_on_board[(int)square] = piece;
    
    // Update the position hash.
    position_hash ^= hash_square(square);
}

void Game::remove_piece(Piece piece, Square square)
{
    if (piece == Piece::none)
    {
        return;
    }
    
    // Update the position hash.
    position_hash ^= hash_square(square);
    
    Bitboard piece_position = 1 << (int)square;
    Bitboard &piece_bitboard = get_piece_bitboard(piece);
    Bitboard &color_bitboard = get_color_bitboard(piece_color(piece));
    
    // Update bitboards.
    piece_bitboard &= ~piece_position;
    color_bitboard &= ~piece_position;
    all_bitboard &= ~piece_position;
    
    // Update the piece array.
    pieces_on_board[(int)square] = Piece::none;
}

Piece Game::piece_on(Square square)
{
    return pieces_on_board[(int)square];
}

bool Game::insufficient_material()
{
    // Use std::count()?
    std::map<Piece, std::vector<Square>> squares_of_piece;

    // Get a list of all the squares each piece type occupies.
    for (int i = 0; i < 64; i++)
    {
        Piece piece = piece_on((Square)i);

        // If any pawns, rooks, or queens exist on the board, we know a
        // checkmate is possible.
        switch (piece)
        {
            case Piece::w_pawn:
            case Piece::b_pawn:
            case Piece::w_rook:
            case Piece::b_rook:
            case Piece::w_queen:
            case Piece::b_queen:
                return false;
            default:
                break;
        }

        squares_of_piece[piece].push_back((Square)i);
    }
    
    
    if  (squares_of_piece.find(Piece::w_bishop) != squares_of_piece.end())
    {
        vector<Square> w_bishop_squares = squares_of_piece[Piece::w_bishop];
        if (squares_of_piece[Piece::w_bishop].size() > 1)
        {
            
        }
    }
    
}

std::string Game::fen()
{
    std::string fen_str;
    
    // Generate an FEN string for each row.
    for (int i = 0; i < 8; i++)
    {
        // Go through each square in this row.
        for (int position = i * 8; position < 8; position++)
        {
            Piece piece = piece_on((Square)position);
            if (piece != Piece::none)
            {
                fen_str += piece_fen[piece];
            }
            else
            {
                char last_char = fen_str.back();
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
        fen_str += "/";
    }
    // Remove the slash at the end.
    fen_str.pop_back();

    return fen_str;
}