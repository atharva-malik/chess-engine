#include "chess.h"

Board::Board() {
    this->FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    this->turn = 0;
    this->half_moves = 0;
    this->full_moves = 1;
    Board::set_board_bitboard_from_FEN(this->FEN, true);
}

Board::Board(std::string fen) {
    this->FEN = fen;
    this->turn = 0;
    this->half_moves = 0;
    this->full_moves = 1;
    Board::set_board_bitboard_from_FEN(this->FEN, true);
}

void Board::set_board_bitboard_from_FEN(std::string fen, bool update_bit_boards) {
    //* SETTING THE BOARD FROM FEN *//
    this->turn = 0;
    this->castling_rights[0] = '-';
    this->castling_rights[1] = '-';
    this->castling_rights[2] = '-';
    this->castling_rights[3] = '-';
    this->half_moves = 0;
    this->full_moves = 1;

    std::stringstream ss(fen);
    std::string boardStr, activeColorStr, castlingStr, enPassantStr, halfmoveStr, fullmoveStr;

    if (!(ss >> boardStr >> activeColorStr >> castlingStr >> enPassantStr >> halfmoveStr >> fullmoveStr)) {
        std::cerr << "Invalid FEN string format." << std::endl;
        return;
    }

    // Board parsing
    // int row = 0, col = 0;
    // for (char c : boardStr) {
    //     if (c == '/') {
    //         row++;
    //         col = 0;
    //     } else if (isdigit(c)) {
    //         col += c - '0';
    //     } else {
    //         this->board[row][col] = c;
    //         col++;
    //     }
    // }

    // Active color
    if (!activeColorStr.empty()) {
        if (activeColorStr == "w") this->turn = 0;
        else if (activeColorStr == "b") this->turn = 1;
        else std::cerr << "Invalid active color: " << activeColorStr << std::endl;
    }

    // Castling rights
    if (!castlingStr.empty() && castlingStr != "-") {
        for (int i = 0; i < castlingStr.size() && i < 4; ++i) {
            this->castling_rights[i] = castlingStr[i];
        }
    }

    // En passant target
    // this->en_passant_target = enPassantStr;

    // Halfmove clock
    try {
        this->half_moves = std::stoi(halfmoveStr);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid halfmove clock: " << halfmoveStr << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Halfmove clock out of range: " << halfmoveStr << std::endl;
    }

    // Fullmove number
    try {
        this->full_moves = std::stoi(fullmoveStr);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid fullmove number: " << fullmoveStr << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Fullmove number out of range: " << fullmoveStr << std::endl;
    }
    // Board::print_board();
    
    //* SETTING THE BIT BOARDS FROM FEN *//
    if (update_bit_boards) Board::update_bitboards(this->FEN);
}

