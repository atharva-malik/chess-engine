"""
The module needs to:
    - have a class for the board which will have:
        - an init method to create the board from FEN (__init__)
        - a method to print the board (__str__)
        - a method to move a piece
        - a method to undo the move
        - a method to check if the game is over
        - a method to declare outcomes
    - have a class for the pieces which will have:
        - a string for piece colour
        - an int for piece type
            - 0 for w_pawn, 1 for w_knight, 2 for w_bishop, 3 for w_rook, 4 for w_queen, 5 for w_king
            - 6 for b_pawn, 7 for b_knight, 8 for b_bishop, 9 for b_rook, 10 for b_queen, 11 for b_king
"""

cdef class Piece:
    cdef str colour
    cdef int piece_type
    cdef int piece_value

    def __cinit__(self, str colour="W", int piece_type=0, int piece_value=1):
        self.colour = colour
        self.piece_type = piece_type
        self.piece_value = piece_value

    def __str__(self):
        return self.colour + str(self.piece_type)

cdef class Board:
    cdef str[8][8] Board
    cdef int[][] bitboards
    ###------- Definitions of the white bitboards -------###
    cdef long white_pawns       = 0000000000000000000000000000000000000000000000000000000000000000
    cdef long white_knights     =
    cdef long white_bishops     =
    cdef long white_rooks       =
    cdef long white_queens      =
    cdef long white_king        =
    cdef long w_en_passant_rank =
    ###-------         end of definitions         -------###
    ###------- Definitions of the black bitboards -------###
    cdef long black_pawns       = 
    cdef long black_knights     = 
    cdef long black_bishops     = 
    cdef long black_rooks       = 
    cdef long black_queens      = 
    cdef long black_king        = 
    cdef long b_en_passant_rank = 
    ###-------         end of definitions         -------###
    cdef str FEN
    cdef int turn

    def __cinit__(self, str FEN="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"):
        self.FEN = FEN
        self.Board = self.createArrayBoard(fen=FEN)
        self.bitboards = []
        self.white_pawns = 0
        self.turn = 0
        self.create_board()