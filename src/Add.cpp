#include "Add.h"

#include <algorithm>
#include <iostream>

std::string Add::compute(const std::string& inputs) const
{
    const auto a = first()->compute(inputs);
    const auto b = second()->compute(inputs);

    return std::string(a+b);   
}

void Add::printSymbol(std::ostream& ostr) const
{
    ostr << '+';
}
