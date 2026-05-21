#pragma once
#include <iostream>

class NotImplemented : public std::logic_error {
public:
    NotImplemented() : std::logic_error("Function not yet implemented") { };
    virtual char const * what() const { return "Function not yet implemented."; }
};