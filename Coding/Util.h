#pragma once

#include <vector>
#include <string>

namespace Util
{
	
std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

bool is_prime(int n);
float logn(float n, float x); 

};