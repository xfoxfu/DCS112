#include <iostream>
#include <sstream>
#include <exception>
#include <iterator>
#include <string>
#include <cassert>
#include "error.hpp"
#include "color.hpp"

using namespace std;

Error::Error(string total,
             string message,
             unsigned int begin,
             unsigned int length) : total(total),
                                    begin(begin),
                                    length(length),
                                    message(message)
{
    if (length <= 0)
    {
        this->total.push_back(' ');
        this->length = 1;
    }
}

Error::Error(Span total_span,
             Span error_span,
             string message) : Error(string(total_span.begin, total_span.end),
                                     message,
                                     distance(total_span.begin, error_span.begin),
                                     distance(error_span.begin, error_span.end))
{
}

string Error::to_string() const
{
    stringstream ss;
    ss << *this;
    return ss.str();
}

ostream &operator<<(ostream &out, const Error &error)
{
    // 2x!5!!
    // 012345
    // cout << "DEBUG: (" << error.total << ") 0, " << error.begin << ", " << error.begin + error.length << endl;
    assert(error.length >= 1);
    assert(error.begin + error.length <= error.total.size());
    // assert(error.total.substr(error.begin, error.begin + error.length) == " ");

    string hint = "INPUT:" + to_string(error.begin) + ": ";
    out
        // output hint line
        << color::FG_YELLOW << hint << color::OP_RESET
        // output string before error
        << error.total.substr(0, error.begin)
        // output error section
        << color::BG_RED << color::OP_BOLD << error.total.substr(error.begin, error.length) << color::OP_RESET
        // output rest string
        << error.total.substr(error.begin + error.length) << endl
        // output hint line
        << string(hint.length() + error.begin, ' ')
        << color::FG_YELLOW << string(error.length, '^') << color::OP_RESET << endl
        // output error message
        << color::FG_RED << "Error: " << color::OP_RESET
        << color::OP_BOLD << error.message << color::OP_RESET;
    return out;
}
