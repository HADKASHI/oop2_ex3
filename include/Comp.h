#pragma once

#include "BinaryOperation.h"
#include <string>
#include <memory>

class Comp : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;

    //int inputCount() const override { return first()->inputCount() + second()->inputCount() - 1; }

    std::string compute(const std::string& inputs) const override;
    void printSymbol(std::ostream& ostr) const override;

private:
   
};
