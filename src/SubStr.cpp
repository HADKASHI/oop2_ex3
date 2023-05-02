#include "SubStr.h"
#include <iostream>
#include <string>

SubStr::SubStr(int index, int chars) :m_start_index(index), m_chars_amount(chars){}

std::string SubStr::compute(const std::string& input) const
{    
    return std::string(input.substr(m_start_index, m_chars_amount));
    return "";

}

void SubStr::print(std::ostream& ostr, bool first_print) const
{
    ostr << "substr_" << m_start_index << "_" << m_chars_amount;
}



