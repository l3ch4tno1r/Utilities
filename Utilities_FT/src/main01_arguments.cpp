#include <iostream>

#include "Utilities/Arguments.h"

int main(int argc, char** argv)
{
    LCN::Utilities::Arguments args{ argc, argv };

    for(const auto arg : args)
        std::cout << arg << '\n';
}