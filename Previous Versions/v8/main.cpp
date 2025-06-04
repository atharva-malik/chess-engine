/**
File for the uci implementation
**/
#include <iostream>
#include "includes/3rdparty/chess.hpp"
#include "includes/helpers.cpp"
#include "includes/experiment/nnue_eval.cpp"
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

    init_nnue("includes/experiment/NNUE/nn-62ef826d1a6d.nnue");

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    // std::cout << "Elapsed time: " << duration.count() << " μs\n\n";
    Board board;
    Bot bot;

    std::cout << "NNUE evaluation test" << std::endl;
    board.setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    start = std::chrono::high_resolution_clock::now();
    std::cout << evaluate_fen_nnue((char *)board.getFen().c_str())/100.0f << std::endl;
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time: " << duration.count() << " μs\n\n";

    std::cout << "Static evaluation test" << std::endl;
    bot = Bot(board.getFen());
    start = std::chrono::high_resolution_clock::now();
    std::cout << bot.eval_mid(bot.board) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time: " << duration.count() << " μs\n\n";

    std::cout << "NNUE evaluation test" << std::endl;
    board.setFen("r3kb1r/2p3pp/p1n1p3/1pn1P3/8/1Nq5/P1B2PPP/R1BQ1RK1 b kq - 1 15");
    start = std::chrono::high_resolution_clock::now();
    std::cout << evaluate_fen_nnue((char *)board.getFen().c_str())/100.0f << std::endl;
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time: " << duration.count() << " μs\n\n";

    std::cout << "Static evaluation test" << std::endl;
    bot = Bot(board.getFen());
    start = std::chrono::high_resolution_clock::now();
    std::cout << bot.eval_mid(bot.board) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time: " << duration.count() << " μs\n\n";

    std::cout << "NNUE evaluation test" << std::endl;
    board.setFen("6k1/5p2/6p1/8/7p/8/6PP/6K1 w - - 0 1");
    start = std::chrono::high_resolution_clock::now();
    std::cout << evaluate_fen_nnue((char *)board.getFen().c_str())/100.0f << std::endl;
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time: " << duration.count() << " μs\n\n";

    std::cout << "Static evaluation test" << std::endl;
    bot = Bot(board.getFen());
    start = std::chrono::high_resolution_clock::now();
    std::cout << bot.eval_end(bot.board) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time: " << duration.count() << " μs\n\n";

    // UciPlayer player;
    // player.bot = Bot("r2qkbnr/ppp1pppp/8/3p1b2/3P1P2/4PN2/PPnN1KPP/R1BQ1B1R w kq - 1 6", 'm');
    // std::string command = "";
    // // Bot::LogToFile("\n\n\n\nSTARTING UCI bot");
    // auto start = std::chrono::high_resolution_clock::now();
    // ProcessGoCommand("go movetime 10000", player); //TODO: Remove this line, it's just for testing the exception handling
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    // std::cout << "Elapsed time: " << duration.count() << " μs\n\n";
    // while (lower(command) != "quit")
    // {
    //     std::getline(std::cin, command);
    //     ReceiveCommand(command, player);
    // }
    // Bot::LogToFile("CLOSING UCI bot");
    return 0;
}
