/**
 *  @file Constructors.cpp
 *  @brief Defines the constructors for the Bot class used in a chess engine.
 *
 ** This file implements multiple constructors for the Bot class, which initialise
 ** a chess game state using a FEN string and determine the initial game stage.
 ** If loading opening book data fails, the bot transitions directly to midgame.
*/

#include "bot.h"

Bot::Bot() {
    /**
     * @brief Default constructor for the Bot class.
     *
     ** Initializes the bot's internal chess board to the standard starting position using FEN notation.
     ** Sets the initial game stage to opening ('o'). If the opening book fails to load,
     ** the game stage is immediately set to midgame ('m').
    */
    
    this->board.setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    this->game_stage = 'o';
    if (!this->load_openings_data()) {
        this->game_stage = 'm';
    }
}

Bot::Bot(std::string fen) {
    /**
     *  @brief Constructs a Bot instance with a specified board position.
     *
     ** Initialises the bot's internal board using the given FEN string, allowing setup of
     ** custom positions or non-standard starting states. Sets the initial game stage to opening ('o').
     ** If the opening book fails to load, the game stage defaults to midgame ('m').
     *
     *  @param fen A Forsyth–Edwards Notation (FEN) string representing the desired initial board state.
    */
    
    this->board.setFen(fen);
    this->game_stage = 'o';
    if (!this->load_openings_data()) {
        this->game_stage = 'm';
    }
}

Bot::Bot(std::string fen, char game_stage) {
    /**
     *  @brief Constructs a Bot instance with a specified board state and game stage.
     *
     ** Initialises the bot's internal board using the provided FEN string and sets the
     ** game stage explicitly. This allows customised initialisation for different
     ** scenarios (e.g., opening, midgame, or endgame). If the opening book fails to load,
     ** the stage is overridden to midgame ('m') to reflect the lack of opening data.
     *
     *  @param fen A Forsyth–Edwards Notation (FEN) string representing the initial board state.
     *  @param game_stage A character denoting the game stage ('o' = opening, 'm' = midgame, 'e' = endgame).
    */
    this->board.setFen(fen);
    this->game_stage = game_stage;
    if (game_stage == 'o' && !this->load_openings_data()) {
        this->game_stage = 'm';
    }
}