void Board::print_bitboard(std::bitset<64> bitboard) {
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            std::cout << bitboard[i * 8 + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Board::update_bitboards(std::string fen) {
    std::stringstream ss(fen);
    std::string boardStr, activeColorStr, castlingStr, enPassantStr, halfmoveStr, fullmoveStr;

    if (!(ss >> boardStr >> activeColorStr >> castlingStr >> enPassantStr >> halfmoveStr >> fullmoveStr)) {
        std::cerr << "Invalid FEN string format." << std::endl;
        return;
    }

    // Reset ALL bitboards
    this->white_pawns.reset();
    this->white_rooks.reset();
    this->white_knights.reset();
    this->white_bishops.reset();
    this->white_queens.reset();
    this->white_king.reset();
    this->black_pawns.reset();
    this->black_rooks.reset();
    this->black_knights.reset();
    this->black_bishops.reset();
    this->black_queens.reset();
    this->black_king.reset();
    this->en_passant_target.reset();

    int row = 7, col = 0;
    for (char c : boardStr) {
        if (c == '/') {
            row--;
            col = 0;
        } else if (isdigit(c)) {
            col += c - '0';
        } else {
            switch (c){
            case 'P':
                this->white_pawns.set(row * 8 + col);
                break;
            case 'R':
                this->white_rooks.set(row * 8 + col);
                break;
            case 'N':
                this->white_knights.set(row * 8 + col);
                break;
            case 'B':
                this->white_bishops.set(row * 8 + col);
                break;
            case 'Q':
                this->white_queens.set(row * 8 + col);
                break;
            case 'K':
                this->white_king.set(row * 8 + col);
                break;
            case 'p':
                this->black_pawns.set(row * 8 + col);
                break;
            case 'r':
                this->black_rooks.set(row * 8 + col);
                break;
            case 'n':
                this->black_knights.set(row * 8 + col);
                break;
            case 'b':
                this->black_bishops.set(row * 8 + col);
                break;
            case 'q':
                this->black_queens.set(row * 8 + col);
                break;
            case 'k':
                this->black_king.set(row * 8 + col);
                break;
            default:
                break;
            }
            col++;
        }
    }
    std::cout << std::endl;

    // Setting the en passant target
    if (enPassantStr != "-") {
        int en_passant_target_index = (enPassantStr[0] - 'a') + (enPassantStr[1] - '1') * 8;
        this->en_passant_target.set(en_passant_target_index);
    }

    // Print all the bitboards
    // std::cout << "White Pawns: " << this->white_pawns << std::endl;
    // std::cout << "White Rooks: " << this->white_rooks << std::endl;
    // std::cout << "White Knights: " << this->white_knights << std::endl;
    // std::cout << "White Bishops: " << this->white_bishops << std::endl;
    // std::cout << "White Queens: " << this->white_queens << std::endl;
    // std::cout << "White King: " << this->white_king << std::endl;
    // std::cout << "Black Pawns: " << this->black_pawns << std::endl;
    // std::cout << "Black Rooks: " << this->black_rooks << std::endl;
    // std::cout << "Black Knights: " << this->black_knights << std::endl;
    // std::cout << "Black Bishops: " << this->black_bishops << std::endl;
    // std::cout << "Black Queens: " << this->black_queens << std::endl;
    // std::cout << "Black King: " << this->black_king << std::endl;
    // std::cout << "En Passant Target: " << this->en_passant_target << std::endl;
}

void Board::create_FEN_from_bitboards() {
    std::string fen = "";
    // Piece placement
    for (int i = 7; i >= 0; i--) {
        int emptyCount = 0;
        for (int j = 0; j < 8; j++) {
            int square = i * 8 + j;
            if (this->white_pawns[square]) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "P";
            } else if (this->white_rooks[square]) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "R";
            } else if (this->white_knights[square]) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "N";
            } else if (this->white_bishops[square]) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "B";
            } else if (this->white_queens[square]) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "Q";
            } else if (this->white_king[square]) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "K";
            } else if (this->black_pawns[square]) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "p";
            } else if (this->black_rooks[square]) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "r";
            } else if (this->black_knights[square]) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "n";
            } else if (this->black_bishops[square]) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "b";
            } else if (this->black_queens[square]) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "q";
            } else if (this->black_king[square]) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += "k";
            } else {
                emptyCount++;
            }
        }
        if (emptyCount > 0) {
            fen += std::to_string(emptyCount);
        }
        if (i > 0) fen += "/";
    }
    std::cout << fen << std::endl;
}

std::vector<Move> Board::generate_moves(bool turn){
    /*
    The id is the piece identifier, with:
        0 = white pawng
        1 = black pawn
        2 = rook
        3 = knight
        4 = bishop
        5 = queen
        6 = king
    */
    std::vector<Move> moves;
    std::bitset<64> white_pieces = this->white_pawns | this->white_rooks | this->white_knights | this->white_bishops | this->white_queens | this->white_king;
    std::bitset<64> black_pieces = this->black_pawns | this->black_rooks | this->black_knights | this->black_bishops | this->black_queens | this->black_king;
    if (!turn){
        // White's turn
        // for (Move move: this->generate_rook_moves(Board::convert_to_square_no("e5"), white_pieces, black_pieces)){
        //     moves.push_back(move);
        // }
    }
    else{
        // Black's turn
    }
    return moves;
}

int Board::convert_to_square_no(std::string square){
    return this->square_to_no[square];
}

