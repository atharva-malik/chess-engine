/**
File for testing the chess implementation
**/
#include <iostream>
#include "includes/chess.hpp"
#include "includes/bot.cpp"

using namespace chess;

uint64_t perft(Board& board, int depth) {
    Movelist moves;
    movegen::legalmoves(moves, board);

    if (depth == 1) {
        return moves.size();
    }

    uint64_t nodes = 0;

    for (int i = 0; i < moves.size(); i++) {
        const auto move = moves[i];
        board.makeMove(move);
        nodes += perft(board, depth - 1);
        board.unmakeMove(move);
    }

    return nodes;
}

int main () {
    Bot bot("8/7p/5kp1/p1b1r3/P1P5/1P3B1P/4p2K/4B3 w - - 2 45", 'm');
    // Bot bot("8/8/8/k7/4Q3/8/8/3K4 w - - 0 1");
    
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << bot.get_best_move(5, bot.board, 'w') << std::endl;
    // bot.evaluate(bot.board);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function execution time: " << duration.count() << " microseconds" << std::endl;

    // char turn = 'w';
    // std::string move;
    // for (int i = 0; i < 25; i++) {
    //     if (turn == 'w') {
    //         move = bot.get_best_move(5, bot.board, 'w');
    //         std::cout << move << std::endl;
    //         bot.board.makeMove(uci::uciToMove(bot.board, move));
    //         std::cout << bot.board.getFen() << std::endl;
    //         turn = 'b';
    //     } else {
    //         move = bot.get_best_move(5, bot.board, 'b');
    //         std::cout << move << std::endl;
    //         bot.board.makeMove(uci::uciToMove(bot.board, move));
    //         std::cout << bot.board.getFen() << std::endl;
    //         turn = 'w';
    //     }

    // }

    return 0;
}
