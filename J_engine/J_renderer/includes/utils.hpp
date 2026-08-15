#pragma once
#include <iostream>
#include <exception>
#include <vector>
#include <filesystem>
#include <stdexcept>

using namespace std;
namespace fs = filesystem;

namespace utilities {
    class NotImplemented : public logic_error {
    public:
        NotImplemented() : logic_error("Functionality not yet implemented") { };
        virtual char const * what() const { return "Functionality not yet implemented."; }
    };

    class TextureError : public exception {
    public:
        TextureError(const char* msg, const char* asset_path) : message(msg), asset_path(asset_path) {}

        const char* what() const noexcept override {
            fs::path curr_path = fs::current_path();
            static string full_msg = message + "\n[Asset Location: " + asset_path + "]" + "\n[cwd: " + curr_path.string() + "]";
            return full_msg.c_str();
        }
    private:
        string message;
        string asset_path;
    };
    
    class FileNotFoundError : public exception {
    public:
        FileNotFoundError(const char* msg, const char* file_path) : message(msg), file_path(file_path) {}

        const char* what() const noexcept override {
            fs::path curr_path = fs::current_path();
            static string full_msg = message + "\n[File Location: " + file_path + "]" + "\n[cwd: " + curr_path.string() + "]";
            return full_msg.c_str();
        }
    private:
        string message;
        string file_path;
    };
}