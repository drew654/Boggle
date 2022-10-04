#ifndef _BOGGLE
#define _BOGGLE

#include <string>
#include <vector>

class boggle {
    public:
        boggle();
        void shuffle();
        void print_board();
        void solve();
        void solve2();
        void play_game();
    private:
        class mapped_word {
            public:
                std::string word;
                std::vector<std::pair<int, int>> letters;
                void print_word_chart();
            private:
                std::vector<std::vector<char>> word_chart();
                void print_board(std::vector<std::vector<char>> b);
        };
        std::vector<std::vector<char>> board;
        struct timeval start_time;
        unsigned int elapsed_seconds;
        void initialize();
        void play();
        void print_screen();
        void write_board_to_screen();
        void write_timer_to_screen();
        std::vector<std::vector<std::string>> screen;
        std::vector<std::string> board_words;
        std::vector<std::string> player_words;
        bool is_word(std::string input);
        bool is_partial_word(std::string input);
        void find_words_at(unsigned int row, unsigned int col, std::vector<std::string> &words, std::string cur);
        std::vector<std::string> find_all_words();
        void find_words_at2(unsigned int row, unsigned int col, std::vector<mapped_word> &words, mapped_word cur);
        void find_all_words2();
        void search_for_word_at(unsigned int row, unsigned int col, std::vector<mapped_word> &words, mapped_word cur, std::string word);
        bool partial_word_of(std::string piece, std::string whole);
        bool word_in_board(std::string input);
        std::string uint_to_string(unsigned int input, unsigned int size);
};

#endif