#pragma once
#include "Operation.h"

class SubStr : public Operation
{
public:
    SubStr(int index, int chars);
    virtual std::string compute(const std::string& input) const override;
    void print(std::ostream& ostr, bool first_print = false) const override;
    
private:
    int m_start_index, m_chars_amount;
};
