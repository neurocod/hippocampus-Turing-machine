#include "ChessboardNN.h"
#include "Tokenizer.h"

void printHelp() {
	std::cout << R"(Type 'quit' to quit,
'repr' to see output embedding aimed for target nets.
Other commands are processed by Neural Net directly:
type chess moves in 'E2 E4' format,
'print' to print chessboard after each move,
'noprint' to stop chessboard printing
'enumerate' to enumerate all possible chess moves for current board,
input any tokens and 'count a' or 'count b' commands to count them.)";
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
