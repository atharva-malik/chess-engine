/**
File for testing the chess implementation
**/
#include <iostream>
#include "includes/chess.hpp"
#include "includes/bot.cpp"
#include <chrono>

using namespace chess;

int main () {
    Bot bot("kq6/2p5/8/8/8/8/5Q2/6K1 w - - 0 1");

    char turn = 'w';
    std::string move;
    for (int i = 0; i < 200; i++) {
        // if (turn == 'w') {
        //     auto start = std::chrono::high_resolution_clock::now();
        //     move = bot.get_best_move(5, bot.board, 'w');
        //     auto end = std::chrono::high_resolution_clock::now();
        //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        //     std::cout << move << std::endl;
        //     bot.board.makeMove(uci::uciToMove(bot.board, move));
        //     std::cout << bot.board.getFen() << std::endl;
        //     std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;
        //     // std::cout << bot.board.getFen() << std::endl;
        //     turn = 'b';
        // } else {
        //     std::cout << "Enter your move: ";
        //     std::cin >> move;
        //     bot.board.makeMove(uci::uciToMove(bot.board, move));
        //     turn = 'w';
        // }
        if (turn == 'w') {
            auto start = std::chrono::high_resolution_clock::now();
            move = bot.get_best_move(bot.board, 'w');
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << move << std::endl;
            bot.board.makeMove(uci::uciToMove(bot.board, move));
            // std::cout << bot.board.getFen() << std::endl;
            std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;
            // std::cout << bot.board.getFen() << std::endl;
            turn = 'b';
        } else {
            auto start = std::chrono::high_resolution_clock::now();
            move = bot.get_best_move(bot.board, 'b');
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << move << std::endl;
            bot.board.makeMove(uci::uciToMove(bot.board, move));
            // std::cout << bot.board.getFen() << std::endl;
            std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;
            turn = 'w';
        }

    }

    return 0;
}
