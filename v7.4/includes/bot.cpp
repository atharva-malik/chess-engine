#include "bot.h"

//5806 milliseconds
Bot::Bot() {
    this->board.setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    this->game_stage = 'o';
    if (!this->load_openings_data()) {
        this->game_stage = 'm';
    }
}

Bot::Bot(std::string fen) {
    this->board.setFen(fen);
    this->game_stage = 'o';
    if (!this->load_openings_data()) {
        this->game_stage = 'm';
    }
}

Bot::Bot(std::string fen, char game_stage) {
    this->board.setFen(fen);
    this->game_stage = game_stage;
    if (!this->load_openings_data()) {
        this->game_stage = 'm';
    }
}

bool Bot::load_openings_data() {
    std::ifstream file(Bot::OpeningBookPath);
    if (!file.is_open()) {
        Bot::LogToFile("Error: Could not open openings.json");
        return false;
    }

    try {
        this->openings_data = json::parse(file);
        return true;
    } catch (json::parse_error& e) {
        Bot::LogToFile("JSON parse error: "); Bot::LogToFile(e.what());
        return false;
    }
}

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
            lower_depth = depth > 2 ? depth - 2 : 2;
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
            lower_depth = depth > 2 ? depth - 2 : 2;
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
            lower_depth = depth > 2 ? depth - 2 : 2;
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
            lower_depth = depth > 2 ? depth - 2 : 2;
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

std::string Bot::get_opening_move(const std::string& fen, char colour) {
    if (this->openings_data.empty()) { // Check if the JSON data is loaded
        Bot::LogToFile("Error: Openings data not loaded.");
        this->game_stage = 'm';
        return Bot::get_best_move(this->board, colour);
    }
    std::string converted_fen = Bot::convert_fen(fen);
    if (this->openings_data.contains(converted_fen)) {
        std::vector<std::string> moves = this->openings_data[converted_fen].get<std::vector<std::string>>();
        return (moves)[this->get_random_index(moves)];
    } else {
        this->game_stage = 'm';
        return Bot::get_best_move(this->board, colour);
    }
}

std::string Bot::convert_fen(std::string fen) {
    std::string converted_fen;
    converted_fen = fen.erase(fen.length() - 4, 4);
    return converted_fen;
}

int Bot::get_random_index(const std::vector<std::string>& vec){
    if (vec.empty()) {
        std::cerr << "Warning: Vector is empty. Returning 0." << std::endl;
        return 0;
    }

    static std::random_device rd; // Static for persistent engine
    static std::mt19937 gen(rd());  // Static for persistent engine

    std::uniform_int_distribution<> distrib(0, vec.size() - 1);
    return distrib(gen);

}

void Bot::print_board(const std::string& fen) {
    char pieceChars[] = {
        'r', 'n', 'b', 'q', 'k', 'p',
        'R', 'N', 'B', 'Q', 'K', 'P'
    };

    char board[8][8];
    int rank = 0; // Start at rank 0 (white's back rank)
    int file = 0;

    for (char c : fen) {
        if (c == ' ') {
            break;
        } else if (c == '/') {
            rank++; // Increment rank
            file = 0;
        } else if (isdigit(c)) {
            int emptySquares = c - '0';
            for (int i = 0; i < emptySquares; ++i) {
                board[rank][file++] = '.';
            }
        } else {
            for (char piece : pieceChars) {
                if (c == piece) {
                    board[rank][file++] = c;
                    break;
                }
            }
        }
    }

    for (int r = 7; r >= 0; --r) { //reverse the printing of the ranks.
        for (int f = 0; f < 8; ++f) {
            std::cout << board[r][f] << " ";
        }
        std::cout << std::endl;
    }
}

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
        float eval = quiescence_tl(board, alpha, beta, evaluate);
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
        float eval = quiescence(board, alpha, beta, evaluate);
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

