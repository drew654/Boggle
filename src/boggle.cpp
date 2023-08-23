#include "boggle.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <time.h>
#include <sys/time.h>

using std::cout, std::endl;
using std::string, std::vector, std::pair;
using namespace std::chrono_literals;

int v_count(vector<int> v, int i) {
    int count = 0;
    for (auto index : v) {
        if (index == i) {
            ++count;
        }
    }
    return count;
}

int v_highest_count(vector<int> v) {
    int highest_count = 0;
    for (auto i : v) {
        if (v_count(v, i) > highest_count) {
            highest_count = v_count(v, i);
        }
    }
    return highest_count;
}

bool v_contains(vector<string> v, string element) {
    for (auto i : v) {
        if (i == element) {
            return true;
        }
    }
    return false;
}

string str_tolower(string input) {
    string output = "";
    for (unsigned int i = 0; i < input.size(); ++i) {
        output += tolower(input.at(i));
    }
    return output;
}

string str_toupper(string input) {
    string output = "";
    for (unsigned int i = 0; i < input.size(); ++i) {
        output += toupper(input.at(i));
    }
    return output;
}

boggle::boggle() {
    bool has_neccessary_files = true;
    std::ifstream inFS;
    inFS.open("src/Collins_Scrabble_Words_2019_with_definitions.txt");
    if (!inFS.is_open()) {
        cout << "Error: Missing dictionary: \"Collins_Scrabble_Words_2019_with_definitions.txt\" in \"src\" folder" << endl;
        has_neccessary_files = false;
    }
    inFS.close();
    if (!has_neccessary_files) {
        exit(1);
    }
    load_dictionary();
    vector<vector<char>> b(5, vector<char>(5));
    board = b;
    this->shuffle();
    vector<vector<char>> s(20, vector<char>(60, ' '));
    screen = s;
    elapsed_seconds = 0;
    state = title;
    vector<mapped_word> t(25);
    thread_cursors = t;
    vector<vector<string>> i(10);
    inputted_words = i;
    score = 0;
}

void boggle::print_board() {
    cout << "┌───────────┐" << endl;
    for (unsigned int row = 0; row < board.size(); ++row) {
        cout << "│ ";
        for (unsigned int col = 0; col < board.at(row).size(); ++col) {
            cout << board.at(row).at(col) << (col < board.at(row).size()-1 ? " " : "");
        }
        cout << " │" << endl;
    }
    cout << "└───────────┘" << endl;
    cout << endl;
}

void boggle::load_dictionary() {
    std::ifstream inFS;
    inFS.open("src/Collins_Scrabble_Words_2019_with_definitions.txt");
    string line;
    getline(inFS, line);
    getline(inFS, line);
    while (getline(inFS, line)) {
        int tab_index = line.find('\t');
        string word = line.substr(0, tab_index);
        string definition = line.substr(tab_index + 1, line.size() - 1);
        dictionary.insert({word, definition});
    }
    inFS.close();
}

void boggle::shuffle() {
    vector<vector<char>> dice {
        {'Q', 'B', 'Z', 'J', 'X', 'K'},
        {'T', 'O', 'U', 'O', 'T', 'O'},
        {'O', 'V', 'W', 'R', 'G', 'R'},
        {'A', 'A', 'A', 'F', 'S', 'R'},
        {'A', 'U', 'M', 'E', 'E', 'G'},
        {'H', 'H', 'L', 'R', 'D', 'O'},
        {'N', 'H', 'D', 'T', 'H', 'O'},
        {'L', 'H', 'N', 'R', 'O', 'D'},
        {'A', 'F', 'A', 'I', 'S', 'R'},
        {'Y', 'I', 'F', 'A', 'S', 'R'},
        {'T', 'E', 'L', 'P', 'C', 'I'},
        {'S', 'S', 'N', 'S', 'E', 'U'},
        {'R', 'I', 'Y', 'P', 'R', 'H'},
        {'D', 'O', 'R', 'D', 'L', 'N'},
        {'C', 'C', 'W', 'N', 'S', 'T'},
        {'T', 'T', 'O', 'T', 'E', 'M'},
        {'S', 'C', 'T', 'I', 'E', 'P'},
        {'E', 'A', 'N', 'D', 'N', 'N'},
        {'M', 'N', 'N', 'E', 'A', 'G'},
        {'U', 'O', 'T', 'O', 'W', 'N'},
        {'A', 'E', 'A', 'E', 'E', 'E'},
        {'Y', 'I', 'F', 'P', 'S', 'R'},
        {'E', 'E', 'E', 'E', 'M', 'A'},
        {'I', 'T', 'I', 'T', 'I', 'E'},
        {'E', 'T', 'I', 'L', 'I', 'C'}
    };

    srand(time(0));

    int die = 0;
    vector<int> dice_order(dice.size(), rand() % dice.size());
    
    while (v_highest_count(dice_order) > 1) {
        for (unsigned int i = 0; i < dice_order.size(); ++i) {
            for (unsigned int j = 0; j < dice_order.size(); ++j) {
                if (dice_order.at(i) == dice_order.at(j) && i != j) {
                    dice_order.at(i) = rand() % dice.size();
                }
            }
        }
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            board.at(i).at(j) = dice.at(dice_order.at(die)).at(rand() % dice.at(die).size());
            ++die;
        }
    }
}

