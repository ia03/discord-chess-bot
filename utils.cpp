#include <vector>
#include <random>
#include "types.h"
#include "utils.h"


// Returns the opposite color. Returns none if the input is not white or
// black.
Color reverse_color(const Color color)
{
    // Return the opposite color.
    if (color == Color::white)
    {
        return Color::black;
    }
    else if (color == Color::black)
    {
        return Color::white;
    }
    else
    {
        return Color::none;
    }
}


// Creates a move.
Move create_move(
        const Square origin_sq,
        const Square dest_sq,
        const Promotion_piece promo_piece,
        const Move_type move_type
)
{
    // Set the values of the move.
    auto move = set_origin_sq(Move::none, origin_sq);
    move = set_dest_sq(move, dest_sq);
    move = set_promo_piece(move, promo_piece);
    move = set_move_type(move, move_type);
    
    return move;
}


// Creates a normal move (no castling, promotion, or en passant)
// from one square to another.
Move create_normal_move(const Square origin_sq, const Square dest_sq)
{
    return create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::none,
            Move_type::normal
    );
}

// Creates all the promotion moves from one square to another.
std::array<Move, 4> create_promo_moves(
        const Square origin_sq,
        const Square dest_sq
)
{
    std::array<Move, 4> moves;
    
    // Create the queen move.
    moves[0] = create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::queen,
            Move_type::promotion
    );
    
    // Create the rook move.
    moves[1] = create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::rook,
            Move_type::promotion
    );
    
    // Create the bishop move.
    moves[2] = create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::bishop,
            Move_type::promotion
    );
    
    // Create the knight move.
    moves[3] = create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::knight,
            Move_type::promotion
    );
    
    return moves;
}


// Creates an en passant move from one square to another.
Move create_en_passant_move(const Square origin_sq, const Square dest_sq)
{
    return create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::none,
            Move_type::en_passant
    );
}


// Creates a castling move from one square to another. The origin and
// destination squares are those of the king.
Move create_castling_move(const Square origin_sq, const Square dest_sq)
{
    return create_move(
            origin_sq,
            dest_sq,
            Promotion_piece::none,
            Move_type::castling
    );
}


// Sets the origin square.
Move set_origin_sq(const Move move, const Square origin_sq)
{
    // Origin square is bits 0-5.
    return static_cast<Move>(
            static_cast<unsigned int>(move) |
            static_cast<unsigned int>(origin_sq));
}


// Sets the destination square.
Move set_dest_sq(const Move move, const Square dest_sq)
{
    // Destination square is bits 6-11.
    return static_cast<Move>(
            static_cast<unsigned>(move) |
            (static_cast<unsigned>(dest_sq) << 6));
}


// Sets the promotion piece type.
Move set_promo_piece(const Move move, const Promotion_piece promo_piece)
{
    // Promotion piece flag is bits 12-13.
    return static_cast<Move>(
            static_cast<unsigned>(move) |
            static_cast<unsigned>(promo_piece));
}


// Sets the move type.
Move set_move_type(const Move move, const Move_type move_type)
{
    // Special move flag is bits 14-15.
    return static_cast<Move>(
            static_cast<unsigned>(move) |
            static_cast<unsigned>(move_type));
}


// Extracts the origin square.
Square extract_origin_sq(const Move move)
{
    // Origin square is bits 0-5.
    return static_cast<Square>(
            static_cast<unsigned>(move) & 0b0000000000111111);
}


// Extracts the destination square.
Square extract_dest_sq(const Move move)
{
    // Destination square is bits 6-11.
    return static_cast<Square>(
            ((static_cast<unsigned>(move) & 0b0000111111000000) >> 6));
}


// Extracts the promotion piece type.
Promotion_piece extract_promo_piece(const Move move)
{
    // Promotion piece flag is bits 12-13.
    return static_cast<Promotion_piece>(
            static_cast<unsigned>(move) & 0b0011000000000000);
}


// Extracts the move type.
Move_type extract_move_type(const Move move)
{
    // Special move flag is bits 14-15.
    return (Move_type)(
            static_cast<unsigned>(move) & 0b1100000000000000);
}


// Gets the color of a piece type.
Color piece_color(const Piece piece)
{
    switch (piece)
    {
        case Piece::w_pawn:
        case Piece::w_knight:
        case Piece::w_bishop:
        case Piece::w_rook:
        case Piece::w_queen:
        case Piece::w_king:
            return Color::white;

        case Piece::b_pawn:
        case Piece::b_knight:
        case Piece::b_bishop:
        case Piece::b_rook:
        case Piece::b_queen:
        case Piece::b_king:
            return Color::black;

        default:
            return Color::none;
    }
}


// Gets the type and position of the enemy pawn captured in an en passant
// move and passes them back using the first 2 arguments.
void find_enemy_pawn_ep(
        Piece &enemy_pawn_type,
        Square &enemy_pawn_square,
        const Square dest_sq,
        const Color turn
)
{
    // If this is White's move, the captured pawn will be 1 square
    // south of the destination square. If this is Black's move, the
    // captured pawn will be 1 square north of the destination square.
    if (turn == Color::white)
    {
        enemy_pawn_type = Piece::b_pawn;
        enemy_pawn_square = south_of(dest_sq);
    }
    else
    {
        enemy_pawn_type = Piece::w_pawn;
        enemy_pawn_square = north_of(dest_sq);
    }
}


