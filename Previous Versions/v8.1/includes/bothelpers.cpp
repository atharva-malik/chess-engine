void Bot::order_moves(Movelist& moves, Board& board){
    std::vector<std::pair<int, Move>> scored_moves;
    int scores[13] = {1, 3, 3, 5, 9, 10, 1, 3, 3, 5, 9, 10, 0};
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
    std::sort(scored_moves.begin(), scored_moves.end(), [](const std::pair<int, Move>& a, const std::pair<int, Move>& b) {
        return a.first > b.first;
    });

    moves.clear();
    for (const auto& scored_move : scored_moves) {
        moves.add(scored_move.second);
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

    if (this->game_stage == 'e') {
        return 11;
    }

    if (pieceCount > 28 && pawnCount > 12) {
        return 3;
    } else if (pieceCount > 22 && pawnCount > 8) {
        return 5;
    // } else if (pieceCount > 5 && pawnCount > 1) {
    //     return 7; //! Uncomment this if you don't mind slightly longer thinking times
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

void Bot::print_board(Board board) {
    std::cout << board << std::endl;
    Bot::LogToFile("Board printed: " + board.getFen());
}