#pragma once

#include <chrono>
#include <format>
#include <string>

class PimplSampleImpl
{
    using clock_t = std::chrono::high_resolution_clock;
    using duration_t = std::chrono::duration<double, std::milli>;

  public:
    PimplSampleImpl(std::string&& name)
        : _name(std::move(name))
        , _start(clock_t::now()) {};

    std::string HowOldAreYou() {
        const auto d = clock_t::now() - _start;
        return std::format("My name is {} and I'm {} milliseconds old.\n", _name, duration_t{d}.count());
    };

  private:
    std::string _name;
    clock_t::time_point _start;
};
