#ifndef _BOGGLE
#define _BOGGLE

#include <string>
#include <vector>

class boggle {
    public:
        boggle();
        void shuffle();
        float get_time();
        void print_board();
    private:
        std::vector<std::vector<char>> board;
        float timer;
        std::vector<std::string> words;
        std::vector<std::string> player_words;
        bool is_word(std::string input);
};

#endif