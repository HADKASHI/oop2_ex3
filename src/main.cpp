#include "FunctionCalculator.h"
#include <string>
#include <iostream>

int main() try
{
	auto iss = std::istringstream("d");
	iss.exceptions(std::ios::failbit | std::ios::badbit);
    FunctionCalculator(std::cout).run(std::cin);
}

catch (std::exception& e)
{
	std::cerr << "Exception: " << e.what() << "\n";
	return EXIT_FAILURE;
}

catch (...)
{
	std::cerr << "Unknown exception\n";
	return EXIT_FAILURE;
}