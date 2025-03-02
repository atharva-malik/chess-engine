import chess
from json import load
from random import choice

#! What's New?
# Move ordering!

class Bot:
    def __init__(self, board=chess.Board(), colour="w", turn=0):
        self.board = board #* Initialise the board
        self.colour = colour #* Set the colour, w for white, b for black
        self.turn = turn #* Set the turn, 0 for white, 1 for black
        self.gameStage = "O" #* Set the game stage, O for opening, M for middle game, E for end game
        self.openingBook = load(fp=open("Initial Experiments\\OpeningBook\\book.json", "r"))
    
    def getBestMove(self, depth=10, board=chess.Board(), colour=None):
        colour = self.colour if colour == None else colour
        if self.gameStage == "O":
            return self.openingMove()
        elif self.gameStage == "M":
            return self.minimax(depth, board, colour)
    
    def openingMove(self):
        moves = self.openingBook
        try:
            return choice(moves[self.getFenForOpening()])
        except KeyError:
            self.gameStage = "M"
            return self.middleGameMove()

    def getFenForOpening(self):
        return self.board.fen()[0:-4]

    def middleGameMove(self, depth=10, board=chess.Board(), colour=None):
        colour = self.colour if colour == None else colour
        if colour == "w":
            best_eval = float('-inf')
            best_move = None
            moves = self.orderMoves(list(board.legal_moves), board)
            for move in moves:
                board.push(move)
                evaluation = self.minimax(depth, -9999, 9999, False, board)
                board.pop()
                if evaluation > best_eval:
                    best_eval = evaluation
                    best_move = move
        else:
            best_eval = float('inf')
            best_move = None
            moves = self.orderMoves(list(board.legal_moves), board)
            for move in moves:
                board.push(move)
                evaluation = self.minimax(depth, -9999, 9999, True, board)
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
            return self.evaluateThePosition(board)
        
        if isMaximizing:
            maxEval = -9999
            moves = self.orderMoves(list(board.legal_moves), board)
            for move in moves:
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
            moves = self.orderMoves(list(board.legal_moves), board)
            for move in moves:
                board.push(move)
                evaluation = self.minimax(depth - 1, alpha, beta, True, board)
                board.pop()
                minEval = min(minEval, evaluation)
                beta = min(beta, evaluation)
                if beta <= alpha:
                    break
            return minEval

    def evaluateThePosition(self, board):
        """The evaluation function for the game

        Returns:
            float: the evaluation of the position
        """
        
        #? Going to start with a simple evaluation where it values piece value over anything else
        evaluation = 0
        pieces = self.getNumberOfPieces(board)
        
        evaluation += (pieces["P"] - pieces["p"]) * 1
        evaluation += (pieces["N"] - pieces["n"]) * 3
        evaluation += (pieces["B"] - pieces["b"]) * 3
        evaluation += (pieces["R"] - pieces["r"]) * 5
        evaluation += (pieces["Q"] - pieces["q"]) * 9
        
        return evaluation

    def orderMoves(self, moves: list, board):
        # Prioritize captures and checks
        scored_moves = []
        for move in moves:
            score = 0
            if board.is_capture(move):
                score += 10
            if board.gives_check(move):
                score += 5
            scored_moves.append((move, score))
        return [move for move, score in sorted(scored_moves, key=lambda x: x[1], reverse=True)]

    def getNumberOfPieces(self, board):
        pieces = {"r": 0, "n": 0, "b": 0, "q": 0, "k": 0, "p": 0, "R": 0, "N": 0, "B": 0, "Q": 0, "K": 0, "P": 0}
        for i in str(board):
            if i in pieces.keys():
                pieces[i] += 1
        return pieces

    def __str__(self):
        symbols = {
            'R': '♜', 'N': '♞', 'B': '♝', 'Q': '♛', 'K': '♚', 'P': '♟',
            'r': '♖', 'n': '♘', 'b': '♗', 'q': '♕', 'k': '♔', 'p': '♙'
        }
        board_str = str(self.board)
        for piece, symbol in symbols.items():
            board_str = board_str.replace(piece, symbol)
        return board_str


if __name__ == "__main__":
    import interface