bool boggle::is_word(string input) {
    if (dictionary.find(str_toupper(input)) != dictionary.end()) {
        return true;
    }
    else {
        return false;
    }
}

bool boggle::is_partial_word(string input) {
    for (auto i : dictionary) {
        if (str_tolower(input) == str_tolower(i.first.substr(0, input.size()))) {
            return true;
        }
    }
    return false;
}

void boggle::find_words_at(vector<vector<char>> b, unsigned int row, unsigned int col, vector<mapped_word> &words, mapped_word cur, unsigned int t_i) {
    if (row >= b.size() || col >= b.at(0).size() || (!is_partial_word(cur.word) && cur.word != "")) {
        return;
    }
    else {
        cur.word += tolower(b.at(row).at(col));
        cur.letters.push_back({row, col});
    }
    if (is_word(cur.word)) {
        word_buffer.push_back(cur.word);
        words.push_back(cur);
    }
    thread_cursors.at(t_i) = cur;

    // Sets - so that the current letter isn't used again
    char t = b.at(row).at(col);
    b.at(row).at(col) = '-';

    find_words_at(b, row - 1, col - 1, words, cur, t_i);
    find_words_at(b, row - 1, col, words, cur, t_i);
    find_words_at(b, row - 1, col + 1, words, cur, t_i);
    find_words_at(b, row, col + 1, words, cur, t_i);
    find_words_at(b, row + 1, col + 1, words, cur, t_i);
    find_words_at(b, row + 1, col, words, cur, t_i);
    find_words_at(b, row + 1, col - 1, words, cur, t_i);
    find_words_at(b, row, col - 1, words, cur, t_i);

    b.at(row).at(col) = t;

    return;
}

void boggle::find_all_words() {
    vector<mapped_word> words;
    mapped_word cur;

    std::thread t0([this, &words, cur] {this->find_words_at(board, 0, 0, words, cur, 0); });
    std::thread t1([this, &words, cur] {this->find_words_at(board, 0, 1, words, cur, 1); });
    std::thread t2([this, &words, cur] {this->find_words_at(board, 0, 2, words, cur, 2); });
    std::thread t3([this, &words, cur] {this->find_words_at(board, 0, 3, words, cur, 3); });
    std::thread t4([this, &words, cur] {this->find_words_at(board, 0, 4, words, cur, 4); });
    std::thread t5([this, &words, cur] {this->find_words_at(board, 1, 0, words, cur, 5); });
    std::thread t6([this, &words, cur] {this->find_words_at(board, 1, 1, words, cur, 6); });
    std::thread t7([this, &words, cur] {this->find_words_at(board, 1, 2, words, cur, 7); });
    std::thread t8([this, &words, cur] {this->find_words_at(board, 1, 3, words, cur, 8); });
    std::thread t9([this, &words, cur] {this->find_words_at(board, 1, 4, words, cur, 9); });
    std::thread t10([this, &words, cur] {this->find_words_at(board, 2, 0, words, cur, 10); });
    std::thread t11([this, &words, cur] {this->find_words_at(board, 2, 1, words, cur, 11); });
    std::thread t12([this, &words, cur] {this->find_words_at(board, 2, 2, words, cur, 12); });
    std::thread t13([this, &words, cur] {this->find_words_at(board, 2, 3, words, cur, 13); });
    std::thread t14([this, &words, cur] {this->find_words_at(board, 2, 4, words, cur, 14); });
    std::thread t15([this, &words, cur] {this->find_words_at(board, 3, 0, words, cur, 15); });
    std::thread t16([this, &words, cur] {this->find_words_at(board, 3, 1, words, cur, 16); });
    std::thread t17([this, &words, cur] {this->find_words_at(board, 3, 2, words, cur, 17); });
    std::thread t18([this, &words, cur] {this->find_words_at(board, 3, 3, words, cur, 18); });
    std::thread t19([this, &words, cur] {this->find_words_at(board, 3, 4, words, cur, 19); });
    std::thread t20([this, &words, cur] {this->find_words_at(board, 4, 0, words, cur, 20); });
    std::thread t21([this, &words, cur] {this->find_words_at(board, 4, 1, words, cur, 21); });
    std::thread t22([this, &words, cur] {this->find_words_at(board, 4, 2, words, cur, 22); });
    std::thread t23([this, &words, cur] {this->find_words_at(board, 4, 3, words, cur, 23); });
    std::thread t24([this, &words, cur] {this->find_words_at(board, 4, 4, words, cur, 24); });

    t0.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    t10.join();
    t11.join();
    t12.join();
    t13.join();
    t14.join();
    t15.join();
    t16.join();
    t17.join();
    t18.join();
    t19.join();
    t20.join();
    t21.join();
    t22.join();
    t23.join();
    t24.join();

    board_words = words;
}

