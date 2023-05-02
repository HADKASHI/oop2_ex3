#pragma once

#include "Operation.h"

#include <memory>

class Mul : public Operation
{
public:
    Mul(const int n, const std::shared_ptr<Operation>& func) : m_repititions(n), m_func(func) {}
    std::string compute(const std::string& input) const override;
    void print(std::ostream& ostr, bool first_print = false) const override;
    //void print(std::ostream& ostr, const std::string& input) const override;
    const std::shared_ptr<Operation>& func() const { return m_func; }
private:
    const int m_repititions;
    const std::shared_ptr<Operation> m_func;
};
