#include "Reverse.h"
#include <algorithm>
#include <iostream>
std::string Reverse::compute(const std::string& input) const
{
    std::string copy(input);
    std::reverse(copy.begin(), copy.end());
    return copy;
}


void Reverse::print(std::ostream& ostr, bool first_print) const
{
    ostr << "reverse";
}

