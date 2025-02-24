import chess

class Bot:
    def __init__(self, board=chess.Board(), turn=0):
        self.board = board #* Initialise the board
        self.turn = turn #* Set the turn, 0 for white, 1 for black