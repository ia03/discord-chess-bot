#ifndef DISCORD_CHESS_BOT_GAME_H
#define DISCORD_CHESS_BOT_GAME_H
#include <vector>
#include <unordered_map>
#include <string>
#include "types.h"


// Represents a chess game
class Game
{
private:
    // Store data of previous plies to undo moves
    std::vector<Ply_data> history;
    
    // Keep track of the occurrences of each hash to detect threefold
    // repetition.
    std::unordered_map<Bitstring, int> hash_count;

    // Zobrist hash for the piece positions only
    Bitstring position_hash;

    // Bitstrings for each square/piece combination
    Bitstring w_pawn_bitstrings [64];
    Bitstring w_knight_bitstrings [64];
    Bitstring w_bishop_bitstrings [64];
    Bitstring w_rook_bitstrings [64];
    Bitstring w_queen_bitstrings [64];
    Bitstring w_king_bitstrings [64];

    Bitstring b_pawn_bitstrings [64];
    Bitstring b_knight_bitstrings [64];
    Bitstring b_bishop_bitstrings [64];
    Bitstring b_rook_bitstrings [64];
    Bitstring b_queen_bitstrings [64];
    Bitstring b_king_bitstrings [64];

    // Bitstrings for the side to move
    Bitstring white_bitstring;
    Bitstring black_bitstring;

    // Bitstrings for each possible combination of castling rights.
    // 2 possible values ^ 4 castling rights = 16 combinations
    Bitstring castling_bitstrings [16];

    // Bitstrings for the en passant squares.
    Bitstring en_passant_bitstrings [64];

    // Bitboards - White
    Bitboard w_pawn_bitboard   = 0xFF00;
    Bitboard w_knight_bitboard = 0x0042;
    Bitboard w_bishop_bitboard = 0x0024;
    Bitboard w_rook_bitboard   = 0x0081;
    Bitboard w_queen_bitboard  = 0x0008;
    Bitboard w_king_bitboard   = 0x0010;

    // White occupancy bitboard
    Bitboard white_bitboard    = w_pawn_bitboard |
                                 w_knight_bitboard |
                                 w_bishop_bitboard |
                                 w_rook_bitboard |
                                 w_queen_bitboard |
                                 w_king_bitboard;

    // Bitboards - Black
    Bitboard b_pawn_bitboard   = 0x00FF000000000000;
    Bitboard b_knight_bitboard = 0x4200000000000000;
    Bitboard b_bishop_bitboard = 0x2400000000000000;
    Bitboard b_rook_bitboard   = 0x8100000000000000;
    Bitboard b_queen_bitboard  = 0x0800000000000000;
    Bitboard b_king_bitboard   = 0x1000000000000000;

    // Black occupancy bitboard
    Bitboard black_bitboard    = b_pawn_bitboard |
                                 b_knight_bitboard |
                                 b_bishop_bitboard |
                                 b_rook_bitboard |
                                 b_queen_bitboard |
                                 b_king_bitboard;

    // General occupancy bitboard
    Bitboard all_bitboard = white_bitboard | black_bitboard;

    
    Piece pieces_on_board [64] =
    {
        Piece::w_rook, Piece::w_knight, Piece::w_bishop, Piece::w_queen, Piece::w_king, Piece::w_bishop, Piece::w_knight, Piece::w_rook,
        Piece::w_pawn,   Piece::w_pawn,   Piece::w_pawn,  Piece::w_pawn, Piece::w_pawn,   Piece::w_pawn,   Piece::w_pawn, Piece::w_pawn,
          Piece::none,     Piece::none,     Piece::none,    Piece::none,   Piece::none,     Piece::none,     Piece::none,   Piece::none,
          Piece::none,     Piece::none,     Piece::none,    Piece::none,   Piece::none,     Piece::none,     Piece::none,   Piece::none,
          Piece::none,     Piece::none,     Piece::none,    Piece::none,   Piece::none,     Piece::none,     Piece::none,   Piece::none,
          Piece::none,     Piece::none,     Piece::none,    Piece::none,   Piece::none,     Piece::none,     Piece::none,   Piece::none,
        Piece::b_pawn,   Piece::b_pawn,   Piece::b_pawn,  Piece::b_pawn, Piece::b_pawn,   Piece::b_pawn,   Piece::b_pawn, Piece::b_pawn,
        Piece::b_rook, Piece::b_knight, Piece::b_bishop, Piece::w_queen, Piece::b_king, Piece::b_bishop, Piece::b_knight, Piece::b_rook
    };

    // The square a pawn would end up if it performed en passant.
    // If the last move was not a 2-square pawn move, the value of this is none.
    Square en_passant_square = Square::none;

    // Number of plies that have elapsed since a pawn was moved or a piece was
    // captured. Used for the 50-move rule.
    int rule50 = 0;
    
    // Is true when threefold repetition has been reached.
    bool threefold_repetition = false;

