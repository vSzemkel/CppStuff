/**
 * Checks if target can be represented as first count candidates linear combiation with + only
*/
bool check_combination(const std::vector<int>& candidates, int count, int target)
{
    const auto& value = candidates[--count];
    if (count == 0)
        return target % value == 0;

    for (int ntarget = target - value; ntarget > 0; ntarget -= value)
        if (check_combination(candidates, count, ntarget))
            return true;

    return false;
}