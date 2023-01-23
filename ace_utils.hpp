#ifndef ACE_UTILS_HEADER
#define ACE_UTILS_HEADER

#include <iostream>
#include <string>

void print(const std::string& message);

#ifdef ACE_UTILS_IMPLEMENTATION

void print(const std::string& message)
{
    std::cout<<message<<"\n";
}

#endif
#endif