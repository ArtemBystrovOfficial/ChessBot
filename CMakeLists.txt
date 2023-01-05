
project(ChessBot)

add_executable(chess_bot main.cpp
                         Board.cpp 
                         Figures.cpp 
                         MovesTemplate.cpp
                         Simulation.cpp
                         Constants.cpp)