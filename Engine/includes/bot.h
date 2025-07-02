/**
 *  @file bot.h
 *  @brief Defines the Bot class and supporting evaluation structures for a chess engine.
 *
 ** This header provides the declaration of the Bot class responsible for handling move generation, 
 ** board evaluation, and decision-making within different phases of a chess game (opening, middle, and endgame).
 *
 ** It also defines the PieceTables struct, which contains hand-tuned positional evaluation matrices
 ** for all chess pieces, tailored to reflect strategic concepts like center control, castling, piece activity, 
 ** and king safety across different game stages.
 *
 *! Features:
 *! - Multiple constructors for FEN initialisation and game stage control.
 *! - Heuristics for evaluation using handcrafted piece-square tables and material values.
 *! - Opening book integration using JSON-based storage.
 *! - Static utilities for logging and board visualisation.
 *! - Search algorithms including minimax and negamax with alpha-beta pruning.
 *! - Move ordering and utility functions to assist in efficient decision-making.
 *
 *? Dependencies:
 *? - chess.hpp: Board representation and move generation.
 *? - json.hpp: Parsing of opening book data.
 *
 ** This class forms the core decision-making module of the UCI engine backend.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <thread>
#include <future>
#include <functional>
#include <chrono>
#include "3rdparty/json.hpp"
#include "3rdparty/chess.hpp"

using json = nlohmann::json;
using namespace chess;

struct PieceTables {
    /*
    The goal with the pawns is to let them shelter the king,
    punish creating holes in the castle, and to push them to fight.
    */
    int w_pawn[8][8] = {
        {100,100,100,100,100,100,100,100}, 
        {150,150,150,150,150,150,150,150}, 
        {100,100,120,130,130,120,100,100}, 
        {100,100,110,125,125,110,100,100}, 
        {100,100,100,120,120,100,100,100}, 
        {105, 95, 90,100,100, 90, 95,105}, 
        {105,110,110, 80, 80,110,110,105}, 
        {100,100,100,100,100,100,100,100}
    };
    int b_pawn[8][8] = {
        {100,100,100,100,100,100,100,100},
        {105,110,110, 80, 80,110,110,105}, 
        {105, 95, 90,100,100, 90, 95,105}, 
        {100,100,100,120,120,100,100,100}, 
        {100,100,110,125,125,110,100,100}, 
        {100,100,120,130,130,120,100,100}, 
        {150,150,150,150,150,150,150,150}, 
        {100,100,100,100,100,100,100,100}
    };

    /*
    The goal in the endgame is to try and get the pawns to promote.
    Since the side pawns are more likely to promote, we give them a
    slight advantage.
    */

    int w_pawn_end[8][8] = {
        {100,100,100,100,100,100,100,100}, 
        {160,150,150,150,150,150,150,160}, 
        {140,135,135,135,135,135,135,140}, 
        {125,120,120,120,120,120,120,125}, 
        {115,110,110,110,110,110,110,115}, 
        {107,105,105,105,105,105,105,107}, 
        {100,100,100,100,100,100,100,100}, 
        {100,100,100,100,100,100,100,100}
    };
    int b_pawn_end[8][8] = {
        {100,100,100,100,100,100,100,100}, 
        {100,100,100,100,100,100,100,100}, 
        {107,105,105,105,105,105,105,107}, 
        {115,110,110,110,110,110,110,115}, 
        {125,120,120,120,120,120,120,125}, 
        {140,135,135,135,135,135,135,140}, 
        {160,150,150,150,150,150,150,160}, 
        {100,100,100,100,100,100,100,100}
    };

    /*
    The goal with the knights is to try and get them as close
    to the middle as possible as this is where they control the
    most squares. We also want to punish knights on the edges
    as they do not contribute meaningfully to the game.
    */
    int w_knight[8][8] = {
        {250,240,270,270,270,270,260,250},
        {260,280,300,300,300,300,280,260},
        {270,300,310,315,315,310,300,270},
        {270,305,315,320,320,315,305,270},
        {270,300,315,320,320,315,300,270},
        {270,305,310,315,315,310,305,270},
        {260,280,300,305,305,300,280,260},
        {250,260,270,270,270,270,260,250}
    };
    int b_knight[8][8] ={
        {250,240,270,270,270,270,260,250},
        {260,280,300,300,300,300,280,260},
        {270,300,310,315,315,310,300,270},
        {270,305,315,320,320,315,305,270},
        {270,300,315,320,320,315,300,270},
        {270,305,310,315,315,310,305,270},
        {260,280,300,305,305,300,280,260},
        {250,260,270,270,270,270,260,250}
    };
    
    /*
    The goal with the bishops is to try to suggest diagonal lines.
    This value tables suggests fianchettoing the bishops or placing
    them near the mid, or on long diagonals.
    */
    int w_bishop[8][8] = {
        {280,290,290,290,290,290,290,280},
        {290,300,300,300,300,300,300,290},
        {290,300,305,310,310,305,300,290},
        {290,305,305,310,310,305,305,290},
        {290,300,310,310,310,310,300,290},
        {290,310,310,310,310,310,310,290},
        {290,305,300,300,300,300,305,290},
        {280,290,290,290,290,290,290,280}
    };
    int b_bishop[8][8] = {
        {280,290,290,290,290,290,290,280},
        {290,305,300,300,300,300,305,290},
        {290,310,310,310,310,310,310,290},
        {290,300,310,310,310,310,300,290},
        {290,305,305,310,310,305,305,290},
        {290,300,305,310,310,305,300,290},
        {290,300,300,300,300,300,300,290},
        {280,290,290,290,290,290,290,280}
    };
    
    /*
    The goal with the rook is simple. We want them to centralise and
    attack the seventh rank as that is their most potent rank.
    */
    int w_rook[8][8] = {
        {500,500,500,500,500,500,500,500}, 
        {510,515,515,515,515,515,515,510}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {500,500,500,510,510,500,500,500}
    };
    int b_rook[8][8] = {
        {500,500,500,510,510,500,500,500}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {510,515,515,515,515,515,515,510}, 
        {500,500,500,500,500,500,500,500}
    };
    
    /*
    Since the queens is the most powerful piece, there really is not a 
    square where she is not useful. However we want to try and keep her
    engaged, therefore the positives in the middle and negatives on the
    edges.
    */
    int w_queen[8][8] = {
        {880,890,890,895,895,890,890,880},
        {890,900,900,900,900,900,900,890},
        {890,900,905,905,905,905,900,890},
        {895,900,905,905,905,905,900,895},
        {900,900,905,905,905,905,900,895},
        {890,905,905,905,905,905,900,890}, 
        {890,900,905,900,900,900,900,890}, 
        {880,890,890,895,895,890,890,880}
    };
    int b_queen[8][8] = {
        {880,890,890,895,895,890,890,880}, 
        {890,900,905,900,900,900,900,890}, 
        {890,905,905,905,905,905,900,890}, 
        {900,900,905,905,905,905,900,895}, 
        {895,900,905,905,905,905,900,895}, 
        {890,900,905,905,905,905,900,890}, 
        {890,900,900,900,900,900,900,890}, 
        {880,890,890,895,895,890,890,880}
    };
    
    /*
    The king is the most interesting piece to evaluate. In the opening
    want to try and keep the king safe, and in the endgame we want him
    to try and help the pawns promote. I also like king-side castling
    slightly more, thus the ever so slight bias towards the king-side.
    */
    int w_king_mid[8][8] = {
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9980, 9970, 9970, 9960, 9960, 9970, 9970, 9980}, 
        { 9990, 9980, 9980, 9980, 9980, 9980, 9980, 9990}, 
        { 9990, 9980, 9980, 9980, 9980, 9980, 9980, 9990}, 
        {10020,10050,10010, 9980,10000, 9980,10055,10020}
    };
    int b_king_mid[8][8] = {
        {10020,10050,10010, 9980,10000, 9980,10055,10020}, 
        { 9990, 9980, 9980, 9980, 9980, 9980, 9980, 9990}, 
        { 9990, 9980, 9980, 9980, 9980, 9980, 9980, 9990}, 
        { 9980, 9970, 9970, 9960, 9960, 9970, 9970, 9980}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}
    };
    
    int w_king_end[8][8] = {
        {9950, 9960,  9970,  9980,  9980,  9970, 9960, 9950},
        {9970, 9980,  9980, 10000, 10000,  9980, 9980, 9970},
        {9970, 9980, 10020, 10030, 10030, 10020, 9980, 9970},
        {9970, 9980, 10030, 10030, 10030, 10030, 9980, 9970},
        {9970, 9980, 10030, 10030, 10030, 10030, 9980, 9970},
        {9970, 9980, 10020, 10030, 10030, 10020, 9980, 9970},
        {9970, 9970, 10000, 10000, 10000, 10000, 9970, 9970},
        {9950, 9970,  9970,  9970,  9970,  9970, 9970, 9950},
    };
    int b_king_end[8][8] = {
        {9950, 9970,  9970,  9970,  9970,  9970, 9970, 9950},
        {9970, 9970, 10000, 10000, 10000, 10000, 9970, 9970},
        {9970, 9980, 10020, 10030, 10030, 10020, 9980, 9970},
        {9970, 9980, 10030, 10030, 10030, 10030, 9980, 9970},
        {9970, 9980, 10030, 10030, 10030, 10030, 9980, 9970},
        {9970, 9980, 10020, 10030, 10030, 10020, 9980, 9970},
        {9970, 9980,  9980, 10000, 10000,  9980, 9980, 9970},
        {9950, 9960,  9970,  9980,  9980,  9970, 9960, 9950},
    };
};

