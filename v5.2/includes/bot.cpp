#include "bot.h"

Bot::Bot() {
    this->load_openings_data();
    this->game_stage = 'o';
}

Bot::Bot(std::string fen) {
    this->board.setFen(fen);
    this->load_openings_data();
    this->game_stage = 'o';
}

Bot::Bot(std::string fen, char game_stage) {
    this->board.setFen(fen);
    this->load_openings_data();
    this->game_stage = game_stage;
}

bool Bot::load_openings_data() {
    std::ifstream file("C:\\Atharva\\Programming\\Python\\Python Scripts\\chess-engine\\v5.2\\includes\\OpeningBook\\book.json");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open openings.json" << std::endl;
        return false;
    }

    try {
        this->openings_data = json::parse(file);
        return true;
    } catch (json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    }
}

std::string Bot::get_best_move(int depth, Board& board, char colour) {
    if (this->game_stage == 'o') return Bot::get_opening_move(board.getFen());
    else if (this->game_stage == 'm'){
        // isEndgame(board);
        return this->middle_game_move(depth, board, colour);
    }
}

std::string Bot::get_opening_move(const std::string& fen) {
    if (this->openings_data.empty()) { // Check if the JSON data is loaded
        std::cerr << "Error: Openings data not loaded." << std::endl;
        return {};
    }
    std::string converted_fen = Bot::convert_fen(fen);
    if (this->openings_data.contains(converted_fen)) {
        std::vector<std::string> moves = this->openings_data[converted_fen].get<std::vector<std::string>>();
        return (moves)[this->get_random_index(moves)];
    } else {
        this->game_stage = 'm';
        Color colour = this->board.sideToMove();
        char colour_char = (colour == Color::WHITE) ? 'w' : 'b';
        return Bot::middle_game_move(5, this->board, colour_char);
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
            board.makeMove(move);
            evaluation = this->minimax(depth, -9999, 9999, false, board);
            board.unmakeMove(move);
            if (evaluation > best_eval) {
                best_eval = evaluation;
                best_move = move;
            }
        }
    }
    else {
        best_eval = 99999999999.0f;
        order_moves(moves, board);
        for (int i = 0; i < moves.size(); i++){
            Move move = moves[i];
            board.makeMove(move);
            evaluation = this->minimax(depth, -9999, 9999, true, board);
            board.unmakeMove(move);
            if (evaluation < best_eval){
                best_eval = evaluation;
                best_move = move;
            }
        }
    }
    return uci::moveToUci(best_move);
}

float Bot::minimax(int depth, float alpha, float beta, bool maximizing_player, Board& board){
    std::pair<GameResultReason, GameResult> isGameOver = board.isGameOver();
    if (isGameOver.first == GameResultReason::CHECKMATE){
        if (board.sideToMove() == Color::WHITE) return 9999.0f;
        else return -9999.0f;
    } else if (!(isGameOver.first == GameResultReason::NONE)){
        return 0.0f;
    }
    else if (depth == 0) return this->evaluate(board);

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

float Bot::evaluate(Board& board){
    int score = 0;
    
    Bitboard w_pawns = board.pieces(PieceType("PAWN"), Color(0));
    Bitboard b_pawns = board.pieces(PieceType("PAWN"), Color(1));
    Bitboard w_rooks = board.pieces(PieceType("ROOK"), Color(0));
    Bitboard b_rooks = board.pieces(PieceType("ROOK"), Color(1));
    Bitboard w_knights = board.pieces(PieceType("KNIGHT"), Color(0));
    Bitboard b_knights = board.pieces(PieceType("KNIGHT"), Color(1));
    Bitboard w_bishops = board.pieces(PieceType("BISHOP"), Color(0));
    Bitboard b_bishops = board.pieces(PieceType("BISHOP"), Color(1));
    Bitboard w_queens = board.pieces(PieceType("QUEEN"), Color(0));
    Bitboard b_queens = board.pieces(PieceType("QUEEN"), Color(1));
    Bitboard w_king = board.pieces(PieceType("KING"), Color(0));
    Bitboard b_king = board.pieces(PieceType("KING"), Color(1));

    int square_no = 0;

    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            square_no = rank * 8 + file;
            if (w_pawns.check(square_no)) score += this->piece_tables.w_pawn[rank][file];
            else if (b_pawns.check(square_no)) score -= this->piece_tables.b_pawn[rank][file];
            else if (w_rooks.check(square_no)) score += this->piece_tables.w_rook[rank][file];
            else if (b_rooks.check(square_no)) score -= this->piece_tables.b_rook[rank][file];
            else if (w_knights.check(square_no)) score += this->piece_tables.w_knight[rank][file];
            else if (b_knights.check(square_no)) score -= this->piece_tables.b_knight[rank][file];
            else if (w_bishops.check(square_no)) score += this->piece_tables.w_bishop[rank][file];
            else if (b_bishops.check(square_no)) score -= this->piece_tables.b_bishop[rank][file];
            else if (w_queens.check(square_no)) score += this->piece_tables.w_queen[rank][file];
            else if (b_queens.check(square_no)) score -= this->piece_tables.b_queen[rank][file];
            else if (w_king.check(square_no)) {
                if (this->game_stage == 'e') score += this->piece_tables.w_king_end[rank][file];
                else score += this->piece_tables.w_king_mid[rank][file];
            }
            else if (b_king.check(square_no)) {
                if (this->game_stage == 'e') score -= this->piece_tables.b_king_end[rank][file];
                else score -= this->piece_tables.b_king_mid[rank][file];
            }
        }
    }

    return score/100;
}

void Bot::order_moves(Movelist& moves, Board& board){
    std::vector<std::pair<int, Move>> scored_moves;

    for (const auto& move : moves) {
        int score = 0;

        if (board.isCapture(move)) {
            score += 1000; // prioritize captures
        }
        // if (board.isPromotion(move)) {
        //     score += 500; // prioritize promotions
        // }
        if (this->isCheck(move, board)) {
            score += 300; // prioritize checks
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
