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
    // Board board("8/7p/5kp1/p1b1r3/P1P5/1P3B1P/4p2K/4B3 w - - 2 45");
    Board board("8/7p/5kp1/p1b1rB2/P1P5/1P3B1P/4p2K/4B3 w - - 2 45");

    std::cout << "Board created" << std::endl;
    // board.set_board_bitboard_from_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", true);
    // board.update_bitboards("8/7p/5kp1/p1b1r3/P1P5/1P3B1P/4p2K/4B3 w - - 2 45");
    std::bitset<64> white_pieces = board.white_pawns | board.white_rooks | board.white_knights | board.white_bishops | board.white_queens | board.white_king;
    std::bitset<64> black_pieces = board.black_pawns | board.black_rooks | board.black_knights | board.black_bishops | board.black_queens | board.black_king;
    std::string square;
    std::cin >> square;
    MoveGeneration knight_moves = board.generate_knight_moves(board.convert_to_square_no(square), 'w', white_pieces, black_pieces);
    board.print_bitboard(knight_moves.moves);
    // std::cout << (knight_moves.num_captures) << std::endl;
    // std::cout << (knight_moves.captures[0].captured_piece.type) << std::endl;
    // std::cout << (knight_moves.captures[0].captured_piece.colour) << std::endl;
    // std::cout << (knight_moves.captures[0].captured_piece.value) << std::endl;
    return 0;
}