// Gets the rook type and origin and destination squares based on the king
// origin and destination squares. This information is passed back using
// the first 3 arguments.
void castle_rook_squares(
        Piece &rook_type,
        Square &rook_origin_sq,
        Square &rook_dest_sq,
        const Square king_origin_sq,
        const Square king_dest_sq
)
{
    // White
    if (king_origin_sq == Square::E1)
    {
        rook_type = Piece::w_rook;
                
        // Kingside
        if (king_dest_sq == Square::G1)
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
        if (king_dest_sq == Square::G8)
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
}


// Converts a promotion piece to a piece.
Piece promo_piece_to_piece(
        const Promotion_piece promo_piece,
        const Color color
)
{
    if (color == Color::white)
    {
        switch (promo_piece)
        {
            case Promotion_piece::queen:
                return Piece::w_queen;
            case Promotion_piece::rook:
                return Piece::w_rook;
            case Promotion_piece::bishop:
                return Piece::w_bishop;
            case Promotion_piece::knight:
                return Piece::w_knight;
        }
    }
    else
    {
        switch (promo_piece)
        {
            case Promotion_piece::queen:
                return Piece::b_queen;
            case Promotion_piece::rook:
                return Piece::b_rook;
            case Promotion_piece::bishop:
                return Piece::b_bishop;
            case Promotion_piece::knight:
                return Piece::b_knight;
        }
    }
    return Piece::none;
}


// Generates a random bitstring.
Bitstring rand_hash()
{
    // Use a Mersenne Twister pseudo-random generator to generate a random
    // 64-bit number.
    std::mt19937_64 generator (std::random_device{}());

    return generator();
}


// Generates a vector of moves using a bitboard.
std::vector<Move> gen_moves_from_bitboard(
        const Square origin_sq,
        Bitboard bitboard
)
{
    std::vector<Move> moves;
    
    // Set the origin square.
    const auto template_move = set_origin_sq(Move::none, origin_sq);

    // Get the positions of the set bits in the bitboard and use them to
    // create moves.
    for(auto position = 0; bitboard != 0; position++)
    {
        if ((bitboard & 1) == 1)
        {
            moves.push_back(set_dest_sq(
                    template_move,
                    static_cast<Square>(position)));
        }
        bitboard >>= 1;
    }
    
    return moves;
}


// Counts the number of set bits in a bitboard.
int count_bits_set(Bitboard bitboard)
{
    int i;
    
    // This uses Brian Kernighan's algorithm.
    // Clears the least significant set bit until all bits have been cleared.
    for (i = 0; bitboard != 0; i++)
    {
        bitboard &= bitboard - 1;  // Clear the least significant set bit.
    }
    return i;
}


// Finds the position of the only set bit on a bitboard.
int set_bit_pos(Bitboard bitboard)
{
    int position = 0;
    
    // Keep right shifting the bitboard by 1 bit until the first bit is set.
    while ((bitboard & 1) == 0)
    {
        bitboard >>= 1;
        position++;
    }
    
    return position;
}


// Converts a square to a bitboard with a single bit turned on.
Bitboard square_to_bb(const Square square)
{
    return static_cast<Bitboard>(1) << static_cast<Bitboard>(square);
}


// Returns the square north of the origin square.
Square north_of(const Square origin_sq)
{
    return static_cast<Square>(static_cast<unsigned>(origin_sq) + 8);
}


// Returns the square south of the origin square.
Square south_of(const Square origin_sq)
{
    return static_cast<Square>(static_cast<unsigned>(origin_sq) - 8);
}


// Returns the square east of the origin square.
Square east_of(const Square origin_sq)
{
    return static_cast<Square>(static_cast<unsigned>(origin_sq) + 1);
}


// Returns the square west of the origin square.
Square west_of(const Square origin_sq)
{
    return static_cast<Square>(static_cast<unsigned>(origin_sq) - 1);
}


// Determines if a square is on a bitboard.
bool on_bitboard(const Square square, const Bitboard bitboard)
{
    return on_bitboard(square_to_bb(square), bitboard);
}


// Determines if a bitboard has any set bits that also exist in another
// bitboard.
bool on_bitboard(const Bitboard bitboard1, const Bitboard bitboard2)
{
    return (bitboard1 & bitboard2) != 0;
}


// Find the destination square using the origin square and a list of
// directions. Returns Move::none if the destination square is outside
// the boundaries.
Square find_dest_square(
        const Square origin_sq,
        const std::vector<Direction> &directions
)
{
    auto dest_sq = origin_sq;
    const Bitboard origin_bb = square_to_bb(origin_sq);
    unsigned north_count = 0;
    unsigned east_count = 0;
    unsigned south_count = 0;
    unsigned west_count = 0;
    
    // Keep track of each directional move of the square, making sure it does
    // not exit the boundaries of the board. If it does not, then continue
    // moving it until all the requested moves have been made.
    for (const auto &direction : directions)
    {
        switch (direction)
        {
            case Direction::north:
                if (on_bitboard(origin_bb, rows[7 - north_count]))
                {
                    return Square::none;
                }
                dest_sq = north_of(dest_sq);
                north_count++;
                break;
            case Direction::east:
                if (on_bitboard(origin_bb, cols[7 - east_count]))
                {
                    return Square::none;
                }
                dest_sq = east_of(dest_sq);
                east_count++;
                break;
            case Direction::south:
                if (on_bitboard(origin_bb, rows[south_count]))
                {
                    return Square::none;
                }
                dest_sq = south_of(dest_sq);
                south_count++;
                break;
            case Direction::west:
                if (on_bitboard(origin_bb, cols[west_count]))
                {
                    return Square::none;
                }
                dest_sq = west_of(dest_sq);
                west_count++;
                break;
        }
    }
    
    return dest_sq;
}