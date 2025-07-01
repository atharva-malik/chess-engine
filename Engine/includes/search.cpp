/**
 *  @file search.cpp
 *  @brief Implements core move search algorithms for the chess engine using minimax and negamax strategies.
 *
 ** This file contains recursive depth-limited search functions—negamax and minimax—with alpha-beta pruning,
 ** designed to evaluate possible move sequences and return optimal scoring lines. Also includes a wrapper
 ** function for evaluating a single candidate move.
 *
 ** Evaluation functions may delegate to NNUE-based or heuristic scoring depending on phase and configuration.
*/

#include "nnue_eval.cpp"

float Bot::negamax(int depth, float alpha, float beta, Board& board){
    /**
     *  @brief Negamax search with alpha-beta pruning.
     *
     **  Recursively explores the game tree using negamax, a variant of minimax where the evaluation
     **  is always from the current player's perspective (negated recursively).
     **  Alpha-beta pruning is applied to improve performance by eliminating branches that won't influence the result.
     *
     *  @param depth Remaining depth to search.
     *  @param alpha Best score that the maximizing player is guaranteed to achieve.
     *  @param beta Best score that the minimizing player is guaranteed to allow.
     *  @param board The current board position.
     *  @return A float evaluation score from the current player's perspective.
     *
     *  @note Returns large negative values for checkmate, and 0 for non-checkmate game results.
     *  @see evaluate_fen_nnue
    */
    std::pair<GameResultReason, GameResult> isGameOver = board.isGameOver();
    if (isGameOver.first == GameResultReason::CHECKMATE){
        return -9999.0f * depth; //* Prefer faster checkmates
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
    /**
     *  @brief Minimax search with alpha-beta pruning.
     *
     ** Recursively explores the game tree using minimax algorithm with alpha-beta pruning.
     ** Alternates between maximizing and minimizing players to find the optimal move.
     *
     *  @param depth Remaining depth to search in the game tree.
     *  @param alpha Best score that the maximizing player is guaranteed to achieve.
     *  @param beta Best score that the minimizing player is guaranteed to allow.
     *  @param maximizing_player Flag indicating whether current player is maximizing or minimizing.
     *  @param board The current board position.
     *  @return A float evaluation score representing the best possible outcome.
     *
     *  @note Returns large positive/negative values for checkmate, and 0 for non-checkmate game results.
     *  @note Uses move ordering and alpha-beta pruning to improve search efficiency.
    */

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
    /**
     *  @brief Evaluates a specific move using negamax search algorithm.
     *
     ** Performs a negamax search for a given move at a specified search depth,
     ** taking into account the color perspective of the current player.
     *
     *  @param move The chess move to evaluate
     *  @param board The current board state
     *  @param depth The maximum search depth for the evaluation
     *  @param colour The color perspective (+1 for current player, -1 for opponent)
     *  @return A float representing the evaluation score of the move
    */

    float evaluation = 0;
    board.makeMove(move);
    evaluation = this->negamax(depth, -9999, 9999, board) * -colour;
    board.unmakeMove(move);
    return evaluation;
}