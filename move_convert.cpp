#include <map>
#include <string>
#include "types.h"
#include "utils.h"
#include "game.h"


// String coordinates to/from square indices

std::map<std::string, Square> coord_to_index =
{
    {"a8", Square::A8}, {"b8", Square::B8}, {"c8", Square::C8}, {"d8", Square::D8}, {"e8", Square::E8}, {"f8", Square::F8}, {"g8", Square::G8}, {"h8", Square::H8},
    {"a7", Square::A7}, {"b7", Square::B7}, {"c7", Square::C7}, {"d7", Square::D7}, {"e7", Square::E7}, {"f7", Square::F7}, {"g7", Square::G7}, {"h7", Square::H7},
    {"a6", Square::A6}, {"b6", Square::B6}, {"c6", Square::C6}, {"d6", Square::D6}, {"e6", Square::E6}, {"f6", Square::F6}, {"g6", Square::G6}, {"h6", Square::H6},
    {"a5", Square::A5}, {"b5", Square::B5}, {"c5", Square::C5}, {"d5", Square::D5}, {"e5", Square::E5}, {"f5", Square::F5}, {"g5", Square::G5}, {"h5", Square::H5},
    {"a4", Square::A4}, {"b4", Square::B4}, {"c4", Square::C4}, {"d4", Square::D4}, {"e4", Square::E4}, {"f4", Square::F4}, {"g4", Square::G4}, {"h4", Square::H4},
    {"a3", Square::A3}, {"b3", Square::B3}, {"c3", Square::C3}, {"d3", Square::D3}, {"e3", Square::E3}, {"f3", Square::F3}, {"g3", Square::G3}, {"h3", Square::H3},
    {"a2", Square::A2}, {"b2", Square::B2}, {"c2", Square::C2}, {"d2", Square::D2}, {"e2", Square::E2}, {"f2", Square::F2}, {"g2", Square::G2}, {"h2", Square::H2},
    {"a1", Square::A1}, {"b1", Square::B1}, {"c1", Square::C1}, {"d1", Square::D1}, {"e1", Square::E1}, {"f1", Square::F1}, {"g1", Square::G1}, {"h1", Square::H1}
};

std::map<Square, std::string> index_to_coord =
{
    {Square::A8, "a8"}, {Square::B8, "b8"}, {Square::C8, "c8"}, {Square::D8, "d8"}, {Square::E8, "e8"}, {Square::F8, "f8"}, {Square::G8, "g8"}, {Square::H8, "h8"},
    {Square::A7, "a7"}, {Square::B7, "b7"}, {Square::C7, "c7"}, {Square::D7, "d7"}, {Square::E7, "e7"}, {Square::F7, "f7"}, {Square::G7, "g7"}, {Square::H7, "h7"},
    {Square::A6, "a6"}, {Square::B6, "b6"}, {Square::C6, "c6"}, {Square::D6, "d6"}, {Square::E6, "e6"}, {Square::F6, "f6"}, {Square::G6, "g6"}, {Square::H6, "h6"},
    {Square::A5, "a5"}, {Square::B5, "b5"}, {Square::C5, "c5"}, {Square::D5, "d5"}, {Square::E5, "e5"}, {Square::F5, "f5"}, {Square::G5, "g5"}, {Square::H5, "h5"},
    {Square::A4, "a4"}, {Square::B4, "b4"}, {Square::C4, "c4"}, {Square::D4, "d4"}, {Square::E4, "e4"}, {Square::F4, "f4"}, {Square::G4, "g4"}, {Square::H4, "h4"},
    {Square::A3, "a3"}, {Square::B3, "b3"}, {Square::C3, "c3"}, {Square::D3, "d3"}, {Square::E3, "e3"}, {Square::F3, "f3"}, {Square::G3, "g3"}, {Square::H3, "h3"},
    {Square::A2, "a2"}, {Square::B2, "b2"}, {Square::C2, "c2"}, {Square::D2, "d2"}, {Square::E2, "e2"}, {Square::F2, "f2"}, {Square::G2, "g2"}, {Square::H2, "h2"},
    {Square::A1, "a1"}, {Square::B1, "b1"}, {Square::C1, "c1"}, {Square::D1, "d1"}, {Square::E1, "e1"}, {Square::F1, "f1"}, {Square::G1, "g1"}, {Square::H1, "h1"}
};

// Promotion placeholders to/from binary flags

std::map<std::string, Promotion_piece> promo_str_to_bin =
{
    {"q", Promotion_piece::queen},
    {"r", Promotion_piece::rook},
    {"b", Promotion_piece::bishop},
    {"n", Promotion_piece::knight},
};

