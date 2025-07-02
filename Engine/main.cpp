/**
 *  @file main.cpp
 *  @brief Entry point for the UCI-compatible chess engine interface.
 *
 ** This file implements the command handling loop for communicating with 
 ** GUI clients using the Universal Chess Interface (UCI) protocol. It initialises 
 ** the NNUE evaluation engine, processes incoming UCI commands, and delegates 
 ** behavior to relevant components such as move generation, board state 
 ** management, and game control.
 *
 *? Dependencies:
 *? - chess.hpp: Core chess logic and data structures.
 *? - helpers.cpp: Utility functions for command parsing and engine support.
 *
 ** The program logs interactions for debugging and exits gracefully upon receiving
 ** the 'quit' command.
*/

#include <iostream>
#include "includes/3rdparty/chess.hpp"
#include "includes/helpers.cpp"

using namespace chess;
using namespace helpers;

void ReceiveCommand(std::string message, UciPlayer& player){
    /**
     *  @brief Parses and dispatches incoming UCI commands to appropriate handlers.
     *
     ** This function receives a command string from the GUI or input stream,
     ** trims and normalizes it, then matches the command against known UCI types.
     ** Based on the command type, it invokes the appropriate function on the
     ** provided UciPlayer instance or auxiliary handler functions.
     *
     *  @param message The raw UCI command received as a string.
     *  @param player Reference to the UciPlayer instance that manages engine state and operations.
     *
     *? Supported commands:
     *?  - "uci"           : Respond with engine identification and options.
     *?  - "isready"       : Confirm readiness with "readyok".
     *?  - "ucinewgame"    : Signal a new game to reset state.
     *?  - "position"      : Set up the board with a given FEN or move list.
     *?  - "go"            : Begin calculating best move based on the current position.
     *?  - "quit"          : Exit the engine.
     *?  - "d"             : Print the current board to stdout (non-standard debug command).
     *
     ** Logs unrecognised commands for debugging purposes.
    */
    Bot::LogToFile("Command received: " + message);
    message = trim(message);
	std::string messageType = lower(split(message, ' ')[0]);
    
    if (messageType == "uci") DisplayOptions();
    else if (messageType == "isready") Respond("readyok");
    else if (messageType == "ucinewgame") player.NotifyNewGame();
    else if (messageType == "position") ProcessPositionCommand(message, player);
    else if (messageType == "go") ProcessGoCommand(message, player);
    else if (messageType == "quit" || messageType == "exit" || messageType == "q") player.Quit();
    else if (messageType == "d") Bot::print_board(player.bot.board); //* display the board
    else if (messageType == "h" || messageType == "help") PrintHelp();
    else if (messageType == "perft") ProcessPerftCommand(message, player);
    else if (messageType == "eval") ProcessEvalCommand(message, player);
    else if (messageType == "cls") clearScreen();
    else Respond("Unrecognised command: " + messageType + " | " + message);
}

int main () {
    /**
     *  @brief Entry point for the UCI engine executable.
     *
     ** Initialises the NNUE (Efficiently Updatable Neural Network) evaluation module 
     ** and enters a loop that listens for and processes UCI commands from standard input.
     *
     ** The function continuously reads input commands until the "quit" command is issued,
     ** at which point the engine logs shutdown activity and exits gracefully.
     *
     *  @return Exit status code (0 for successful termination).
    */

    init_nnue("includes\\NNUE\\v4.nnue");
    
    UciPlayer player;
    std::string command = "";
    while (lower(command) != "quit" && lower(command) != "exit" && lower(command) != "q")
    {
        std::getline(std::cin, command);
        ReceiveCommand(command, player);
    }
    Bot::LogToFile("CLOSING UCI bot");
    return 0;
}