float Bot::quiescence_tl(Board& board, float alpha, float beta, std::function<float(Board)> evaluate) {
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

    for (const auto& move : captures) {
        // int capturedPieceValue = piece_values[board.at(move.to())]; // Get captured piece value.

        board.makeMove(move);
        float score = -Bot::quiescence(board, -beta, -alpha, evaluate); // Recursive call.
        board.unmakeMove(move);
        if (score >= beta) {
            return score;
        }
        if (score > alpha) {
            alpha = score;
        }
    }
    return alpha;
}

float Bot::quiescence(Board& board, float alpha, float beta, std::function<float(Board)> evaluate) {
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

    for (const auto& move : captures) {
        // int capturedPieceValue = piece_values[board.at(move.to())]; // Get captured piece value.

        board.makeMove(move);
        float score = -Bot::quiescence(board, -beta, -alpha, evaluate); // Recursive call.
        board.unmakeMove(move);
        if (score >= beta) {
            return score;
        }
        if (score > alpha) {
            alpha = score;
        }
    }
    return alpha;
}

std::vector<Move> Bot::generateCaptures(Board& board) {
    std::vector<Move> captures;
    Move move;
    Movelist moves = Movelist();
    movegen::legalmoves(moves, board);
    for (int i = 0; i < moves.size(); i++) {
        move = moves[i];
        if (!board.isCapture(move)) {
            continue;
        }
        captures.push_back(move);
    }
    return captures;
}

std::vector<Move> Bot::generateChecks(Board& board) {
    std::vector<Move> checks;
    Move move;
    Movelist moves = Movelist();
    movegen::legalmoves(moves, board);
    for (int i = 0; i < moves.size(); i++) {
        move = moves[i];
        if (!Bot::isCheck(move, board)) {
            continue;
        }
        checks.push_back(move);
    }
    return checks;
}

int Bot::volatility(Board& board){
    int volatility;
    Movelist moves = Movelist();
    movegen::legalmoves<movegen::MoveGenType::CAPTURE>(moves, board);
    volatility = moves.size();
    return volatility+4;
}

float Bot::eval_mid(Board board){
    int score = 0;

    Square sq;

    // std::future<float> future_result = std::async(std::launch::async, &Bot::eval_end, this, board);

    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            sq = Square(rank * 8 + file);
            switch (board.at(sq)){
                case 12:
                    // Nothing
                    break;
                case 0:
                    // White Pawn
                    score += this->piece_tables.w_pawn[rank][file];
                    break;
                case 1:
                    // White Knight
                    score += this->piece_tables.w_knight[rank][file];
                    break;
                case 2:
                    // White Bishop
                    score += this->piece_tables.w_bishop[rank][file];
                    break;
                case 3:
                    // White Rook
                    score += this->piece_tables.w_rook[rank][file];
                    break;
                case 4:
                    // White Queen
                    score += this->piece_tables.w_queen[rank][file];
                    break;
                case 5:
                    // White King
                    score += this->piece_tables.w_king_mid[rank][file];
                    break;
                case 6:
                    // Black Pawn
                    score -= this->piece_tables.b_pawn[rank][file];
                    break;
                case 7:
                    // Black Knight
                    score -= this->piece_tables.b_knight[rank][file];
                    break;
                case 8:
                    // Black Bishop
                    score -= this->piece_tables.b_bishop[rank][file];
                    break;
                case 9:
                    // Black Rook
                    score -= this->piece_tables.b_rook[rank][file];
                    break;
                case 10:
                    // Black Queen
                    score -= this->piece_tables.b_queen[rank][file];
                    break;
                case 11:
                    // Black King
                    score -= this->piece_tables.b_king_mid[rank][file];
                    break;
                default:
                    std::cout << "Error" << std::endl;
                    break;
            }
        }
    }
    // float end_eval = future_result.get() * 100.0f;
    //todo: make it hate losing castling rights
    // float phase = Bot::calculate_phase(board);
    // float eval = ((score * (256 - phase)) + (end_eval * phase)) / 256;
    return score/100.0f;
}

