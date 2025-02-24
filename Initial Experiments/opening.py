import chess

board = chess.Board()

print(board)

openings = open("Initial Experiments\OpeningBook\Book.txt", "r").read().splitlines()
print(board.fen())
pos = board.fen()
print(("pos " + pos + " -")[0:-6])
#* Bot is white
while True:
    #* Bot turn
    move = ""
    pos = board.fen()
    if ("pos " + pos + " -")[0:-6] in openings:
        move = openings[openings.index(("pos " + pos + " -")[0:-6]) + 1][0:4]
        board.push_san(move)
        print(move)
    else:
        print("NOOOOO")
        print(openings[0])
        break
    print(board)
    inp = input("Enter Your Move: ")
    board.push_san(inp)

print(board.fen())