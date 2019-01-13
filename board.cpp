#include "types.h"
#include "utils.h"
#include "game.h"


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
    Bitboard piece_position = 1 << (int)square;
    Bitboard &piece_bitboard = get_piece_bitboard(piece);
    Bitboard &color_bitboard = get_color_bitboard(piece_color(piece));

    piece_bitboard |= piece_position;
    color_bitboard |= piece_position;
    all_bitboard |= piece_position;
    
    pieces_on_board[(int)square] = piece;
    
    position_hash ^= hash_square(square);
}

void Game::remove_piece(Piece piece, Square square)
{
    position_hash ^= hash_square(square);
    
    Bitboard piece_position = 1 << (int)square;
    Bitboard &piece_bitboard = get_piece_bitboard(piece);
    Bitboard &color_bitboard = get_color_bitboard(piece_color(piece));
    
    piece_bitboard &= ~piece_position;
    color_bitboard &= ~piece_position;
    all_bitboard &= ~piece_position;
    
    pieces_on_board[(int)square] = Piece::none;
}

void Game::piece_on(Square square)
{
    return pieces_on_board[(int)square];
}