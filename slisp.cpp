#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "interpreter.hpp"
#include "interpreter_semantic_error.hpp"

//This class is based on the plotScript project provided in earlier semesters

//Gives the prompt for the slisp program
void prompt() {
	std::cout << "\nslisp> ";
}

//Will read the line of lisp code given
std::string readline() {
	std::string line;
	std::getline(std::cin, line);
	return line;
}

//Prints given error message
void error(const std::string & err_str) {
	std::cerr << "Error: " << err_str << std::endl;
}

//This is the function that takes any input stream
// and parses and evaluates the code
std::pair<int, bool> eval_from_stream(std::istream & stream) {
	Interpreter interp;
	if (!interp.parse(stream)) {
		error("Error: Invalid Program. Could not parse.");
		return std::pair<int, bool>(EXIT_FAILURE, false);
	}
	else {
		try {
			Expression exp = interp.eval();
			std::cout << exp << std::endl;
		}
		catch (const InterpreterSemanticError & ex) {
			std::cerr << ex.what() << std::endl;
			return std::pair<int, bool>(EXIT_FAILURE, false);
		}
	}
	return std::pair<int, bool>(EXIT_SUCCESS, true);;
}

//This handles the input from a file
int eval_from_file(std::string filename) {
	std::ifstream fileIn;
	fileIn.open(filename);
	if (!fileIn.is_open()) {
		perror(filename.c_str());
		//error("Could not open file for reading.");
		return EXIT_FAILURE;
	}
	std::pair<int, bool> result = eval_from_stream(fileIn);
	return result.first;
}

//This handles the input for "-e"
int eval_from_command(std::string argexp) {
	std::istringstream expression(argexp);
	std::pair<int, bool> result = eval_from_stream(expression);
	return result.first;
}

//This is the REPL(read-evaluate-print-loop)
int repl() {
	Interpreter interp;
	while (!std::cin.eof()) {
		prompt();
		std::string line = readline();
		if (line.empty()) continue;
		std::istringstream expression(line);
		if (!interp.parse(expression)) {
			error("Error: Invalid Program. Could not parse.");
		}
		else {
			try {
				Expression exp = interp.eval();
				std::cout << exp << std::endl;
			}
			catch (const InterpreterSemanticError & ex) {
				std::cerr << ex.what() << std::endl;
			}
		}

	}
	return EXIT_SUCCESS;
}

//The main function that decides which of the above
// functions is the appropriate choice to continue
int main(int argc, char *argv[])
{
	if (argc == 2) {
		return eval_from_file(argv[1]);
	}
	else if (argc == 3) {
		if (std::string(argv[1]) == "-e") {
			return eval_from_command(argv[2]);
		}
		else {
			error("Incorrect number of command line arguments.");
		}
	}
	else {
		return repl();
	}

	return EXIT_SUCCESS;
}