void boggle::solve() {
    bool printing = true;
    std::thread t([this, &printing] {this->print_thread_cursors(printing); });
    find_all_words();
    printing = false;
    t.join();
    generate_solution();
    output_solution();
}

void boggle::mapped_word::print_board(vector<vector<char>> b) {
    cout << "┌───────────┐" << endl;
        for (unsigned int row = 0; row < b.size(); ++row) {
            cout << "│ ";
            for (unsigned int col = 0; col < b.at(row).size(); ++col) {
                cout << b.at(row).at(col) << (col < b.at(row).size()-1 ? " " : "");
            }
            cout << " │" << endl;
        }
    cout << "└───────────┘" << endl;
}

vector<vector<char>> boggle::mapped_word::word_chart() {
    vector<vector<char>> b(5, vector<char>(5, ' '));
    for (unsigned int windex = 0; windex < word.size(); ++windex) {
        b.at(letters.at(windex).first).at(letters.at(windex).second) = toupper(word.at(windex));
    }
    return b;
}

void boggle::mapped_word::print_word_chart() {
    print_board(word_chart());
}

string boggle::mapped_word::get_line(unsigned int line) {
    string output = "";
    vector<vector<char>> c = word_chart();
    for (unsigned int i = 0; i < c.at(line).size(); ++i) {
        output += c.at(line).at(i);
        output += (i < c.at(line).size() - 1 ? " " : "");
    }
    return output;
}

void boggle::print_thread_cursors(bool& printing) {
    while (printing) {
        cout << endl << endl << endl << endl << endl;
        vector<mapped_word> line;
        line = {thread_cursors.at(0), thread_cursors.at(1), thread_cursors.at(2), thread_cursors.at(3), thread_cursors.at(4)};
        print_mapped_word_set(line);
        line = {thread_cursors.at(5), thread_cursors.at(6), thread_cursors.at(7), thread_cursors.at(8), thread_cursors.at(9)};
        print_mapped_word_set(line);
        line = {thread_cursors.at(10), thread_cursors.at(11), thread_cursors.at(12), thread_cursors.at(13), thread_cursors.at(14)};
        print_mapped_word_set(line);
        line = {thread_cursors.at(15), thread_cursors.at(16), thread_cursors.at(17), thread_cursors.at(18), thread_cursors.at(19)};
        print_mapped_word_set(line);
        line = {thread_cursors.at(20), thread_cursors.at(21), thread_cursors.at(22), thread_cursors.at(23), thread_cursors.at(24)};
        print_mapped_word_set(line);
        if (word_buffer.size() > 0) {
            cout << "words found: ";
        }
        for (unsigned int i = 0; i < word_buffer.size(); ++i) {
            cout << word_buffer.at(i) << (i < word_buffer.size() - 1 ? ", " : "");
        }
        cout << endl;
        word_buffer.clear();
        std::this_thread::sleep_for(0.5s);
    }
}

bool boggle::word_in_board(string input) {
    vector<mapped_word> words;
    mapped_word cur;
    vector<pair<int, int>> start_points;
    for (unsigned int row = 0; row < board.size(); ++row) {
        for (unsigned int col = 0; col < board.at(row).size(); ++col) {
            if (board.at(row).at(col) == toupper(input.at(0))) {
                start_points.push_back({row, col});
            }
        }
    }

    for (auto i : start_points) {
        vector<vector<char>> b = board;
        search_for_word_at(b, i.first, i.second, words, cur, input);
    }

    for (auto i : words) {
        if (i.word == input) {
            return true;
        }
    }
    return false;
}

