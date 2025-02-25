"""
    Graphical User Interface for the bot
    Probably in PyGame
    Keep it nice and simple
"""
from minimax_bot import Bot

bot = Bot()
while bot.gameStage == "O":
    bot.board.push_san(bot.getMove())
    print(bot)
    print(bot.gameStage)
    print(bot.getFenForOpening())
    print()