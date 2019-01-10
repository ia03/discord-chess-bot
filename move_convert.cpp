/*
 * Ibrahim Ahmed
 * January 7, 2018
 * String-to-move conversion and vice versa
*/

#include <map>
#include <string>
#include "types.h"
#include "utils.h"
#include "game.h"

// String coordinates to/from square indices

unordered_map<std::string, Square> coord_to_index =
{
    {"a8", 56}, {"b8", 57}, {"c8", 58}, {"d8", 59}, {"e8", 60}, {"f8", 61}, {"g8", 62}, {"h8", 63},
    {"a7", 48}, {"b7", 49}, {"c7", 50}, {"d7", 51}, {"e7", 52}, {"f7", 53}, {"g7", 54}, {"h7", 55},
    {"a6", 40}, {"b6", 41}, {"c6", 42}, {"d6", 43}, {"e6", 44}, {"f6", 45}, {"g6", 46}, {"h6", 47},
    {"a5", 32}, {"b5", 33}, {"c5", 34}, {"d5", 35}, {"e5", 36}, {"f5", 37}, {"g5", 38}, {"h5", 39},
    {"a4", 24}, {"b4", 25}, {"c4", 26}, {"d4", 27}, {"e4", 28}, {"f4", 29}, {"g4", 30}, {"h4", 31},
    {"a3", 16}, {"b3", 17}, {"c3", 18}, {"d3", 19}, {"e3", 20}, {"f3", 21}, {"g3", 22}, {"h3", 23},
    {"a2",  8}, {"b2",  9}, {"c2", 10}, {"d2", 11}, {"e2", 12}, {"f2", 13}, {"g2", 14}, {"h2", 15},
    {"a1",  0}, {"b1",  1}, {"c1",  2}, {"d1",  3}, {"e1",  4}, {"f1",  5}, {"g1",  6}, {"h1",  7}
};

unordered_map<Square, std::string> index_to_coord =
{
    {56, "a8"}, {57, "b8"}, {58, "c8"}, {59, "d8"}, {60, "e8"}, {61, "f8"}, {62, "g8"}, {63, "h8"},
    {48, "a7"}, {49, "b7"}, {50, "c7"}, {51, "d7"}, {52, "e7"}, {53, "f7"}, {54, "g7"}, {55, "h7"},
    {40, "a6"}, {41, "b6"}, {42, "c6"}, {43, "d6"}, {44, "e6"}, {45, "f6"}, {46, "g6"}, {47, "h6"},
    {32, "a5"}, {33, "b5"}, {34, "c5"}, {35, "d5"}, {36, "e5"}, {37, "f5"}, {38, "g5"}, {39, "h5"},
    {24, "a4"}, {25, "b4"}, {26, "c4"}, {27, "d4"}, {28, "e4"}, {29, "f4"}, {30, "g4"}, {31, "h4"},
    {16, "a3"}, {17, "b3"}, {18, "c3"}, {19, "d3"}, {20, "e3"}, {21, "f3"}, {22, "g3"}, {23, "h3"},
    { 8, "a2"}, { 9, "b2"}, {10, "c2"}, {11, "d2"}, {12, "e2"}, {13, "f2"}, {14, "g2"}, {15, "h2"},
    { 0, "a1"}, { 1, "b1"}, { 2, "c1"}, { 3, "d1"}, { 4, "e1"}, { 5, "f1"}, { 6, "g1"}, { 7, "h1"}
};

// Promotion placeholders to/from binary flags

unordered_map<std::string, Promotion_piece> promo_str_to_bin =
{
    {"q", Promotion_piece::queen},
    {"r", Promotion_piece::rook},
    {"b", Promotion_piece::bishop},
    {"n", Promotion_piece::knight},
};

unordered_map<Promotion_piece, std::string> promo_bin_to_str =
{
    {Promotion_piece::queen, "q"},
    {Promotion_piece::rook, "r"},
    {Promotion_piece::bishop, "b"},
    {Promotion_piece::knight, "n"}
};


Move Game::string_to_move(std::string move_str)
{
	int str_len = move_str.length();
	Move_type move_type = Move_type::normal;
	
	// The input string has an invalid length.
	if (!(str_len == 4 or str_len == 5))
		return 0;

	// First two characters are the "from" square.
	std::string from_str = move_str.substr(0, 2);

	// Invalid "from" square.
	if (coord_to_index.find(from_str) == coord_to_index.end())
		return 0;

	Square from_sqr = coord_to_index[from_str];

	// Third and fourth characters are the "to" square.
	std::string to_str = move_str.substr(2, 2);

	// Invalid "to" square.
	if (coord_to_index.find(to_str) == coord_to_index.end())
		return 0;

	Square to_sqr = coord_to_index[to_str];
	Promotion_piece promo_piece = Promotion_piece::none;

	if (str_len == 5)
	{
		// Fifth character is the promotion type (optional).
		std::string promo_str = move_str.substr(4, 1);

		// Invalid promotion type.
		if(promo_str_to_bin.find(promo_str) == promo_str_to_bin.end())
			return 0;
		
		promo_flag = promo_str_to_bin[promo_str];
	}

	Piece piece_moved = piece_on(from_sqr));

	// Check if this is a castling move.
	if (piece_moved == Piece::w_king || piece_moved == Piece::b_king)
	{
		// White kingside castle
		if (from_sqr == Square::E1 && to_sqr == Square::G1)
			move_type = Move_type::castling;
		// White queenside castle
		else if (from_sqr == Square::E1 && to_sqr == Square::C1)
			move_type = Move_type::castling;
		// Black kingside castle
		else if (from_sqr == Square::E8 && to_sqr == Square::G8)
			move_type = Move_type::castling;
		// Black queenside castle
		else if (from_sqr == Square::E8 && to_sqr == Square::C8)
			move_type = Move_type::castling;
	}
	// Check for en passant and promotion.
	else if (piece_moved == Piece::w_pawn || piece_moved == Piece::b_pawn)
	{
		if (to_sqr == en_passant_square)
			move_type = Move_type::en_passant;
		else if (to_sqr >= 56 || to_sqr <= 7)
			move_type = Move_type::promotion;
	}
	// Not a promotion move but a promotion placeholder was included.
	if (move_type != Move_type::promotion && promo_flag != -1)
		return 0;

	return create_move(from_sqr, to_sqr, promo_piece, move_type);
}

std::string Game::move_to_string(Move move)
{
	
}
