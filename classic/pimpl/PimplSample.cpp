
#include "PimplSample.h"
#include "PimplSampleImpl.h"

#include <memory>

namespace
{
    std::unique_ptr<PimplSampleImpl> pimpl;
}

PimplSample::PimplSample(std::string name)
{
    pimpl = std::make_unique<PimplSampleImpl>(std::move(name));
}

PimplSample::~PimplSample()
{
    pimpl.reset();
}

std::string PimplSample::HowOldAreYou()
{
    return pimpl->HowOldAreYou();
}
