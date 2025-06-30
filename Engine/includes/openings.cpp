/**
 *  @file openings.cpp
 *  @brief Handles loading and querying chess opening data for the Bot class.
 *
 ** This file contains functionality for reading a JSON-formatted opening book
 ** and selecting moves based on the current FEN position during the opening stage.
 ** If no relevant opening is found or loading fails, the bot transitions to midgame
 ** evaluation using classical search heuristics.
 *
 *? Key functions:
 *? - load_openings_data: Loads the JSON file containing opening positions and moves.
 *? - opening_move: Returns a random opening move if available; otherwise falls back to search.
 *
 *  @note Opening move selection supports FEN-based lookups with preprocessing via convert_fen().
 *  @warning If the opening book is missing or malformed, fallback evaluation logic is invoked.
*/

bool Bot::load_openings_data() {
    /**
     * @brief Loads opening book data from a JSON file.
     *
     ** Attempts to open and parse the file specified by Bot::OpeningBookPath. If successful,
     ** the data is cached for use by other functions (e.g., opening_move). If loading fails due
     ** to file I/O or parsing errors, the function logs the failure and returns false.
     *
     * @return True if the file was successfully loaded and parsed; otherwise, false.
     *
     *! @warning If this function returns false, the engine will bypass opening book logic
     *!          and enter midgame evaluation immediately.
    */
    
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
    /**
     * @brief Retrieves an opening move for a given FEN, if available.
     *
     ** Checks whether the current FEN exists in the loaded opening book. If a match is found,
     ** a random move from the associated list is returned. If the opening data is empty or
     ** the FEN is not present, the bot transitions to the midgame stage and calls
     ** get_best_move() to compute a move instead.
     *
     *  @param fen The current position in Forsyth–Edwards Notation.
     *  @param colour The side to move ('w' for white, 'b' for black).
     *  @return A UCI-format move string, either from the opening book or best move search.
     *
     * @note The FEN string is first converted using Bot::convert_fen before lookup.
     *! @warning If openings data isn't loaded or doesn't include the FEN, the engine
     *!          defaults to midgame logic.
    */

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