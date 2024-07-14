template <typename C>
static void print(const C& v, std::ostream& task_out = std::cout)
{
    if (v.empty())
        return;
    char sep = ' ';
    const auto lst = int(v.size()) - 1;
    for (int i = 0; i <= lst; ++i) {
        if (i == lst) sep = '\n';
        task_out << v[i] << sep;
    }
}