// TODO: Add progress bar by a boggle private variable passed by reference
void boggle::search_for_word_at(vector<vector<char>> b, unsigned int row, unsigned int col, vector<mapped_word> &words, mapped_word cur, string word) {
    if (row >= b.size() || col >= b.at(row).size() || (!is_partial_word(cur.word) && cur.word != "")) {
        return;
    }
    else if (!partial_word_of(cur.word, word)) {
        return;
    }
    else {
        cur.word += tolower(b.at(row).at(col));
        cur.letters.push_back({row, col});
    }
    if (is_word(cur.word)) {
        words.push_back(cur);
    }

    // Sets - so that the current letter isn't used again
    char t = b.at(row).at(col);
    b.at(row).at(col) = '-';

    search_for_word_at(b, row - 1, col - 1, words, cur, word);
    search_for_word_at(b, row - 1, col, words, cur, word);
    search_for_word_at(b, row - 1, col + 1, words, cur, word);
    search_for_word_at(b, row, col + 1, words, cur, word);
    search_for_word_at(b, row + 1, col + 1, words, cur, word);
    search_for_word_at(b, row + 1, col, words, cur, word);
    search_for_word_at(b, row + 1, col - 1, words, cur, word);
    search_for_word_at(b, row, col - 1, words, cur, word);

    b.at(row).at(col) = t;

    return;
}

bool boggle::partial_word_of(string piece, string whole) {
    if (piece == whole.substr(0, piece.size())) {
        return true;
    }
    else {
        return false;
    }
}

void boggle::play_game() {
    board_words.clear();
    solution.clear();
    player_words.clear();
    wrong_words.clear();
    vector<mapped_word> t(25);
    thread_cursors = t;
    word_buffer.clear();
    game_solved = false;
    shuffle();
    start_game();
    play();
}

void boggle::boot_up() {
    string input;
    while (state == title || state == view_rules || state == post_game) {
        if (state == title) {
            print_screen();
            cout << "What would you like to do? (play, view rules, solve board, define, exit)" << endl;
        }
        else if (state == view_rules) {
            print_screen();
            cout << "What would you like to do? (play, title screen, exit)" << endl;
        }
        if (state != post_game) {
            getline(std::cin, input);
        }
        if (input == "play" || state == post_game) {
            if (state == post_game) {
                cout << "What would you like to do? (title screen, " << (!game_solved ? "solve game, " : "") << "exit)" << endl;
                getline(std::cin, input);
                if (input == "title screen") {
                    cout << endl;
                    state = title;
                }
                else if (input == "solve game" && !game_solved) {
                    state = solve_game;
                    cout << endl;
                    solve();
                    game_solved = true;
                    state = post_game;
                }
                else if (input == "exit") {
                    exit(0);
                }
            }
            else {
                cout << endl;
                state = game;
                play_game();
                state = post_game;
                cout << "Total words found: " << player_words.size() << endl;
                cout << "Final score: " << score << endl;
                std::cin.ignore();
            }
        }
        else if (input == "view rules") {
            state = view_rules;
            cout << endl;
        }
        else if (input == "solve board") {
            cout << "Usage: \"solve board <25 letters>\"" << endl;
        }
        else if (input.substr(0, 11) == "solve board") {
            string input_board = input.substr(12, input.size() - 1);
            bool valid_input = true;
            for (unsigned int i = 0; i < input_board.size(); ++i) {
                if (!isalpha(input_board.at(i))) {
                    valid_input = false;
                }
            }
            if (input_board.size() != 25) {
                cout << "Usage: \"solve board <25 letters>\"" << endl;
            }
            else if (!valid_input) {
                cout << "Usage: \"solve board <25 letters>\"" << endl;
            }
            else if (valid_input) {
                unsigned int i = 0;
                for (unsigned int row = 0; row < 5; ++row) {
                    for (unsigned int col = 0; col < 5; ++col) {
                        board.at(row).at(col) = toupper(input_board.at(i));
                        ++i;
                    }
                }
                state = solve_board;
                solve();
                state = title;
            }
        }
        else if (input == "define") {
            cout << "Usage: \"define <word>\"" << endl;
        }
        else if (input.substr(0, 6) == "define") {
            cout << str_toupper(input.substr(7)) << ": " << define(input.substr(7)) << endl;
        }
        else if (input == "title screen") {
            state = title;
            cout << endl;
        }
        else if (input == "exit") {
            exit(0);
        }
    }
}

void boggle::start_game() {
    gettimeofday(&start_time, NULL);
    state = game;
}

