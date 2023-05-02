#include "Operation.h"
#include <iostream>
void Operation::print(std::ostream& ostr, const std::string& input) const
{
	print(ostr);
	ostr << "(" << input << ")";	
}
