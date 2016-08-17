#include "Util.h"

#include <string>
#include <sstream>


std::vector<std::string> Util::split(const std::string &s, char delim) 
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::vector<std::string> &Util::split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
    std::stringstream ss(s);
    std::string item;
	elems.clear();
    while (std::getline(ss, item, delim)) 
	{
        elems.push_back(item);
    }
    return elems;
}

bool Util::is_prime(int num)
{
     if (num <= 1) return false;
     if (num == 2) return true;
     if (num % 2 == 0) return false;
	 for(int i = 3; i < num / 2; i+= 2)
     {
         if (num % i == 0)
             return false;
     }
     return true;
}


float Util::logn(float n, float x)
{
	return log(x)/log(n);
}
