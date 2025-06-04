#include "nnue_eval.cpp"

float Bot::negamax(int depth, float alpha, float beta, Board& board){
    std::pair<GameResultReason, GameResult> isGameOver = board.isGameOver();
    if (isGameOver.first == GameResultReason::CHECKMATE){
        if (board.sideToMove() == Color::WHITE) return -9999.0f * depth; //* Prefer faster checkmates
        else return -9999.0f * depth; //* Prefer faster checkmates
    } else if (!(isGameOver.first == GameResultReason::NONE)){
        return 0.0f;
    }
    else if (depth == 0) return evaluate_fen_nnue(board.getFen());

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
        if (beta <= alpha) break;  // Beta cutoff
    }
    return best_eval;
}

float Bot::minimax(int depth, float alpha, float beta, bool maximizing_player, Board& board){
    std::pair<GameResultReason, GameResult> isGameOver = board.isGameOver();
    if (isGameOver.first == GameResultReason::CHECKMATE){
        if (board.sideToMove() == Color::WHITE) return -9999.0f;
        else return 9999.0f;
    } else if (!(isGameOver.first == GameResultReason::NONE)){
        return 0.0f;
    }
    else if (depth == 0) return this->eval_mid(board);

    Move move = Move();
    Movelist moves = Movelist();
    movegen::legalmoves(moves, board);
    if (maximizing_player){
        float maxEval = -9999.0f;
        float evaluation = 0;
        order_moves(moves, board);
        for (int i = 0; i < moves.size(); i++){
            move = moves[i];
            board.makeMove(move);
            evaluation = this->minimax(depth - 1, alpha, beta, false, board);
            board.unmakeMove(move);
            maxEval = std::max(maxEval, evaluation);
            alpha = std::max(alpha, evaluation);
            if (beta <= alpha) break;  // Beta cutoff
        }
        return maxEval;
    }
    else{
        float minEval = 9999.0f;
        float evaluation = 0;
        order_moves(moves, board);
        for (int i = 0; i < moves.size(); i++){
            move = moves[i];
            board.makeMove(move);
            evaluation = this->minimax(depth - 1, alpha, beta, true, board);
            board.unmakeMove(move);
            minEval = std::min(minEval, evaluation);
            beta = std::min(beta, evaluation);
            if (beta <= alpha) break;  // Beta cutoff
        }
        return minEval;
    }
}

float Bot::search_move(Move move, Board board, int depth, int colour){
    float evaluation = 0;
    board.makeMove(move);
    evaluation = this->negamax(depth, -9999, 9999, board) * -colour;
    board.unmakeMove(move);
    return evaluation;
}