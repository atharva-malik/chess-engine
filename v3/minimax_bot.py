import chess
from json import load
from random import choice

#! What's New?
# Better Evaluation!
#   - It now takes into account the piece positioning!

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
        
        ###------- Definitions of the piece square values -------###
        #* Pawns
        """
        The goal with the pawns is to let them shelter the king,
        punish creating holes in the castle, and to push them to promotion.
        """
        w_pawn = [
            [0,  0,  0,  0,  0,  0,  0,  0],
            [50, 50, 50, 50, 50, 50, 50, 50],
            [10, 10, 20, 30, 30, 20, 10, 10],
            [5,  5, 10, 25, 25, 10,  5,  5],
            [0,  0,  0, 20, 20,  0,  0,  0],
            [5, -5,-10,  0,  0,-10, -5,  5],
            [5, 10, 10,-20,-20, 10, 10,  5],
            [0,  0,  0,  0,  0,  0,  0,  0]
        ]
        b_pawn = w_pawn.copy()[::-1]
        
        #* Knights
        """
        The goal with the knights is to try and get them as close
        to the middle as possible as this is where they control the
        most squares. We also want to punish knights on the edges
        as they do not contribute meaningfully to the game.
        """
        w_knight = [
            [-50,-40,-30,-30,-30,-30,-40,-50],
            [-40,-20,  0,  0,  0,  0,-20,-40],
            [-30,  0, 10, 15, 15, 10,  0,-30],
            [-30,  5, 15, 20, 20, 15,  5,-30],
            [-30,  0, 15, 20, 20, 15,  0,-30],
            [-30,  5, 10, 15, 15, 10,  5,-30],
            [-40,-20,  0,  5,  5,  0,-20,-40],
            [-50,-40,-30,-30,-30,-30,-40,-50]
        ]
        b_knight = w_knight.copy()[::-1]
        
        #* Bishops
        """
        The goal with the bishops is to try to suggest diagonal lines.
        This value tables suggests fianchettoing the bishops or placing
        them near the mid, or on long diagonals.
        """
        w_bishop = [
            [-20,-10,-10,-10,-10,-10,-10,-20],
            [-10,  0,  0,  0,  0,  0,  0,-10],
            [-10,  0,  5, 10, 10,  5,  0,-10],
            [-10,  5,  5, 10, 10,  5,  5,-10],
            [-10,  0, 10, 10, 10, 10,  0,-10],
            [-10, 10, 10, 10, 10, 10, 10,-10],
            [-10,  5,  0,  0,  0,  0,  5,-10],
            [-20,-10,-10,-10,-10,-10,-10,-20]
        ]
        b_bishop = w_bishop[::-1]
        
        #* Rooks
        """
        The goal with the rook is simple. We want them to centralise and
        attack the fifth rank as that is their most potent rank.
        """
        w_rook = [
            [0,  0,  0,  0,  0,  0,  0,  0],
            [5, 10, 10, 10, 10, 10, 10,  5],
            [-5,  0,  0,  0,  0,  0,  0, -5],
            [-5,  0,  0,  0,  0,  0,  0, -5],
            [-5,  0,  0,  0,  0,  0,  0, -5],
            [-5,  0,  0,  0,  0,  0,  0, -5],
            [-5,  0,  0,  0,  0,  0,  0, -5],
            [0,  0,  0,  5,  5,  0,  0,  0]
        ]
        b_rook = w_rook[::-1]
        
        #* Queens
        """
        Since the queens is the most powerful piece, there really is not a 
        square where she is not useful. However we want to try and keep her
        engaged, therefore the positives in the middle and negatives on the
        edges.
        """
        w_queen = [
            [-20,-10,-10, -5, -5,-10,-10,-20],
            [-10,  0,  0,  0,  0,  0,  0,-10],
            [-10,  0,  5,  5,  5,  5,  0,-10],
            [-5,  0,  5,  5,  5,  5,  0, -5],
            [0,  0,  5,  5,  5,  5,  0, -5],
            [-10,  5,  5,  5,  5,  5,  0,-10],
            [-10,  0,  5,  0,  0,  0,  0,-10],
            [-20,-10,-10, -5, -5,-10,-10,-20]
        ]
        b_queen = w_queen[::-1]
        
        #* Kings
        """
        The king is the most interesting piece to evaluate. In the opening
        want to try and keep the king safe, and in the endgame we want him
        to try and help the pawns promote. I also like king-side castling
        slightly more, thus the ever so slight bias towards the king-side.
        """
        w_king_mid = [
            [-30,-40,-40,-50,-50,-40,-40,-30],
            [-30,-40,-40,-50,-50,-40,-40,-30],
            [-30,-40,-40,-50,-50,-40,-40,-30],
            [-30,-40,-40,-50,-50,-40,-40,-30],
            [-20,-30,-30,-40,-40,-30,-30,-20],
            [-10,-20,-20,-20,-20,-20,-20,-10],
            [20, 20,  0,  0,  0,  0, 20, 20],
            [20, 30, 10,  0,  0, 10, 31, 20]
        ]
        b_king_mid = w_king_mid[::-1]
        
        w_king_end = [
            [-50,-40,-30,-20,-20,-30,-40,-50],
            [-30,-20,-10,  0,  0,-10,-20,-30],
            [-30,-10, 20, 30, 30, 20,-10,-30],
            [-30,-10, 30, 40, 40, 30,-10,-30],
            [-30,-10, 30, 40, 40, 30,-10,-30],
            [-30,-10, 20, 30, 30, 20,-10,-30],
            [-30,-30,  0,  0,  0,  0,-30,-30],
            [-50,-30,-30,-30,-30,-30,-30,-50]
        ]
        b_king_end = w_king_end[::-1]
        
        ###-------           End of definitions           -------###
        
        #? Going to start with a simple evaluation where it values piece value over anything else
        boardList = self.makeBoardList(board)
        whiteEval = 0
        blackEval = 0
        
        for col in range(8):
            for row in range(8):
                piece = boardList[col][row]
                if piece == " " or piece == ".":
                    continue
                if piece == "P":
                    whiteEval += 100 + w_pawn[col][row]
                elif piece == "N":
                    whiteEval += 300 + w_knight[col][row]
                elif piece == "B":
                    whiteEval += 300 + w_bishop[col][row]
                elif piece == "R":
                    whiteEval += 500 + w_rook[col][row]
                elif piece == "Q":
                    whiteEval += 900 + w_queen[col][row]
                elif piece == "K":
                    whiteEval += 10000 + w_king_mid[col][row]
                elif piece == "p":
                    blackEval += 100 + b_pawn[col][row]
                elif piece == "n":
                    blackEval += 300 + b_knight[col][row]
                elif piece == "b":
                    blackEval += 300 + b_bishop[col][row]
                elif piece == "r":
                    blackEval += 500 + b_rook[col][row]
                elif piece == "q":
                    blackEval += 900 + b_queen[col][row]
                elif piece == "k":
                    blackEval += 10000 + b_king_mid[col][row]
        
        return (whiteEval - blackEval)/100

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

    #! Probably going to deprecate this function
    def getNumberOfPieces(self, board):
        pieces = {"r": 0, "n": 0, "b": 0, "q": 0, "k": 0, "p": 0, "R": 0, "N": 0, "B": 0, "Q": 0, "K": 0, "P": 0}
        for i in str(board):
            if i in pieces.keys():
                pieces[i] += 1
        return pieces

    def makeBoardList(self, board):
        board_str = str(board)
        board_list = []
        temp = board_str.split("\n")
        for i in temp:
            board_list.append(i.split(" "))
        return board_list

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