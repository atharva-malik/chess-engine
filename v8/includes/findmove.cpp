std::string Bot::middle_game_move_tl(int depth, Board& board, char colour){
    float best_eval;
    float best_eval_so_far;
    Movelist moves = Movelist();
    Move best_move = Move();
    movegen::legalmoves(moves, board);
    std::vector<std::pair<int, chess::Move>> moves_eval = {};
    std::pair<int, chess::Move> temp;
    Move best_move_so_far = moves[0];
    float evaluation;
    float e_evaluation;
    Move move = Move();
    int move_count = moves.size();
    int lower_depth = depth;
    int iterator_max = move_count < 4 ? move_count : 4; //* Strike a good middle ground between risk and reward

    std::function<float(chess::Board)> eval_mid = [this](chess::Board b) { return this->eval_mid(b); };

    if (move_count == 1) {
        return uci::moveToUci(moves[0]);
    }

    Bot::order_moves(moves, board);

    if (colour == 'w') {
        best_eval_so_far = -99999999999.0f;
        for (int j=1; j <= depth; j+=2){
            best_eval = -99999999999.0f;
            Bot::order_moves_eval(moves, moves_eval);
            moves_eval.clear();

            //* search the first 4 moves with full depth as they are most likely to be good *//
            for (int i=0; i<iterator_max; i++){
                move = moves[i];
                board.makeMove(move);
                e_evaluation = this->minimax(j, -9999, 9999, false, board, eval_mid);
                board.unmakeMove(move);
                temp.first = e_evaluation;
                temp.second = move;
                moves_eval.push_back(temp);
                if (time_exceeded(this->start_time, this->time_limit)) {
                    return uci::moveToUci(best_move_so_far); // Time limit reached, return best move found so far.
                }
                if (e_evaluation > best_eval) {
                    if (best_eval == 9999.0f) return uci::moveToUci(best_move); //* Break if mate
                    best_eval = e_evaluation;
                    best_move = move;
                }
            }

            e_evaluation = best_eval;
            if (best_eval > best_eval_so_far) {
                best_eval_so_far = best_eval;
                best_move_so_far = best_move;
            }

            //* search the rest at a lower depth, but full depth if they turn out to be decent *//
            best_eval = -99999999999.0f;
            lower_depth = depth > 2 ? depth - 2 : 3;
            for (int i=iterator_max; i<move_count; i++){
                move = moves[i];
                board.makeMove(move);
                evaluation = this->minimax(lower_depth, -9999, 9999, false, board, eval_mid);
                board.unmakeMove(move);
                if (time_exceeded(this->start_time, this->time_limit)) {
                    return uci::moveToUci(best_move_so_far); // Time limit reached, return best move found so far.
                }
                if (evaluation > e_evaluation) {
                    if (best_eval == 9999.0f) return uci::moveToUci(best_move); //* Break if mate
                    evaluation = this->minimax(lower_depth, -9999, 9999, false, board, eval_mid);
                    if (evaluation > e_evaluation){
                        best_eval = evaluation;
                        e_evaluation = evaluation;
                        best_move = move;
                        temp.first = evaluation;
                        temp.second = move;
                        moves_eval.push_back(temp);
                    }
                }else{
                    temp.first = evaluation;
                    temp.second = move;
                    moves_eval.push_back(temp);
                }
            }

            if (best_eval > best_eval_so_far) {
                best_eval_so_far = best_eval;
                best_move_so_far = best_move;
            }
        }
    }
    else {
        best_eval_so_far = 99999999999.0f;
        for (int j=1; j <= depth; j+=2){
            best_eval = 99999999999.0f;
            Bot::order_moves_eval(moves, moves_eval);
            moves_eval.clear();

            //* search the first 4 moves with full depth as they are most likely to be good *//
            for (int i=0; i<iterator_max; i++){
                Move move = moves[i];
                board.makeMove(move);
                e_evaluation = this->minimax(j, -9999, 9999, true, board, eval_mid);
                board.unmakeMove(move);
                temp.first = e_evaluation;
                temp.second = move;
                moves_eval.push_back(temp);
                if (time_exceeded(this->start_time, this->time_limit)) {
                    return uci::moveToUci(best_move_so_far); // Time limit reached, return best move found so far.
                }
                if (e_evaluation < best_eval) {
                    if (best_eval == -9999.0f) return uci::moveToUci(best_move); //* Break if mate
                    best_eval = e_evaluation;
                    best_move = move;
                }
            }

            e_evaluation = best_eval;
            if (best_eval < best_eval_so_far) {
                best_eval_so_far = best_eval;
                best_move_so_far = best_move;
            }

            //* search the rest at a lower depth, but full depth if they turn out to be decent *//
            best_eval = 99999999999.0f;
            lower_depth = depth > 2 ? depth - 2 : 3;
            for (int i=iterator_max; i<move_count; i++){
                Move move = moves[i];
                board.makeMove(move);
                evaluation = this->minimax(lower_depth, -9999, 9999, true, board, eval_mid);
                board.unmakeMove(move);
                if (time_exceeded(this->start_time, this->time_limit)) {
                    return uci::moveToUci(best_move_so_far); // Time limit reached, return best move found so far.
                }
                if (evaluation < e_evaluation) {
                    evaluation = this->minimax(lower_depth, -9999, 9999, true, board, eval_mid);
                    if (best_eval == -9999.0f) return uci::moveToUci(best_move); //* Break if mate
                    if (evaluation < e_evaluation){
                        best_eval = evaluation;
                        e_evaluation = evaluation;
                        best_move = move;
                        temp.first = evaluation;
                        temp.second = move;
                        moves_eval.push_back(temp);
                    }
                }else{
                    temp.first = evaluation;
                    temp.second = move;
                    moves_eval.push_back(temp);
                }
            }

            if (best_eval < best_eval_so_far) {
                best_eval_so_far = best_eval;
                best_move_so_far = best_move;
            }
        }
    }
    return uci::moveToUci(best_move_so_far);
}

