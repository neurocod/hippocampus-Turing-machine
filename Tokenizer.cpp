#include "Tokenizer.h"

std::vector<std::string> Tokenizer::tokenize(const std::string& input) {
	std::vector<std::string> tokens;
	std::string current;

	auto flushCurrent = [&]() {
		if (!current.empty()) {
			tokens.push_back(current);
			current.clear();
		}
	};

	for (unsigned char ch : input) {
		if (std::isalnum(ch) || ch == '_') {
			current += static_cast<char>(ch);
		} else if (std::ispunct(ch)) {
			flushCurrent();
			tokens.emplace_back(1, static_cast<char>(ch));
		} else {
			flushCurrent();
		}
	}

	flushCurrent();
	return tokens;
}
std::vector<std::string> Tokenizer::tokenizeReversed(const std::string& input) {
	auto tokens = tokenize(input);
	std::reverse(tokens.begin(), tokens.end());
	return tokens;
}