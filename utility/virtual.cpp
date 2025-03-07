
#include <iostream>
#include <memory>
#include <string>

// Example to show how virtual methods work in simple scenarios

class base_t
{
  public:
    base_t(std::string name) : _name(std::move(name)) {}
    virtual ~base_t() { std::cout << "base_t::~base_t()" << _name << std::endl; }
    virtual void print() const { std::cout << "base_t::print() " << _name << std::endl; }

  protected:
    std::string _name;
};

class derived_t : public base_t
{
  public:
    derived_t(std::string name) : base_t(std::move(name)) {}
    ~derived_t() override { std::cout << "derived_t::~derived_t()" << _name << std::endl; }
    void print() const override { std::cout << "derived_t::print() " << _name << std::endl; }
};

int main(int, char**)
{
    base_t base("Gienia");
    derived_t derived("Jasia");

    base.print();
    derived.print();
    //base_t::print() Gienia
    //derived_t::print() Jasia
    //derived_t::~derived_t()Jasia
    //base_t::~base_t()Jasia
    //base_t::~base_t()Gienia}

    // removing virtual from ~base_t() doesn't change anything
    // removing virtual from print() changes below from derived_t::print() Disguised to base_t::print() Disguised

    const auto duptr = std::make_unique<derived_t>("Disguised");
    auto base_ptr = reinterpret_cast<base_t*>(duptr.get());
    base_ptr->print();

    // Current version of msvc and clang ignore virtual attribute of destructor
}

/*

Format:
clang-format -i virtual.cpp

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 virtual.cpp -o virtual.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address virtual.cpp -o virtual

Run:
py.exe interactive_runner.py py.exe virtual_testing_tool.py 1 -- virtual.exe
virtual.exe < virtual.in

Input:


Output:


*/
