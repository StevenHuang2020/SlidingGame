
#include "sliding_game.h"

SlidingGame::SlidingGame(uint8_t row, uint8_t col) :
	m_Row(row), m_Col(col)
{
	init_data();
	print_data();
}

void SlidingGame::init_data()
{
	m_target.clear();
	for (int i = 1; i < total(); i++)
		m_target.push_back(i);
	m_target.push_back(0);

	m_data = m_target;
}

void SlidingGame::new_game()
{
#if 0
	std::random_shuffle(m_data.begin(), m_data.end());
#else
	// simulate to move N times to generate a random game state.
	auto begin = std::chrono::steady_clock::now();

	for (int i = 0; i < 10000; i++) {

		std::vector<int> indexs;
		get_click_available_index(indexs);
		// get_click_available_index2(indexs);

		int index = indexs[rand() % indexs.size()];
		clicked(index);
	}

	auto end = std::chrono::steady_clock::now();
	auto times = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	std::cout << "Run time = " << times << "[µs]\n";
#endif
}

bool SlidingGame::reset_game(uint8_t row, uint8_t col)
{
	if (row <= 2 || col <= 2) {
		//throw std::invalid_argument("Invalid argument!");
		return false;
	}

	if (row == this->row() && col == this->col())
		return false;

	m_Row = row;
	m_Col = col;

	init_data();
	return true;
}

void SlidingGame::print_data(const std::string& str_pre) const
{
	std::cout << str_pre;
	for (int i = 0; i < row(); i++) {
		for (int j = 0; j < col(); j++) {
			size_t id = i * row() + j;
			std::cout << get_data(id);
		}
	}
}

const int& SlidingGame::get_data(size_t index) const
{
	assert(index >= 0 && index < m_data.size());
	return m_data[index];
}

int SlidingGame::get_index(const int& value) const
{
	auto it = std::find(m_data.begin(), m_data.end(), value);

	if (it != m_data.end())
		return it - m_data.begin();

	return -1;
}

void SlidingGame::get_click_available_index(std::vector<int>& indexs) const
{
	indexs.clear();
	int index_blank = get_index(0);
	if (index_blank == -1)
		return;

	if (index_blank % col() != 0) { //leftest column
		indexs.push_back(index_blank - 1);
	}

	if ((index_blank + 1) % col() != 0) { //rightest column
		indexs.push_back(index_blank + 1);
	}

	if (index_blank >= col()) { //no first row
		indexs.push_back(index_blank - col());
	}

	if (index_blank < (col() * (row() - 1))) { //no last row
		indexs.push_back(index_blank + col());
	}
}

void SlidingGame::get_click_available_index2(std::vector<int>& indexs)
{
	indexs.clear();
	int index_blank = get_index(0);
	if (index_blank == -1)
		return;

	indexs.push_back(index_blank - 1);
	indexs.push_back(index_blank + 1);
	indexs.push_back(index_blank + col());
	indexs.push_back(index_blank - col());

	auto it = std::remove_if(indexs.begin(), indexs.end(), [](const int i) {return i < 0; });
	indexs.erase(it, indexs.end());
	//it = std::remove_if(indexs.begin(), indexs.end(), [](const int i) {return i > 8; });
	//indexs.erase(it, indexs.end());

	int i = 0;
	while (i < indexs.size()) {
		if (indexs[i] >= total()) {
			indexs.erase(indexs.begin() + i, indexs.begin() + i + 1);
		}
		else
		{
			i++;
		}
	}
}

bool SlidingGame::blank_block(const int& value) const
{
	return value == 0;
}

bool SlidingGame::can_move_left(const int& index) const
{
	if (index % col() == 0)
		return false;

	return blank_block(get_data(index - 1));
}

bool SlidingGame::can_move_right(const int& index) const
{
	if ((index + 1) % col() == 0)
		return false;

	return blank_block(get_data(index + 1));
}

bool SlidingGame::can_move_down(const int& index) const
{
	if (index >= col() * (row() - 1))
		return false;

	return blank_block(get_data(index + col()));
}

bool SlidingGame::can_move_up(const int& index) const
{
	if (index < col())
		return false;

	return blank_block(get_data(index - col()));
}

bool SlidingGame::swap(const int& index1, const int& index2)
{
	assert(index1 >= 0 && index1 < total());
	assert(index2 >= 0 && index2 < total());
	if (index1 != index2) {
		int tmp = m_data[index1];
		m_data[index1] = m_data[index2];
		m_data[index2] = tmp;
		return true;
	}
	return false;
}

bool SlidingGame::check_over() const
{
	return (m_data == m_target);
}

bool SlidingGame::clicked(const int& index)
{
	if (index < 0 || index >= total())
		return false;

	const int v = get_data(index);
	if (blank_block(v))
		return false;

	bool b_left = can_move_left(index);
	bool b_right = can_move_right(index);
	bool b_up = can_move_up(index);
	bool b_down = can_move_down(index);

	if (b_left)
	{
		return swap(index, index - 1);
	}
	else if (b_right)
	{
		return swap(index, index + 1);
	}
	else if (b_up) {
		return swap(index, index - col());
	}
	else if (b_down) {
		return swap(index, index + col());
	}
	else {
		assert("Can't move this index!");
	}

	return false;
}
