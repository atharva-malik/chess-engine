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
    Bot bot("r1b1k1n1/ppbp4/n1p1p2p/5p2/NP1P3N/P7/2PQPPPP/R3KB1R w KQ - 1 17", 'm');
    // Bot bot("r1b2kn1/pp1p4/nbp1p2p/4Np2/NP1P3q/P7/2PQPPPP/R3KB1R w KQ - 4 15", 'm');

    Move move;
    std::string _move;

    // _move = bot.get_best_move(bot.board, 'w');


    auto start = std::chrono::high_resolution_clock::now();
    _move = bot.best_experimental(bot.board, 'w', 5);
    move = uci::uciToMove(bot.board, _move);
    bot.board.makeMove(move);
    std::cout << move << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "White time taken: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "FEN: " << bot.board.getFen() << std::endl;
    bot.board.unmakeMove(move);


    // start = std::chrono::high_resolution_clock::now();
    // _move = bot.middle_game_move(5, bot.board, 'w');
    // move = uci::uciToMove(bot.board, _move);
    // bot.board.makeMove(move);
    // std::cout << move << std::endl;
    // end = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "White time taken: " << duration.count() << " milliseconds" << std::endl;
    // std::cout << "FEN: " << bot.board.getFen() << std::endl;
    // bot.board.unmakeMove(move);

    UciPlayer player;
    std::string command = "";
    Bot::LogToFile("\n\nSTARTING UCI bot");
    while (lower(command) != "quit")
    {
        std::getline(std::cin, command);
        ReceiveCommand(command, player);
    }
    Bot::LogToFile("CLOSING UCI bot");
    return 0;
}
