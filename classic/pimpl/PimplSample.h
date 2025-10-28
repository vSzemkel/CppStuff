#pragma once

#include <string>

class PimplSample
{
  public:
    PimplSample(std::string name);
    virtual ~PimplSample();
    std::string HowOldAreYou();
};
