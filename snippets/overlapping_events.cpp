struct event_t {
    int64_t start;
    int64_t finish;
    int64_t weight;
    bool operator<(const event_t& other) { return start < other.start; }   // for sort
    bool operator>(const event_t& other) { return finish > other.finish; } // for pq
};

    std::sort(sections.begin(), sections.end());

    std::priority_queue<event_t, std::vector<event_t>, std::greater<>> pq; // {end, weight}
    for (const auto& s : sections) {
        while (!pq.empty() && pq.top().finish < s.start) {
            // do stuff
            pq.pop();
        }

        // do stuff

        pq.push(s);
    }
