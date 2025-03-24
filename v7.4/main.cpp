/**
File for the uci implementation
**/
#include <iostream>
#include "includes/chess.hpp"
#include "includes/helpers.cpp"
#include <chrono>
// #include "includes/ucibot.cpp" //* Already included in helpers

using namespace chess;
using namespace helpers;

void ReceiveCommand(std::string message, UciPlayer& player){
    LogToFile("Command received: " + message);
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
    else LogToFile("Unrecognized command: " + messageType + " | " + message);
}

int main () {
    Bot bot("5k2/p6p/5Pp1/3Br3/2P4B/1P1pb2P/P3p2K/4R3 b - - 2 39", 'm');
    
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << bot.get_best_move(bot.board, 'w') << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;

    bot = Bot("k7/8/1K6/3N4/8/7B/8/8 w - - 0 1", 'm');
    
    start = std::chrono::high_resolution_clock::now();
    std::cout << bot.get_best_move(bot.board, 'w') << std::endl;
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;

    bot = Bot("4rq2/Q2P1k2/4p1p1/5pp1/8/1P6/PBP3N1/2K1R3 w - - 0 1", 'm');
    
    start = std::chrono::high_resolution_clock::now();
    std::cout << bot.get_best_move(bot.board, 'w') << std::endl;
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;

    UciPlayer player;
    std::string command = "";
    LogToFile("\n\nSTARTING UCI bot");
    while (lower(command) != "quit")
    {
        std::getline(std::cin, command);
        ReceiveCommand(command, player);
    }
    LogToFile("CLOSING UCI bot");
    return 0;
}
