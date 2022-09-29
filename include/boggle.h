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
        void solve();
        void solve2();
    private:
        class mapped_word {
            public:
                std::string word;
                std::vector<std::pair<int, int>> letters;
        };
        std::vector<std::vector<char>> board;
        float timer;
        std::vector<std::string> board_words;
        std::vector<std::string> player_words;
        bool is_word(std::string input);
        bool is_partial_word(std::string input);
        void find_words_at(unsigned int row, unsigned int col, std::vector<std::string> &words, std::string cur);
        std::vector<std::string> find_all_words();
        void find_words_at2(unsigned int row, unsigned int col, std::vector<mapped_word> &words, mapped_word cur);
        void find_all_words2();
};

#endif