/**
 *  @file helpers.cpp
 *  @brief Defines utility functions used throughout the chess engine for string manipulation,
 *         UCI protocol message parsing, and standardised engine responses.
 *
 ** This file provides support routines for handling and parsing commands compliant with the
 ** Universal Chess Interface (UCI), managing user input, formatting strings, and delegating
 ** communication between the chess engine and the graphical user interface.
 *
 *? Key helper functions include:
 *? - String manipulation: `trim()`, `lower()`, `split()`
 *? - UCI protocol parsing and option handling: `ProcessPositionCommand()`, `DisplayOptions()`, `ProcessGoCommand()`
 *? - Response formatting and logging: `Respond()`, `TryGetLabelledValue()`, `TryGetLabelledValueInt()`
 *
 ** These functions help simplify logic in higher-level modules like the UciPlayer and Bot classes,
 ** improving modularity and code clarity across the engine’s control flow.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "ucibot.cpp"


namespace helpers
{
    /**
     * @namespace helpers
     * @brief A collection of utility functions that support UCI command parsing, string manipulation,
     *        and communication for the chess engine.
     *
     ** The helpers namespace modularizes auxiliary tasks such as parsing engine options,
     ** sanitizing input, formatting UCI-compliant responses, and extracting data from command strings.
     ** These functions simplify engine logic and improve separation of concerns.
    */
    
    std::string trim(const std::string& str) {
        /**
         *  @brief Trims leading and trailing whitespace from a string.
         *
         *  @param str The input string to be trimmed.
         *  @return A copy of the string with whitespace removed from both ends.
        */

        // Find first non-whitespace character
        auto start = str.find_first_not_of(" \t\n\r");
    
        // Find last non-whitespace character
        auto end = str.find_last_not_of(" \t\n\r");
    
        // If no non-whitespace characters, return an empty string
        if (start == std::string::npos || end == std::string::npos) {
            return "";
        }
    
        // Return trimmed string
        return str.substr(start, end - start + 1);
    }

    std::string lower(std::string s){
        /**
         *  @brief Splits a string into substrings based on the 'space' character
         *         as a delimiter.
         *
         *  @param s The string to split.
         *  @return A vector of substrings resulting from the split operation.
        */
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
        return s;
    }

    std::vector<std::string> split(const std::string& str, char delimiter) {
        /**
         * @brief Splits a string into substrings based on a delimiter.
         *
         * @param str The string to split.
         * @param delimiter The character to split the string at.
         * @return A vector of substrings resulting from the split operation.
        */

        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
    
        // Extract substrings separated by the delimiter
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
    
        return tokens;
    }

    void Respond(std::string message) {
        /**
         * @brief Outputs a message to the console and logs it to file.
         *
         * @param message The message string to output and log.
        */

        std::cout << message << std::endl;
        Bot::LogToFile("Response sent: " + message);
    }

    bool stringContains(const std::string& str, const std::string& substring) {
        /**
         * @brief Checks if a substring exists within a string.
         *
         * @param str The string to search in.
         * @param substring The substring to search for.
         * @return True if substring exists within str; otherwise, false.
        */

        return str.find(substring) != std::string::npos;
    }

    std::string TryGetLabelledValue(const std::string& text, const std::string& label, const std::vector<std::string>& allLabels, const std::string& defaultValue = "") {
        /**
         *  @brief Extracts the value associated with a given label from a UCI-style string.
         *
         *  @param text The input text containing multiple labels and values.
         *  @param label The label to locate.
         *  @param allLabels All known labels that might appear in the text.
         *  @param defaultValue The value to return if no match is found.
         *  @return The value corresponding to the label, or the defaultValue if not found.
        */
        
        std::string trimmedText = trim(text);
    
        if (trimmedText.find(label) != std::string::npos) {
            size_t valueStart = trimmedText.find(label) + label.length();
            size_t valueEnd = trimmedText.length();
    
            for (const std::string& otherID : allLabels) {
                if (otherID != label && trimmedText.find(otherID) != std::string::npos) {
                    size_t otherIDStartIndex = trimmedText.find(otherID);
                    if (otherIDStartIndex > valueStart && otherIDStartIndex < valueEnd) {
                        valueEnd = otherIDStartIndex;
                    }
                }
            }
    
            return trim(trimmedText.substr(valueStart, valueEnd - valueStart));
        }
    
        return defaultValue;
    }

    void DisplayOptions() {
        /**
         * @brief Displays the available UCI engine options to the interface.
         *
         * Outputs engine identification and declares a set of configurable UCI options.
        */

        //! These options are NOT changeable by the user.
        //! They only exist to pass the UCI protocol requirements.

        Respond("id name Fury");
        Respond("id author Atharva\n");

        Respond("option name Move Overhead type spin default 10 min 0 max 5000");
        Respond("option name Debug Log File type string default <empty>");
        Respond("option name NumaPolicy type string default auto");
        Respond("option name Threads type spin default 1 min 1 max 1024");
        Respond("option name Hash type spin default 16 min 1 max 33554432");
        Respond("option name Clear Hash type button");
        Respond("option name Ponder type check default false");
        Respond("option name MultiPV type spin default 1 min 1 max 256");
        Respond("option name Skill Level type spin default 20 min 0 max 20");
        Respond("option name Move Overhead type spin default 10 min 0 max 5000");
        Respond("option name nodestime type spin default 0 min 0 max 10000");
        Respond("option name UCI_Chess960 type check default false");
        Respond("option name UCI_LimitStrength type check default false");
        Respond("option name UCI_Elo type spin default 1320 min 1320 max 3190");
        Respond("option name UCI_ShowWDL type check default false");
        Respond("option name SyzygyPath type string default <empty>");
        Respond("option name SyzygyProbeDepth type spin default 1 min 1 max 100");
        Respond("option name Syzygy50MoveRule type check default true");
        Respond("option name SyzygyProbeLimit type spin default 7 min 0 max 7");
        Respond("option name EvalFile type string default nn-1111cefa1111.nnue");
        Respond("option name EvalFileSmall type string default nn-37f18f62d772.nnue");
        Respond("uciok");
    }

    int TryGetLabelledValueInt(const std::string& text, const std::string& label, const std::vector<std::string>& allLabels, int defaultValue = 0) {
        /**
         *  @brief Extracts an integer value associated with a given label from a UCI-style string.
         *
         *  @param text The input string with labelled values.
         *  @param label The label whose associated value is desired.
         *  @param allLabels A list of all known labels.
         *  @param defaultValue The default value to return if parsing fails.
         *  @return The extracted integer value, or defaultValue if parsing fails.
        */

        // Convert the default integer value to a string
        std::string defaultValueStr = std::to_string(defaultValue);
        
        // Call TryGetLabelledValue to extract the potential value string
        std::string valueString = TryGetLabelledValue(text, label, allLabels, defaultValueStr);
    
        // Find the first value in valueString (before a space if exists)
        std::istringstream valueStream(valueString);
        std::string firstWord;
        valueStream >> firstWord;
    
        // Convert the extracted word to an integer
        try {
            return std::stoi(firstWord);
        } catch (...) {
            return defaultValue;
        }
    }

    // Format: 'position startpos moves e2e4 e7e5'
	// Or: 'position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4 e7e5'
	// Note: 'moves' section is optional
    void ProcessPositionCommand(std::string message, UciPlayer& player) {
        /**
         *  @brief Parses a "position" command string and sets up the board for the UciPlayer.
         *
         ** Interprets FEN strings or standard starting positions and applies subsequent move history
         ** if provided. Useful for setting the board in preparation for a "go" command.
         *
         *  @param message The UCI position command.
         *  @param player A reference to the UciPlayer receiving the updated position.
        */

        // FEN
        if (stringContains(lower(message), "startpos")){
            player.SetPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        }
        else if (stringContains(lower(message), "fen")) {
            std::string customFen = TryGetLabelledValue(message, "fen", {"position", "fen", "moves"});
            player.SetPosition(customFen);
        }
        else{
            std::cout << "Invalid position command (expected 'startpos' or 'fen')" << std::endl;
        }

        // Moves
        std::string allMoves = TryGetLabelledValue(message, "moves", {"position", "fen", "moves"});
        if (!allMoves.empty()){
            std::vector<std::string> moveList = split(allMoves, ' ');
            for(auto move : moveList){
                player.MakeMove(move);
            }

            Bot::LogToFile("Make moves after setting position: " + std::to_string(moveList.size()));
        }
    }

    void ProcessGoCommand(std::string message, UciPlayer& player) {
        /**
         *  @brief Handles the UCI "go" command by triggering move calculation.
         *
         **  Responds with the best move calculated by the UciPlayer and outputs it in UCI format.
         *
         *  @param message The raw UCI "go" command text.
         *  @param player The UciPlayer instance tasked with move generation.
        */
        
        Respond("bestmove " + player.getBestMove());
    }

    void clearScreen() {
        /**
         *  @brief Clears the console screen.
         *
         **  Uses system-specific commands to clear the terminal or console output.
        */
        #if defined(_WIN32) || defined(_WIN64)
            std::system("cls"); // For Windows
        #else
            std::system("clear"); // For Unix-based systems
        #endif
    } 

    void PrintHelp() {
        /**
         *  @brief Outputs a help message listing available commands and their descriptions.
         *
         **  Provides users with a quick reference for interacting with the UCI engine.
        */

        Respond("Available commands:");
        Respond("------------------------------------------------");
        Respond("uci               - Display engine identification and options.");
        Respond("isready           - Confirm engine is ready to process commands.");
        Respond("ucinewgame        - Notify engine of a new game start.");
        Respond("eval [-d] <depth> - Evaluate the current position with a specified depth (defaults to 1).");
        Respond("position commands:");
        Respond("   position startpos               - Set up the board with the starting position.");
        Respond("   position startpos moves <moves> - Set up the board with the starting position and apply a sequence of moves.");
        Respond("   position <fen>                  - Set up the board with a specific FEN string.");
        Respond("   position <fen> moves <moves>    - Set up the board with a specific FEN string and apply a sequence of moves.");
        Respond("go commands:");
        Respond("   go movetime [time in ms]                                                               - Calculate the best move based on current position.");
        Respond("   go wtime [time in ms] btime [time in ms]                                               - Calculate the best move based on current position.");
        Respond("   go wtime [time in ms] btime [time in ms] winc [increment in ms] binc [increment in ms] - Calculate the best move based on current position.");
        Respond("perft commands:");
        Respond("   perft [depth]    - Run a perft test at a given depth.");
        Respond("   perft -v [depth] - Run a verbose perft test at a given depth.");
        Respond("quit           - Exit the engine gracefully.");
        Respond("d              - Display the current board state");
        Respond("cls            - Clear the screen.");
        Respond("------------------------------------------------");
    }

    uint64_t perft(int depth, Board board) {
        /**
         *  @brief Computes the perft (performance test) node count for a given search depth.
         *
         ** Recursively calculates the number of legal move sequences (nodes) from the current board
         ** position up to the specified depth. This is typically used for debugging and verifying
         ** the correctness of move generation in chess engines.
         *
         *  @param depth The search depth to explore. A depth of 1 returns the count of legal moves.
         *  @param board The current board position.
         *  @return The total number of leaf nodes reachable from this position at the given depth.
         *
         *  @note Assumes move generation is fully legal and uses move application with state tracking.
         *! @warning This function performs a full brute-force enumeration and is computationally expensive.
        */
        Movelist moves;
        movegen::legalmoves(moves, board);
    
        if (depth == 1) {
            return moves.size();
        }
    
        uint64_t nodes = 0;
    
        for (const auto& move : moves) {
            board.makeMove<true>(move);
            nodes += perft(depth - 1, board);
            board.unmakeMove(move);
        }
    
        return nodes;
    }

    void perft_verbose(int depth, Board board) {
        /**
         *  @brief Performs a verbose perft test and prints node counts for each legal move.
         *
         ** Generates all legal moves from the current position and, for each move, recursively calculates
         ** the number of leaf nodes reachable within the specified depth. Prints each move and its corresponding
         ** node count to the console for debugging and verification purposes.
         *
         *  @param depth The number of plies to search (depth-1 is passed to the perft function).
         *  @param board The current board state to begin the test from.
         *
         *  @note The output includes per-move node counts followed by a total node count summary.
         *! @warning As depth increases, this function can become computationally expensive and verbose.
        */

        Movelist moves;
        movegen::legalmoves(moves, board);
        
        if (depth < 1) {
            Respond("Depth must be at least 1 for perft_verbose.");
            return;
        }
        else if (depth == 1) {
            Respond("nodes: " + std::to_string(moves.size()));
            return;
        }
        uint64_t nodes = 0;
        uint64_t result = 0;
        auto t0 = std::chrono::high_resolution_clock::now();
        for (const auto& move : moves) {
            std::cout << uci::moveToUci(move) << ": ";
            board.makeMove<true>(move);
            result = perft(depth - 1, board);
            std::cout << result << std::endl;
            board.unmakeMove(move);
            nodes += result;
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

        Respond("\n\nnodes: " + std::to_string(nodes) + " nps: " + std::to_string((nodes * 1000) / (ms + 1)) + " ms: " + std::to_string(ms));
    }

    void ProcessPerftCommand(std::string message, UciPlayer& player) {
        /**
         *  @brief Processes a "perft" command to calculate the number of legal moves from the current position.
         *
         **  This function interprets the perft command and outputs the result in a UCI-compliant format.
         *
         *  @param message The raw UCI "perft" command text.
         *  @param player The UciPlayer instance managing the current game state.
        */

        // Extract depth from command
        int depth = TryGetLabelledValueInt(message, "depth", {"perft", "depth"}, 1);
        if (stringContains(lower(message), "-v")) {
            Respond("Running perft with depth " + std::to_string(depth) + " (verbose mode)");
            perft_verbose(depth, player.bot.board);
        } else {
            Respond("Running perft with depth " + std::to_string(depth));
            auto t0 = std::chrono::high_resolution_clock::now();
            auto nodes = perft(depth, player.bot.board);
            auto t1 = std::chrono::high_resolution_clock::now();
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
            Respond("nodes: " + std::to_string(nodes) + " nps: " + std::to_string((nodes * 1000) / (ms + 1)) + " ms: " + std::to_string(ms));
        }
    }

    void ProcessEvalCommand(std::string message, UciPlayer& player) {
        /**
         *  @brief Processes an "eval" command to evaluate the current board position.
         *
         **  This function interprets the eval command and outputs the evaluation score in a UCI-compliant format.
         *
         *  @param message The raw UCI "eval" command text.
         *  @param player The UciPlayer instance managing the current game state.
        */

        int depth = TryGetLabelledValueInt(message, "-d", {"eval", "-d"}, -1);
        int eval = player.bot.stat_eval(player.bot.board, depth);
        if (eval > -9999.0f && eval < 9999.0f) {
            Respond("Eval: " + std::to_string(eval));
        } else {
            // If eval is outside the range, we assume it's a mate score
            int mateScore = depth - std::floor(std::abs(eval) / 9999);
            Respond("Eval: #" + std::to_string(mateScore));
        }
    }
} // namespace helpers