float Bot::eval_end(Board board){
    int score = 0;

    Square sq;

    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            sq = Square(rank * 8 + file);
            // As the game gets towards the end of the game, the value of the pieces change
            // They no longer have 'bad' squares except for the king and the pawns. The other
            // pieces should just support these pieces and help them promote.
            // todo: If this makes it worse, rewrite piece tables to be more active
            switch (board.at(sq)){
                case 12:
                    // Nothing
                    break;
                case 0:
                    // White Pawn
                    score += this->piece_tables.w_pawn_end[rank][file];
                    break;
                case 1:
                    // White Knight
                    score += 300;
                    break;
                case 2:
                    // White Bishop
                    score += 300;
                    break;
                case 3:
                    // White Rook
                    score += 500;
                    break;
                case 4:
                    // White Queen
                    score += 900;
                    break;
                case 5:
                    // White King
                    score += this->piece_tables.w_king_end[rank][file];
                    break;
                case 6:
                    // Black Pawn
                    score -= this->piece_tables.b_pawn_end[rank][file];
                    break;
                case 7:
                    // Black Knight
                    score -= 300;
                    break;
                case 8:
                    // Black Bishop
                    score -= 300;
                    break;
                case 9:
                    // Black Rook
                    score -= 500;
                    break;
                case 10:
                    // Black Queen
                    score -= 900;
                    break;
                case 11:
                    // Black King
                    score -= this->piece_tables.b_king_end[rank][file];
                    break;
                default:
                    std::cout << "Error" << std::endl;
                    break;
            }
        }
    }
    
    return score/100.0f;
}

void Bot::order_moves(Movelist& moves, Board& board){
    std::vector<std::pair<int, Move>> scored_moves;
    int scores[13] = {1, 3, 3, 5, 9, 10, 1, 3, 3, 5, 9, 10, 0};

    if (this->killer_moves.empty()){
        for (const auto& move : moves) {
            int score = 0;

            if (move.typeOf() == Move::CASTLING){
                score += 100; // prioritize castling
            } else if (board.isCapture(move)) {
                Square from = move.from();
                Square to = move.to();
                int capturedPiece = board.at(to);
                int aggressivePiece = board.at(from);
                score += 950 + (scores[capturedPiece] - scores[aggressivePiece]) * 100;
            }
            
            if (this->isCheck(move, board)) {
                score += 300; // prioritize checks
            }

            if (move.typeOf() == Move::PROMOTION) {
                score += 500; // prioritize promotions
            }
            scored_moves.push_back({score, move});
        }
    }else {
        for (const auto& move : moves) {
            int score = 0;

            for (auto move2: killer_moves){
                if (move2 == move){
                    score += 1000;
                    break;
                }
            }

            if (move.typeOf() == Move::CASTLING){
                score += 100; // prioritize castling
            } else if (board.isCapture(move)) {
                Square from = move.from();
                Square to = move.to();
                int capturedPiece = board.at(to);
                int aggressivePiece = board.at(from);
                score += 1000 + (scores[capturedPiece] - scores[aggressivePiece]) * 100;
            }
            
            if (this->isCheck(move, board)) {
                score += 300; // prioritize checks
            }

            if (move.typeOf() == Move::PROMOTION) {
                score += 500; // prioritize promotions
            }


            scored_moves.push_back({score, move});
        }
    }

    std::sort(scored_moves.begin(), scored_moves.end(), [](const std::pair<int, Move>& a, const std::pair<int, Move>& b) {
        return a.first > b.first;
    });

    moves.clear();
    for (const auto& scored_move : scored_moves) {
        moves.add(scored_move.second);
    }
}