MoveGeneration Board::generate_rook_moves(int square_no, char colour, std::bitset<64> friendly, std::bitset<64> unfriendly){
    /*
    This function generates all the possible moves for a rook.
    The logic is to get the rook's position, then get all possible moves in each direction
    */
    std::bitset<64> attacks = 0;
    MoveGeneration result;

    int capturesSize = 0;

    // init ranks & files
    int r, f;
    
    // init target rank & files
    int tr = square_no / 8;
    int tf = 8-((square_no % 8)+1); // +1 to offset due to the start from 0. Then flip to get the correct file.

    // generate rook attacks
    for (r = tr + 1; r <= 7; r++)
    {
        std::bitset<64> mask = 0;
        mask.set(r * 8 + tf);
        if ((mask & friendly).any()) break; // friendly piece, do not include move
        else if ((mask & unfriendly).any()){
            // unfriendly piece, can capture, so add to captures, but not to the bitboard
            Move capture;
            Piece captured_piece = Board::get_piece_at_square(r*8 + tf);
            capture.from_square = {square_no, {'r', colour, 5}};
            capture.to_square = {r*8 + tf, captured_piece};
            capture.is_capture = true;
            capture.captured_piece = captured_piece;
            result.captures[capturesSize] = capture;
            capturesSize++;
            break;
        }
        attacks |= mask;
    }
    for (r = tr - 1; r >= 0; r--)
    {
        std::bitset<64> mask = 0;
        mask.set(r * 8 + tf);
        if ((mask & friendly).any()) break; // friendly piece, do not include move
        else if ((mask & unfriendly).any()) {
            // unfriendly piece, can capture, so add to captures, but not to the bitboard
            Move capture;
            Piece captured_piece = Board::get_piece_at_square(r*8 + tf);
            capture.from_square = {square_no, {'r', colour, 5}};
            capture.to_square = {r*8 + tf, captured_piece};
            capture.is_capture = true;
            capture.captured_piece = captured_piece;
            result.captures[capturesSize] = capture;
            capturesSize++;
            break;
        }
        attacks |= mask;
    }
    for (f = tf + 1; f <= 7; f++)
    {
        std::bitset<64> mask = 0;
        mask.set(tr * 8 + f);
        if ((mask & friendly).any()) break; // friendly piece, do not include move
        else if ((mask & unfriendly).any()) {
            // unfriendly piece, can capture, so add to captures, but not to the bitboard
            Move capture;
            Piece captured_piece = Board::get_piece_at_square(tr*8 + f);
            capture.from_square = {square_no, {'r', colour, 5}};
            capture.to_square = {tr*8 + f, captured_piece};
            capture.is_capture = true;
            capture.captured_piece = captured_piece;
            result.captures[capturesSize] = capture;
            capturesSize++;
            break;
        }
        attacks |= mask;
    }
    for (f = tf - 1; f >= 0; f--)
    {
        std::bitset<64> mask = 0;
        mask.set(tr * 8 + f);
        if ((mask & friendly).any()) break; // friendly piece, do not include move
        else if ((mask & unfriendly).any()) {
            // unfriendly piece, can capture, so add to captures, but not to the bitboard
            Move capture;
            Piece captured_piece = Board::get_piece_at_square(tr*8 + f);
            capture.from_square = {square_no, {'r', colour, 5}};
            capture.to_square = {tr*8 + f, captured_piece};
            capture.is_capture = true;
            capture.captured_piece = captured_piece;
            result.captures[capturesSize] = capture;
            capturesSize++;
            break;
        }
        attacks |= mask;
    }

    if (capturesSize > 0) {
        result.is_captures = true;
        result.num_captures = capturesSize;
    }
    // return attack map
    result.moves = attacks;
    return result;
}