std::map<Promotion_piece, std::string> promo_bin_to_str =
{
    {Promotion_piece::queen, "q"},
    {Promotion_piece::rook, "r"},
    {Promotion_piece::bishop, "b"},
    {Promotion_piece::knight, "n"}
};


// Generates and returns a move using a string. The first two characters
// indicate the starting position, the two characters after that indicate
// the ending position. The fifth optional character indicates the
// promotion piece. Returns Move::none if the string is invalid.
// Examples: "b5f8" or "f7F8q"
Move Game::string_to_move(const std::string move_str) const
{
    const auto str_len = move_str.length();
    auto move_type = Move_type::normal;

    // The input string has an invalid length.
    if (!(str_len == 4 || str_len == 5))
    {
        return Move::none;
    }

    // First two characters are the origin square - extract them.
    const auto origin_str = move_str.substr(0, 2);

    // Check if the origin square is invalid.
    if (coord_to_index.find(origin_str) == coord_to_index.end())
    {
        return Move::none;
    }

    const auto origin_sq = coord_to_index[origin_str];

    // Third and fourth characters are the destination square - extract them.
    const auto destination_str = move_str.substr(2, 2);

    // Check if the destination square is invalid.
    if (coord_to_index.find(destination_str) == coord_to_index.end())
    {
        return Move::none;
    }

    const auto dest_sq = coord_to_index[destination_str];
    
    const Bitboard dest_bb = square_to_bb(dest_sq);

    auto promo_piece = Promotion_piece::none;

    // If the promotion piece was included, process it.
    if (str_len == 5)
    {
        // Fifth character is the promotion type (optional).
        const auto promo_str = move_str.substr(4, 1);

        // Make sure the promotion type is valid.
        if(promo_str_to_bin.find(promo_str) == promo_str_to_bin.end())
        {
            return Move::none;
        }

        promo_piece = promo_str_to_bin[promo_str];
    }

    const auto piece_moved = piece_on(origin_sq);

    // Check if this is a castling move.
    if (piece_moved == Piece::w_king || piece_moved == Piece::b_king)
    {
        // White kingside castle
        if (origin_sq == Square::E1 && dest_sq == Square::G1)
        {
            move_type = Move_type::castling;
        }
        // White queenside castle
        else if (origin_sq == Square::E1 && dest_sq == Square::C1)
        {
            move_type = Move_type::castling;
        }
        // Black kingside castle
        else if (origin_sq == Square::E8 && dest_sq == Square::G8)
        {
            move_type = Move_type::castling;
        }
        // Black queenside castle
        else if (origin_sq == Square::E8 && dest_sq == Square::C8)
        {
            move_type = Move_type::castling;
        }
    }

    // Check for en passant and promotion.
    else if (piece_moved == Piece::w_pawn || piece_moved == Piece::b_pawn)
    {
        // To en passant square - en passant.
        if (dest_sq == en_passant_square)
        {
            move_type = Move_type::en_passant;
        }
        // To 8th or 1st row - promotion.
        else if (on_bitboard(dest_bb, row_8) || on_bitboard(dest_bb, row_1))
        {
            move_type = Move_type::promotion;
        }
    }

    // Check if this is a promotion move but a promotion placeholder was
    // included.
    if (move_type != Move_type::promotion &&
        str_len == 5)
    {
        return Move::none;
    }

    // Check if this is a promotion move but a promotion placeholder was not
    // included.
    if (move_type == Move_type::promotion &&
        str_len == 4)
    {
        return Move::none;
    }

    // Create the move.
    return create_move(origin_sq, dest_sq, promo_piece, move_type);
}


// Converts a move to its string representation.
std::string Game::move_to_string(const Move move) const
{
    // Extract the origin and destination squares.
    const Square origin_sq = extract_origin_sq(move);
    const Square dest_sq = extract_dest_sq(move);
    const Move_type move_type = extract_move_type(move);


    const std::string origin_str = index_to_coord[origin_sq];
    const std::string dest_str = index_to_coord[dest_sq];

    // Combine the string representations of the origin and destination
    // squares.
    std::string move_str = origin_str + dest_str;

    // If this is a promotion move, add the promotion piece string too.
    if (move_type == Move_type::promotion)
    {
        const Promotion_piece promo_piece = extract_promo_piece(move);
        const std::string promo_str = promo_bin_to_str[promo_piece];
        move_str += promo_str;
    }

    return move_str;
}
