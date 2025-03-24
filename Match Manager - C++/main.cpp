#include <iostream>
#include "includes/bot7.2.cpp"
#include "includes/bot7.3.cpp"

//rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

float playAMatch(Bot1::Bot bot1, Bot2::Bot bot2){
    Board board;
    Move move;
    std::pair<GameResultReason, GameResult> isGameOver;
    while (true){
        move = uci::uciToMove(bot1.board, bot1.get_best_move(bot1.board, 'w'));
        std::cout<<uci::moveToUci(move)<<std::endl;
        bot1.board.makeMove(move);
        bot2.board.makeMove(move);
        board.makeMove(move);

        isGameOver = board.isGameOver();
        if (isGameOver.first == GameResultReason::CHECKMATE){
            if (board.sideToMove() == Color::WHITE) {std::cout<<board.getFen()<<std::endl;return -1;}
            else return 1;
        } else if (!(isGameOver.first == GameResultReason::NONE)){
            return 0.0f;
        }

        move = uci::uciToMove(bot2.board, bot2.get_best_move(bot2.board, 'b'));
        std::cout<<uci::moveToUci(move)<<std::endl;
        bot1.board.makeMove(move);
        bot2.board.makeMove(move);
        board.makeMove(move);

        isGameOver = board.isGameOver();
        if (isGameOver.first == GameResultReason::CHECKMATE){
            if (board.sideToMove() == Color::WHITE) {std::cout<<board.getFen()<<std::endl;return -1;}
            else return 1;
        } else if (!(isGameOver.first == GameResultReason::NONE)){
            return 0.0f;
        }
    }

}

int main(){
    Bot1::Bot bot1;
    Bot2::Bot bot2;
    std::cout << playAMatch(bot1, bot2) << std::endl;
    return 0;
}