#ifndef _BOGGLE
#define _BOGGLE

#include <string>
#include <vector>

enum screen_state {title, game, post_game, checking_words, view_rules, solve_game, solve_board};

class boggle {
    public:
        boggle();
        void boot_up();
    private:
        class mapped_word {
            public:
                std::string word;
                std::vector<std::pair<int, int>> letters;
                void print_word_chart();
                std::string get_line(unsigned int line);
            private:
                std::vector<std::vector<char>> word_chart();
                void print_board(std::vector<std::vector<char>> b);
        };
        screen_state state;
        std::vector<std::vector<char>> board;
        struct timeval start_time;
        unsigned int elapsed_seconds;
        bool game_solved;
        unsigned int score;
        void shuffle();
        void print_board();
        void solve();
        void play_game();
        void print_thread_cursors(bool& printing);
        void update_time();
        void start_game();
        void play();
        void play_visible();
        void play_invisible();
        void print_screen();
        void clear_screen();
        void write_text_box_to_screen(std::string input, unsigned int top_row, unsigned int bottom_row, unsigned int left_col, unsigned int right_col);
        void write_title_to_screen();
        void write_board_to_screen(unsigned int r, unsigned int c);
        void write_timer_to_screen(unsigned int r, unsigned int c);
        void write_score_to_screen(unsigned int r, unsigned int c);
        void write_wrong_words_to_screen(unsigned int top_row, unsigned int bottom_row, unsigned int left_col, unsigned int right_col);
        void write_player_words_to_screen(unsigned int top_row, unsigned int bottom_row, unsigned int left_col, unsigned int right_col);
        void write_inputted_words_to_screen(unsigned int top_row, unsigned int bottom_row, unsigned int left_col, unsigned int right_col);
        std::vector<std::vector<char>> screen;
        std::vector<mapped_word> board_words;
        std::vector<std::vector<mapped_word>> solution;
        std::vector<std::string> player_words;
        std::vector<std::string> wrong_words;
        std::vector<std::vector<std::string>> inputted_words;
        bool is_word(std::string input);
        bool is_partial_word(std::string input);
        void find_words_at(std::vector<std::vector<char>> b, unsigned int row, unsigned int col, std::vector<mapped_word> &words, mapped_word cur, unsigned int t_i);
        void find_all_words();
        void search_for_word_at(std::vector<std::vector<char>> b, unsigned int row, unsigned int col, std::vector<mapped_word> & words, mapped_word cur, std::string word);
        bool partial_word_of(std::string piece, std::string whole);
        bool word_in_board(std::string input);
        void remove_duplicate_of_last(std::vector<std::string>& input);
        void print_mapped_word_set(std::vector<mapped_word> word);
        void print_solution();
        void generate_solution();
        void output_mapped_word_set(std::vector<mapped_word> word, std::ofstream& outFS);
        void output_solution();
        std::vector<mapped_word> thread_cursors;
        std::vector<std::string> word_buffer;
        void sort_words(unsigned int index);
        void add_score(std::string input);
        std::string define(std::string input);
};

#endif
