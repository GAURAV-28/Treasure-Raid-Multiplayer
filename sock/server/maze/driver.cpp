#include "maze.hpp"
#include <string>
#include <iostream>
#include <bits/stdc++.h>

int main(){

    MazeGenerator *mg = new MazeGenerator();
    const std::string yy = mg->generate();

    std::cout<<yy<<std::endl;

    delete mg;
    
    return 0;
}