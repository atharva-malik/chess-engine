import chess, json, random

class Bot:
    def __init__(self, board=chess.Board(), turn=0):
        self.board = board #* Initialise the board
        self.turn = turn #* Set the turn, 0 for white, 1 for black
        self.gameStage = "O" #* Set the game stage, O for opening, M for middle game, E for end game
    
    def getMove(self):
        if self.gameStage == "O":
            return self.openingMove()
        elif self.gameStage == "M":
            return self.middleGameMove()
        elif self.gameStage == "E":
            return self.endGameMove()
    
    def openingMove(self):
        moves = json.load(fp=open("Initial Experiments\\OpeningBook\\book.json", "r"))
        try:
            return random.choice(moves[self.getFenForOpening()])
        except KeyError:
            self.gameStage = "M"
            return self.middleGameMove()
    
    def middleGameMove(self):
        pass

    def endGameMove(self):
        pass

    def getFenForOpening(self):
        return self.board.fen()[0:-4]

    def __str__(self):
        return str(self.board)