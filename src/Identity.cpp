#include "Identity.h"

#include <iostream>

std::string Identity::compute(const std::string& input) const
{
    return input; // .front();
}

void Identity::print(std::ostream& ostr, bool first_print) const
{
    ostr << "id";
}


