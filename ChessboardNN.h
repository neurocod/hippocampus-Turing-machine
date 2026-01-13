#pragma once
#include "ConsoleHippoBrain.h"

class ChessboardNN: public ConsoleHippoBrain {
public:
	ChessboardNN();
	std::vector<NC*> outputEmbedding()const override;
};