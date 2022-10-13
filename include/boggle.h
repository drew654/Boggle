#ifndef _BOGGLE
#define _BOGGLE

#include <string>
#include <vector>

enum screen_state {title, game, post_game};

class boggle {
    public:
        boggle();
        void shuffle();
        void print_board();
        void solve();
        void play_game();
        void boot_up();
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
        screen_state state;
        std::vector<std::vector<char>> board;
        struct timeval start_time;
        unsigned int elapsed_seconds;
        void start_game();
        void play();
        void play_visible();
        void play_invisible();
        void print_screen();
        void write_title_to_screen();
        void write_board_to_screen(unsigned int r, unsigned int c);
        void write_timer_to_screen(unsigned int r, unsigned int c);
        void write_wrong_words_to_screen(unsigned int top_row, unsigned int bottom_row, unsigned int left_col, unsigned int right_col);
        void write_player_words_to_screen(unsigned int top_row, unsigned int bottom_row, unsigned int left_col, unsigned int right_col);
        void write_inputted_words_to_screen(unsigned int top_row, unsigned int bottom_row, unsigned int left_col, unsigned int right_col);
        std::vector<std::vector<char>> screen;
        std::vector<std::string> board_words;
        std::vector<std::string> player_words;
        std::vector<std::string> wrong_words;
        std::vector<std::string> inputted_words;
        bool is_word(std::string input);
        bool is_partial_word(std::string input);
        void find_words_at(unsigned int row, unsigned int col, std::vector<mapped_word> &words, mapped_word cur);
        void find_all_words();
        void search_for_word_at(unsigned int row, unsigned int col, std::vector<mapped_word> &words, mapped_word cur, std::string word);
        bool partial_word_of(std::string piece, std::string whole);
        bool word_in_board(std::string input);
};

#endif