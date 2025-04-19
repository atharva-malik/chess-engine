#include <stdexcept>

class timeout : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Time over!";
        }
};