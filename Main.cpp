#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <cctype>
#include <stdlib.h>
#include <conio.h>

bool vector_contains_word(const std::vector<std::string>& vec, const std::string& word)
{
	for (const auto& w : vec)
	{
		if (w == word)
		{
			return true;
		}
	}
	return false;
}

std::vector<std::string> get_intersection(const std::vector<std::string>& vec1, const std::vector<std::string>& vec2)
{
	std::vector<std::string> intersection;
	for (const auto& w : vec1)
	{
		if (vector_contains_word(vec2, w))
		{
			intersection.push_back(w);
		}
	}
	return intersection;
}

bool found(std::string word, char looking) {
	for (int i = 0; i < 5; i++) {
		if (word[i] == looking) return true;
	}
	return false;
}

void colorize(const std::string& guess, const std::string& target, std::vector<int>& colors)
{
	for (int i = 0; i < colors.size(); i++)
	{
		if(found(target, guess[i])) colors[i]++;
		
		if (guess[i] == target[i]) colors[i]++;
	}

}

bool CheckifWon(const std::vector<int>& vec)
{
	for (const auto& s : vec) {
		if (s != 2) return false;
	}
	return true;
}

int main()
{
	std::cout << "LOADING...";

	std::vector<std::string> five_words;
	{
		std::ifstream five_word_file("sgb-words.txt");
		for (std::string line; std::getline(five_word_file, line); )
		{
			if (line.empty()) continue;
			
			five_words.push_back(line);
		}
	}

	std::vector<std::string> freq_words;
	{
		std::ifstream freq_word_file("20k.txt");
		for (std::string line; std::getline(freq_word_file, line); )
		{
			if (line.empty()) continue;
			
			freq_words.push_back(line);
		}
	}

	freq_words.resize(2000); // level adjustment 

	auto filtered_words = get_intersection(five_words, freq_words);

	system("cls");
	while (true) {
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> dist(0, filtered_words.size() - 1);
		const std::string target = filtered_words[dist(rng)];

		//std::cout << target << std::endl; // - if u want to see correct anwser on top of console
		bool Won = false;
		std::cout << "====================================================" << std::endl;
		std::cout << "Guess a five letter word, you have also six chances: " << std::endl;

		int chances = 5;
		while (!Won && chances>0){
		
			
			std::string guess;
			std::getline(std::cin, guess);

			for (auto& c : guess) c = std::tolower(c);					
			
			if (guess.size() != 5)
			{
				std::cout << "This isn't FIVE letter word" << std::endl;
				continue;
			}

			if (!vector_contains_word(five_words, guess))
			{
				std::cout << guess << "? That word doesn't exitst in english dictionary." << std::endl;
				continue;
			}

			std::vector<int> color_word(5, 0);
			colorize(guess, target, color_word);
			
			for (int i = 0; i < 5; i++)
			{
				int tmp = color_word[i];
				switch (tmp) {
				case 0:
					std::cout << "\033[0m" << guess[i];
					continue;
				case 1:
					std::cout << "\033[33m" << guess[i];
					continue;
				case 2:
					std::cout << "\033[32m" << guess[i];
					continue;
				}
			}

			std::cout << "\033[0m" << std::endl;
			
			Won = CheckifWon(color_word);
			if (Won) std::cout << "YOU WON!  ";
			chances--;

		}

		if (chances == 0) {
			std::cout << "YOU LOSE IDIOT! " << std::endl;
			std::cout << "Correct anwser: " << target << std::endl;
		}
		std::cout << "Wanna play again?(PRESS 1) If not press any other key." << std::endl;

		if (_getch() == '1') {
			system("cls");
			Won = false;
		}else break;

	}
	return 0;
}