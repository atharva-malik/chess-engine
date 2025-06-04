/**
File for the uci implementation
**/
#include <iostream>
#include "includes/chess.hpp"
#include "includes/helpers.cpp"
// #include "includes/ucibot.cpp" //* Already included in helpers

using namespace chess;

int main () {
    // Bot bot = Bot("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    Bot bot = Bot("5k2/p6p/5Pp1/3Br3/2P4B/1P2b2P/P2pp2K/4R3 w - - 0 40");
    
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << bot.middle_game_move(7, bot.board, 'w') << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::cout << bot.middle_game_move_threading(5, bot.board, 'w') << std::endl;
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;
    return 0;
}
//TODO: RUN THESE TESTS TMRW