class Bot{
    /**
     *  @class Bot
     *  @brief Implements a chess engine capable of move evaluation and decision-making across all phases of the game.
     *
     ** The Bot class encapsulates the primary logic and structure required for a UCI-compatible chess engine.
     ** It supports position evaluation, strategic move selection, and integration with an optional opening book.
     *
     *? Key capabilities:
     *? - Position evaluation via piece-square tables and phase-based heuristics.
     *? - Move selection using minimax/negamax with alpha-beta pruning.
     *? - Game phase segmentation: opening (using book), middlegame (multi-threaded), endgame (specialized logic).
     *? - Integration with a JSON-formatted opening book file for deterministic early play.
     *? - Logging utilities for diagnostics.
     *
     *! Construction options:
     *! - Empty constructor initializes default board state.
     *! - FEN-based constructors allow custom game state setup with or without explicit game phase.
     *
     *? Public methods:
     *? - print_board: Static utility for console output of board state.
     *? - get_best_move: Returns the engine’s best move based on the current board and configuration.
     *? - LogToFile: Static logger for recording diagnostic messages.
     *
     *! Internal mechanisms include:
     *! - Evaluation tables for all pieces and game stages.
     *! - Search helpers for move pruning, ordering, and scoring.
     *! - Phase-aware logic to adjust strategy dynamically.
     *
     *? Dependencies:
     *? - Board and Move types from chess.hpp.
     *? - nlohmann::json for opening book handling.
    */
    public:
        Bot();
        Bot(std::string fen, char game_stage);
        Bot(std::string fen);

