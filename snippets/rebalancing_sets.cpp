/**
 * @brief Given N elements partition them into two sets of sizes K and N - K
 * with a property that K set contains minimal elements of N according to some predicate
 * // see: /kickstart/2022/scheduling_a_meeting.cpp
 */

int64_t totalIn{0};
std::set<std::pair<int, int>> inSet, outSet;
for (int i = 0; i < K; ++i)
inSet.emplace(0, i);
for (int i = K; i < N; ++i)
outSet.emplace(0, i);

while (!inSet.empty() && !outSet.empty() && inSet.rbegin()->first > outSet.begin()->first) {
    const auto toIn = *outSet.begin();
    const auto toOut = *inSet.rbegin();
    totalIn -= toOut.first - toIn.first;
    outSet.insert(inSet.extract(toOut));
    inSet.insert(outSet.extract(toIn));
}
