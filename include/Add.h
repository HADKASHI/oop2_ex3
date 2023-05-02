#pragma once

#include "BinaryOperation.h"
#include <memory>

class Add : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;

    std::string compute(const std::string& inputs) const override;
    void printSymbol(std::ostream& ostr) const override;
};
