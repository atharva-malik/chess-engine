std::string Bot::middle_game_x_thread(int depth, Board& board, char colour){
    Movelist moves = Movelist();
    movegen::legalmoves(moves, board);
    order_moves(moves, board);
    
    float evaluation;
    Move move = Move();

    const int num_iterations = moves.size();
    std::vector<std::thread> threads;
    std::vector<std::pair<double, std::string>> data(num_iterations); // Vector to store results and moves
    std::mutex results_mutex; // Mutex to protect access to the results vector. IMPORTANT!

    // Create and start a thread for each iteration.
    for (int i = 0; i < num_iterations; ++i) {
        threads.emplace_back(
            // Use a lambda function to capture 'i' by value and handle the result.
            [&, i]() {
                float result = this->search_move(moves[i], board, depth, 1); // Calculate the result
                // Use a lock_guard to ensure thread-safe access to the results vector.
                std::lock_guard<std::mutex> guard(results_mutex);
                data[i].first = result; // Store the result in the correct position.
                data[i].second = uci::moveToUci(moves[i]); // Store the move in the correct position.
            }
        );
        // std::cout << "Thread " << threads.back().get_id() << " started for iteration " << i << std::endl;
    }

    // Wait for all threads to complete.
    //! std::cout << "Waiting for threads to finish...\n";
    for (auto& thread : threads) {
        if (thread.joinable())
            thread.join();
    }
    //! std::cout << "All threads finished.\n";

    std::sort(data.begin(), data.end(), [](auto &a, auto &b) {
        return a.first > b.first;
    });
    // Print the results.
    std::cout << "Results: ";
    for (auto result : data) {
        std::cout << result.first << " ";
        std::cout << result.second << std::endl;
    }
    std::cout << std::endl;

    return data[0].second;
}