/**
File for the uci implementation
**/
#include <iostream>
#include "includes/3rdparty/chess.hpp"
#include "includes/helpers.cpp"

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
    else if (messageType == "quit") player.Quit();
    else if (messageType == "d") Bot::print_board(player.bot.board); //* display the board
    else Bot::LogToFile("Unrecognized command: " + messageType + " | " + message);
}

int main () {
    init_nnue("includes\\NNUE\\v4.nnue");
    
    UciPlayer player;
    std::string command = "";
    while (lower(command) != "quit")
    {
        std::getline(std::cin, command);
        ReceiveCommand(command, player);
    }
    Bot::LogToFile("CLOSING UCI bot");
    return 0;
}
