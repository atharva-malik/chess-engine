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

struct Piece {
    char type;
    char color;
    int value;
};

struct Square {
    int x;
    int y;
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
        void move_piece();
        void undo_move();
        void check_game_over();
        void declare_outcome();

    private:
        // char board[8][8] = {};
        std::string chessCoordinates[8][8] = {
            {"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"},
            {"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7"},
            {"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6"},
            {"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5"},
            {"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4"},
            {"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3"},
            {"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2"},
            {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"}
        };        
        int half_moves;
        int full_moves;
        std::string FEN = "";
        int turn;
        char castling_rights[4] = {'K', 'Q', 'k', 'q'};
        Move last_move;
};