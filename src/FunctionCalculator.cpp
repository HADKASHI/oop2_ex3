
#include "FunctionCalculator.h"

#include "Add.h"
#include "Mul.h"
#include "Comp.h"
#include "Identity.h"
#include "Swap.h"
#include "Reverse.h"
#include "SubStr.h"
#include <iostream>
#include <algorithm>



FunctionCalculator::FunctionCalculator(std::ostream& ostr)
    : m_actions(createActions()), m_operations(createOperations()), m_ostr(ostr)
{
}

void FunctionCalculator::run(std::istream& istr)
{
    m_ostr << '\n';
    printOperations();
    m_ostr << "Enter command ('help' for the list of available commands): ";

    std::string str;

    while (m_running && std::getline(istr, str))
    {
        m_istr = std::istringstream(str);
        m_istr.exceptions(std::ios::failbit | std::ios::badbit);

        const auto action = readAction();
        runAction(action);
        m_ostr << '\n';
        printOperations();
        m_ostr << "Enter command ('help' for the list of available commands): ";
    } 
}

void FunctionCalculator::eval()
{
    if (auto index = readOperationIndex(); index)
    {
        const auto& operation = m_operations[*index];
        auto input = std::string();
        m_istr >> input;
        //if(input.size() > m_maxStrLength) throw exception

        if (!(m_istr.eof() || (m_istr >> std::ws).eof()))
            //throw too many arguments
            ;
        //if(operation->compute(input).size() > maxStrLen) throw exception
        operation->print(m_ostr, input);
        m_ostr << " = " << operation->compute(input) << '\n';
    }
}
void FunctionCalculator::substr()
{
    int start_index,chars_length;
    m_istr >> start_index>> chars_length;
    m_operations.push_back(std::make_shared<SubStr>(start_index, chars_length));
}

void FunctionCalculator::mul()
{
    int n;
    m_istr >> n;
    if (auto i = readOperationIndex(); i)
    {
        m_operations.push_back(std::make_shared<Mul>(n, m_operations[*i]));
    }
}

void FunctionCalculator::del()
{
    if (auto i = readOperationIndex(); i)
    {
        m_operations.erase(m_operations.begin() + *i);
    }
}

void FunctionCalculator::help()
{
    m_ostr << "The available commands are:\n";
    for (const auto& action : m_actions)
    {
        m_ostr << "* " << action.command << action.description << '\n';
    }
    m_ostr << '\n';
}

void FunctionCalculator::exit()
{
    m_ostr << "Goodbye!\n";
    m_running = false;
}

void FunctionCalculator::printOperations() const
{
    m_ostr << "List of available string operations:\n";
    for (decltype(m_operations.size()) i = 0; i < m_operations.size(); ++i)
    {
        m_ostr << i << ".\t";
        m_operations[i]->print(m_ostr,true);
        m_ostr << '\n';
    }
    m_ostr << '\n';
}

std::optional<int> FunctionCalculator::readOperationIndex()
{
    auto i = 0;
    m_istr >> i;
    

    if (i >= m_operations.size())
    {
        m_ostr << "Operation #" << i << " doesn't exist\n";
        return {};
    }
    return i;
}

FunctionCalculator::Action FunctionCalculator::readAction()
{
    auto action = std::string();
    m_istr >> action;

    const auto i = std::ranges::find(m_actions, action, &ActionDetails::command);
    if (i != m_actions.end())
    {
        return i->action;
    }

    //exception here - invalid action
    return Action::Invalid;
}

void FunctionCalculator::runAction(Action action)
{
    switch (action)
    {
        default:
            m_ostr << "Unknown enum entry used!\n";
            break;

        case Action::Invalid:
            m_ostr << "Command not found\n";
            break;

        case Action::Eval:         eval();                     break;
        case Action::SubStr:       substr();                   break;
        case Action::Add:          binaryFunc<Add>();          break;
        case Action::Mul:          mul();                      break;
        case Action::Comp:         binaryFunc<Comp>();         break;
        case Action::Del:          del();                      break;
        case Action::Help:         help();                     break;
        case Action::Exit:         exit();                     break;
    }
}

FunctionCalculator::ActionMap FunctionCalculator::createActions() const
{
    return ActionMap
    {
        {
            "eval",
            "(uate) num x - compute the result of function #num on the "
            "following x string",
            Action::Eval
        },
         {
            "substr",
            " x y - creates a substring starting from index x with the length of y characters "
            "following x string",
            Action::SubStr
        },
        {
            "add",
            "(on) num1 num2 - Creates an operation that is the concatenation of "
            "operation #num1 and operation #num2",
            Action::Add
        },
        {
            "mul",
            "(tiply) n num - Creates an operation that is the "
            "multiply n of operation #num",
            Action::Mul
        },
        {
            "comp",
            "(osite) num1 num2 - creates an operation that is the composition "
            "of operation #num1 and operation #num2",
            Action::Comp
        },
        {
            "del",
            "(ete) num - delete operation #num from the operation list",
            Action::Del
        },
        {
            "help",
            " - print this command list",
            Action::Help
        },
        {
            "exit",
            " - exit the program",
            Action::Exit
        }
    };
}

FunctionCalculator::OperationList FunctionCalculator::createOperations() const
{
    return OperationList
    {
        std::make_shared<Identity>(),
        std::make_shared<Swap>(),
        std::make_shared<Reverse>()       
        
    };
}
