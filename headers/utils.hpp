#pragma once

#include <iostream>

struct Logging
{
public:
    void print_all()
    {
        std::cout << std::endl; // Base case: no arguments
    }

    template <typename T, typename... Args> void print_all(T first, Args... rest)
    {
        std::cout << first << " ";
        print_all(rest...);
    }
};

void Log();
