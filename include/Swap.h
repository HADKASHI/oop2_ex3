#pragma once
#include "Operation.h"

class Swap : public Operation
{
public:

    virtual std::string compute(const std::string& input) const override;
    void print(std::ostream& ostr, bool first_print = false) const override;

};
