#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <chrono>

/*
* Initial(3*3):
 7   4   5
 8   1   3
 0   2   6

Target:
 1   2   3
 4   5   6
 7   8   0
*/

class SlidingGame
{
public:
    SlidingGame(uint8_t row = 3, uint8_t col = 3);
    ~SlidingGame(){};

public:
    void new_game();
    bool check_over() const;
    int clicked(const int& index);
    bool reset_game(uint8_t row, uint8_t col);
    uint8_t row() const { return m_Row; }
    uint8_t col() const { return m_Col; }
    uint8_t total() const { return m_Row * m_Col; }
    const std::vector<int>& get_data() { return m_data; }

private:
    const int& get_data(size_t index) const;
    void init_data();
    void print_data(const std::string& str_pre = "") const;
    bool blank_block(const int& value) const;
    bool can_move_left(const int& index) const;
    bool can_move_right(const int& index) const;
    bool can_move_down(const int& index) const;
    bool can_move_up(const int& index) const;
    bool swap(const int& index1, const int& index2);
    int get_index(const int& value) const;
    void get_click_available_index(std::vector<int>& indexs) const;
    void get_click_available_index2(std::vector<int>& indexs); //deprecated

private:
    uint8_t m_Row;
    uint8_t m_Col;
    std::vector<int> m_target;
    std::vector<int> m_data;
    std::vector<int> m_lastData;
};
