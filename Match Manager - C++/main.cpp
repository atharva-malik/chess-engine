#include <iostream>
#include "includes/bot7.2.h"
#include "includes/bot7.3.h"

//rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

int main(){
    Bot1::Bot bot1("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Bot2::Bot bot2("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    for (int i = 0; i < 35; i++){
        bot1.board.makeMove(uci::uciToMove(bot1.board, bot1.get_best_move(bot1.board, 'w')));
        bot2.board.makeMove(uci::uciToMove(bot2.board, bot2.get_best_move(bot2.board, 'b')));
    }
    return 0;
}