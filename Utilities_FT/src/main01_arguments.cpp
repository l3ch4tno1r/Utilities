#include <iostream>
#include <cassert>

#include "Utilities/Arguments.h"

int main(int argc, char** argv)
{
    LCN::Utilities::Arguments args{ argc, argv };

    for(const auto arg : args)
        std::cout << arg << '\n';
    
    int a;
    assert(std::errc{} == (args["-a"] >> a));

    std::cout << a << '\n';
}