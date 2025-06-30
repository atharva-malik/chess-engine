/**
 *  @file nnue_eval.cpp
 *  @brief Provides wrapper functions for evaluating chess positions using an embedded NNUE network.
 *
 ** This file serves as the interface between the chess engine and an external NNUE (Efficiently Updatable Neural Network)
 ** evaluator. It supports initialization and evaluation of positions using piece/square data arrays or a full FEN string.
 ** Returned scores are normalized for better heuristic integration within the engine's evaluation framework.
 *
 *? Features:
 *? - Lightweight wrappers over lower-level NNUE probing functions
 *? - Score normalization from centipawns to pawn units
 *? - FEN-based direct evaluation support for easy debugging or position analysis
 *
 *  @note The scores returned by `evaluate_fen_nnue` are halved for scaling compatibility with classical evaluation.
*/

#include "NNUE/nnue.cpp"
#include "NNUE/misc.cpp"

// init NNUE
void init_nnue(char *filename)
{
    // call NNUE probe lib function
    nnue_init(filename);
}

// get NNUE score directly
int evaluate_nnue(int player, int *pieces, int *squares)
{
    // call NNUE probe lib function
    return nnue_evaluate(player, pieces, squares);
}

// get NNUE score from FEN input
float evaluate_fen_nnue(std::string fen)
{
    // call NNUE probe lib function
    // The NNUE score is in centipawns, so we need to divide by 100 to get actual score
    // The NNUE also tends to make the score around twice as large as it should be, so we divide by 2 again
    // to get a more accurate score.
    return nnue_evaluate_fen((char *)fen.c_str())/200.0f;
}
