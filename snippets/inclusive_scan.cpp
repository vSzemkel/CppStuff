
std::vector<int64_t> g_input, g_partial;

const auto range_sum = [](const int i, const int j){return g_partial[j + 1] - g_partial[i];};

    int N; std::cin >> N;
    g_input.resize(N);
    for (auto& n : g_input)
        std::cin >> n;

    g_partial.resize(N + 1);
    std::partial_sum(g_input.begin(), g_input.end(), g_partial.begin() + 1);

    const auto s = range_sum(2,17);

void partial_init(const std::vector<int64_t>& src, std::vector<int64_t>& dst){const int size=src.size();dst.assign(size,0);for(int i=0;i<size;++i)for(int a=i;a<size;a|=a+1)dst[a]+=src[i];}
void partial_update(std::vector<int64_t>& src, std::vector<int64_t>& dst, const size_t pos, const int64_t val){const auto size=src.size();for(auto i=pos;i<size;i|=i+1)dst[i]+=val-src[pos];src[pos]=val;}
int64_t partial_sum(const std::vector<int64_t>& src, const int i, const int j){int64_t ret{0};for(int k=j+1;k>0;k&=k-1)ret+=src[k-1];for(int k=i;k>0;k&=k-1)ret-=src[k-1];return ret;}