void boggle::play() {
    std::thread x([this] { this->play_visible(); });
    std::thread s0([this] {this->sort_words(0); });
    std::thread s1([this] {this->sort_words(1); });
    std::thread s2([this] {this->sort_words(2); });
    std::thread s3([this] {this->sort_words(3); });
    std::thread s4([this] {this->sort_words(4); });
    std::thread s5([this] {this->sort_words(5); });
    std::thread s6([this] {this->sort_words(6); });
    std::thread s7([this] {this->sort_words(7); });
    std::thread s8([this] {this->sort_words(8); });
    std::thread s9([this] {this->sort_words(9); });

    s0.join();
    s1.join();
    s2.join();
    s3.join();
    s4.join();
    s5.join();
    s6.join();
    s7.join();
    s8.join();
    s9.join();
    x.join();

    print_screen();
    state = post_game;
}

void boggle::play_visible() {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    elapsed_seconds = current_time.tv_sec - start_time.tv_sec + 1;
    print_screen();
    unsigned int inputted_count = 0;
    while (elapsed_seconds <= 120) {
        string input;
        cout << "Enter a word: ";
        std::cin >> input;
        inputted_words.at(inputted_count % 10).push_back(input);
        ++inputted_count;
        gettimeofday(&current_time, NULL);
        elapsed_seconds = current_time.tv_sec - start_time.tv_sec;
        print_screen();
    }
    
    while (inputted_words.at(0).size() > 0 || 
           inputted_words.at(1).size() > 0 ||
           inputted_words.at(2).size() > 0 ||
           inputted_words.at(3).size() > 0 ||
           inputted_words.at(4).size() > 0 ||
           inputted_words.at(5).size() > 0 ||
           inputted_words.at(6).size() > 0 ||
           inputted_words.at(7).size() > 0 ||
           inputted_words.at(8).size() > 0 ||
           inputted_words.at(9).size() > 0) {
                std::this_thread::sleep_for(1s);
                print_screen();
                cout << "sorting";
                gettimeofday(&current_time, NULL);
                for (unsigned int i = 0; i <= current_time.tv_sec % 3; ++i) {
                    cout << ".";
                }
                cout << endl;
           } 
}

void boggle::print_screen() {
    if (state == title) {
        clear_screen();
        write_title_to_screen();
    }
    else if (state == game) {
        clear_screen();
        write_board_to_screen(0, 0);
        write_timer_to_screen(0, screen.at(0).size() - 9);
        write_score_to_screen(0, screen.at(0).size() - 20);
        write_wrong_words_to_screen(2, 17, 30, 45);
        write_player_words_to_screen(2, 17, 15, 30);
        write_inputted_words_to_screen(2, 17, 45, 60);
    }
    else if (state == checking_words) {
        clear_screen();
        write_board_to_screen(0, 0);
        write_wrong_words_to_screen(2, 17, 30, 45);
        write_player_words_to_screen(2, 17, 15, 30);
        write_inputted_words_to_screen(2, 17, 45, 60);
    }
    else if (state == view_rules) {
        clear_screen();
        string input = "You have two minutes to type as many words as you can.\n\nHere are the requirements for words:\n\nEach letter after the first must be a horizontal, vertical, or diagonal neighbor of the one before it.\n\nNo individual letter on the board may be used more than\nonce in a word.\n\nNo capitalized or hyphenated words are allowed.";
        write_text_box_to_screen(input, 0, screen.size() - 1, 0, screen.at(0).size() - 1);
    }
    
    cout << "┌";
    for (unsigned int col = 0; col < screen.at(0).size() + 2; ++col) {
        cout << "─";
    }
    cout << "┐" << endl;
    for (unsigned int row = 0; row < screen.size(); ++row) {
        cout << "│ ";
        for (unsigned int col = 0; col < screen.at(row).size(); ++col) {
            switch (screen.at(row).at(col)) {
                case '!': cout << "┌"; break;
                case '@': cout << "─"; break;
                case '#': cout << "┐"; break;
                case '$': cout << "│"; break;
                case '%': cout << "┘"; break;
                case '^': cout << "└"; break;
                default: cout << screen.at(row).at(col);
            }
        }
        cout << " │";
        cout << endl;
    }
    cout << "└";
    for (unsigned int col = 0; col < screen.at(0).size() + 2; ++col) {
        cout << "─";
    }
    cout << "┘" << endl;
}

void boggle::clear_screen() {
    for (unsigned int row = 0; row < screen.size(); ++row) {
        for (unsigned int col = 0; col < screen.at(row).size(); ++col) {
            screen.at(row).at(col) = ' ';
        }
    }
}

