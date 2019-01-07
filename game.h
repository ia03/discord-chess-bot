/*
 * Ibrahim Ahmed
 * January 7, 2018
 * Game class definition
*/

#ifndef DISCORD_CHESS_BOT_GAME_H
#define DISCORD_CHESS_BOT_GAME_H
#include <vector>
#include <string>
#include "types.h"


// Represents a chess game
class Game
{
private:
    std::vector<Ply_data> history;

    // Zobrist hash for the piece positions only
    Bitstring position_hash;

    // Bitboards - White
    Bitboard w_pawn_bitboard;
    Bitboard w_knight_bitboard;
    Bitboard w_bishop_bitboard;
    Bitboard w_rook_bitboard;
    Bitboard w_queen_bitboard;
    Bitboard w_king_bitboard;

    // White occupancy bitboard
    Bitboard white_bitboard;

    // Bitboards - Black
    Bitboard b_pawn_bitboard;
    Bitboard b_knight_bitboard;
    Bitboard b_bishop_bitboard;
    Bitboard b_rook_bitboard;
    Bitboard b_queen_bitboard;
    Bitboard b_king_bitboard;

    // Black occupancy bitboard
    Bitboard black_bitboard;

    // General occupancy bitboard
    Bitboard all_bitboard;

    Piece pieces_on_board [64];
    Color colors_on_board [64];

    // The square a pawn would end up if it performed en passant.
    // If the last move was not a 2-square pawn move, the value of this is -1.
    Square en_passant_square;

    // Number of plies that have elapsed since a pawn was moved or a piece was
    // captured. Used for the 50-move rule.
    uint8_t rule50;

    // Only the least significant 4 bits are used.
    // Bit 0: white kingside castle
    // Bit 1: white queenside castle
    // Bit 2: black kingside castle
    // Bit 3: black queenside castle
    // Bits that are turned on represent castles that have not yet been
    // permanently invalidated. This is initially set to 15 such that all 4
    // bits are on.
    uint8_t castling_rights;

    Color turn;

    // Initializes the random bitstrings require for Zobrist hashing.
    void init_zobrist();

    // Initializes the Zobrist hash for the piece positions.
    void init_hash();

    // Generates the Zobrist key for the current position.
    Bitstring hash();

    // Invalidates a castling right.
    void invalidate_castle(Castle castle);

    // Adds a piece to the board.
    void add_piece(Piece piece, Square square);

    // Adds a piece to the board.
    void remove_piece(Piece piece, Square square);

    // Gets the type of piece on a certain square.
    Piece piece_on(Square square);

    // Gets the color of a piece on a certain square.
    Color color_of(Square square);

    // Ends the current turn.
    void end_turn();

    // Makes a move and saves the ply data required to undo that move if it is
    // legal. If the move is illegal, it undoes the move after making it and
    // returns false. The move is assumed to be pseudo-legal.
    bool make_move(Move move);

    // Undoes the last move.
    void undo();

    // Generates all pseudo-legal moves for the current player.
    std::vector<Move> pseudo_legal_moves();

    // Generates all pseudo-legal pawn moves for the current player.
    std::vector<Move> pseudo_legal_pawn_moves();

    // Generates all pseudo-legal knight moves for the current player.
    std::vector<Move> pseudo_legal_knight_moves();

    // Generates all pseudo-legal bishop moves for the current player.
    std::vector<Move> pseudo_legal_bishop_moves();

    // Generates all pseudo-legal rook moves for the current player. Castling
    // does not count as a rook move.
    std::vector<Move> pseudo_legal_rook_moves();

    // Generates all pseudo-legal queen moves for the current player.
    std::vector<Move> pseudo_legal_queen_moves();

    // Generates all pseudo-legal king moves for the current player.
    std::vector<Move> pseudo_legal_king_moves();

    // Checks if a move is pseudo-legal.
    bool is_pseudo_legal(Move move);

    // Search function used for the root ply. It uses minimax and alpha-beta
    // pruning to return the best legal move for the current position.
    Move best_move();

    // The recursive function that returns the best evaluation found for a
    // ply. It utilizes minimax with alpha-beta pruning. This will not be
    // used for the root ply.
    int minimax(int depth, int alpha, int beta, bool is_maximizing);
public:
    Game();

    void make_move(Move move);
};


extern unordered_map<std::string, Square> coord_to_index;
extern std::string index_to_coord [64];

extern unordered_map<std::string, Move> promo_str_to_bin;
extern unordered_map<Move, std::string> promo_bin_to_str;

#endif //DISCORD_CHESS_BOT_GAME_H
