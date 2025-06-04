#include "constructors.cpp"
#include "evaluate.cpp"
#include "openings.cpp"
#include "search.cpp"
#include "bothelpers.cpp"
#include "findmove.cpp"

std::string Bot::get_best_move(Board& board, char colour, int depth=-1) {
    this->killer_moves.clear();
    this->transpositionTable.clear();
    if (this->game_stage == 'o') return Bot::opening_move(board.getFen(), colour);
    else if (this->game_stage == 'm'){
        int d = depth == -1 ? Bot::determineDepth(board) : depth;
        return this->middle_game_x_thread(d, board, colour);
    }else {
        return this->middle_game_x_thread(11, board, colour);
    }
}
