#include "bot.h"

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