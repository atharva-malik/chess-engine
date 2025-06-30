/**
 *  @file ucibot.cpp
 *  @brief Implements the UciPlayer class, which acts as the interface between the UCI protocol and the Bot engine.
 *
 ** This file defines the UciPlayer class responsible for managing board state, delegating evaluation
 ** and search logic to the underlying Bot, and handling basic UCI protocol interactions such as starting
 ** a new game, setting up a position, processing go commands, and retrieving best moves.
 *
 *? Key responsibilities:
 *? - Maintain a Bot instance for search and move generation
 *? - Manage board state updates and position setup
 *? - Convert moves from UCI format and relay to Bot methods
 *? - Return current FEN or best move to external controllers
 *
 *  @note This class provides a simplified abstraction for integrating the Bot engine into UCI-compliant interfaces.
*/

#include "bot.cpp"

class UciPlayer {
    /**
     *  @class UciPlayer
     *  @brief A UCI-compliant interface layer for the chess Bot engine.
     *
     ** UciPlayer manages the lifecycle of a game session, handles board position updates,
     ** processes UCI protocol commands, and delegates move generation and evaluation to the
     ** underlying Bot instance. It provides streamlined interaction between the UCI interface
     ** and the core engine logic.
     *
     *? Responsibilities:
     *? - Wraps the Bot engine for protocol-driven move generation
     *? - Parses and applies UCI position and move commands
     *? - Exposes the current board state and best move in UCI format
    */
    public:
        Bot bot;
        UciPlayer();

        //* Uci Methods
        void NotifyNewGame();
        void SetPosition(std::string fen);
        void ProcessPositionCommand(std::string message);
        void ProcessGoCommand(std::string message);
        void Quit();

        //* Essential Bot Methods
        void MakeMove(std::string move);

        std::string getFen();
        std::string getBestMove();
};

UciPlayer::UciPlayer() {
    /**
     * @brief Default constructor for UciPlayer.
     *
     * Initialises a new Bot instance with the standard starting position.
    */
    this->bot = Bot();
}

void UciPlayer::NotifyNewGame() {
    /**
     *  @brief Resets the internal Bot instance to start a new game.
     *
     ** Called when a new game is initiated via the UCI protocol.
    */
    this->bot = Bot();
}

void UciPlayer::Quit() {
    /**
     *  @brief Quits the current game session by resetting the Bot instance.
     *
     ** Calls NotifyNewGame() to reset the internal Bot to its initial state,
     ** effectively ending the current game and preparing for a new one.
     *
     ** Part of the UCI protocol implementation for gracefully terminating a game.
    */
    UciPlayer::NotifyNewGame();
}

void UciPlayer::SetPosition(std::string fen) {
    /**
     *  @brief Sets the board position for the current game session.
     *
     ** Initialises a new Bot instance with the specified Forsyth-Edwards Notation (FEN) string,
     ** effectively resetting the board to the given position.
     *
     * @param fen A standard chess FEN string representing the board state
    */
    
    this->bot = Bot(fen);
}

void UciPlayer::MakeMove(std::string move) {
    /**
     *  @brief Applies a move to the current board state.
     *
     ** Converts a UCI-formatted move string to a move object and applies it
     ** to the internal board representation.
     *
     *  @param move A UCI-formatted move string representing the chess move to be made
    */

    this->bot.board.makeMove(uci::uciToMove(this->bot.board, move));
}

std::string UciPlayer::getFen() {
    /**
     * @brief Retrieves the current board state in Forsyth-Edwards Notation (FEN).
     *
     * @return A standard FEN string representing the current board state
    */
    return this->bot.board.getFen();
}

std::string UciPlayer::getBestMove() {
    /**
     *  @brief Retrieves the best move for the current board state.
     *
     ** Determines the optimal move for the side currently to move using the bot's 
     ** move selection algorithm. The side is specified by the current board's side to move.
     *
     *  @return A string representing the best move in UCI move notation
    */
    
    char colour;
    if (this->bot.board.sideToMove() == Color::WHITE) colour = 'w';
    else colour = 'b';
    return this->bot.get_best_move(this->bot.board, colour);
}