#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include "json.hpp"
#include "chess.hpp"

using json = nlohmann::json;
using namespace chess;

struct PieceTables {
    /*
    The goal with the pawns is to let them shelter the king,
    punish creating holes in the castle, and to push them to promotion.
    */
    int w_pawn[8][8] = {
        {100,100,100,100,100,100,100,100}, 
        {150,150,150,150,150,150,150,150}, 
        {110,110,120,130,130,120,110,110}, 
        {105,105,110,125,125,110,105,105}, 
        {100,100,100,120,120,100,100,100}, 
        {105, 95, 90,100,100, 90, 95,105}, 
        {105,110,110, 80, 80,110,110,105}, 
        {100,100,100,100,100,100,100,100}
    };
    int b_pawn[8][8] = {
        {100,100,100,100,100,100,100,100}, 
        {105,110,110, 80, 80,110,110,105},
        {105, 95, 90,100,100, 90, 95,105}, 
        {100,100,100,120,120,100,100,100}, 
        {105,105,110,125,125,110,105,105}, 
        {110,110,120,130,130,120,110,110}, 
        {150,150,150,150,150,150,150,150}, 
        {100,100,100,100,100,100,100,100}
    };

    /*
    The goal with the knights is to try and get them as close
    to the middle as possible as this is where they control the
    most squares. We also want to punish knights on the edges
    as they do not contribute meaningfully to the game.
    */
    int w_knight[8][8] = {
        {250,240,270,270,270,270,260,250},
        {260,280,300,300,300,300,280,260},
        {270,300,310,315,315,310,300,270},
        {270,305,315,320,320,315,305,270},
        {270,300,315,320,320,315,300,270},
        {270,305,310,315,315,310,305,270},
        {260,280,300,305,305,300,280,260},
        {250,260,270,270,270,270,260,250}
    };
    int b_knight[8][8] ={
        {250,240,270,270,270,270,260,250},
        {260,280,300,300,300,300,280,260},
        {270,300,310,315,315,310,300,270},
        {270,305,315,320,320,315,305,270},
        {270,300,315,320,320,315,300,270},
        {270,305,310,315,315,310,305,270},
        {260,280,300,305,305,300,280,260},
        {250,260,270,270,270,270,260,250}
    };
    
    /*
    The goal with the bishops is to try to suggest diagonal lines.
    This value tables suggests fianchettoing the bishops or placing
    them near the mid, or on long diagonals.
    */
    int w_bishop[8][8] = {
        {280,290,290,290,290,290,290,280},
        {290,300,300,300,300,300,300,290},
        {290,300,305,310,310,305,300,290},
        {290,305,305,310,310,305,305,290},
        {290,300,310,310,310,310,300,290},
        {290,310,310,310,310,310,310,290},
        {290,305,300,300,300,300,305,290},
        {280,290,290,290,290,290,290,280}
    };
    int b_bishop[8][8] = {
        {280,290,290,290,290,290,290,280},
        {290,305,300,300,300,300,305,290},
        {290,310,310,310,310,310,310,290},
        {290,300,310,310,310,310,300,290},
        {290,305,305,310,310,305,305,290},
        {290,300,305,310,310,305,300,290},
        {290,300,300,300,300,300,300,290},
        {280,290,290,290,290,290,290,280}
    };
    
    /*
    The goal with the rook is simple. We want them to centralise and
    attack the seventh rank as that is their most potent rank.
    */
    int w_rook[8][8] = {
        {500,500,500,500,500,500,500,500}, 
        {505,510,510,510,510,510,510,505}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {500,500,500,505,505,500,500,500}
    };
    int b_rook[8][8] = {
        {500,500,500,505,505,500,500,500}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {495,500,500,500,500,500,500,495}, 
        {505,510,510,510,510,510,510,505}, 
        {500,500,500,500,500,500,500,500}
    };
    
    /*
    Since the queens is the most powerful piece, there really is not a 
    square where she is not useful. However we want to try and keep her
    engaged, therefore the positives in the middle and negatives on the
    edges.
    */
    int w_queen[8][8] = {
        {880,890,890,895,895,890,890,880},
        {890,900,900,900,900,900,900,890},
        {890,900,905,905,905,905,900,890},
        {895,900,905,905,905,905,900,895},
        {900,900,905,905,905,905,900,895},
        {890,905,905,905,905,905,900,890}, 
        {890,900,905,900,900,900,900,890}, 
        {880,890,890,895,895,890,890,880}
    };
    int b_queen[8][8] = {
        {880,890,890,895,895,890,890,880}, 
        {890,900,905,900,900,900,900,890}, 
        {890,905,905,905,905,905,900,890}, 
        {900,900,905,905,905,905,900,895}, 
        {895,900,905,905,905,905,900,895}, 
        {890,900,905,905,905,905,900,890}, 
        {890,900,900,900,900,900,900,890}, 
        {880,890,890,895,895,890,890,880}
    };
    
