#include <iostream>

#include "solution.h"

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cout << "Usage: checker.exe \"<expression>\"\n";
		return 1;
	}

	std::cout << argv[1] << "\n" << (isCorrect(argv[1]) ? "correct" : "NOT correct") << "\n";
}