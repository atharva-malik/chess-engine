/**
 *  @file bot.cpp
 *  @brief Core implementation of the Bot class responsible for chess engine decision-making.
 *
 *? This source file composes the main logic of the chess engine by integrating functionality 
 *? across several modular components, including:
 *?  - constructors.cpp: Bot class constructors and FEN initialisation.
 *?  - evaluate.cpp: Positional and material evaluation routines.
 *?  - openings.cpp: Opening book parsing and selection.
 *?  - search.cpp: Search algorithms (e.g., negamax and minimax) with pruning techniques.
 *?  - bothelpers.cpp: Utility functions for move ordering, checks, and evaluations.
 *?  - findmove.cpp: Interfaces to determine and return the best move from the current position.
 *
 ** Also includes the definition for Bot::get_best_move, a high-level dispatcher that selects 
 ** the appropriate strategy (opening, middlegame, or endgame) based on game phase and board state.
*/

#include "constructors.cpp"
#include "evaluate.cpp"
#include "openings.cpp"
#include "search.cpp"
#include "bothelpers.cpp"
#include "findmove.cpp"

std::string Bot::get_best_move(Board& board, char colour, int depth=-1) {
    /**
     *  @brief Selects and returns the best move for the given board and player.
     *
     *? This function acts as a dispatcher that chooses the appropriate move generation
     *? strategy based on the current game phase:
     *? - 'o' (opening): Uses preloaded opening book data if available.
     *? - 'm' (middlegame): Applies multi-threaded search using evaluation heuristics.
     *? - otherwise (endgame): Falls back to deeper middlegame logic.
     *
     ** If a custom depth is not specified (depth = -1), the engine determines an appropriate
     ** search depth dynamically based on the board state.
     *
     *  @param board  Reference to the current board state.
     *  @param colour The player to move ('w' for white, 'b' for black).
     *  @param depth  Optional search depth override. Defaults to -1 (auto-determined).
     *  @return A string representing the best move in UCI format.
    */

    if (this->game_stage == 'o') return Bot::opening_move(board.getFen(), colour);
    else if (this->game_stage == 'm'){
        int d = depth == -1 ? Bot::determineDepth(board) : depth;
        return this->middle_game_x_thread(d, board, colour);
    }else {
        return this->middle_game_x_thread(11, board, colour);
    }
}
