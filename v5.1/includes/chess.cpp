#include <iostream>
#include <sstream>
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

// void Board::print_board(){
//     std::cout << "    A B C D E F G H" << std::endl << std::endl; // Print column labels
//     for (int i = 0; i < 8; i++) {
//         std::cout << 8 - i << "   "; // Print row labels
//         for (int j = 0; j < 8; j++) {
//             std::cout << board[i][j] << " ";
//         }
//         std::cout << std::endl;
//     }
//     std::cout << std::endl; // Add a newline at the end
// }

void Board::print_bitboard(std::bitset<64> bitboard) {
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            std::cout << bitboard[i * 8 + j] << " ";
        }
        std::cout << std::endl;
    }
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
    std::cout << "White Pawns: " << this->white_pawns << std::endl;
    std::cout << "White Rooks: " << this->white_rooks << std::endl;
    std::cout << "White Knights: " << this->white_knights << std::endl;
    std::cout << "White Bishops: " << this->white_bishops << std::endl;
    std::cout << "White Queens: " << this->white_queens << std::endl;
    std::cout << "White King: " << this->white_king << std::endl;
    std::cout << "Black Pawns: " << this->black_pawns << std::endl;
    std::cout << "Black Rooks: " << this->black_rooks << std::endl;
    std::cout << "Black Knights: " << this->black_knights << std::endl;
    std::cout << "Black Bishops: " << this->black_bishops << std::endl;
    std::cout << "Black Queens: " << this->black_queens << std::endl;
    std::cout << "Black King: " << this->black_king << std::endl;
    std::cout << "En Passant Target: " << this->en_passant_target << std::endl;
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
