"""
    Graphical User Interface for the bot
    Probably in PyGame
    Keep it nice and simple
"""
from minimax_bot import Bot
import chess

bot = Bot(chess.Board(fen="8/7p/5kp1/p1b1r3/P1P5/1P3B1P/4p2K/4B3 w - - 2 45"))
# while bot.gameStage == "O":
#     bot.board.push_san(bot.getMove())
#     print(bot)
#     print(bot.gameStage)
#     print(bot.getFenForOpening())
#     print()
print(bot)
print(bot.minimax(10, -9999, 9999, False, bot.board))