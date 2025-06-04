#include "nnue_eval.cpp"

float Bot::negamax(int depth, float alpha, float beta, Board& board){
    std::pair<GameResultReason, GameResult> isGameOver = board.isGameOver();
    if (isGameOver.first == GameResultReason::CHECKMATE){
        if (board.sideToMove() == Color::WHITE) return -9999.0f * depth; //* Prefer faster checkmates
        else return -9999.0f * depth; //* Prefer faster checkmates
    } else if (!(isGameOver.first == GameResultReason::NONE)){
        return 0.0f;
    }
    else if (depth == 0) {
        uint64_t hash = board.hash();
        if (transpositionTable.find(hash) != transpositionTable.end()){
            return transpositionTable[hash];
        }
        float eval = evaluate_fen_nnue(board.getFen());
        transpositionTable[hash] = eval;
        return eval;
    }

    Move move = Move();
    Movelist moves = Movelist();
    movegen::legalmoves(moves, board);
    float best_eval = -999999999999.9f;
    float evaluation = 0;
    order_moves(moves, board);
    for (auto move : moves) {
        // move = moves[i];
        board.makeMove(move);
        evaluation = -this->negamax(depth - 1, -beta, -alpha, board);
        board.unmakeMove(move);
        best_eval = std::max(best_eval, evaluation);
        alpha = std::max(alpha, evaluation);
        if (beta <= alpha) {killer_moves.push_back(move);break;};  // Beta cutoff
    }
    return best_eval;
}

float Bot::search_move(Move move, Board board, int depth, int colour){
    float evaluation = 0;
    board.makeMove(move);
    evaluation = this->negamax(depth, -9999, 9999, board) * -colour;
    board.unmakeMove(move);
    return evaluation;
}