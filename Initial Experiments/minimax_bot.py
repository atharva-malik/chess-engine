import chess, json, random, time

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

    def getBestMove(self, depth=10, board=chess.Board(), colour="w"):
        if colour == "w":
            best_eval = float('-inf')
            best_move = None
            for move in board.legal_moves:
                board.push(move)
                evaluation = self.minimax(depth, -9999, 9999, True, board)
                board.pop()
                if evaluation > best_eval:
                    best_eval = evaluation
                    best_move = move
        #TODO: This should not be used yet, see if it works later
        else:
            best_eval = float('inf')
            best_move = None
            for move in board.legal_moves:
                board.push(move)
                evaluation = self.minimax(depth, -9999, 9999, False, board)
                board.pop()
                if evaluation < best_eval:
                    best_eval = evaluation
                    best_move = move
        return best_move

    def minimax(self, depth, alpha, beta, isMaximizing, board=chess.Board()):
        if board.is_checkmate():
            if board.outcome().winner == chess.WHITE:
                return 9999
            elif board.outcome().winner == chess.BLACK:
                return -9999
            else:
                return 0
        elif depth == 0:
            #TODO: Test what the heck is wrong with this
            return self.evaluateMiddleGame(board)
        
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
        
    
    def evaluateMiddleGame(self, board):
        """The evaluation function for the middle game

        Returns:
            float: the evaluation of the position
        """
        #? Going to start with a simple evaluation where it values piece value over anything else
        pieces = self.getNumberOfPieces(board)
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
        
        return (whiteEval - blackEval)

    def getNumberOfPieces(self, board):
        pieces = {"r": 0, "n": 0, "b": 0, "q": 0, "k": 0, "p": 0, "R": 0, "N": 0, "B": 0, "Q": 0, "K": 0, "P": 0}
        for i in str(board):
            if i in pieces.keys():
                pieces[i] += 1
        return pieces

    def __str__(self):
        symbols = {
            'r': '♜', 'n': '♞', 'b': '♝', 'q': '♛', 'k': '♚', 'p': '♟',
            'R': '♖', 'N': '♘', 'B': '♗', 'Q': '♕', 'K': '♔', 'P': '♙'
        }
        board_str = str(self.board)
        for piece, symbol in symbols.items():
            board_str = board_str.replace(piece, symbol)
        return board_str


if __name__ == "__main__":
    import interface