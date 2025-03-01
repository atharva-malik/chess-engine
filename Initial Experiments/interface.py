"""
    Graphical User Interface for the bot
    Probably in PyGame
    Keep it nice and simple
"""
from minimax_bot import Bot
import chess

bot = Bot(chess.Board(fen="8/7p/5kp1/p1b1r3/P1P5/1P3B1P/4p2K/4B3 w - - 2 45"))
# bot = Bot()
# while bot.gameStage == "O":
#     bot.board.push_san(bot.getMove())
#     print(bot)
#     print(bot.gameStage)
#     print(bot.getFenForOpening())
#     print()
turn = 0
while True:
    print(str(bot) + "\n")
    print(bot.evaluateThePosition(bot.board))
    if turn == 0:
        bot.board.push_san(bot.getBestMove(5, bot.board, "w"))
        turn = 1
    elif turn == 1:
        bot.board.push_san(bot.getBestMove(5, bot.board, "b"))
        turn = 0