#include "boggle.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

using std::cout, std::endl;
using std::string, std::vector;
using std::ifstream;

boggle::boggle() {
    vector<vector<char>> vec(5, vector<char>(5));
    board = vec;
    this->shuffle();
}

void boggle::print_board() {
    for (unsigned int i = 0; i < board.size(); ++i) {
        for (unsigned int j = 0; j < board.at(i).size(); ++j) {
            cout << board.at(i).at(j) << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

// TODO: Fix so that dice are in random spot on the board
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
    int k = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            board.at(i).at(j) = dice.at(k).at(rand() % 5);
            ++k;
        }
    }

    // board = {
    //     {'B', 'O', 'O', 'F', 'E'},
    //     {'L', 'D', 'N', 'F', 'I'},
    //     {'C', 'S', 'P', 'D', 'W'},
    //     {'T', 'C', 'N', 'E', 'W'},
    //     {'A', 'Y', 'E', 'I', 'L'}};
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

void boggle::find_words_at(unsigned int row, unsigned int col, vector<string> &words, string cur) {
    if (row >= board.size() || col >= board.at(0).size() || (!is_partial_word(cur) && cur != "")) {
        return;
    }
    else {
        // cout << "cur: |" << cur << "|" << endl;
        cur += tolower(board.at(row).at(col));
    }
    if (is_word(cur)) {
        words.push_back(cur);
    }

    // Sets - so that the current letter isn't used again
    char t = board.at(row).at(col);
    board.at(row).at(col) = '-';

    find_words_at(row - 1, col, words, cur);
    find_words_at(row, col + 1, words, cur);
    find_words_at(row + 1, col, words, cur);
    find_words_at(row, col - 1, words, cur);

    // TODO: Figure out why checking diagonals takes too long

    // find_words_at(row - 1, col - 1, words, cur);
    // find_words_at(row - 1, col, words, cur);
    // find_words_at(row - 1, col + 1, words, cur);
    // find_words_at(row, col + 1, words, cur);
    // find_words_at(row + 1, col + 1, words, cur);
    // find_words_at(row + 1, col, words, cur);
    // find_words_at(row + 1, col - 1, words, cur);
    // find_words_at(row, col - 1, words, cur);

    board.at(row).at(col) = t;

    return;
}

vector<string> boggle::find_all_words() {
    vector<string> words;
    for (unsigned int row = 0; row < board.size(); ++row) {
        for (unsigned int col = 0; col < board.at(0).size(); ++col) {
            cout << "starting at: " << board.at(row).at(col) << endl;
            find_words_at(row, col, words, "");
        }
    }
    return words;
}

void boggle::solve() {
    vector<string> s = find_all_words();
    cout << endl;
    cout << "result: " << endl;
    for (auto i : s) {
        cout << i << endl;
    }
}