std::string Bot::end_game_move_tl(int depth, Board& board, char colour){
    float best_eval;
    float best_eval_so_far;
    Movelist moves = Movelist();
    Move best_move = Move();
    movegen::legalmoves(moves, board);
    std::vector<std::pair<int, chess::Move>> moves_eval = {};
    std::pair<int, chess::Move> temp;
    Move best_move_so_far = moves[0];
    float evaluation;
    float e_evaluation;
    Move move = Move();
    int move_count = moves.size();
    int lower_depth = depth;
    int iterator_max = move_count < 4 ? move_count : 4; //* Strike a good middle ground between risk and reward

    std::function<float(chess::Board)> eval_mid = [this](chess::Board b) { return this->eval_end(b); };

    if (move_count == 1) {
        return uci::moveToUci(moves[0]);
    }

    Bot::order_moves(moves, board);

    if (colour == 'w') {
        best_eval_so_far = -99999999999.0f;
        for (int j=1; j <= depth; j+=2){
            best_eval = -99999999999.0f;
            Bot::order_moves_eval(moves, moves_eval);
            moves_eval.clear();

            //* search the first 4 moves with full depth as they are most likely to be good *//
            for (int i=0; i<iterator_max; i++){
                move = moves[i];
                board.makeMove(move);
                e_evaluation = this->minimax(j, -9999, 9999, false, board, eval_mid);
                board.unmakeMove(move);
                temp.first = e_evaluation;
                temp.second = move;
                moves_eval.push_back(temp);
                if (time_exceeded(this->start_time, this->time_limit)) {
                    return uci::moveToUci(best_move_so_far); // Time limit reached, return best move found so far.
                }
                if (e_evaluation > best_eval) {
                    if (best_eval == 9999.0f) return uci::moveToUci(best_move); //* Break if mate
                    best_eval = e_evaluation;
                    best_move = move;
                }
            }

            e_evaluation = best_eval;
            if (best_eval > best_eval_so_far) {
                best_eval_so_far = best_eval;
                best_move_so_far = best_move;
            }

            //* search the rest at a lower depth, but full depth if they turn out to be decent *//
            best_eval = -99999999999.0f;
            lower_depth = depth > 2 ? depth - 2 : 3;
            for (int i=iterator_max; i<move_count; i++){
                move = moves[i];
                board.makeMove(move);
                evaluation = this->minimax(lower_depth, -9999, 9999, false, board, eval_mid);
                board.unmakeMove(move);
                if (time_exceeded(this->start_time, this->time_limit)) {
                    return uci::moveToUci(best_move_so_far); // Time limit reached, return best move found so far.
                }
                if (evaluation > e_evaluation) {
                    if (best_eval == 9999.0f) return uci::moveToUci(best_move); //* Break if mate
                    evaluation = this->minimax(lower_depth, -9999, 9999, false, board, eval_mid);
                    if (evaluation > e_evaluation){
                        best_eval = evaluation;
                        e_evaluation = evaluation;
                        best_move = move;
                        temp.first = evaluation;
                        temp.second = move;
                        moves_eval.push_back(temp);
                    }
                }else{
                    temp.first = evaluation;
                    temp.second = move;
                    moves_eval.push_back(temp);
                }
            }
            if (best_eval > best_eval_so_far) {
                best_eval_so_far = best_eval;
                best_move_so_far = best_move;
            }
        }
    }
    else {
        best_eval_so_far = 99999999999.0f;
        for (int j=1; j <= depth; j+=2){
            best_eval = 99999999999.0f;
            Bot::order_moves_eval(moves, moves_eval);
            moves_eval.clear();

            //* search the first 4 moves with full depth as they are most likely to be good *//
            for (int i=0; i<iterator_max; i++){
                Move move = moves[i];
                board.makeMove(move);
                e_evaluation = this->minimax(j, -9999, 9999, true, board, eval_mid);
                board.unmakeMove(move);
                temp.first = e_evaluation;
                temp.second = move;
                moves_eval.push_back(temp);
                if (time_exceeded(this->start_time, this->time_limit)) {
                    return uci::moveToUci(best_move_so_far); // Time limit reached, return best move found so far.
                }
                if (e_evaluation < best_eval) {
                    if (best_eval == -9999.0f) return uci::moveToUci(best_move); //* Break if mate
                    best_eval = e_evaluation;
                    best_move = move;
                }
            }

            e_evaluation = best_eval;
            if (best_eval < best_eval_so_far) {
                best_eval_so_far = best_eval;
                best_move_so_far = best_move;
            }

            //* search the rest at a lower depth, but full depth if they turn out to be decent *//
            best_eval = 99999999999.0f;
            lower_depth = depth > 2 ? depth - 2 : 3;
            for (int i=iterator_max; i<move_count; i++){
                Move move = moves[i];
                board.makeMove(move);
                evaluation = this->minimax(lower_depth, -9999, 9999, true, board, eval_mid);
                board.unmakeMove(move);
                if (time_exceeded(this->start_time, this->time_limit)) {
                    return uci::moveToUci(best_move_so_far); // Time limit reached, return best move found so far.
                }
                if (evaluation < e_evaluation) {
                    evaluation = this->minimax(lower_depth, -9999, 9999, true, board, eval_mid);
                    if (best_eval == -9999.0f) return uci::moveToUci(best_move); //* Break if mate
                    if (evaluation < e_evaluation){
                        best_eval = evaluation;
                        e_evaluation = evaluation;
                        best_move = move;
                        temp.first = evaluation;
                        temp.second = move;
                        moves_eval.push_back(temp);
                    }
                }else{
                    temp.first = evaluation;
                    temp.second = move;
                    moves_eval.push_back(temp);
                }
            }

            if (best_eval < best_eval_so_far) {
                best_eval_so_far = best_eval;
                best_move_so_far = best_move;
            }
        }
    }
    return uci::moveToUci(best_move_so_far);
}

