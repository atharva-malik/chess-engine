/**
File for the uci implementation
**/
#include <iostream>
#include "includes/3rdparty/chess.hpp"
#include "includes/helpers.cpp"
// #include "includes/nnue_eval.cpp"
// #include "includes/ucibot.cpp" //* Already included in helpers

using namespace chess;
using namespace helpers;

void ReceiveCommand(std::string message, UciPlayer& player){
    Bot::LogToFile("Command received: " + message);
    message = trim(message);
	std::string messageType = lower(split(message, ' ')[0]);
    
    if (messageType == "uci") DisplayOptions();
    else if (messageType == "isready") Respond("readyok");
    else if (messageType == "ucinewgame") player.NotifyNewGame();
    else if (messageType == "position") ProcessPositionCommand(message, player);
    else if (messageType == "go") ProcessGoCommand(message, player);
    // else if (messageType == "stop") { //TODO: Implement stop command
    // // if (player.IsThinking)
    // // {
    // //     player.StopThinking();
    // // }
    // }
    else if (messageType == "quit") player.Quit(); //todo implement this
    else if (messageType == "d") Respond(player.getFen()); //* display the board
    else Bot::LogToFile("Unrecognized command: " + messageType + " | " + message);
}

int main () {
    // init_nnue("C:\\Atharva\\Programming\\Python\\Python Scripts\\chess-engine\\v8.1-a\\includes\\NNUE\\v4.nnue");
    init_nnue("C:\\Users\\atharva.malik\\Documents\\GitHub\\chess-engine\\v8.1-a\\includes\\NNUE\\v4.nnue");
    Bot bot("1r1q1rk1/p1pn1ppp/2Bb1n2/5p2/2p2P2/2N3P1/PP1PP2P/R1BQK2R w KQ - 0 11", 'm');
    // for (int i = 0; i < 3; i++){
    //     auto start = std::chrono::high_resolution_clock::now();
    //     std::cout << bot.minimax(4, -9999, 9999, true, bot.board) << std::endl;
    //     auto end = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;
    // }

    // for (int i = 0; i < 3; i++){
    //     auto start = std::chrono::high_resolution_clock::now();
    //     std::cout << bot.negamax(4, -9999, 9999, bot.board) << std::endl;
    //     auto end = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;
    // }
    Movelist moves = Movelist();
    movegen::legalmoves(moves, bot.board);
    std::cout << moves.size() << std::endl;
    std::cout << bot.determineDepth(bot.board) << std::endl;

    std::cout << "Mid-40-threads" << std::endl;
    std::cout << "------------------------" << std::endl;

    for (int i = 0; i < 3; i++){
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << bot.mid_40_thread(5, bot.board, 'w') << std::endl;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;
    }

    // std::cout << "Nigamax" << std::endl;
    // std::cout << "------------------------" << std::endl;

    // for (int i = 0; i < 1; i++){ //todo change to 3
    //     auto start = std::chrono::high_resolution_clock::now();
    //     std::cout << bot.nigamax(5, bot.board, -1) << std::endl;
    //     auto end = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;
    // }

    std::cout << "Mid-x-threads" << std::endl;
    std::cout << "------------------------" << std::endl;

    for (int i = 0; i < 3; i++){
        auto start = std::chrono::high_resolution_clock::now();
        // std::cout << bot.negamax(4, -9999, 9999, bot.board) << std::endl;
        bot.mid_x_threads(5, bot.board, 'w');
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;
    }

    // std::cout << "Negamax" << std::endl;
    // std::cout << "------------------------" << std::endl;

    // for (int i = 0; i < 3; i++){
    //     auto start = std::chrono::high_resolution_clock::now();
    //     std::cout << bot.negamax(6, -9999, 9999, bot.board) << std::endl;
    //     auto end = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;
    // }

    std::cout << "Get-best-move" << std::endl;
    std::cout << "------------------------" << std::endl;

    for (int i = 0; i < 3; i++){
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << bot.get_best_move(bot.board, 'w', 5) << std::endl;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;
    }


    // UciPlayer player;
    // std::string command = "";
    // while (lower(command) != "quit")
    // {
    //     std::getline(std::cin, command);
    //     ReceiveCommand(command, player);
    // }
    // Bot::LogToFile("CLOSING UCI bot");
    return 0;
}
