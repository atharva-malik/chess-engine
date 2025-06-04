float Bot::minimax_tl(int depth, float alpha, float beta, bool maximizing_player, Board& board, std::function<float(Board)> evaluate){
    std::pair<GameResultReason, GameResult> isGameOver = board.isGameOver();
    if (isGameOver.first == GameResultReason::CHECKMATE){
        if (board.sideToMove() == Color::WHITE) return -9999.0f;
        else return 9999.0f;
    } else if (!(isGameOver.first == GameResultReason::NONE)){
        return 0.0f;
    }
    // else if (depth == 0) return Bot::quiescence(board, alpha, beta, Bot::volatility(board));
    if (time_exceeded(this->start_time, this->time_limit)) return 1234.567f; //* Time limit reached, return a flag value.
    else if (depth == 0) {
        uint64_t hash = board.hash();
        if (transpositionTable.find(hash) != transpositionTable.end()){
            return transpositionTable[hash];
        }
        float eval = quiescence_tl(board, maximizing_player, alpha, beta, evaluate);
        transpositionTable[hash] = eval;
        return eval;
    }

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
            evaluation = this->minimax(depth - 1, alpha, beta, false, board, evaluate);
            board.unmakeMove(move);
            maxEval = std::max(maxEval, evaluation);
            alpha = std::max(alpha, evaluation);
            if (beta <= alpha) {killer_moves.push_back(move);break;};  // Beta cutoff
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
            evaluation = this->minimax(depth - 1, alpha, beta, true, board, evaluate);
            board.unmakeMove(move);
            minEval = std::min(minEval, evaluation);
            beta = std::min(beta, evaluation);
            if (beta <= alpha) {killer_moves.push_back(move);break;};  // Beta cutoff
        }
        return minEval;
    }
}

float Bot::minimax(int depth, float alpha, float beta, bool maximizing_player, Board& board, std::function<float(Board)> evaluate){
    std::pair<GameResultReason, GameResult> isGameOver = board.isGameOver();
    if (isGameOver.first == GameResultReason::CHECKMATE){
        if (board.sideToMove() == Color::WHITE) return -9999.0f;
        else return 9999.0f;
    } else if (!(isGameOver.first == GameResultReason::NONE)){
        return 0.0f;
    }
    // else if (depth == 0) return Bot::quiescence(board, alpha, beta, Bot::volatility(board));
    else if (depth == 0) {
        uint64_t hash = board.hash();
        if (transpositionTable.find(hash) != transpositionTable.end()){
            return transpositionTable[hash];
        }
        float eval = quiescence(board, maximizing_player, alpha, beta, evaluate);
        transpositionTable[hash] = eval;
        return eval;
    }

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
            evaluation = this->minimax(depth - 1, alpha, beta, false, board, evaluate);
            board.unmakeMove(move);
            maxEval = std::max(maxEval, evaluation);
            alpha = std::max(alpha, evaluation);
            if (beta <= alpha) {killer_moves.push_back(move);break;};  // Beta cutoff
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
            evaluation = this->minimax(depth - 1, alpha, beta, true, board, evaluate);
            board.unmakeMove(move);
            minEval = std::min(minEval, evaluation);
            beta = std::min(beta, evaluation);
            if (beta <= alpha) {killer_moves.push_back(move);break;};  // Beta cutoff
        }
        return minEval;
    }
}

float Bot::quiescence_tl(Board& board, bool maximizing_player, float alpha, float beta, std::function<float(Board)> evaluate) {
    float stand_pat = evaluate(board); // Static evaluation

    if (time_exceeded(this->start_time, this->time_limit)) return 1234.567f; //* Time limit reached, return a flag value.

    if (stand_pat >= beta) {
        return beta;
    }
    if (alpha < stand_pat) {
        alpha = stand_pat;
    }

    // std::vector<Move> captures = this->generateCaptures(board); // Generate capture moves.
    // std::vector<Move> checks = this->generateChecks(board); // Generate check moves.
    Movelist captures = Movelist();
    movegen::legalmoves<movegen::MoveGenType::CAPTURE>(captures, board);

    if (maximizing_player){
        float maxEval = -9999.0f;
        float evaluation = 0;
        order_moves(captures, board);
        for (const auto& move : captures){
            board.makeMove(move);
            evaluation = this->quiescence_tl(board, false, alpha, beta, evaluate);
            board.unmakeMove(move);
            maxEval = std::max(maxEval, evaluation);
            alpha = std::max(alpha, evaluation);
            if (beta <= alpha) {killer_moves.push_back(move);break;};  // Beta cutoff
        }
        return maxEval;
    }
    else{
        float minEval = 9999.0f;
        float evaluation = 0;
        order_moves(captures, board);
        for (const auto& move : captures){
            board.makeMove(move);
            evaluation = this->quiescence_tl(board, true, alpha, beta, evaluate);
            board.unmakeMove(move);
            minEval = std::min(minEval, evaluation);
            beta = std::min(beta, evaluation);
            if (beta <= alpha) {killer_moves.push_back(move);break;};  // Beta cutoff
        }
        return minEval;
    }
    return stand_pat;
}

float Bot::quiescence(Board& board, bool maximizing_player, float alpha, float beta, std::function<float(Board)> evaluate) {
    float stand_pat = evaluate(board); // Static evaluation
    if (stand_pat >= beta) {
        return beta;
    }
    if (alpha < stand_pat) {
        alpha = stand_pat;
    }

    // std::vector<Move> captures = this->generateCaptures(board); // Generate capture moves.
    // std::vector<Move> checks = this->generateChecks(board); // Generate check moves.
    Movelist captures = Movelist();
    movegen::legalmoves<movegen::MoveGenType::CAPTURE>(captures, board);

    if (maximizing_player){
        float maxEval = -9999.0f;
        float evaluation = 0;
        order_moves(captures, board);
        for (const auto& move : captures){
            board.makeMove(move);
            evaluation = this->quiescence_tl(board, false, alpha, beta, evaluate);
            board.unmakeMove(move);
            maxEval = std::max(maxEval, evaluation);
            alpha = std::max(alpha, evaluation);
            if (beta <= alpha) {killer_moves.push_back(move);break;};  // Beta cutoff
        }
        return maxEval;
    }
    else{
        float minEval = 9999.0f;
        float evaluation = 0;
        order_moves(captures, board);
        for (const auto& move : captures){
            board.makeMove(move);
            evaluation = this->quiescence_tl(board, true, alpha, beta, evaluate);
            board.unmakeMove(move);
            minEval = std::min(minEval, evaluation);
            beta = std::min(beta, evaluation);
            if (beta <= alpha) {killer_moves.push_back(move);break;};  // Beta cutoff
        }
        return minEval;
    }
    return stand_pat;
}