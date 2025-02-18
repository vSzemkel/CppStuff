
#include <iostream>
#include <format>
#include <source_location>
#include <string>

// Five Special Members Chatty Calls

class five_chatty_calls_t {
  public:
    five_chatty_calls_t(const std::string& name, const std::source_location& location = std::source_location::current()) : _name(name) {
        std::cout << std::format("Constructor called for {} at {}:{}\n", _name, location.file_name(), location.line());
    }

    five_chatty_calls_t(const five_chatty_calls_t& other, const std::source_location& location = std::source_location::current()) 
        : _name(other._name) {
        std::cout << std::format("Copy constructor called for {} at {}:{}\n", _name, location.file_name(), location.line());
    }

    five_chatty_calls_t(five_chatty_calls_t&& other, const std::source_location& location = std::source_location::current()) noexcept 
        : _name(std::move(other._name)) {
        std::cout << std::format("Move constructor called for {} at {}:{}\n", _name, location.file_name(), location.line());
    }

    five_chatty_calls_t& operator=(const five_chatty_calls_t& other) {
        if (this != &other) {
            _name = other._name;
            std::cout << std::format("Copy assignment operator called for {}\n", _name);
        }
        return *this;
    }

    five_chatty_calls_t& operator=(five_chatty_calls_t&& other) noexcept {
        if (this != &other) {
            _name = std::move(other._name);
            std::cout << std::format("Move assignment operator called for {}\n", _name);
        }
        return *this;
    }

    ~five_chatty_calls_t() {
        std::cout << std::format("Destructor called for {}\n", _name);
    }

    const std::string& name() const {
        return _name;
    }

  private:
    std::string _name;
};

int main(int, char**)
{
    five_chatty_calls_t a("AAA");
    auto b = a;
    a = std::move(b);
    five_chatty_calls_t c{a};
    b = c;
}

/*

cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 five_chatty_calls.cpp -o five_chatty_calls.exe

*/
