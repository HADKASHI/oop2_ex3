
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
#include <fstream>



FunctionCalculator::FunctionCalculator(std::ostream& ostr)
    : m_actions(createActions()), m_operations(createOperations()), m_ostr(ostr)
{
    bool valid;
    do
    {
        valid = true;

        std::cout << "Please enter max operations number (between 3 to 100): ";
        try
        {
            std::string input;
            std::getline(std::cin, input);
            m_istr = std::istringstream(input);
            m_istr.exceptions(std::ios::failbit | std::ios::badbit);
            resize();
        }
        catch (std::domain_error& e)
        {
            m_ostr << e.what() << std::endl;
            valid = false;
        }
        catch (std::ios_base::failure)
        {
            m_ostr << "Command: '" << m_istr.str() << "' is not legal" << std::endl;
            m_ostr << "A string has been inserted instead of a integer\n" << std::endl;
            valid = false;
        }

        catch (std::exception& e)
        {
            m_ostr << "Command: '" << m_istr.str() << "' is not legal" << std::endl;
            m_ostr << e.what() << std::endl;
            valid = false;
        }
        
        m_istr.clear();
        m_istr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    } while (!valid);
}

void FunctionCalculator::run(std::istream& istr)
{
    m_ostr << '\n';
    printOperations();
    m_ostr << "Enter command ('help' for the list of available commands): ";

    std::string str;

    while (std::getline(istr, str))
    {
        try {
            m_istr = std::istringstream(str);
            m_istr.exceptions(std::ios::failbit | std::ios::badbit);
            const auto action = readAction();
            runAction(action);
        }

        catch (std::ios_base::failure)
        {
            m_ostr << "Command: '" << m_istr.str() << "' is not legal" << std::endl;
            m_ostr << "A string has been inserted instead of a integer\n" << std::endl;
            if (m_inFile)
            {
                try {
                    if (stopReadFile())
                        break;
                }
                catch (std::exception& e)
                {
                    m_ostr << e.what() << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        }

        catch (std::exception& e)
        {
            m_ostr << "Command: '" << m_istr.str() << "' is not legal" << std::endl;
            m_ostr << e.what() << std::endl;
            if (m_inFile)
            {
                try {
                    if (stopReadFile())
                        break;
                }
                catch (std::exception& e)
                {
                    m_ostr << e.what() << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        }

        if (!m_running)
            break;

        m_istr.clear();
        m_istr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
        if(m_istr.eof())
            throw std::out_of_range("too few arguments");

        m_istr >> input;
      
        if (!(m_istr.eof() || (m_istr >> std::ws).eof()))
            throw std::out_of_range("too many arguments");

        if(operation->compute(input).size() > m_maxStrLen) 
            throw std::out_of_range("string is too long\n");
        
        operation->print(m_ostr, input);
        m_ostr << " = " << operation->compute(input) << '\n';
    }
}
void FunctionCalculator::substr()
{
    if (m_operations.size() == capacity.m_upperLimit)
        throw std::exception("No space for new operations. consider resize");

    int start_index,chars_length;
    if (m_istr.eof())
        throw std::out_of_range("too few arguments");
    m_istr >> start_index;

    if (start_index < 0 || start_index > m_maxStrLen)
        throw std::out_of_range(std::to_string(start_index) + " is out of range\n");

    if (m_istr.eof())
        throw std::out_of_range("too few arguments"); 
    m_istr >> chars_length;

    if (chars_length < 0 || chars_length > m_maxStrLen)
        throw std::out_of_range(std::to_string(chars_length) + " is out of range\n");

    if (!(m_istr.eof() || (m_istr >> std::ws).eof()))
        throw std::out_of_range("too many arguments");

    m_operations.push_back(std::make_shared<SubStr>(start_index, chars_length));
}

void FunctionCalculator::mul()
{
    if (m_operations.size() == capacity.m_upperLimit)
        throw std::exception("No space for new operations. consider resize");

    if (m_istr.eof())
        throw std::out_of_range("too few arguments");
    int n;
    m_istr >> n;

    if (n < 0)
        throw std::invalid_argument(std::to_string(n) + " is not allowed\n");

    
    if (auto i = readOperationIndex(); i)
    {
        if (!(m_istr.eof() || (m_istr >> std::ws).eof()))
            throw std::out_of_range("too many arguments");
        m_operations.push_back(std::make_shared<Mul>(n, m_operations[*i]));
    }
}

void FunctionCalculator::del()
{
    if (m_operations.size() == 0)
        throw std::exception("Nothing to delete");
    if (auto i = readOperationIndex(); i)
    {
        if (!(m_istr.eof() || (m_istr >> std::ws).eof()))
            throw std::out_of_range("too many arguments");

        m_operations.erase(m_operations.begin() + *i);
    }
}

void FunctionCalculator::read()
{
    std::string str;

    if (m_istr.eof())
        throw std::out_of_range("too few arguments");

    m_istr >> str;
    
    if (!(m_istr.eof() || (m_istr >> std::ws).eof()))
        throw std::out_of_range("too many arguments");

    std::ifstream file;

    file.open(str);

    if (!file.is_open())
        throw std::exception("File not found");

    m_inFile = true;

    run(file);    
}

void FunctionCalculator::resize()
{
    if (m_istr.eof())
        throw std::out_of_range("too few arguments");

    int cap;
    m_istr >> cap;

    if (!(m_istr.eof() || (m_istr >> std::ws).eof()))
        throw std::out_of_range("too many arguments");

    if (cap < 3 || cap > 100)
        throw std::domain_error("Invalid capacity. Please try again\n");

    if (cap < m_operations.size())
    {
        m_ostr << "Attention: your'e about to delete operations\n" <<
            "Do you want to proceed? 'y/n' ";
        
        std::string choose;
        std::cin >> choose;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choose != "y" && choose != "n")
            throw std::exception("Invalid argument");

        if (choose == "y")
            m_operations.resize(cap);
    }
    capacity.m_upperLimit = cap;
}

void FunctionCalculator::help()
{
    if (!(m_istr.eof() || (m_istr >> std::ws).eof()))
        throw std::out_of_range("too many arguments");

    m_ostr << "The available commands are:\n";
    for (const auto& action : m_actions)
    {
        m_ostr << "* " << action.command << action.description << '\n';
    }
    m_ostr << '\n';
}

void FunctionCalculator::exit()
{
    if (!(m_istr.eof() || (m_istr >> std::ws).eof()))
        throw std::out_of_range("too many arguments");

    m_ostr << "Goodbye!\n";
    m_running = false;
}

bool FunctionCalculator::stopReadFile()
{
    m_ostr << "Do you want the program to stop reading from the file? y/n\n";
    
    std::string input;
    std::cin >> input;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    if (input != "y" && input != "n")
        throw std::exception("Invalid argument");

    if (input == "y")
        return true;
    return false;
}

void FunctionCalculator::printOperations() const
{
    m_ostr << "Attention: there are " << capacity.m_upperLimit - m_operations.size() <<
        " more operations to add\n";
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
    if (m_istr.eof())
        throw std::out_of_range("too few arguments");

    auto i = 0;
    m_istr >> i;
    
    if (i >= m_operations.size() || i < 0)
        throw std::invalid_argument("Operation #" + std::to_string(i) + " doesn't exist\n");
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

    throw std::invalid_argument("Action not found\n");
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
        case Action::Read:         
        {
            auto inFile = m_inFile;
            read();
            m_inFile = inFile;
            break;
        }
        case Action::Resize:       resize();                   break;
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
            "read",
            " file path - open and reads commands from file",
            Action::Read
        },
        {
            "resize",
            " num - resizes operations capacity to #num size",
            Action::Resize
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
