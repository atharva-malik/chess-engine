# import chess

# board = chess.Board()

# print(board)

# openings = open("Initial Experiments\OpeningBook\Book.txt", "r").read().splitlines()
# print(board.fen())
# pos = board.fen()
# print(("pos " + pos + " -")[0:-6])
# #* Bot is white
# while True:
#     #* Bot turn
#     move = ""
#     pos = board.fen()
#     if ("pos " + pos + " -")[0:-6] in openings:
#         move = openings[openings.index(("pos " + pos + " -")[0:-6]) + 1][0:4]
#         board.push_san(move)
#         print(move)
#     else:
#         print("NOOOOO")
#         print(("pos " + pos + " -")[0:-6])
#         break

# print(board.fen())

#! Messy code below
import json

openings = open("Initial Experiments\OpeningBook\Book.txt", "r").read().split("pos ")
while True:
    try:
        openings.remove("")
    except:
        break

position_list = []
moves = []

for i in openings:
    # print(i)
    testList = i.split()
    finalList =  []
    flag = False
    for j in testList:
        if flag and j != "-" and j[0] not in "1234567890":
            # print(j[0:4])
            finalList.append(j[0:4])
        else:
            if j == "-":
                flag = True
    position_list.append((i.split("\n")[0]))
    moves.append(finalList.copy())
    # finalList.append(i.split("\n"))

print(position_list)
# print(moves)

# print(len(position_list))
# print(len(moves))

# print(openings)
output_dict = {}
with open('output.json', 'w') as f:
    for i in range(len(position_list)):
        output_dict.update({position_list[i]: moves[i]})
        print(i, position_list[2], moves[2])
    json.dump(output_dict, f)
    print(output_dict)
print(moves)