std::string Bot::middle_game_move(int depth, Board& board, char colour){
    float best_eval;
    Move best_move = Move();
    Movelist moves = Movelist();
    movegen::legalmoves(moves, board);
    float evaluation;
    float e_evaluation;
    Move move = Move();
    int move_count = moves.size();
    int iterator_max = move_count < 4 ? move_count : 4; //* Strike a good middle ground between risk and reward

    std::function<float(chess::Board)> eval_mid = [this](chess::Board b) { return this->eval_mid(b); };

    if (move_count == 1) {
        return uci::moveToUci(moves[0]);
    }

    Bot::order_moves(moves, board);

    if (colour == 'w') {
        best_eval = -99999999999.0f;
        for (int i=0; i<iterator_max; i++){
            move = moves[i];
            board.makeMove(move);
            e_evaluation = this->minimax(depth, -9999, 9999, false, board, eval_mid);
            board.unmakeMove(move);
            if (e_evaluation > best_eval) {
                best_eval = e_evaluation;
                best_move = move;
                if (best_eval == 9999.0f) return uci::moveToUci(best_move); //* Break if mate
            }
        }

        e_evaluation = best_eval;
        best_eval = -99999999999.0f;
        for (int i=iterator_max; i<move_count; i++){
            move = moves[i];
            board.makeMove(move);
            evaluation = this->minimax(depth-2, -9999, 9999, false, board, eval_mid);
            board.unmakeMove(move);
            if (evaluation > e_evaluation) {
                evaluation = this->minimax(depth, -9999, 9999, false, board, eval_mid);
                if (evaluation > e_evaluation){
                    best_eval = evaluation;
                    e_evaluation = evaluation;
                    best_move = move;
                }
                if (best_eval == 9999.0f) return uci::moveToUci(best_move); //* Break if mate
            }
        }
    }
    else {
        best_eval = 99999999999.0f;
        for (int i=0; i<iterator_max; i++){
            Move move = moves[i];
            board.makeMove(move);
            e_evaluation = this->minimax(depth, -9999, 9999, true, board, eval_mid);
            board.unmakeMove(move);
            if (e_evaluation < best_eval) {
                best_eval = e_evaluation;
                best_move = move;
                if (best_eval == -9999.0f) return uci::moveToUci(best_move); //* Break if mate
            }
        }

        e_evaluation = best_eval;
        best_eval = 99999999999.0f;
        for (int i=iterator_max; i<move_count; i++){
            Move move = moves[i];
            board.makeMove(move);
            evaluation = this->minimax(depth-2, -9999, 9999, true, board, eval_mid);
            board.unmakeMove(move);
            if (evaluation < e_evaluation) {
                evaluation = this->minimax(depth-2, -9999, 9999, true, board, eval_mid);
                if (evaluation < e_evaluation){
                    best_eval = evaluation;
                    e_evaluation = evaluation;
                    best_move = move;
                }
                if (best_eval == -9999.0f) return uci::moveToUci(best_move); //* Break if mate
            }
        }
    }
    return uci::moveToUci(best_move);
}