void Bot::order_moves_eval(Movelist& moves, std::vector<std::pair<int, Move>> moves_eval){
    if (!moves_eval.empty()){
        std::sort(moves_eval.begin(), moves_eval.end(), [](const std::pair<int, Move>& a, const std::pair<int, Move>& b) {
            return a.first > b.first;
        });
        
        moves.clear();
        for (const auto& scored_move : moves_eval) {
            moves.add(scored_move.second);
        }
    }
}

bool Bot::isCheck(Move move, Board& board) {
    Color stm = board.sideToMove();
    int square = 0;
    if (stm == Color(0)) {
        square = board.pieces(PieceType("KING"), Color(1)).lsb();
    } else {
        square = board.pieces(PieceType("KING"), Color(0)).lsb();
    }
    Square king(square);
    board.makeMove(move);
    bool is_check = board.isAttacked(king, stm);
    board.unmakeMove(move);
    return is_check;
}

int Bot::determineDepth(const Board& board) {
    int pieceCount = 0;
    int pawnCount = 0;
    Bitboard pawns = board.pieces(PieceType::PAWN);
    pawnCount = pawns.count();
    Bitboard r = board.pieces(PieceType::ROOK);
    Bitboard b = board.pieces(PieceType::BISHOP);
    Bitboard q = board.pieces(PieceType::QUEEN);
    Bitboard k = board.pieces(PieceType::KNIGHT);
    Bitboard K  = board.pieces(PieceType::KING);
    pieceCount = r.count() + b.count() + q.count() + k.count() + K.count() + pawnCount;
    // todo: use the queen later for better evaluation
    // bool queenOnBoard = false;

    if (this->game_stage == 'e') {
        return 11;
    }

    //TWEAK THESE VALUES

    //! IDEALISTICALLY, THESE ARE THE VALUES
    // if (pieceCount > 28 && pawnCount > 12) {
    //     return 3;
    // } else if (pieceCount > 22 && pawnCount > 8) {
    //     return 4;
    // }
    // // else if (pieceCount > 19 && pawnCount > 4) {
    // //     return 5;}
    // else if (pieceCount > 12 && pawnCount > 2) {
    //     return 5;
    // } else if (pieceCount > 8 && pawnCount > 1) {
    //     return 6;
    // } else if (pieceCount > 5 && pawnCount > 1) {
    //     return 7;
    // } else {
    //     this->game_stage = 'e';
    //     return 11;
    // }

    if (pieceCount > 28 && pawnCount > 12) {
        return 3;
    } else if (pieceCount > 22 && pawnCount > 8) {
        return 5;
    } else if (pieceCount > 12 && pawnCount > 2) {
        return 7;
    } else if (pieceCount > 5 && pawnCount > 1) {
        return 9;
    } else {
        this->game_stage = 'e';
        return 11;
    }
}

void Bot::LogToFile(const std::string& message) {
    std::ofstream outfile("log.txt", std::ios_base::app);
    outfile << message << std::endl;
}

float Bot::calculate_phase(Board board){
    float phase;
    float PawnPhase = 0;
    float KnightPhase = 1;
    float BishopPhase = 1;
    float RookPhase = 2;
    float QueenPhase = 4;
    float TotalPhase = PawnPhase*16 + KnightPhase*4 + BishopPhase*4 + RookPhase*4 + QueenPhase*2;

    phase = TotalPhase;

    phase -= board.pieces(PieceType::PAWN).count() * PawnPhase;
    phase -= board.pieces(PieceType::KNIGHT).count() * KnightPhase;
    phase -= board.pieces(PieceType::BISHOP).count() * BishopPhase;
    phase -= board.pieces(PieceType::ROOK).count() * RookPhase;
    phase -= board.pieces(PieceType::QUEEN).count() * QueenPhase;

    phase = (phase * 256 + (TotalPhase / 2)) / TotalPhase;
    return phase;
}

bool Bot::time_exceeded(std::chrono::high_resolution_clock::time_point startTime, double time_limit_milliseconds){
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
    return elapsed_milliseconds.count() >= time_limit_milliseconds;
}
