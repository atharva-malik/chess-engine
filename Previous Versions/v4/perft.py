import chess, time

def perft(board, depth):
    if depth == 0:
        return 1

    nodes = 0
    for move in board.legal_moves:
        board.push(move)
        nodes += perft(board, depth - 1)
        board.pop()

    return nodes

def perft_test(fen, depth):
    board = chess.Board(fen)
    return perft(board, depth)

if __name__ == "__main__":
    # Starting position
    #start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    #test_depth = 7

    #start = time.time()
    #nodes = perft_test(start_fen, test_depth)
    #print(f"Perft test at depth {test_depth}: {nodes} nodes")
    #print("Time taken: ", time.time()-start)

    # Kiwipete position
    kiwipete_fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
    kiwipete_depth = 7

    start = time.time()
    kiwipete_nodes = perft_test(kiwipete_fen, kiwipete_depth)
    print(f"Kiwipete position perft test at depth {kiwipete_depth}: {kiwipete_nodes} nodes")
    print("Time taken: ", time.time()-start)
