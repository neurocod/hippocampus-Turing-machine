#pragma once
#include <string>
#include <vector>

class Tokenizer {
public:
	static std::vector<std::string> tokenize(const std::string& input);
	static std::vector<std::string> tokenizeReversed(const std::string& input);
};
