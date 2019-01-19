#ifndef DISCORD_CHESS_BOT_GAME_H
#define DISCORD_CHESS_BOT_GAME_H
#include <array>
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
    std::array<Bitstring, 64> w_pawn_bitstrings;
    std::array<Bitstring, 64> w_knight_bitstrings;
    std::array<Bitstring, 64> w_bishop_bitstrings;
    std::array<Bitstring, 64> w_rook_bitstrings;
    std::array<Bitstring, 64> w_queen_bitstrings;
    std::array<Bitstring, 64> w_king_bitstrings;

    std::array<Bitstring, 64> b_pawn_bitstrings;
    std::array<Bitstring, 64> b_knight_bitstrings;
    std::array<Bitstring, 64> b_bishop_bitstrings;
    std::array<Bitstring, 64> b_rook_bitstrings;
    std::array<Bitstring, 64> b_queen_bitstrings;
    std::array<Bitstring, 64> b_king_bitstrings;

    // Bitstrings for the side to move
    Bitstring white_bitstring;
    Bitstring black_bitstring;

    // Bitstrings for each possible combination of castling rights.
    // 2 possible values ^ 4 castling rights = 16 combinations
    std::array<Bitstring, 16> castling_bitstrings;

    // Bitstrings for the en passant squares.
    std::array<Bitstring, 64> en_passant_bitstrings;

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

    
    std::array<Piece, 64> pieces_on_board =
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
    void update_castling_rights(const Square origin_sq, const Square dest_sq);

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
    Bitboard &get_piece_bitboard(const Piece piece);
    
    // Returns a reference to the specified color's bitboard.
    Bitboard &get_color_bitboard(const Color color);
    
    // Adds a piece to the board.
    void add_piece(const Piece piece, const Square square);

    // Adds a piece to the board.
    void remove_piece(const Piece piece, const Square square);

    // Gets the type of piece on a certain square.
    Piece piece_on(const Square square) const;
    
    // Checks if the specified square is occupied.
    bool is_occupied(const Square square) const;
    
    // Checks if the specified square is occupied by a certain colour.
    bool is_occupied(const Square square, const Color color) const;

    // Undoes the last move.
    void undo();

    // Generates all pseudo-legal moves for the current player.
    std::vector<Move> pseudo_legal_moves() const;
    
    // Generates all pseudo-legal moves for White.
    std::vector<Move> pseudo_legal_w_moves() const;
    
    // Generates all pseudo-legal moves for Black.
    std::vector<Move> pseudo_legal_b_moves() const;
    
    // If the destination square is invalid or is occupied by a friendly
    // piece, this returns Move::none. Otherwise, it returns a normal move
    // with the origin and destination squares set. Used with
    // find_dest_square() to generate simple moves.
    Move pseudo_legal_normal_move(
            const Square origin_sq,
            const Square dest_sq
    ) const;
    
    // Generates a normal move from the origin square to wherever the
    // directions lead to if that is a square within the boundaries of the
    // board and is not occupied by any friendly pieces.
    Move pseudo_legal_normal_move(
            const Square origin_sq,
            const std::vector<Direction> &directions
    ) const;
    
    // Generates the pawn north-by-1 move using the origin square. Returns
    // Move::none if the move would not be pseudo-legal.
    Move pawn_north_move(const Square origin_sq) const;
    
    // Generates the pawn south-by-1 move using the origin square. Returns
    // Move::none if the move would not be pseudo-legal.
    Move pawn_south_move(const Square origin_sq) const;
    
    // Generates the pawn north-by-2 move using the origin square. Returns
    // Move::none if the move would not be pseudo-legal.
    Move pawn_north_north_move(const Square origin_sq) const;
    
    // Generates the pawn south-by-2 move using the origin square. Returns
    // Move::none if the move would not be pseudo-legal.
    Move pawn_south_south_move(const Square origin_sq) const;
    
    // Generates the pawn north-east capture move using the origin square.
    // Returns Move::none if the move would not be pseudo-legal.
    Move pawn_north_east_move(const Square origin_sq) const;
    
    // Generates the pawn south-east capture move using the origin square.
    // Returns Move::none if the move would not be pseudo-legal.
    Move pawn_south_east_move(const Square origin_sq) const;
    
    // Generates the pawn north-west capture move using the origin square.
    // Returns Move::none if the move would not be pseudo-legal.
    Move pawn_north_west_move(const Square origin_sq) const;
    
    // Generates the pawn south-west capture move using the origin square.
    // Returns Move::none if the move would not be pseudo-legal.
    Move pawn_south_west_move(const Square origin_sq) const;
    
    // Generates the pawn north-east en passant move using the origin square.
    // Returns Move::none if the move would not be pseudo-legal.
    Move pawn_ep_north_east_move(const Square origin_sq) const;
    
    // Generates the pawn south-east en passant move using the origin square.
    // Returns Move::none if the move would not be pseudo-legal.
    Move pawn_ep_south_east_move(const Square origin_sq) const;
    
    // Generates the pawn north-west en passant move using the origin square.
    // Returns Move::none if the move would not be pseudo-legal.
    Move pawn_ep_north_west_move(const Square origin_sq) const;
    
    // Generates the pawn south-west en passant move using the origin square.
    // Returns Move::none if the move would not be pseudo-legal.
    Move pawn_ep_south_west_move(const Square origin_sq) const;
    
    // Generates the 4 north-by-1 promotion moves using the origin square.
    // Returns Move::none if the moves would not be pseudo-legal.
    std::array<Move, 4> pawn_promo_north_moves(const Square origin_sq) const;
    
    // Generates the 4 south-by-1 promotion moves using the origin square.
    // Returns Move::none if the moves would not be pseudo-legal.
    std::array<Move, 4> pawn_promo_south_moves(const Square origin_sq) const;
    
    // Generates the 4 north-east promotion capture moves using the origin
    // square. Returns Move::none if the moves would not be pseudo-legal.
    std::array<Move, 4> pawn_promo_north_east_moves(const Square origin_sq) const;
    
    // Generates the 4 south-east promotion capture moves using the origin
    // square. Returns Move::none if the moves would not be pseudo-legal.
    std::array<Move, 4> pawn_promo_south_east_moves(const Square origin_sq) const;
    
    // Generates the 4 north-west promotion capture moves using the origin
    // square. Returns Move::none if the moves would not be pseudo-legal.
    std::array<Move, 4> pawn_promo_north_west_moves(const Square origin_sq) const;
    
    // Generates the 4 south-west promotion capture moves using the origin
    // square. Returns Move::none if the moves would not be pseudo-legal.
    std::array<Move, 4> pawn_promo_south_west_moves(const Square origin_sq) const;

    // Generates all pseudo-legal moves for a white pawn that belongs to the
    // player to move this turn.
    std::vector<Move> pseudo_legal_w_pawn_moves(const Square square) const;
    
    // Generates all pseudo-legal moves for a black pawn that belongs to the
    // player to move this turn.
    std::vector<Move> pseudo_legal_b_pawn_moves(const Square square) const;

    // Generates all pseudo-legal knight moves for a knight that belongs to the
    // player to move this turn.
    std::vector<Move> pseudo_legal_knight_moves(const Square square) const;

    // Generates all pseudo-legal bishop moves for a bishop that belongs to the
    // player to move this turn.
    std::vector<Move> pseudo_legal_bishop_moves(const Square square) const;

    // Generates all pseudo-legal rook moves for a rook that belongs to the
    // player to move this turn. Castling does not count as a rook move.
    std::vector<Move> pseudo_legal_rook_moves(const Square square) const;

    // Generates all pseudo-legal queen moves for a queen that belongs to the
    // player to move this turn.
    std::vector<Move> pseudo_legal_queen_moves(const Square square) const;

    // Generates all pseudo-legal king moves for the king that belongs to the
    // player to move this turn.
    std::vector<Move> pseudo_legal_king_moves() const;

    // The recursive function that returns the best evaluation found for a
    // ply. It utilizes minimax with alpha-beta pruning. This will not be
    // used for the root ply.
    int minimax(int depth, int alpha, int beta, bool is_maximizing);

    // Uses piece-square tables to evaluate a square.
    int eval_square(const Square square) const;

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
    bool make_move(const Move move);

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
    Move string_to_move(const std::string string) const;

    // Converts a move to its string representation.
    std::string move_to_string(const Move move) const;

    // Returns the FEN representation of the board.
    std::string fen() const;

    // Checks if the game has ended, and if so, why.
    Game_state game_state();
    
    // Using a list of possible moves, checks if the game has ended, and if
    // so, why.
    Game_state game_state(const std::vector<Move> &possible_moves);
};

#endif  //DISCORD_CHESS_BOT_GAME_H
