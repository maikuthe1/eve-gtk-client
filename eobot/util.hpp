// Supress unused variable warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"



#ifndef UTIL_HPP_INCLUDED
#define UTIL_HPP_INCLUDED

#include <math.h>
#include <vector>
#include <string>

int path_length(int x1, int y1, int x2, int y2);
std::vector<std::string> Args(std::string str);
std::string Lowercase(std::string str);
std::vector<std::string> explode2(const std::string, const std::string);

#endif // UTIL_HPP_INCLUDED
