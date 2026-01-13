#include "ChessboardNN.h"
#include "Tokenizer.h"

void printHelp() {
	std::cout << "Type 'quit' to quit, type chess moves in 'E2 E4' format, "
        "'enumerate' to enumerate all possible chess moves for current board, "
        "use `noprint` and `print` to print board after each move, "
        "input any tokens and 'count a' or 'count b' commands to count them\n";
}
int main(int argc, char* argv[]) {
    std::string input;
    for (int i = 1; i < argc; ++i) {
        if (!input.empty())
            input += ' ';
        input += argv[i];
    }
    if (!input.empty())
        std::cout << ">" << input;
    const auto tokens = Tokenizer::tokenizeReversed(input);
    for (auto&& token : tokens) {
        if (token == "help" || token=="-help") {
            printHelp();
            return 0;
        }
    }
	ChessboardNN brain;
    brain._remainingInput = tokens;
    brain.inferenceCycle();
	return 0;
}