void boggle::write_text_box_to_screen(string input, unsigned int top_row, unsigned int bottom_row, unsigned int left_col, unsigned int right_col) {
    unsigned int i = 0;
    for (unsigned int r = top_row; r <= bottom_row && i < input.size(); ++r) {
        for (unsigned int c = left_col; c <= right_col && i < input.size(); ++c) {
            if (input.at(i) == '\n') {
                ++r;
                c = left_col - 1;
            }
            else {
                if (c == right_col && input.at(i) != ' ') {
                    screen.at(r).at(c) = '-';
                    ++r;
                    c = left_col;
                }
                screen.at(r).at(c) = input.at(i);
            }
            ++i;
        }
    }
}

void boggle::write_title_to_screen() {
    vector<vector<char>> t = {{'!', '@', '@', '@', '@', '@', '@', '@', '@', '#',},
                              {'$', ' ', 'B', 'o', 'g', 'g', 'l', 'e', ' ', '$',},
                              {'^', '@', '@', '@', '@', '@', '@', '@', '@', '%'}};

    unsigned int r = screen.size() / 3;
    unsigned int c = (screen.at(r).size() / 2) - (t.at(0).size() / 2);
    for (unsigned int row = 0; row < t.size(); ++row) {
        for (unsigned int col = 0; col < t.at(row).size(); ++col) {
            screen.at(r + row).at(c + col) = t.at(row).at(col);
        }
    }

    string line = "But it's in the command line";
    r += t.size() + 1;
    c = (screen.at(r).size() / 2) - (line.size() / 2);
    for (unsigned int col = 0; col < line.size(); ++col) {
        screen.at(r).at(c + col) = line.at(col);
    }

    line = "...so good luck.";
    r += 3;
    c = (screen.at(r).size() / 2) - (line.size() / 2);
    for (unsigned int col = 0; col < line.size(); ++col) {
        screen.at(r).at(c + col) = line.at(col);
    }
}

void boggle::write_board_to_screen(unsigned int r, unsigned int c) {
    screen.at(r).at(c) = '!';
    ++c;
    for (unsigned int col = 0; col < board.at(0).size() * 2 + 1; ++col) {
        screen.at(r).at(c) = '@';
        ++c;
    }
    screen.at(r).at(c) = '#';
    ++c;
    ++r;

    for (unsigned int row = 0; row < board.size(); ++row) {
        c = 0;
        screen.at(r).at(c) = '$';
        ++c;
        screen.at(r).at(c) = ' ';
        ++c;
        
        for (unsigned int col = 0; col < board.at(row).size(); ++col) {
            screen.at(r).at(c) = board.at(row).at(col);
            ++c;
            if (col < board.at(row).size() - 1) {
                screen.at(r).at(c) = ' ';
                ++c;
            }
        }

        screen.at(r).at(c) = ' ';
        ++c;
        screen.at(r).at(c) = '$';
        ++c;
        ++r;
    }

    c = 0;
    screen.at(r).at(c) = '^';
    ++c;
    for (unsigned int col = 0; col < board.at(0).size() * 2 + 1; ++col) {
        screen.at(r).at(c) = '@';
        ++c;
    }
    screen.at(r).at(c) = '%';
    ++c;
}

void boggle::write_timer_to_screen(unsigned int r, unsigned int c) {
    string s = "Time: ";
    for (unsigned int i = 0; i < s.size(); ++i) {
        screen.at(r).at(c + i) = s.at(i);
    }
    c += s.size();

    int remaining_time = 120 - elapsed_seconds + 1;
    if (remaining_time >= 100) {
        screen.at(r).at(c) = (remaining_time % 1000 / 100) + '0';
        screen.at(r).at(c + 1) = (remaining_time % 100 / 10) + '0';
        screen.at(r).at(c + 2) = (remaining_time % 10 / 1) + '0';
    }
    else if (remaining_time >= 10) {
        screen.at(r).at(c) = (remaining_time % 100 / 10) + '0';
        screen.at(r).at(c + 1) = (remaining_time % 10 / 1) + '0';
        screen.at(r).at(c + 2) = ' ';
    }
    else if (remaining_time > 0) {
        screen.at(r).at(c) = (remaining_time % 10 / 1) + '0';
        screen.at(r).at(c + 1) = ' ';
        screen.at(r).at(c + 2) = ' ';
    }
    else {
        screen.at(r).at(c) = '0';
        screen.at(r).at(c + 1) = ' ';
        screen.at(r).at(c + 2) = ' ';
    }
}

