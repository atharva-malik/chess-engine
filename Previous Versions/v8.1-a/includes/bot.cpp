#include "constructors.cpp"
#include "evaluate.cpp"
#include "openings.cpp"
#include "search.cpp"
#include "bothelpers.cpp"
#include "findmove.cpp"


std::string Bot::get_best_move_tl(Board& board, char colour, double time_limit, int depth=-1) {
    this->killer_moves.clear();
    this->transpositionTable.clear();
    this->start_time = std::chrono::high_resolution_clock::now();
    this->time_limit = time_limit-100;
    if (this->game_stage == 'o') return Bot::get_opening_move(board.getFen(), colour);
    else if (this->game_stage == 'm'){
        int d = depth == -1 ? Bot::determineDepth(board) : depth;
        return this->middle_game_move_tl(d, board, colour);
    }else {
        // int d = Bot::determineDepth(board); //* This will always return 11, as the game stage is 'e'
        return this->middle_game_move_tl(11, board, colour); // TODO: Implement endgame move selection
    }
}

std::string Bot::get_best_move(Board& board, char colour, int depth=-1) {
    this->killer_moves.clear();
    this->transpositionTable.clear();
    if (this->game_stage == 'o') return Bot::get_opening_move(board.getFen(), colour);
    else if (this->game_stage == 'm'){
        int d = depth == -1 ? Bot::determineDepth(board) : depth;
        return this->middle_game_move(d, board, colour);
    }else {
        // int d = Bot::determineDepth(board); //* This will always return 11, as the game stage is 'e'
        return this->middle_game_move(11, board, colour); // TODO: Implement endgame move selection
    }
}