MoveGeneration Board::generate_bishop_moves(int square_no, char colour, std::bitset<64> friendly, std::bitset<64> unfriendly){
    /*
    This function generates all the possible moves for a bishop.
    The logic is to get the bishop's position, then get all possible moves in each direction.
    */
    // result attacks bitboard
    std::bitset<64> attacks = 0;
    MoveGeneration result;

    int capturesSize = 0;
    
    // init ranks & files
    int r, f;
    
    // init target rank & files
    int tr = square_no / 8;
    int tf = 8-((square_no % 8)+1); // +1 to offset due to the start from 0. Then flip to get the correct file.
    
    // generate bishop atacks
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        std::bitset<64> mask = 0;
        mask.set(r * 8 + f);
        if ((mask & friendly).any()) break; // friendly piece, do not include move
        else if ((mask & unfriendly).any()){
            // unfriendly piece, can capture, so add to captures, but not to the bitboard
            Move capture;
            Piece captured_piece = Board::get_piece_at_square(r * 8 + f);
            capture.from_square = {square_no, {'r', colour, 5}};
            capture.to_square = {r * 8 + f, captured_piece};
            capture.is_capture = true;
            capture.captured_piece = captured_piece;
            result.captures[capturesSize] = capture;
            capturesSize++;
            break;
        }
        attacks |= mask;
    }
    
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        std::bitset<64> mask = 0;
        mask.set(r * 8 + f);
        if ((mask & friendly).any()) break; // friendly piece, do not include move
        else if ((mask & unfriendly).any()){
            // unfriendly piece, can capture, so add to captures, but not to the bitboard
            Move capture;
            Piece captured_piece = Board::get_piece_at_square(r * 8 + f);
            capture.from_square = {square_no, {'r', colour, 5}};
            capture.to_square = {r * 8 + f, captured_piece};
            capture.is_capture = true;
            capture.captured_piece = captured_piece;
            result.captures[capturesSize] = capture;
            capturesSize++;
            break;
        }
        attacks |= mask;
    }
    
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        std::bitset<64> mask = 0;
        mask.set(r * 8 + f);
        if ((mask & friendly).any()) break; // friendly piece, do not include move
        else if ((mask & unfriendly).any()){
            // unfriendly piece, can capture, so add to captures, but not to the bitboard
            Move capture;
            Piece captured_piece = Board::get_piece_at_square(r * 8 + f);
            capture.from_square = {square_no, {'r', colour, 5}};
            capture.to_square = {r * 8 + f, captured_piece};
            capture.is_capture = true;
            capture.captured_piece = captured_piece;
            result.captures[capturesSize] = capture;
            capturesSize++;
            break;
        }
        attacks |= mask;
    }
    
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        std::bitset<64> mask = 0;
        mask.set(r * 8 + f);
        if ((mask & friendly).any()) break; // friendly piece, do not include move
        else if ((mask & unfriendly).any()){
            // unfriendly piece, can capture, so add to captures, but not to the bitboard
            Move capture;
            Piece captured_piece = Board::get_piece_at_square(r * 8 + f);
            capture.from_square = {square_no, {'r', colour, 5}};
            capture.to_square = {r * 8 + f, captured_piece};
            capture.is_capture = true;
            capture.captured_piece = captured_piece;
            result.captures[capturesSize] = capture;
            capturesSize++;
            break;
        }
        attacks |= mask;
    }
    if (capturesSize > 0) {
        result.is_captures = true;
        result.num_captures = capturesSize;
    }
    // return attack map
    result.moves = attacks;
    return result;
}

MoveGeneration Board::generate_queen_moves(int square_no, char colour, std::bitset<64> friendly, std::bitset<64> unfriendly){
    /*
    This function generates all the possible moves for a queen.
    The logic is to get the queen's position, then get all possible moves in each direction.
    */
    // result attacks bitboard
    std::bitset<64> attacks = 0;
    MoveGeneration result;

    MoveGeneration rook_moves = Board::generate_rook_moves(square_no, colour, friendly, unfriendly);
    MoveGeneration bishop_moves = Board::generate_bishop_moves(square_no, colour, friendly, unfriendly);
    attacks = rook_moves.moves | bishop_moves.moves;
    if (rook_moves.is_captures){
        for (int i = 0; i < rook_moves.num_captures; i++){
            result.captures[i] = rook_moves.captures[i];
        }
        result.num_captures = rook_moves.num_captures;
        result.is_captures = true;
    }
    if (bishop_moves.is_captures){
        for (int i = 0; i < bishop_moves.num_captures; i++){
            result.captures[i + rook_moves.num_captures] = bishop_moves.captures[i];
        }
        result.num_captures = rook_moves.num_captures + bishop_moves.num_captures;
        result.is_captures = true;
    }
    result.moves = attacks;
    return result;
}

