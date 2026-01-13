# CMakeList.txt : CMake project for HippoBrain, include source and define
# project specific logic here.

# Add source to this project's executable.
add_executable (HippoBrain
"NeuroCluster.h"
"NeuroCluster.cpp"
"Link.h"
"Link.cpp"
"SemanticNN.h"
"SemanticNN.cpp"
"HippoBrain.h"
"HippoBrain.cpp"
"ChessboardNN.h"
"ChessboardNN.cpp"
"Tokenizer.cpp"
"main.cpp"
 "ConsoleHippoBrain.h" "ConsoleHippoBrain.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET HippoBrain PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
