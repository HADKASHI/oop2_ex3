#include "Mul.h"

#include <algorithm>
#include <iostream>
#include <iterator>



std::string Mul::compute(const std::string& input) const
{
    const auto string_result = func()->compute(input);
    std::string res = "";
    auto num = m_repititions;
    while (num-->0) {
        res += string_result;
    }
    return res;  
}


void Mul::print(std::ostream& ostr, bool first_print) const  {
    if (!first_print)
        ostr << '(';
    func()->print(ostr);
    ostr << " * " << m_repititions;
    if (!first_print)  
        ostr << ')';
}
/*void Mul::print(std::ostream& ostr, const std::string& input) const {
    ostr << '(';
    func()->print(ostr, input);
    ostr << ' * '<< m_repititions<< ')';
}*/


