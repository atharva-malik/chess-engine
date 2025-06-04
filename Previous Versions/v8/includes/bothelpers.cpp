int Bot::volatility(Board& board){
    int volatility;
    Movelist moves = Movelist();
    movegen::legalmoves<movegen::MoveGenType::CAPTURE>(moves, board);
    volatility = moves.size();
    return volatility+4;
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

void Bot::order_moves_eval(Movelist& moves, std::vector<std::pair<int, Move>>& moves_eval){
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
        return 5;
    } else if (pieceCount > 5 && pawnCount > 1) {
        return 7;
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

void Bot::timer(std::promise<void>& promise, double time_limit_milliseconds){
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    try {
        // Simulate some work that might throw an exception
        while (this->isThinking){
            if (start_time + std::chrono::milliseconds((int)time_limit_milliseconds) < std::chrono::high_resolution_clock::now()){
                throw std::runtime_error("Time up!");
            }
        }
        promise.set_value(); // Indicate successful completion
    } catch (const std::exception& e) {
        // Store the exception in the promise
        promise.set_exception(std::current_exception());
    }
}
