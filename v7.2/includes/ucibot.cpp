#include "bot.cpp"

class UciPlayer {
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
    this->bot = Bot();
}

void UciPlayer::NotifyNewGame() {
    this->bot = Bot();
}

void UciPlayer::Quit() {
    //! //TODO: NEEDS TO BE FIXED
    UciPlayer::NotifyNewGame();
}

void UciPlayer::SetPosition(std::string fen) {
    this->bot = Bot(fen);
}

void UciPlayer::MakeMove(std::string move) {
    this->bot.board.makeMove(uci::uciToMove(this->bot.board, move));
}

std::string UciPlayer::getFen() {
    return this->bot.board.getFen();
}

std::string UciPlayer::getBestMove() {
    char colour;
    if (this->bot.board.sideToMove() == Color::WHITE) colour = 'w';
    else colour = 'b';
    return this->bot.get_best_move(this->bot.board, colour);
}