void boggle::write_score_to_screen(unsigned int r, unsigned int c) {
    string s = "Score: ";
    for (unsigned int i = 0; i < s.size(); ++i) {
        screen.at(r).at(c + i) = s.at(i);
    }
    c += s.size();

    if (score >= 100) {
        screen.at(r).at(c) = (score % 1000 / 100) + '0';
        screen.at(r).at(c + 1) = (score % 100 / 10) + '0';
        screen.at(r).at(c + 2) = (score % 10 / 1) + '0';
    }
    else if (score >= 10) {
        screen.at(r).at(c) = (score % 100 / 10) + '0';
        screen.at(r).at(c + 1) = (score % 10 / 1) + '0';
        screen.at(r).at(c + 2) = ' ';
    }
    else if (score > 0) {
        screen.at(r).at(c) = (score % 10 / 1) + '0';
        screen.at(r).at(c + 1) = ' ';
        screen.at(r).at(c + 2) = ' ';
    }
    else {
        screen.at(r).at(c) = '0';
        screen.at(r).at(c + 1) = ' ';
        screen.at(r).at(c + 2) = ' ';
    }
}

void boggle::write_wrong_words_to_screen(unsigned int top_row, unsigned int bottom_row, unsigned int left_col, unsigned int right_col) {
    unsigned int r = top_row;
    unsigned int c = left_col;

    string s = "Incorrect:";
    for (unsigned int i = 0; i < s.size(); ++i) {
        screen.at(r).at(c + i) = s.at(i);
    }
    ++r;

    unsigned int start_point = 0;
    if (wrong_words.size() > bottom_row) {
        start_point = wrong_words.size() - bottom_row;
    }
    for (unsigned int i = 0; i < bottom_row; ++i) {
        if (i < wrong_words.size()) {
            s = wrong_words.at(i + start_point);
            for (unsigned int j = 0; j < right_col - left_col; ++j) {
                if (j < s.size()) {
                    screen.at(r + i).at(c + j) = s.at(j);
                }
                else {
                    screen.at(r + i).at(c + j) = ' ';
                }
            }
        }
        else {
            for (unsigned int j = 0; j < right_col - left_col; ++j) {
                screen.at(r + i).at(c + j) = ' ';
            }
        }
    }
}

void boggle::write_player_words_to_screen(unsigned int top_row, unsigned int bottom_row, unsigned int left_col, unsigned int right_col) {
    unsigned int r = top_row;
    unsigned int c = left_col;

    string s = "Correct (" + std::to_string(player_words.size()) + "): ";
    for (unsigned int i = 0; i < s.size(); ++i) {
        screen.at(r).at(c + i) = s.at(i);
    }
    ++r;

    unsigned int start_point = 0;
    if (player_words.size() > bottom_row) {
        start_point = player_words.size() - bottom_row;
    }
    for (unsigned int i = 0; i < bottom_row; ++i) {
        if (i < player_words.size()) {
            s = player_words.at(i + start_point);
            for (unsigned int j = 0; j < right_col - left_col; ++j) {
                if (j < s.size()) {
                    screen.at(r + i).at(c + j) = s.at(j);
                }
                else {
                    screen.at(r + i).at(c + j) = ' ';
                }
            }
        }
        else {
            for (unsigned int j = 0; j < right_col - left_col; ++j) {
                screen.at(r + i).at(c + j) = ' ';
            }
        }
    }
}

void boggle::write_inputted_words_to_screen(unsigned int top_row, unsigned int bottom_row, unsigned int left_col, unsigned int right_col) {
    vector<string> i_words;
    unsigned int r = top_row;
    unsigned int c = left_col;

    string s = "Inputted:";
    for (unsigned int i = 0; i < s.size(); ++i) {
        screen.at(r).at(c + i) = s.at(i);
    }
    ++r;

    for (unsigned int i = 0; i < inputted_words.size(); ++i) {
        for (unsigned int j = 0; j < inputted_words.at(i).size(); ++j) {
            i_words.push_back(inputted_words.at(i).at(j));
        }
    }

    unsigned int start_point = 0;
    if (i_words.size() > bottom_row) {
        start_point = i_words.size() - bottom_row;
    }
    for (unsigned int i = 0; i < bottom_row; ++i) {
        if (i < i_words.size()) {
            s = i_words.at(i + start_point);
            for (unsigned int j = 0; j < right_col - left_col; ++j) {
                if (j < s.size()) {
                    screen.at(r + i).at(c + j) = s.at(j);
                }
                else {
                    screen.at(r + i).at(c + j) = ' ';
                }
            }
        }
        else {
            for (unsigned int j = 0; j < right_col - left_col; ++j) {
                screen.at(r + i).at(c + j) = ' ';
            }
        }
    }
}

void boggle::remove_duplicate_of_last(vector<string>& input) {
    for (unsigned int i = 0; i < input.size() - 1; ++i) {
        if (input.at(i) == input.at(input.size() - 1)) {
            input.erase(input.begin() + i);
        }
    }
}

