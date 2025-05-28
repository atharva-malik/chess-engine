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

// det NNUE score from FEN input
float evaluate_fen_nnue(std::string fen)
{
    // call NNUE probe lib function
    // The NNUE score is in centipawns, so we need to divide by 100 to get actual score
    // The NNUE also tends to make the score around twice as large as it should be, so we divide by 2 again
    // to get a more accurate score.
    return nnue_evaluate_fen((char *)fen.c_str())/200.0f;
}
