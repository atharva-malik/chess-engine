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
                e_evaluation = this->minimax_tl(j, -9999, 9999, false, board, eval_mid);
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
                evaluation = this->minimax_tl(lower_depth, -9999, 9999, false, board, eval_mid);
                board.unmakeMove(move);
                if (time_exceeded(this->start_time, this->time_limit)) {
                    return uci::moveToUci(best_move_so_far); // Time limit reached, return best move found so far.
                }
                if (evaluation > e_evaluation) {
                    if (best_eval == 9999.0f) return uci::moveToUci(best_move); //* Break if mate
                    evaluation = this->minimax_tl(lower_depth, -9999, 9999, false, board, eval_mid);
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
                e_evaluation = this->minimax_tl(j, -9999, 9999, true, board, eval_mid);
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
                evaluation = this->minimax_tl(lower_depth, -9999, 9999, true, board, eval_mid);
                board.unmakeMove(move);
                if (time_exceeded(this->start_time, this->time_limit)) {
                    return uci::moveToUci(best_move_so_far); // Time limit reached, return best move found so far.
                }
                if (evaluation < e_evaluation) {
                    evaluation = this->minimax_tl(lower_depth, -9999, 9999, true, board, eval_mid);
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
                e_evaluation = this->minimax_tl(j, -9999, 9999, false, board, eval_mid);
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
                evaluation = this->minimax_tl(lower_depth, -9999, 9999, false, board, eval_mid);
                board.unmakeMove(move);
                if (time_exceeded(this->start_time, this->time_limit)) {
                    return uci::moveToUci(best_move_so_far); // Time limit reached, return best move found so far.
                }
                if (evaluation > e_evaluation) {
                    if (best_eval == 9999.0f) return uci::moveToUci(best_move); //* Break if mate
                    evaluation = this->minimax_tl(lower_depth, -9999, 9999, false, board, eval_mid);
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
                e_evaluation = this->minimax_tl(j, -9999, 9999, true, board, eval_mid);
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
                evaluation = this->minimax_tl(lower_depth, -9999, 9999, true, board, eval_mid);
                board.unmakeMove(move);
                if (time_exceeded(this->start_time, this->time_limit)) {
                    return uci::moveToUci(best_move_so_far); // Time limit reached, return best move found so far.
                }
                if (evaluation < e_evaluation) {
                    evaluation = this->minimax_tl(lower_depth, -9999, 9999, true, board, eval_mid);
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

// std::string Bot::middle_game_move(int depth, Board& board, char colour){
//     float best_eval;
//     Move best_move = Move();
//     Movelist moves = Movelist();
//     movegen::legalmoves(moves, board);
//     float evaluation;
//     float e_evaluation;
//     Move move = Move();
//     int move_count = moves.size();
//     int iterator_max = move_count < 4 ? move_count : 4; //* Strike a good middle ground between risk and reward

//     if (move_count == 1) {
//         return uci::moveToUci(moves[0]);
//     }

//     Bot::order_moves(moves, board);

//     if (colour == 'w') {
//         best_eval = -99999999999.0f;
//         for (int i=0; i<iterator_max; i++){
//             move = moves[i];
//             board.makeMove(move);
//             e_evaluation = this->minimax(depth, -9999, 9999, false, board);
//             board.unmakeMove(move);
//             if (e_evaluation > best_eval) {
//                 best_eval = e_evaluation;
//                 best_move = move;
//                 if (best_eval == 9999.0f) return uci::moveToUci(best_move); //* Break if mate
//             }
//         }

//         e_evaluation = best_eval;
//         best_eval = -99999999999.0f;
//         for (int i=iterator_max; i<move_count; i++){
//             move = moves[i];
//             board.makeMove(move);
//             evaluation = this->minimax(depth-2, -9999, 9999, false, board);
//             board.unmakeMove(move);
//             if (evaluation > e_evaluation) {
//                 evaluation = this->minimax(depth, -9999, 9999, false, board);
//                 if (evaluation > e_evaluation){
//                     best_eval = evaluation;
//                     e_evaluation = evaluation;
//                     best_move = move;
//                 }
//                 if (best_eval == 9999.0f) return uci::moveToUci(best_move); //* Break if mate
//             }
//         }
//     }
//     else {
//         best_eval = 99999999999.0f;
//         for (int i=0; i<iterator_max; i++){
//             Move move = moves[i];
//             board.makeMove(move);
//             e_evaluation = this->minimax(depth, -9999, 9999, true, board);
//             board.unmakeMove(move);
//             if (e_evaluation < best_eval) {
//                 best_eval = e_evaluation;
//                 best_move = move;
//                 if (best_eval == -9999.0f) return uci::moveToUci(best_move); //* Break if mate
//             }
//         }

//         e_evaluation = best_eval;
//         best_eval = 99999999999.0f;
//         for (int i=iterator_max; i<move_count; i++){
//             Move move = moves[i];
//             board.makeMove(move);
//             evaluation = this->minimax(depth-2, -9999, 9999, true, board);
//             board.unmakeMove(move);
//             if (evaluation < e_evaluation) {
//                 evaluation = this->minimax(depth-2, -9999, 9999, true, board);
//                 if (evaluation < e_evaluation){
//                     best_eval = evaluation;
//                     e_evaluation = evaluation;
//                     best_move = move;
//                 }
//                 if (best_eval == -9999.0f) return uci::moveToUci(best_move); //* Break if mate
//             }
//         }
//     }
//     return uci::moveToUci(best_move);
// }

double square(int x) {
    // Simulate some computation
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    double result = static_cast<double>(x) * x;
    // std::cout << "Thread " << std::this_thread::get_id() << " calculated square of " << x << " is " << result << std::endl;
    return result;
}

std::string Bot::mid_40_thread(int depth, Board& board, char colour){
    // float best_eval;
    // Move best_move = Move();
    // Movelist moves = Movelist();
    // movegen::legalmoves(moves, board);
    // float evaluation;
    // Move move = Move();

    // if (colour == 'w') {
    //     best_eval = -99999999999.0f;
    //     order_moves(moves, board);
    //     for (int i = 0; i < moves.size(); i++) {
    //         move = moves[i];
    //         evaluation = this->search_move(move, board, depth, 1);
    //         if (evaluation > best_eval) {
    //             best_eval = evaluation;
    //             best_move = move;
    //             if (best_eval == 9999.0f) break; //* Break if mate
    //         }
    //     }
    // }
    // else {
    //     best_eval = 99999999999.0f;
    //     order_moves(moves, board);
    //     for (int i = 0; i < moves.size(); i++){
    //         Move move = moves[i];
    //         evaluation = this->search_move(move, board, depth, 1);
    //         if (evaluation < best_eval){
    //             best_eval = evaluation;
    //             best_move = move;
    //             if (best_eval == -9999.0f) break; //* Break if mate
    //         }
    //     }
    // }
    // return uci::moveToUci(best_move);

    Movelist moves = Movelist();
    movegen::legalmoves(moves, board);
    order_moves(moves, board);
    
    float evaluation;
    Move move = Move();

    const int num_iterations = moves.size();
    std::vector<std::thread> threads;
    std::vector<std::pair<double, std::string>> data(num_iterations); // Vector to store results and moves
    std::mutex results_mutex; // Mutex to protect access to the results vector.  IMPORTANT!

    // Create and start a thread for each iteration.
    for (int i = 0; i < num_iterations; ++i) {
        threads.emplace_back(
            // Use a lambda function to capture 'i' by value and handle the result.
            [&, i]() {
                float result = this->search_move(moves[i], board, depth, 1); // Calculate the result
                // Use a lock_guard to ensure thread-safe access to the results vector.
                std::lock_guard<std::mutex> guard(results_mutex);
                data[i].first = result; // Store the result in the correct position.
                data[i].second = uci::moveToUci(moves[i]); // Store the move in the correct position.
            }
        );
        // std::cout << "Thread " << threads.back().get_id() << " started for iteration " << i << std::endl;
    }

    // Wait for all threads to complete.
    std::cout << "Waiting for threads to finish...\n";
    for (auto& thread : threads) {
        if (thread.joinable())
            thread.join();
    }
    std::cout << "All threads finished.\n";

    if (colour == 'w') {
        std::sort(data.begin(), data.end(), [](auto &a, auto &b) {
            return a.first > b.first;
        });

    } else {
        std::sort(data.begin(), data.end(), [](auto &a, auto &b) {
            return a.first < b.first;
        });
    }
    // // Print the results.
    // std::cout << "Results: ";
    // for (auto result : data) {
    //     std::cout << result.first << " ";
    //     std::cout << result.second << std::endl;
    // }
    // std::cout << std::endl;

    return data[0].second;
}

std::string Bot::mid_x_threads(int depth, Board& board, char colour){
    Movelist moves = Movelist();
    movegen::legalmoves(moves, board);
    order_moves(moves, board);
    
    float evaluation;
    Move move = Move();

    const int num_iterations = 12; // Number of iterations to run in parallel
    std::vector<std::thread> threads;
    std::vector<double> results(num_iterations); // Vector to store results
    std::mutex results_mutex; // Mutex to protect access to the results vector.  IMPORTANT!

    // Create and start a thread for each iteration.
    for (int i = 0; i < num_iterations; ++i) {
        threads.emplace_back(
            // Use a lambda function to capture 'i' by value and handle the result.
            [&, i]() {
                float result = this->search_x_moves(moves, board, depth, 1, i*3, 3); // Calculate the result
                // Use a lock_guard to ensure thread-safe access to the results vector.
                std::lock_guard<std::mutex> guard(results_mutex);
                results[i] = result; // Store the result in the correct position.
            }
        );
        // std::cout << "Thread " << threads.back().get_id() << " started for iteration " << i << std::endl;
    }

    // Wait for all threads to complete.
    std::cout << "Waiting for threads to finish...\n";
    for (auto& thread : threads) {
        if (thread.joinable())
            thread.join();
    }
    std::cout << "All threads finished.\n";

    // Print the results.
    std::cout << "Results: ";
    for (double result : results) {
        std::cout << result << " ";
    }
    std::cout << std::endl;

    // Calculate the sum of the results using the accumulator
    double sum = std::accumulate(results.begin(), results.end(), 0.0);
    // std::cout << "Sum of results: " << sum << std::endl;

    return "asdf";
}

std::string Bot::middle_game_move(int depth, Board& board, char colour){
    float best_eval;
    Move best_move = Move();
    Movelist moves = Movelist();
    movegen::legalmoves(moves, board);
    float evaluation;
    Move move = Move();

    if (colour == 'w') {
        best_eval = -99999999999.0f;
        order_moves(moves, board);
        for (int i = 0; i < moves.size(); i++) {
            move = moves[i];
            // Bot::int_move(move, board);
            board.makeMove(move);
            // evaluation = this->minimax(depth, -9999, 9999, false, board);
            evaluation = -this->negamax(depth, -9999, 9999, board);
            // Bot::int_unmove(move, board);
            board.unmakeMove(move);
            if (evaluation > best_eval) {
                best_eval = evaluation;
                best_move = move;
                if (best_eval == 9999.0f) break; //* Break if mate
            }
        }
    }
    else {
        best_eval = 99999999999.0f;
        order_moves(moves, board);
        for (int i = 0; i < moves.size(); i++){
            Move move = moves[i];
            // Bot::int_move(move, board);
            board.makeMove(move);
            // evaluation = this->minimax(depth, -9999, 9999, true, board);
            evaluation = this->negamax(depth, -9999, 9999, board);
            // Bot::int_unmove(move, board);
            board.unmakeMove(move);
            if (evaluation < best_eval){
                best_eval = evaluation;
                best_move = move;
                if (best_eval == -9999.0f) break; //* Break if mate
            }
        }
    }
    return uci::moveToUci(best_move);
}
