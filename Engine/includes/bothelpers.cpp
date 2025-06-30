/**
 *  @file bothelpers.cpp
 *  @brief Implements support utilities for move filtering, evaluation, logging, and board interaction.
 *
 ** This module provides helper functions used internally by the Bot class to support move ordering,
 ** game phase analysis, logging, and board state formatting. 
 *
 *? These routines enhance engine behavior by:
 *? - Scoring and sorting moves to optimize search performance.
 *? - Identifying tactical motifs such as checks and promotions.
 *? - Determining adaptive search depths based on game complexity.
 *? - Logging state changes and diagnostics to persistent storage.
 *? - Performing basic transformations on game notation (FEN).
 *? - Producing randomized selection indices to add non-determinism.
 *? - Printing the current board visually for debugging or output purposes.
 *
 ** These utilities encapsulate non-core logic to keep evaluation and search code modular
 ** and easier to maintain. They are invoked throughout the engine’s execution pipeline to streamline
 ** and contextualize decision-making.
*/

void Bot::order_moves(Movelist& moves, Board& board){
    /**
     *  @brief Orders moves heuristically to improve search efficiency.
     *
     *? Scores each move in the given list based on tactical features such as:
     *? - Castling (encouraged).
     *? - Captures (prioritized by MVV-LVA).
     *? - Checks.
     *? - Promotions.
     *
     ** The moves are then sorted in descending order of importance and reassigned to the list.
     *
     *  @param moves  Reference to the list of candidate moves to be ordered.
     *  @param board  Current board state for evaluating move effects.
    */
    std::vector<std::pair<int, Move>> scored_moves;
    int scores[13] = {1, 3, 3, 5, 9, 10, 1, 3, 3, 5, 9, 10, 0};
    for (const auto& move : moves) {
        int score = 0;

        if (move.typeOf() == Move::CASTLING){
            score += 100; // prioritise castling
        } else if (board.isCapture(move)) {
            Square from = move.from();
            Square to = move.to();
            int capturedPiece = board.at(to);
            int aggressivePiece = board.at(from);
            score += 950 + (scores[capturedPiece] - scores[aggressivePiece]) * 100;
        }
        
        if (this->isCheck(move, board)) {
            score += 300; // prioritise checks
        }

        if (move.typeOf() == Move::PROMOTION) {
            score += 500; // prioritise promotions
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
    /**
     *  @brief Determines if a given move results in a check.
     *
     ** Temporarily applies the move, then checks if the opposing king becomes attacked.
     ** The move is reverted after evaluation to preserve board state.
     *
     *  @param move   Move to evaluate.
     *  @param board  Current board state.
     *  @return true if the move results in a check, false otherwise.
    */
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
    /**
     *  @brief Dynamically determines an appropriate search depth based on board complexity.
     *
     ** Calculates the number of remaining pieces and pawns to infer the game phase.
     ** Returns a deeper search depth in simplified/endgame scenarios.
     ** Also updates internal game_stage indicator when transitioning to endgame.
     *
     *  @param board  Current board state.
     *  @return Suggested search depth for engine decision-making.
    */
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
    /**
     *  @brief Logs a message to a persistent file for debugging or auditing.
     *
     ** Appends the given message to "log.txt" with a newline.
     *
     *  @param message  The log message to write.
    */
    std::ofstream outfile("log.txt", std::ios_base::app);
    outfile << message << std::endl;
}

float Bot::calculate_phase(Board board){
    /**
     *  @brief Calculates the current phase of the game (opening, middlegame, or endgame).
     *
     ** Uses a weighted material-based phase model where fewer heavy pieces indicate
     ** transition to the endgame. Scales the result to a 0–256 range.
     *
     *  @param board  Current board state.
     *  @return Phase value (0 = endgame, 256 = opening).
    */

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
    /**
     *  @brief Converts a full FEN string to a simplified version used internally.
     *
     ** Trims trailing data such as halfmove clocks and fullmove numbers to isolate
     ** board and turn representation.
     *
     *  @param fen  The full FEN string.
     *  @return Trimmed FEN string for use in hashing or evaluation.
    */
    std::string converted_fen;
    converted_fen = fen.erase(fen.length() - 4, 4);
    return converted_fen;
}

int Bot::get_random_index(const std::vector<std::string>& vec){
    /**
     *  @brief Selects a random index from a given vector.
     *
     ** Used for randomized behavior such as picking between equally viable moves.
     ** Logs a warning and returns 0 if the input vector is empty.
     *
     *  @param vec  Input vector of strings.
     *  @return Random index from vec, or 0 if vec is empty.
    */
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
    /**
     *  @brief Prints the current board state to the console.
     *
     ** Outputs the board's ASCII representation to stdout and logs the FEN string
     ** for tracking.
     *
     *  @param board  The board state to display.
    */
    std::cout << board << std::endl;
    Bot::LogToFile("Board printed: " + board.getFen());
}