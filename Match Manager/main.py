import chess
import chess.engine
from simple_colors import *
from random import choice

def play_game(engine1_path, engine2_path, p1, p2, fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"):
    # Load the chess engine from the given .exe file path
    try:
        engine1 = chess.engine.SimpleEngine.popen_uci(engine1_path)
        engine2 = chess.engine.SimpleEngine.popen_uci(engine2_path)
        board = chess.Board(fen)
        
        while not board.is_game_over():
            # Engine 1's move
            result = engine1.play(board, chess.engine.Limit()) #? chess.engine.Limit(time=2.0) in seconds
            board.push(result.move)
            # print(green(f"{p1}'s move: " + result.move.uci()))

            if board.is_game_over():
                break

            # Engine 2's move
            result = engine2.play(board, chess.engine.Limit()) #? chess.engine.Limit(time=2.0) in seconds
            board.push(result.move)
            # print(blue(f"{p2}'s move: " + result.move.uci()))

        # Game over
        print(green("Game over!"), "bold")
        print(blue("Result:" + board.result(), "bold"))
        if (board.result() == "1-0"):
            return 1
        elif (board.result() == "0-1"):
            return 2
        else:
            return 0
    except FileNotFoundError:
        print(f"Could not find the engine at {engine1_path}. Please check the path and try again.")
        return -1
    except Exception as e:
        print(f"An error occurred: {e}")
        return -2


p1results = 0
p2results = 0
draws = 0
corrupted = 0

engine1_path = "engines\\v7.2-uci.exe"
engine2_path = "engines\\v7.3-threading.exe"

engine1_name = "v7.2-uci"
engine2_name = "v7.3-threading"

num_of_games = 100

print(red(f"Running {num_of_games} games.", "bold"))
print(red(f"{engine1_name} vs {engine2_name}", "bold"))

for i in range(num_of_games):
    if choice(range(1, 3)) == 1:
        print(f"Starting match {i+1} with {engine1_name} as white and {engine2_name} as black")
        result = play_game(engine1_path, engine2_path, engine1_name, engine2_name)
    else:
        print(f"Starting match {i+1} with {engine2_name} as white and {engine1_name} as black")
        result = play_game(engine2_path, engine1_path, engine2_name, engine1_name)
    if result == 1:
        p1results += 1
    elif result == 2:
        p2results += 1
    elif result == 0:
        draws += 1
    else:
        corrupted += 1
