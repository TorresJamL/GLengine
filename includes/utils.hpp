#pragma once
#include <iostream>
#include <exception>
#include <vector>

using namespace std;
class NotImplemented : public logic_error {
public:
    NotImplemented() : logic_error("Functionality not yet implemented") { };
    virtual char const * what() const { return "Functionality not yet implemented."; }
};

class TextureError : public exception {
public:
    TextureError(const char* msg, const char* asset_path) : message(msg), asset_path(asset_path) {}

    const char* what() const noexcept {
        string full_msg = "[Asset Location: " + asset_path + "]\n" + message;
        return full_msg.c_str();
    }
private:
    string message;
    string asset_path;
};

namespace G_MATH {
    
} 