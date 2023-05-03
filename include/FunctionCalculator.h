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
    void resize();
    void help();
    void exit();

    bool stopReadFile();

    template <typename FuncType>
    void binaryFunc()
    {
        if (m_operations.size() == capacity.m_upperLimit)
            throw std::exception("No space for new operations. consider resize");

        if (auto f0 = readOperationIndex(), f1 = readOperationIndex(); f0 && f1)
        {
            if (!(m_istr.eof() || (m_istr >> std::ws).eof()))
                throw std::out_of_range("too many arguments");

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
        Resize,
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
    int m_maxStrLen = 20;
    
    struct Capacity
    {
        int m_upperLimit = 100;
        int m_lowerLimit = 3;
    };
    Capacity capacity;

    std::optional<int> readOperationIndex();
    Action readAction();
    void runAction(Action action);

    ActionMap createActions() const;
    OperationList createOperations() const ;
};
