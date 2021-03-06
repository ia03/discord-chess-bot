#include <array>
#include "types.h"
#include "utils.h"
#include "game.h"


// Piece-square tables - from http://www.chessbin.com/post/Piece-Square-Table.aspx

const std::array<int, 64> pawn_pst =
{
      0,   0,   0,   0,   0,   0,   0,   0,
    105, 110, 110,  75,  75, 110, 110, 105,
    105,  95,  90, 100, 100,  90,  95, 105,
    100, 100, 100, 125, 125, 100, 100, 100,
    105, 105, 110, 127, 127, 110, 105, 105,
    110, 110, 120, 130, 130, 120, 110, 110,
    150, 150, 150, 150, 150, 150, 150, 150,
      0,   0,   0,   0,   0,   0,   0,   0
};

const std::array<int, 64> knight_pst =
{
    250, 260, 280, 270, 270, 280, 260, 250,
    260, 280, 300, 305, 305, 300, 280, 260,
    270, 305, 310, 315, 315, 310, 305, 270,
    270, 300, 315, 320, 320, 315, 300, 270,
    270, 305, 315, 320, 320, 315, 305, 270,
    270, 300, 310, 315, 315, 310, 300, 270,
    260, 280, 300, 300, 300, 300, 280, 260,
    250, 260, 270, 270, 270, 270, 260, 250
};

const std::array<int, 64> bishop_pst =
{
    305, 315, 285, 315, 315, 285, 315, 305,
    315, 330, 325, 325, 325, 325, 330, 335,
    315, 335, 335, 335, 335, 335, 335, 315,
    315, 325, 335, 335, 335, 335, 325, 315,
    315, 330, 330, 335, 335, 330, 330, 315,
    315, 325, 330, 335, 335, 330, 325, 315,
    315, 325, 325, 325, 325, 325, 325, 315,
    305, 315, 315, 315, 315, 315, 315, 305
};

const std::array<int, 64> rook_pst =
{
    500, 500, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 500, 500, 500, 500
};

const std::array<int, 64> queen_pst =
{
    900, 900, 900, 900, 900, 900, 900, 900,
    900, 900, 900, 900, 900, 900, 900, 900,
    900, 900, 900, 900, 900, 900, 900, 900,
    900, 900, 900, 900, 900, 900, 900, 900,
    900, 900, 900, 900, 900, 900, 900, 900,
    900, 900, 900, 900, 900, 900, 900, 900,
    900, 900, 900, 900, 900, 900, 900, 900,
    900, 900, 900, 900, 900, 900, 900, 900
};

const std::array<int, 64> king_pst =
{
    20,   30,  10,   0,   0,  10,  30,  20,
    20,   20,   0,   0,   0,   0,  20,  20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    -20, -30, -30, -40, -40, -30, -30, -20,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30
};

// Used to flip the board for black pieces.
const std::array<int, 64> flip =
{
    56,  57,  58,  59,  60,  61,  62,  63,
    48,  49,  50,  51,  52,  53,  54,  55,
    40,  41,  42,  43,  44,  45,  46,  47,
    32,  33,  34,  35,  36,  37,  38,  39,
    24,  25,  26,  27,  28,  29,  30,  31,
    16,  17,  18,  19,  20,  21,  22,  23,
    8,   9,  10,  11,  12,  13,  14,  15,
    0,   1,   2,   3,   4,   5,   6,   7
};


 // Initializes the evaluation variable.
void Game::init_eval()
{
    evaluation = 0;

    // Add up the evaluation of all the pieces.
    for (auto square_index = 0; square_index < 64; square_index++)
    {
        evaluation += eval_square(static_cast<Square>(square_index));
    }
}


// Obtains the evaluation of the board in its current state.
int Game::evaluate() const
{
      return evaluation;
}


// Uses piece-square tables to evaluate a square.
int Game::eval_square(const Square square) const
{
    const Piece piece = piece_on(square);
    const Color color = piece_color(piece);
    const int *piece_square_table = nullptr;

    // Obtain the appropriate piece-square table.
    switch (piece)
    {
        case Piece::none:
            return 0;
        case Piece::w_pawn:
        case Piece::b_pawn:
            piece_square_table = pawn_pst.data();
            break;
        case Piece::w_knight:
        case Piece::b_knight:
            piece_square_table = knight_pst.data();
            break;
        case Piece::w_bishop:
        case Piece::b_bishop:
            piece_square_table = bishop_pst.data();
            break;
        case Piece::w_rook:
        case Piece::b_rook:
            piece_square_table = rook_pst.data();
            break;
        case Piece::w_queen:
        case Piece::b_queen:
            piece_square_table = queen_pst.data();
            break;
        case Piece::w_king:
        case Piece::b_king:
            piece_square_table = king_pst.data();
            break;
    }

    // Use the value of the square in the piece-square table as the
    // evaluation.
    if (color == Color::white)
    {
        return piece_square_table[static_cast<unsigned>(square)];
    }
    // Black pieces have a negated evaluation, and the board should be
    // flipped when evaluating them.
    else
    {
        return -piece_square_table[flip[static_cast<unsigned>(square)]];
    }
}

