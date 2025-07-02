/**
 *  @file evaluate.cpp
 *  @brief Implements board evaluation functions for the Bot class in a chess engine.
 *
 ** This file defines evaluation logic used to estimate the strength of a board position
 ** from White's perspective, based on heuristics such as piece-square tables and static
 ** material values. It includes phase-weighted midgame evaluation as well as specialised
 ** endgame evaluation tailored for different strategic priorities. These scores are used
 ** by the engine to make decisions and compare candidate moves.
 *
 ** The evaluation pipeline leverages asynchronous computation to calculate endgame
 ** evaluations concurrently with midgame analysis, improving overall performance.
*/


float Bot::eval_mid(Board board){
    /**
     * @brief Evaluates the board state during the midgame phase.
     *
     ** Calculates the midgame positional score based on piece-square tables for both sides.
     ** Launches an asynchronous evaluation of the endgame using Bot::eval_end(), and blends the result
     ** based on the current game phase (calculated by Bot::calculate_phase()) for a smooth transition
     ** between midgame and endgame heuristics.
     *
     *  @param board The current game board to evaluate.
     *  @return A floating-point score representing the evaluation from white's perspective
     *          (positive = advantage to white, negative = advantage to black).
    */
    int score = 0;

    Square sq;

    std::future<float> future_result = std::async(std::launch::async, &Bot::eval_end, this, board);

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
    float end_eval = future_result.get() * 100.0f;
    float phase = Bot::calculate_phase(board);
    float eval = ((score * (256 - phase)) + (end_eval * phase)) / 256;
    return eval/100.0f;
}

float Bot::eval_end(Board board){
    /**
     *  @brief Evaluates the board state during the endgame phase.
     *
     ** Calculates a simplified endgame score by combining fixed piece values and endgame
     ** piece-square tables. Emphasises king and pawn positioning, while assigning static values
     ** to other pieces to reflect their strategic utility in the late game.
     *
     *  @param board The current game board to evaluate.
     *  @return A floating-point score representing the evaluation from white's perspective
     *          (positive = advantage to white, negative = advantage to black).
    */

    int score = 0;

    Square sq;

    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            sq = Square(rank * 8 + file);
            // As the game gets towards the end of the game, the value of the pieces change
            // They no longer have 'bad' squares except for the king and the pawns. The other
            // pieces should just support these pieces and help them promote.
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

float Bot::stat_eval(Board board, int depth=-1) {
    /**
     *  @brief Static evaluation function for the board.
     *
     ** This function evaluates the board state using a combination of midgame and endgame evaluations,
     ** depending on the current game phase. It uses Bot::eval_mid() and Bot::eval_end() to get the scores
     ** and blends them based on the phase of the game.
     *
     *  @param board The current game board to evaluate.
     *  @param depth The search depth (not used in this static evaluation).
     *  @return A floating-point score representing the evaluation from white's perspective
     *          (positive = advantage to white, negative = advantage to black).
    */

    if (depth != -1){
        return this->negamax(depth, -1000000.0f, 1000000.0f, board);
    }
    return this->negamax(0, -1000000.0f, 1000000.0f, board);
}