    // Only the least significant 4 bits are used.
    // Bit 0: white kingside castle
    // Bit 1: white queenside castle
    // Bit 2: black kingside castle
    // Bit 3: black queenside castle
    // Bits that are turned on represent castles that have not yet been
    // permanently invalidated. This is initially set to 15 such that all 4
    // bits are on.
    Castling_right castling_rights = Castling_right::all_castling;

    Color turn = Color::white;

    // Initializes the random bitstrings require for Zobrist hashing.
    void init_zobrist();

    // Initializes the Zobrist hash for the piece positions.
    void init_hash();

    // Returns the bitstring for a square.
    Bitstring hash_square(Square square) const;

    // Ends the current turn.
    void end_turn();

    // Generates the Zobrist key for the current position.
    Bitstring hash() const;
    
    // Checks the origin and destination squares of a move and updates
    // the castling rights accordingly.
    void update_castling_rights(Square origin_sq, Square dest_sq);

    // Invalidates white kingside castling.
    void invalidate_w_kingside_castling();
    
    // Invalidates white queenside castling.
    void invalidate_w_queenside_castling();
    
    // Invalidates white castling.
    void invalidate_white_castling();
    
    // Invalidates black kingside castling.
    void invalidate_b_kingside_castling();
    
    // Invalidates black queenside castling.
    void invalidate_b_queenside_castling();
    
    // Invalidates black castling.
    void invalidate_black_castling();
    
    // Returns a reference to the specified piece type's bitboard.
    Bitboard &get_piece_bitboard(Piece piece);
    
    // Returns a reference to the specified color's bitboard.
    Bitboard &get_color_bitboard(Color color);
    
    // Adds a piece to the board.
    void add_piece(Piece piece, Square square);

    // Adds a piece to the board.
    void remove_piece(Piece piece, Square square);

    // Gets the type of piece on a certain square.
    Piece piece_on(Square square) const;

    // Undoes the last move.
    void undo();

    // Generates all pseudo-legal moves for the current player.
    std::vector<Move> pseudo_legal_moves() const;

    // Generates all pseudo-legal moves for a white pawn that belongs to the
    // player to move this turn.
    std::vector<Move> pseudo_legal_w_pawn_moves(Square square) const;
    
    // Generates all pseudo-legal moves for a black pawn that belongs to the
    // player to move this turn.
    std::vector<Move> pseudo_legal_b_pawn_moves(Square square) const;

    // Generates all pseudo-legal knight moves for a knight that belongs to the
    // player to move this turn.
    std::vector<Move> pseudo_legal_knight_moves(Square square) const;

    // Generates all pseudo-legal bishop moves for a bishop that belongs to the
    // player to move this turn.
    std::vector<Move> pseudo_legal_bishop_moves(Square square) const;

    // Generates all pseudo-legal rook moves for a rook that belongs to the
    // player to move this turn. Castling does not count as a rook move.
    std::vector<Move> pseudo_legal_rook_moves(Square square) const;

    // Generates all pseudo-legal queen moves for a queen that belongs to the
    // player to move this turn.
    std::vector<Move> pseudo_legal_queen_moves(Square square) const;

    // Generates all pseudo-legal king moves for the king that belongs to the
    // player to move this turn.
    std::vector<Move> pseudo_legal_king_moves() const;

    // The recursive function that returns the best evaluation found for a
    // ply. It utilizes minimax with alpha-beta pruning. This will not be
    // used for the root ply.
    int minimax(int depth, int alpha, int beta, bool is_maximizing);

    // Uses piece-square tables to evaluate a square.
    int eval_square(Square square) const;

    // Uses piece-square tables to evaluate the board in its current state.
    int evaluate() const;

    // Checks if there are not enough pieces on the board for a checkmate
    // to be possible.
    bool insufficient_material() const;

    // Checks if the specified square is under attack by a specified player.
    bool square_attacked(Square square, Color attacker) const;
    
    // Checks if the specified player's king is in check.
    bool king_in_check(Color color) const;
public:
    // Initializes Zobrist hashing.
    Game();

    // Gets the color of the player who is to play this turn.
    const Color get_turn() const;

    // Makes a move and saves the ply data required to undo that move if it is
    // legal. If the move is illegal, it undoes the move after making it and
    // returns false. The move is assumed to be pseudo-legal.
    bool make_move(Move move);

    // Checks if a move is pseudo-legal.
    bool is_pseudo_legal(Move move);

    // Search function used for the root ply. It uses minimax and alpha-beta
    // pruning to return the best legal move for the current position.
    Move best_move();

    // Generates and returns a move using a string. The first two characters
    // indicate the starting position, the two characters after that indicate
    // the ending position. The fifth optional character indicates the
    // promotion piece. Returns Move::none if the string is invalid.
    // Examples: "b5f8" or "f7F8q"
    Move string_to_move(std::string string) const;

    // Converts a move to its string representation.
    std::string move_to_string(Move move) const;

    // Returns the FEN representation of the board.
    std::string fen() const;

    // Checks if the game has ended, and if so, why.
    Game_state game_state();
    
    // Using a list of possible moves, checks if the game has ended, and if
    // so, why.
    Game_state game_state(std::vector<Move> possible_moves);
};

#endif  //DISCORD_CHESS_BOT_GAME_H
