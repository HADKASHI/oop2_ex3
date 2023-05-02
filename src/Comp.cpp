#include "Comp.h"

#include <iostream>

std::string Comp::compute(const std::string& inputs) const
{
    const auto resultOfFirst = first()->compute(inputs);
    return second()->compute(resultOfFirst);
}
void Comp::printSymbol(std::ostream& ostr) const
{
    ostr << " -> ";
}



