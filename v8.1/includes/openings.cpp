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

std::string Bot::opening_move(const std::string& fen, char colour) {
    if (this->openings_data.empty()) { // Check if the JSON data is loaded
        Bot::LogToFile("Error: Openings data not loaded.");
        this->game_stage = 'm';
        return Bot::get_best_move(this->board, colour, -1);
    }
    std::string converted_fen = Bot::convert_fen(fen);
    if (this->openings_data.contains(converted_fen)) {
        std::vector<std::string> moves = this->openings_data[converted_fen].get<std::vector<std::string>>();
        return (moves)[this->get_random_index(moves)];
    } else {
        this->game_stage = 'm';
        return Bot::get_best_move(this->board, colour, -1);
    }
}