#include "boggle.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <sys/time.h>
#include <thread>

using std::cout, std::endl;
using std::string, std::vector, std::pair;
using std::ifstream;
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

boggle::boggle() {
    vector<vector<char>> b(5, vector<char>(5));
    board = b;
    this->shuffle();
    vector<vector<char>> s(20, vector<char>(60, ' '));
    screen = s;
    elapsed_seconds = 0;
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
    ifstream inFS;
    inFS.open("word-list.txt");
        
    string index;
    while (!inFS.eof()) {
        getline(inFS, index);
        if (input == index) {
            return true;
        }
    }

    inFS.close();
    return false;
}

bool boggle::is_partial_word(string input) {
    ifstream inFS;
    inFS.open("word-list.txt");

    string index;
    while (!inFS.eof()) {
        getline(inFS, index);
        if (input == index.substr(0, input.size())) {
            return true;
        }
    }

    inFS.close();
    return false;
}

void boggle::find_words_at(unsigned int row, unsigned int col, vector<mapped_word> &words, mapped_word cur) {
    if (row >= board.size() || col >= board.at(0).size() || (!is_partial_word(cur.word) && cur.word != "")) {
        return;
    }
    else {
        cur.word += tolower(board.at(row).at(col));
        cur.letters.push_back({row, col});
    }
    if (is_word(cur.word)) {
        cout << "word: " << cur.word << endl;
        words.push_back(cur);
    }

    // Sets - so that the current letter isn't used again
    char t = board.at(row).at(col);
    board.at(row).at(col) = '-';

    find_words_at(row - 1, col - 1, words, cur);
    find_words_at(row - 1, col, words, cur);
    find_words_at(row - 1, col + 1, words, cur);
    find_words_at(row, col + 1, words, cur);
    find_words_at(row + 1, col + 1, words, cur);
    find_words_at(row + 1, col, words, cur);
    find_words_at(row + 1, col - 1, words, cur);
    find_words_at(row, col - 1, words, cur);

    board.at(row).at(col) = t;

    return;
}

// TODO: Make it so that there are no duplicate words in the end
void boggle::find_all_words() {
    vector<mapped_word> words;
    mapped_word cur;
    for (unsigned int row = 0; row < board.size(); ++row) {
        for (unsigned int col = 0; col < board.at(row).size(); ++col) {
            cout << "starting at: " << board.at(row).at(col) << endl;
            find_words_at(row, col, words, cur);
        }
    }
    cout << endl;

    for (auto word : words) {
        cout << word.word << endl;
        word.print_word_chart();
    }
}

void boggle::solve() {
    find_all_words();
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
        search_for_word_at(i.first, i.second, words, cur, input);
    }

    for (auto i : words) {
        if (i.word == input) {
            return true;
        }
    }
    return false;
}

void boggle::search_for_word_at(unsigned int row, unsigned int col, vector<mapped_word> &words, mapped_word cur, string word) {
    if (row >= board.size() || col >= board.at(row).size() || (!is_partial_word(cur.word) && cur.word != "")) {
        return;
    }
    else if (!partial_word_of(cur.word, word)) {
        return;
    }
    else {
        cur.word += tolower(board.at(row).at(col));
        cur.letters.push_back({row, col});
    }
    if (is_word(cur.word)) {
        words.push_back(cur);
    }

    // Sets - so that the current letter isn't used again
    char t = board.at(row).at(col);
    board.at(row).at(col) = '-';

    search_for_word_at(row - 1, col - 1, words, cur, word);
    search_for_word_at(row - 1, col, words, cur, word);
    search_for_word_at(row - 1, col + 1, words, cur, word);
    search_for_word_at(row, col + 1, words, cur, word);
    search_for_word_at(row + 1, col + 1, words, cur, word);
    search_for_word_at(row + 1, col, words, cur, word);
    search_for_word_at(row + 1, col - 1, words, cur, word);
    search_for_word_at(row, col - 1, words, cur, word);

    board.at(row).at(col) = t;

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
    initialize();
    play();
}

void boggle::initialize() {
    gettimeofday(&start_time, NULL);
    write_board_to_screen(0, 0);
}

void boggle::play() {
    std::thread x([this] { this->play_visible(); });
    std::thread y([this] { this->play_invisible(); });
    y.join();
    x.join();
}

void boggle::play_visible() {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    elapsed_seconds = current_time.tv_sec - start_time.tv_sec + 1;
    for (int i = 0; i < 30; ++i) {
        cout << endl;
    }
    print_screen();
    while (elapsed_seconds <= 120) {
        gettimeofday(&current_time, NULL);
        elapsed_seconds = current_time.tv_sec - start_time.tv_sec;

        string input;
        cout << "Enter a word: ";
        std::cin >> input;
        inputted_words.push_back(input);
        print_screen();
    }
}

void boggle::play_invisible() {
    while (elapsed_seconds < 120) {
        std::this_thread::sleep_for(0.01s);
        if (inputted_words.size() > 0) {
            if (word_in_board(inputted_words.at(0))) {
                player_words.push_back(inputted_words.at(0));
                inputted_words.erase(inputted_words.begin());
            }
            else {
                wrong_words.push_back(inputted_words.at(0));
                inputted_words.erase(inputted_words.begin());
            }
        }
    }
    
}

void boggle::print_screen() {
    write_timer_to_screen(0, screen.at(0).size() - 9);
    write_wrong_words_to_screen(2, 17, 30, 45);
    write_player_words_to_screen(2, 17, 15, 30);
    write_inputted_words_to_screen(2, 17, 45, 60);
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

    unsigned int remaining_time = 120 - elapsed_seconds + 1;
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
    else {
        screen.at(r).at(c) = (remaining_time % 10 / 1) + '0';
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

    string s = "Correct:";
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
    unsigned int r = top_row;
    unsigned int c = left_col;

    string s = "Inputted:";
    for (unsigned int i = 0; i < s.size(); ++i) {
        screen.at(r).at(c + i) = s.at(i);
    }
    ++r;

    unsigned int start_point = 0;
    if (inputted_words.size() > bottom_row) {
        start_point = inputted_words.size() - bottom_row;
    }
    for (unsigned int i = 0; i < bottom_row; ++i) {
        if (i < inputted_words.size()) {
            s = inputted_words.at(i + start_point);
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