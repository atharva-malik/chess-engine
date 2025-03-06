/**
File for testing the chess implementation
**/

#include <iostream>
#include "includes/chess.h"
#include "includes/bot.h"
/*
*   board.create_FEN_from_bitboards();
*   std::cout << "Bitboards updated" << std::endl;
*   board.print_bitboard(board.white_pawns);
*   std::cout << std::endl << std::endl;
*   board.print_bitboard(board.black_pawns);
*   std::cout << std::endl << std::endl;
*   board.print_bitboard(board.white_rooks);
*   std::cout << std::endl << std::endl;
*   board.print_bitboard(board.black_rooks);
*   std::cout << std::endl << std::endl;
*   board.print_bitboard(board.white_knights);
*   std::cout << std::endl << std::endl;
*   board.print_bitboard(board.black_knights);
*   std::cout << std::endl << std::endl;
*   board.print_bitboard(board.white_bishops);
*   std::cout << std::endl << std::endl;
*   board.print_bitboard(board.black_bishops);
*   std::cout << std::endl << std::endl;
*   board.print_bitboard(board.white_queens);
*   std::cout << std::endl << std::endl;
*   board.print_bitboard(board.black_queens);
*   std::cout << std::endl << std::endl;
*   board.print_bitboard(board.white_king);
*   std::cout << std::endl << std::endl;
*   board.print_bitboard(board.black_king);
*/
int main() {
    Board board("8/7p/5kp1/p1b1r3/P1P5/1P3B1P/4p2K/4B3 w - - 2 45");

    std::cout << "Board created" << std::endl;
    // board.set_board_bitboard_from_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", true);
    // board.update_bitboards("8/7p/5kp1/p1b1r3/P1P5/1P3B1P/4p2K/4B3 w - - 2 45");
    return 0;
}