        Board board;

        static void print_board(Board board);
        
        std::string get_best_move(Board& board, char colour, int depth);
        
        static void LogToFile(const std::string& message);

        float stat_eval(Board board, int depth);

    private:
        json openings_data;
        PieceTables piece_tables;
        char game_stage = 'o';
        int piece_values[13] = {1, 3, 3, 5, 9, 100, 1, 3, 3, 5, 9, 100, 0};
        
        std::string opening_move(const std::string& fen, char colour);
        std::string middle_game_move(int depth, Board& board, char colour);
        std::string middle_game_x_thread(int depth, Board& board, char colour);
        std::string end_game_move(int depth, Board& board, char colour);

        // Helper functions
        float minimax(int depth, float alpha, float beta, bool maximizing_player, Board& board);
        float negamax(int depth, float alpha, float beta, Board& board);
        
        float eval_mid(Board board);
        float eval_end(Board board);
        
        // Helpers for the Helpers
        std::string convert_fen(std::string fen);
        std::string OpeningBookPath = "includes\\OpeningBook\\book.json";
        
        int determineDepth(const Board& board);
        int get_random_index(const std::vector<std::string>& vec);
        
        float search_move(Move move, Board board, int depth, int colour);
        float calculate_phase(Board board);
        
        bool isCheck(Move move, Board& board);
        bool load_openings_data();

        void order_moves(Movelist& moves, Board& board);
};