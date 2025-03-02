import chess, psutil
from json import load
from random import choice
from time import time
from os import getpid

#! What's New?
# Going to use Cython to speed up the minimax algorithm

DEF_FANCY_INT = 9999

cdef class Bot:
    cdef object board
    cdef str colour
    cdef int turn
    cdef str gameStage
    cdef dict openingBook

    def __cinit__(self, board=chess.Board(), colour="w", turn=0):
        self.board = board #* Initialise the board
        self.colour = colour #* Set the colour, w for white, b for black
        self.turn = turn #* Set the turn, 0 for white, 1 for black
        self.gameStage = "O" #* Set the game stage, O for opening, M for middle game, E for end game
        self.openingBook = load(fp=open("Initial Experiments\\OpeningBook\\book.json", "r"))        

    cpdef str getBestMove(self, int depth=10, object board=chess.Board(), str colour=None):
        colour = self.colour if colour == None else colour
        time_start = time()
        if self.gameStage == "O":
            return self.openingMove(depth, board, colour)
        elif self.gameStage == "M":
            return self.middleGameMove(depth, board, colour)
    
    cdef str openingMove(self, int depth, object board, str colour):
        cdef dict moves
        moves = self.openingBook
        try:
            return choice(moves[self.getFenForOpening()])
        except KeyError:
            self.gameStage = "M"
            return self.middleGameMove(depth, board, colour)

    cdef str getFenForOpening(self):
        return self.board.fen()[0:-4]

    cdef str middleGameMove(self, int depth=10, object board=chess.Board(), str colour=None):
        cdef float best_eval
        cdef object best_move
        cdef list moves
        cdef float evaluation
        cdef object move

        colour = self.colour if colour == None else colour

        if colour == "w":
            best_eval = float('-inf')
            best_move = None
            moves = self.orderMoves(list(board.legal_moves), board)
            for move in moves:
                board.push(move)
                time_start = time()
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
        return best_move.uci()

    # cdef float minimax(self, int depth, float alpha, float beta, bint maximizing_player, object board):
    #     if board.is_checkmate():
    #         if board.outcome().winner == chess.WHITE:
    #             return 9999
    #         else:
    #             return -9999
    #     elif depth == 0:
    #         return self.evaluateThePosition(board)

    #     if maximizing_player:
    #         value = -9999
    #         for child in self.orderMoves(list(board.legal_moves), board):
    #             board.push(child)
    #             value = max(value, self.minimax(depth - 1, alpha, beta, False, board))
    #             board.pop()
    #             alpha = max(alpha, value)
    #             if alpha >= beta:
    #                 break  # Beta cutoff
    #         return value
    #     else:
    #         value = 9999
    #         for child in self.orderMoves(list(board.legal_moves), board):
    #             board.push(child)
    #             value = min(value, self.minimax(depth - 1, alpha, beta, True, board))
    #             board.pop()
    #             beta = min(beta, value)
    #             if alpha >= beta:
    #                 break  # Alpha cutoff
    #         return value

    cpdef float minimax(self, int depth, float alpha, float beta, bint isMaximizing, board): # **cdef here - Correct and more efficient!**
        if board.is_checkmate():
            if board.outcome().winner == chess.WHITE:
                return DEF_FANCY_INT
            else:
                return -DEF_FANCY_INT
        elif board.is_stalemate():
            return 0
        elif depth == 0:
            return self.evaluateThePosition(board)

        cdef float maxEval, minEval, evaluation
        cdef move
        cdef list moves

        if isMaximizing:
            maxEval = -DEF_FANCY_INT
            moves = self.orderMoves(list(board.legal_moves), board)
            for move in moves:
                board.push(move)
                evaluation = self.minimax(depth - 1, alpha, beta, False, board) # Recursive call to cdef minimax - FAST
                board.pop()
                maxEval = max(maxEval, evaluation)
                alpha = max(alpha, evaluation)
                if beta <= alpha:
                    break
            return maxEval
        else:
            # ... (rest of the minimizing part, same logic but using cdef minimax for recursion)
            minEval = DEF_FANCY_INT
            moves = self.orderMoves(list(board.legal_moves), board)
            for move in moves:
                board.push(move)
                evaluation = self.minimax(depth - 1, alpha, beta, True, board) # Recursive call to cdef minimax - FAST
                board.pop()
                minEval = min(minEval, evaluation)
                beta = min(beta, evaluation)
                if beta <= alpha:
                    break
            return minEval

    cpdef float evaluateThePosition(self, board):
        cdef list w_pawn, b_pawn, w_knight, b_knight, w_bishop, b_bishop, w_rook, b_rook, w_queen, b_queen, w_king_mid, b_king_mid, w_king_end, b_king_end
        cdef list boardList = self.makeBoardList(board)
        cdef float evaluation = 0
        cdef int col = 0
        cdef int row = 0
        cdef str piece
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

        for col in range(8):
            for row in range(8):
                piece = boardList[col][row]
                if piece == " " or piece == ".":
                    continue
                if piece == "P":
                    evaluation += 100 + w_pawn[col][row]
                elif piece == "N":
                    evaluation += 300 + w_knight[col][row]
                elif piece == "B":
                    evaluation += 300 + w_bishop[col][row]
                elif piece == "R":
                    evaluation += 500 + w_rook[col][row]
                elif piece == "Q":
                    evaluation += 900 + w_queen[col][row]
                elif piece == "K":
                    evaluation += 10000 + w_king_mid[col][row]
                elif piece == "p":
                    evaluation -= 100 + b_pawn[col][row]
                elif piece == "n":
                    evaluation -= 300 + b_knight[col][row]
                elif piece == "b":
                    evaluation -= 300 + b_bishop[col][row]
                elif piece == "r":
                    evaluation -= 500 + b_rook[col][row]
                elif piece == "q":
                    evaluation -= 900 + b_queen[col][row]
                elif piece == "k":
                    evaluation -= 10000 + b_king_mid[col][row]
        
        return (evaluation)/100

    cdef list orderMoves(self, list moves, object board):
        # Prioritize captures and checks
        cdef list scored_moves = []
        cdef object move
        cdef float score

        for move in moves:
            score = 0
            if board.is_capture(move):
                score += 10
            if board.gives_check(move):
                score += 5
            scored_moves.append((move, score))
        return [move for move, score in sorted(scored_moves, key=lambda x: x[1], reverse=True)]

    cpdef object getBoard(self):
        return self.board

    cdef list makeBoardList(self, object board):
        cdef str board_str
        cdef list board_list
        cdef list temp
        cdef str i
        board_str = str(board)
        board_list = []
        temp = board_str.split("\n")
        for i in temp:
            board_list.append(i.split(" "))
        return board_list

    def __str__(self):
        cdef dict symbols
        cdef str board_str
        cdef str piece
        cdef str symbol

        symbols = {
            'R': '♜', 'N': '♞', 'B': '♝', 'Q': '♛', 'K': '♚', 'P': '♟',
            'r': '♖', 'n': '♘', 'b': '♗', 'q': '♕', 'k': '♔', 'p': '♙'
        }
        board_str = str(self.board)
        for piece, symbol in symbols.items():
            board_str = board_str.replace(piece, symbol)
        return board_str


# def set_process_priority(pid, priority_class):
#     try:
#         process = psutil.Process(pid)
#         process.nice(priority_class)

#     except psutil.NoSuchProcess as e:
#         raise psutil.NoSuchProcess(f"Process with PID {pid} not found: {e}")
#     except psutil.AccessDenied as e:
#         raise psutil.AccessDenied(f"Permission denied to change process priority: {e}")
#     except ValueError as e:
#         raise ValueError(f"Invalid priority class: {e}")

# def set_own_priority(priority_class):
#     pid = getpid()
#     set_process_priority(pid, priority_class)

if __name__ == "__main__":
    # set_own_priority(psutil.HIGH_PRIORITY_CLASS)
    import interface