    /*
    The king is the most interesting piece to evaluate. In the opening
    want to try and keep the king safe, and in the endgame we want him
    to try and help the pawns promote. I also like king-side castling
    slightly more, thus the ever so slight bias towards the king-side.
    */
    int w_king_mid[8][8] = {
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9980, 9970, 9970, 9960, 9960, 9970, 9970, 9980}, 
        { 9990, 9980, 9980, 9980, 9980, 9980, 9980, 9990}, 
        {10020,10020,10000,10000,10000,10000,10020,10020}, 
        {10020,10030,10010,10000,10000,10010,10031,10020}
    };
    int b_king_mid[8][8] = {
        {10020,10030,10010,10000,10000,10010,10031,10020}, 
        {10020,10020,10000,10000,10000,10000,10020,10020}, 
        { 9990, 9980, 9980, 9980, 9980, 9980, 9980, 9990}, 
        { 9980, 9970, 9970, 9960, 9960, 9970, 9970, 9980}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}, 
        { 9970, 9960, 9960, 9950, 9950, 9960, 9960, 9970}
    };
    
    int w_king_end[8][8] = {
        {9950, 9960,  9970,  9980,  9980,  9970, 9960, 9950},
        {9970, 9980,  9980, 10000, 10000,  9980, 9980, 9970},
        {9970, 9980, 10020, 10030, 10030, 10020, 9980, 9970},
        {9970, 9980, 10030, 10030, 10030, 10030, 9980, 9970},
        {9970, 9980, 10030, 10030, 10030, 10030, 9980, 9970},
        {9970, 9980, 10020, 10030, 10030, 10020, 9980, 9970},
        {9970, 9970, 10000, 10000, 10000, 10000, 9970, 9970},
        {9950, 9970,  9970,  9970,  9970,  9970, 9970, 9950},
    };
    int b_king_end[8][8] = {
        {9950, 9970,  9970,  9970,  9970,  9970, 9970, 9950},
        {9970, 9970, 10000, 10000, 10000, 10000, 9970, 9970},
        {9970, 9980, 10020, 10030, 10030, 10020, 9980, 9970},
        {9970, 9980, 10030, 10030, 10030, 10030, 9980, 9970},
        {9970, 9980, 10030, 10030, 10030, 10030, 9980, 9970},
        {9970, 9980, 10020, 10030, 10030, 10020, 9980, 9970},
        {9970, 9980,  9980, 10000, 10000,  9980, 9980, 9970},
        {9950, 9960,  9970,  9980,  9980,  9970, 9960, 9950},
    };
};

class Bot{
    public:
        Bot();
        Bot(std::string fen, char game_stage);;
        Bot(std::string fen);

        Board board;

        void print_board(const std::string& fen);
        
        std::string get_best_move(Board& board, char colour);
        // void Bot::move(std::string uci_move, Board& board);
    private:
        json openings_data;
        char game_stage = 'o';
        PieceTables piece_tables;
        int piece_values[13] = {1, 3, 3, 5, 9, 100, 1, 3, 3, 5, 9, 100, 0};
        
        std::string get_opening_move(const std::string& fen);
        std::string middle_game_move(int depth, Board& board, char colour);
        
        // void Bot::int_move(Move move, Board& board);
        // void Bot::int_unmove(Move move, Board& board);
        
        // Helper functions
        float minimax(int depth, float alpha, float beta, bool maximizing_player, Board& board);
        float quiescence(Board& board, float alpha, float beta);
        float evaluate(Board& board);
        
        std::vector<Move> generateCaptures(Board& board);
        std::vector<Move> generateChecks(Board& board);
        int determineDepth(const Board& board);
        bool isCheck(Move move, Board& board);
        bool load_openings_data();
        void order_moves(Movelist& moves, Board& board);
        std::string convert_fen(std::string fen);
        int get_random_index(const std::vector<std::string>& vec);
};