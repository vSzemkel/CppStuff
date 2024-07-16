template <typename C>
static void print(const C& v, std::ostream& task_out = std::cout)
{
    if (v.empty())
        return;
    char sep = ' ';
    auto lst = v.size();
    for (const auto& e : v) {
        if (--lst == 0) sep = '\n';
        task_out << e << sep;
    }
}
