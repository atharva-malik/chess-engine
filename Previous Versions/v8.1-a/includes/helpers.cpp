#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "ucibot.cpp"


namespace helpers
{
    std::string trim(const std::string& str) {
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
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
        return s;
    }

    std::vector<std::string> split(const std::string& str, char delimiter) {
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
        std::cout << message << std::endl;
        Bot::LogToFile("Response sent: " + message);
    }

    bool stringContains(const std::string& str, const std::string& substring) {
        return str.find(substring) != std::string::npos;
    }

    std::string TryGetLabelledValue(const std::string& text, const std::string& label, const std::vector<std::string>& allLabels, const std::string& defaultValue = "") {
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
        std::string move;
        if (stringContains(lower(message), "movetime")){
            // int moveTimeMs = TryGetLabelledValueInt(message, "movetime", {"go", "movetime", "wtime", "btime", "winc", "binc", "movestogo"}, 0);

            // player.bot.isThinking = true;
            // std::promise<void> promise;
            // std::future<void> future = promise.get_future();

            // moveTimeMs -= 50; // Give some buffer time to avoid timeout

            // std::thread timerThread([&]() { player.bot.timer(promise, moveTimeMs); });
            // try {
            //     std::string move = player.getBestMoveTimed(moveTimeMs);
            //     player.bot.isThinking = false;
            //     future.get(); // Wait for the worker thread to finish or throw
            //     Respond("bestmove " + move);
            // } catch (const std::exception& e) {
            //     Respond("bestmove "); // todo: FIX THIS SHIT
            // }
            // timerThread.join();
            Respond("bestmove " + player.getBestMove());
        } else {
			// int timeRemainingWhiteMs = TryGetLabelledValueInt(message, "wtime", {"go", "movetime", "wtime", "btime", "winc", "binc", "movestogo"}, 0);
			// int timeRemainingBlackMs = TryGetLabelledValueInt(message, "btime", {"go", "movetime", "wtime", "btime", "winc", "binc", "movestogo"}, 0);
			// int incrementWhiteMs = TryGetLabelledValueInt(message, "winc", {"go", "movetime", "wtime", "btime", "winc", "binc", "movestogo"}, 0);
			// int incrementBlackMs = TryGetLabelledValueInt(message, "binc", {"go", "movetime", "wtime", "btime", "winc", "binc", "movestogo"}, 0);

            // // todo: implement timed searches
			// double thinkTime = player.ChooseThinkTime(timeRemainingWhiteMs, timeRemainingBlackMs, incrementWhiteMs, incrementBlackMs);
			// player.bot.LogToFile("Thinking for: " + std::to_string(thinkTime) + " ms.");

            // player.bot.isThinking = true;
            // std::promise<void> promise;
            // std::future<void> future = promise.get_future();

            // thinkTime -= 50; // Give some buffer time to avoid timeout

            // std::thread timerThread([&]() { player.bot.timer(promise, thinkTime); });
            // try {
            //     std::string move = player.getBestMoveTimed(thinkTime);
            //     player.bot.isThinking = false;
            //     future.get(); // Wait for the worker thread to finish or throw
            //     Respond("bestmove " + move);
            // } catch (const std::exception& e) {
            //     Respond("bestmove "); // todo: FIX THIS SHIT
            // }
            // timerThread.join();
            Respond("bestmove " + player.getBestMove());
		}

    }
} // namespace helpers
