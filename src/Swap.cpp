#include "Swap.h"
#include <algorithm>
#include <iostream>
std::string Swap::compute(const std::string& input) const
{   
    std::string res;
    for (auto& c : input)
        if (std::isupper(static_cast<unsigned char>(c)))
            res+= tolower(c);
        else 
            res += toupper(c);
    return res;
}


void Swap::print(std::ostream& ostr, bool first_print) const
{
    ostr << "swapCase";
}

