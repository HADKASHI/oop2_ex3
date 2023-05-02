#pragma once

#include "Operation.h"
#include <string>
#include <memory>

// Represents the identity operation
// Returns the same set that it gets as input
// Used as the leaf in every operation tree
class Identity : public Operation
{
public:
    std::string compute(const std::string& input) const override;
    void print(std::ostream& ostr, bool first_print =false) const override;

};