MoveGeneration Board::generate_knight_moves(int square_no, char colour, std::bitset<64> friendly, std::bitset<64> unfriendly){
    /*
    This function generates all the possible moves for a knight.
    The logic is to get the knight's position, then get all possible moves in each direction.
    */
    
    // Just converting it to a bitboard.set() friendly format
    // It flips the square number to match the bitboard.set() format
    square_no = ((square_no / 8)*8)+(8-((square_no % 8)+1));
    
    // result attacks bitboard
    
    std::cout << square_no << std::endl;
    std::bitset<64> attacks = 0;
    // piece bitboard
    std::bitset<64> bitboard = 0;
    MoveGeneration result;
    // set piece on board
    bitboard.set(square_no);
    Board::print_bitboard(bitboard);
    // generate knight attacks
    if ((bitboard >> 17 & this->not_h_file_bitboard).any()) attacks |= bitboard >> 17;
    if ((bitboard >> 15 & this->not_a_file_bitboard).any()) attacks |= bitboard >> 15;
    if ((bitboard >> 10 & this->not_hg_file_bitboard).any()) attacks |= bitboard >> 10;
    if ((bitboard >> 6 & this->not_ab_file_bitboard).any()) attacks |= bitboard >> 6;
    if ((bitboard << 17 & this->not_a_file_bitboard).any()) attacks |= bitboard << 17;
    if ((bitboard << 15 & this->not_h_file_bitboard).any()) attacks |= bitboard << 15;
    if ((bitboard << 10 & this->not_ab_file_bitboard).any()) attacks |= bitboard << 10;
    if ((bitboard << 6 & this->not_hg_file_bitboard).any()) attacks |= bitboard << 6;

    // return attack map
    result.moves = attacks;
    return result;
}

Piece Board::get_piece_at_square(int square_no){
    Piece piece;
    std::bitset<64> piece_bitboard = 0;
    piece_bitboard.set(square_no);
    if ((piece_bitboard & this->white_pawns).any()){
        piece.type = 'p';
        piece.colour = 'w';
        piece.value = 1;
    }else if ((piece_bitboard & this->white_bishops).any()){
        piece.type = 'b';
        piece.colour = 'w';
        piece.value = 3;
    }else if ((piece_bitboard & this->white_knights).any()){
        piece.type = 'n';
        piece.colour = 'w';
        piece.value = 3;
    }else if ((piece_bitboard & this->white_rooks).any()){
        piece.type = 'r';
        piece.colour = 'w';
        piece.value = 5;
    }else if ((piece_bitboard & this->white_queens).any()){
        piece.type = 'q';
        piece.colour = 'w';
        piece.value = 9;
    }else if ((piece_bitboard & this->white_king).any()){
        piece.type = 'k';
        piece.colour = 'w';
        piece.value = 1000;
    }else if ((piece_bitboard & this->white_pawns).any()){
        piece.type = 'p';
        piece.colour = 'w';
        piece.value = 1;
    }else if ((piece_bitboard & this->black_pawns).any()){
        piece.type = 'p';
        piece.colour = 'b';
        piece.value = 1;
    }else if ((piece_bitboard & this->black_bishops).any()){
        piece.type = 'b';
        piece.colour = 'b';
        piece.value = 3;
    }else if ((piece_bitboard & this->black_knights).any()){
        piece.type = 'n';
        piece.colour = 'b';
        piece.value = 3;
    }else if ((piece_bitboard & this->black_rooks).any()){
        piece.type = 'r';
        piece.colour = 'b';
        piece.value = 5;
    }else if ((piece_bitboard & this->black_queens).any()){
        piece.type = 'q';
        piece.colour = 'b';
        piece.value = 9;
    }else if ((piece_bitboard & this->black_king).any()){
        piece.type = 'k';
        piece.colour = 'b';
        piece.value = 1000;
    }
    return piece;
}
