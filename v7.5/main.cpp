/**
File for the uci implementation
**/
#include <iostream>
#include "includes/chess.hpp"
#include "includes/helpers.cpp"
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
