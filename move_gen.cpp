#include <vector>
#include "game.h"


std::vector<Move> Game::pseudo_legal_moves() const
{
    std::vector<Move> pawn_moves = pseudo_legal_pawn_moves();
    std::vector<Move> knight_moves = pseudo_legal_knight_moves();
    std::vector<Move> bishop_moves = pseudo_legal_bishop_moves();
    std::vector<Move> rook_moves = pseudo_legal_rook_moves();
    std::vector<Move> queen_moves = pseudo_legal_queen_moves();
    std::vector<Move> king_moves = pseudo_legal_king_moves();
    
    std::vector<Move> all_moves;
    all_moves.insert(all_moves.end(), pawn_moves.begin(), pawn_moves.end());
    all_moves.insert(all_moves.end(), knight_moves.begin(), knight_moves.end());
    all_moves.insert(all_moves.end(), bishop_moves.begin(), bishop_moves.end());
    all_moves.insert(all_moves.end(), rook_moves.begin(), rook_moves.end());
    all_moves.insert(all_moves.end(), queen_moves.begin(), queen_moves.end());
    all_moves.insert(all_moves.end(), king_moves.begin(), king_moves.end());
    
    return all_moves;
}