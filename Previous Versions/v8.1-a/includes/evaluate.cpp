float Bot::eval_mid(Board board){
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
    //todo: make it hate losing castling rights
    float phase = Bot::calculate_phase(board);
    float eval = ((score * (256 - phase)) + (end_eval * phase)) / 256;
    return eval/100.0f;
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