void boggle::print_mapped_word_set(vector<mapped_word> word) {
    vector<string> output;
    string line = "";
    for (unsigned int i = 0; i < word.size(); ++i) {
        line += "┌───────────┐";
        line += (i < word.size() - 1 ? " " : "");
    }
    output.push_back(line);
    line = "";
    for (unsigned int j = 0; j < 5; ++j) {
        for (unsigned int i = 0; i < word.size(); ++i) {
            line += "│ ";
            line += word.at(i).get_line(j);
            line += " │";
            line += (i < word.size() - 1 ? " " : "");
        }
        output.push_back(line);
        line = "";
    }
    for (unsigned int i = 0; i < word.size(); ++i) {
        line += "└───────────┘";
        line += (i < word.size() - 1 ? " " : "");
    }
    output.push_back(line);
    
    for (unsigned int i = 0; i < output.size(); ++i) {
        cout << output.at(i) << endl;
    }
}

void boggle::print_solution() {
    for (unsigned int i = 0; i < solution.size(); ++i) {
        print_mapped_word_set(solution.at(i));
    }
}

void boggle::generate_solution() {
    vector<vector<mapped_word>> sol;
    for (auto w : board_words) {
        bool duplicate = false;
        for (unsigned int s = 0; s < sol.size(); ++s) {
            if (w.word == sol.at(s).at(0).word) {
                sol.at(s).push_back(w);
                duplicate = true;
            }
        }
        if (!duplicate) {
            sol.push_back({w});
        }
    }
    solution = sol;
}

void boggle::output_mapped_word_set(vector<mapped_word> word, std::ofstream& outFS) {
    vector<string> output;
    string line = "";
    for (unsigned int i = 0; i < word.size(); ++i) {
        line += "┌───────────┐";
        line += (i < word.size() - 1 ? " " : "");
    }
    output.push_back(line);
    line = "";
    for (unsigned int j = 0; j < 5; ++j) {
        for (unsigned int i = 0; i < word.size(); ++i) {
            line += "│ ";
            line += word.at(i).get_line(j);
            line += " │";
            line += (i < word.size() - 1 ? " " : "");
        }
        output.push_back(line);
        line = "";
    }
    for (unsigned int i = 0; i < word.size(); ++i) {
        line += "└───────────┘";
        line += (i < word.size() - 1 ? " " : "");
    }
    output.push_back(line);
    
    for (unsigned int i = 0; i < output.size(); ++i) {
        outFS << output.at(i) << endl;
    }
}

void boggle::output_solution() {
    std::ofstream outFS;
    string file_name = "output/solution_";
    for (unsigned int row = 0; row < board.size(); ++row) {
        for (unsigned int col = 0; col < board.at(row).size(); ++col) {
            file_name += board.at(row).at(col);
        }
    }
    file_name += ".txt";
    outFS.open(file_name);
    for (unsigned int i = 0; i < solution.size(); ++i) {
        outFS << solution.at(i).at(0).word << endl;
        output_mapped_word_set(solution.at(i), outFS);
        outFS << endl;
    }
    outFS.close();
    cout << "Solution outputted to \"" << file_name << "\"" << endl << endl;
}

void boggle::sort_words(unsigned int index) {
    while (elapsed_seconds <= 120 || inputted_words.at(index).size() > 0) {
        std::this_thread::sleep_for(0.1s);
        if (inputted_words.at(index).size() > 0) {
            if (v_contains(player_words, inputted_words.at(index).at(0))) {
                inputted_words.at(index).erase(inputted_words.at(index).begin());
            }
            else if (v_contains(wrong_words, inputted_words.at(index).at(0))) {
                inputted_words.at(index).erase(inputted_words.at(index).begin());
            }
            else if (word_in_board(inputted_words.at(index).at(0))) {
                player_words.push_back(inputted_words.at(index).at(0));
                add_score(inputted_words.at(index).at(0));
                inputted_words.at(index).erase(inputted_words.at(index).begin());
            }
            else {
                wrong_words.push_back(inputted_words.at(index).at(0));
                inputted_words.at(index).erase(inputted_words.at(index).begin());
            }
        }
    }
}

void boggle::add_score(string input) {
    if (input.size() < 5) {
        score += 1;
    }
    else if (input.size() == 5) {
        score += 2;
    }
    else if (input.size() == 6) {
        score += 3;
    }
    else if (input.size() == 7) {
        score += 5;
    }
    else if (input.size() >= 8) {
        score += 11;
    }
}

string boggle::define(string input) {
    return dictionary.at(str_toupper(input));
}
