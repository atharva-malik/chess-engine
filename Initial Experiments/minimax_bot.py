import chess, json, random

class Bot:
    def __init__(self, board=chess.Board(), colour="w", turn=0):
        self.board = board #* Initialise the board
        self.colour = colour #* Set the colour, w for white, b for black
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

    def minimax(self, depth, alpha, beta, isMaximizing, board=chess.Board()):
        if depth == 0:
            return self.evaluateMiddleGame()
        elif board.is_checkmate():
            return None
        
        if isMaximizing:
            maxEval = -9999
            for move in board.legal_moves:
                board.push(move)
                evaluation = self.minimax(depth - 1, alpha, beta, False, board)
                board.pop()
                maxEval = max(maxEval, evaluation)
                alpha = max(alpha, evaluation)
                if beta <= alpha:
                    break
            return maxEval
        else:
            minEval = 9999
            for move in board.legal_moves:
                board.push(move)
                evaluation = self.minimax(depth - 1, alpha, beta, True, board)
                board.pop()
                minEval = min(minEval, evaluation)
                beta = min(beta, evaluation)
                if beta <= alpha:
                    break
            return minEval
        
    
    def evaluateMiddleGame(self, perspective=1):
        """The evaluation function for the middle game

        Args:
            perspective (int, optional): A positive would mean from white's perspective, negative would mean from. Defaults to 1.

        Returns:
            float: the evaluation of the position
        """
        #? Going to start with a simple evaluation where it values piece value over anything else
        pieces = self.getNumberOfPieces()
        whiteEval = 0
        blackEval = 0
        
        whiteEval += pieces["P"] * 1
        whiteEval += pieces["N"] * 3
        whiteEval += pieces["B"] * 3
        whiteEval += pieces["R"] * 5
        whiteEval += pieces["Q"] * 9
        
        blackEval += pieces["p"] * 1
        blackEval += pieces["n"] * 3
        blackEval += pieces["b"] * 3
        blackEval += pieces["r"] * 5
        blackEval += pieces["q"] * 9
        
        return (whiteEval - blackEval) * perspective

    def getNumberOfPieces(self):
        pieces = {"r": 0, "n": 0, "b": 0, "q": 0, "k": 0, "p": 0, "R": 0, "N": 0, "B": 0, "Q": 0, "K": 0, "P": 0}
        board = str(self.board)
        for i in board:
            if i in pieces.keys():
                pieces[i] += 1
        return pieces

    def __str__(self):
        return str(self.board)