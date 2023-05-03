#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iosfwd>
#include <optional>
#include <iostream>
#include <sstream>

class Operation;

class FunctionCalculator
{
public:
    FunctionCalculator(std::ostream& ostr);
    void run(std::istream& istr);

private:
    void eval();
    void substr();
    void mul();
    void del();
    void read();
    void help();
    void exit();

    template <typename FuncType>
    void binaryFunc()
    {
        if (auto f0 = readOperationIndex(), f1 = readOperationIndex(); f0 && f1)
        {
            m_operations.push_back(std::make_shared<FuncType>(m_operations[*f0], m_operations[*f1]));
        }
    }

    void printOperations() const;

    enum class Action
    {
        Invalid,
        Eval,
        SubStr,
        Add,
        Mul,
        Comp,
        Del,
        Read,
        Help,
        Exit,
    };

    struct ActionDetails
    {
        std::string command;
        std::string description;
        Action action;
    };

    using ActionMap = std::vector<ActionDetails>;
    using OperationList = std::vector<std::shared_ptr<Operation>>;

    const ActionMap m_actions;
    OperationList m_operations;
    bool m_running = true;
    bool m_inFile = false;
    std::istringstream m_istr;
    std::ostream& m_ostr;

    std::optional<int> readOperationIndex();
    Action readAction();
    void runAction(Action action);

    ActionMap createActions() const;
    OperationList createOperations() const ;
};
