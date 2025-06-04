/*
* The module needs to:
*     - have a class for the board which will have:
*         - an init method to create the board from FEN (__init__)
*         - a method to print the board (__str__)
*         - a method to move a piece
*         - a method to undo the move
*         - a method to check if the game is over
*         - a method to declare outcomes
*     - have a class for the pieces which will have:
*         - a string for piece colour
*         - an int for piece type
*             - 0 for w_pawn, 1 for w_knight, 2 for w_bishop, 3 for w_rook, 4 for w_queen, 5 for w_king
*             - 6 for b_pawn, 7 for b_knight, 8 for b_bishop, 9 for b_rook, 10 for b_queen, 11 for b_king
*/

#pragma once
#include <bitset>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>


struct Piece {
    char type;
    char colour;
    int value;
    // Square square;
};

struct Square {
    // int x;
    // int y;
    int number;
    Piece piece;
};

struct Move {
    Square from_square;
    Square to_square;
    bool is_capture;
    bool is_en_passant;
    bool is_castling;
    bool is_promotion;
    Piece promotion_piece;
    Piece captured_piece;
    std::string moveStr; //*todo: if it is slower, convert to char[4]?
};

struct MoveGeneration{
    std::bitset<64> moves;
    bool is_captures;
    int num_captures = 0;
    Move captures[8];
};

class Board {
    public:
        std::bitset<64> en_passant_target = 0;
        ///------- Definitions of the white bitboards -------///
        std::bitset<64> white_pawns       = 0b0000000000000000000000000000000000000000000000001111111100000000;
        std::bitset<64> white_rooks       = 0b0000000000000000000000000000000000000000000000000000000010000001;
        std::bitset<64> white_knights     = 0b0000000000000000000000000000000000000000000000000000000001000010;
        std::bitset<64> white_bishops     = 0b0000000000000000000000000000000000000000000000000000000000100100;
        std::bitset<64> white_queens      = 0b0000000000000000000000000000000000000000000000000000000000010000;
        std::bitset<64> white_king        = 0b0000000000000000000000000000000000000000000000000000000000001000;
        ///-------         end of definitions         -------///

        ///------- Definitions of the black bitboards -------///
        std::bitset<64> black_pawns       = 0b0000000011111111000000000000000000000000000000000000000000000000;
        std::bitset<64> black_rooks       = 0b1000000100000000000000000000000000000000000000000000000000000000;
        std::bitset<64> black_knights     = 0b0100001000000000000000000000000000000000000000000000000000000000;
        std::bitset<64> black_bishops     = 0b0010010000000000000000000000000000000000000000000000000000000000;
        std::bitset<64> black_queens      = 0b0001000000000000000000000000000000000000000000000000000000000000;
        std::bitset<64> black_king        = 0b0000100000000000000000000000000000000000000000000000000000000000;
        ///-------         end of definitions         -------///
        Board(); //! Done
        Board(std::string fen); //! Done
        void set_board_bitboard_from_FEN(std::string fen, bool update_bit_boards); //! DONE
        void create_FEN_from_bitboards(); //! Done
        void update_bitboards(std::string fen); //! Done
        void print_board(); //! Done
        void print_bitboard(std::bitset<64> bitboard); //! Done
        std::vector<Move> generate_moves(bool turn); //* 0 if white, 1 if black
        MoveGeneration generate_white_pawn_moves(int square_no);
        MoveGeneration generate_black_pawn_moves(int square_no);
        MoveGeneration generate_knight_moves(int square_no, char colour, std::bitset<64> friendly, std::bitset<64> unfriendly); //! DONE
        MoveGeneration generate_bishop_moves(int square_no, char colour, std::bitset<64> friendly, std::bitset<64> unfriendly); //! DONE
        MoveGeneration generate_rook_moves(int square_no, char colour, std::bitset<64> friendly, std::bitset<64> unfriendly); //! DONE
        MoveGeneration generate_queen_moves(int square_no, char colour, std::bitset<64> friendly, std::bitset<64> unfriendly); //! DONE
        MoveGeneration generate_king_moves(int square_no);
        MoveGeneration order_moves();
        Piece get_piece_at_square(int square_no);
        int convert_to_square_no(std::string square);
        void move_piece();
        void undo_move();
        void check_game_over();
        void declare_outcome();

    private:
        // char board[8][8] = {};
        std::string no_to_square[8][8] = {
            {"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"},
            {"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7"},
            {"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6"},
            {"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5"},
            {"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4"},
            {"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3"},
            {"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2"},
            {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"}
        };
        std::map<std::string, int> square_to_no = {
            {"a8", 63}, {"b8", 62}, {"c8", 61}, {"d8", 60}, {"e8", 59}, {"f8", 58}, {"g8", 57}, {"h8", 56},
            {"a7", 55}, {"b7", 54}, {"c7", 53}, {"d7", 52}, {"e7", 51}, {"f7", 50}, {"g7", 49}, {"h7", 48},
            {"a6", 47}, {"b6", 46}, {"c6", 45}, {"d6", 44}, {"e6", 43}, {"f6", 42}, {"g6", 41}, {"h6", 40},
            {"a5", 39}, {"b5", 38}, {"c5", 37}, {"d5", 36}, {"e5", 35}, {"f5", 34}, {"g5", 33}, {"h5", 32},
            {"a4", 31}, {"b4", 30}, {"c4", 29}, {"d4", 28}, {"e4", 27}, {"f4", 26}, {"g4", 25}, {"h4", 24},
            {"a3", 23}, {"b3", 22}, {"c3", 21}, {"d3", 20}, {"e3", 19}, {"f3", 18}, {"g3", 17}, {"h3", 16},
            {"a2", 15}, {"b2", 14}, {"c2", 13}, {"d2", 12}, {"e2", 11}, {"f2", 10}, {"g2", 9},  {"h2", 8},
            {"a1", 7},  {"b1", 6},  {"c1", 5},  {"d1", 4},  {"e1", 3},  {"f1", 2},  {"g1", 1},  {"h1", 0}
        };
        int half_moves;
        int full_moves;
        std::string FEN = "";
        int turn;
        char castling_rights[4] = {'K', 'Q', 'k', 'q'};
        Move last_move;
        std::bitset<64> not_a_file_bitboard = 0b1111111011111110111111101111111011111110111111101111111011111110;
        std::bitset<64> not_h_file_bitboard = 0b0111111101111111011111110111111101111111011111110111111101111111;
        std::bitset<64> not_ab_file_bitboard = 0b0011111100111111001111110011111100111111001111110011111100111111;
        std::bitset<64> not_hg_file_bitboard = 0b1111110011111100111111001111110011111100111111001111110011111100;
};