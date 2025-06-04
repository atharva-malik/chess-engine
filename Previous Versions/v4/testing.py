import cProfile
import pstats
import chess
import minimax_bot  # Your Cython module name
import minimax_bot_test  # Your pure Python module name

# Create a chess board (use the position where you see slowdown)
board = chess.Board(fen="8/7p/5kp1/p1b1r3/P1P5/1P3B1P/4p2K/4B3 w - - 2 45")

depth = 5 # Or the depth you are testing with
# 1. Profile the CYTHON version
cProfile.run(f'minimax_bot.Bot().minimax(depth={depth}, alpha=-9999, beta=9999, isMaximizing=True, board=board)', 'profile_cython.prof')
stats_cython = pstats.Stats('profile_cython.prof')
stats_cython.sort_stats('cumulative').print_stats(30)  # Show top 30 functions by cumulative time

# 2. Profile the PURE PYTHON version
cProfile.run(f'minimax_bot_test.Bot().minimax(depth={depth}, alpha=-9999, beta=9999, isMaximizing=True, board=board)', 'profile_python.prof')
stats_python = pstats.Stats('profile_python.prof')
stats_python.sort_stats('cumulative').print_stats(30) # Show top 30 functions

print("\n**Cython Stats:**")
stats_cython.sort_stats('cumulative').print_stats(10)
print("\n**Python Stats:**")
stats_python.sort_stats('cumulative').print_stats(10)
