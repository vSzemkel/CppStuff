### What are sanitizers, e.g. address sanitizer
-fsanitize-address
-fsanitize=undefined
-fsanitize=thread

### What is std::array for?
- stack allocated, fast
- std container interface to use with algorithms
- can swap, can lex compare

### Is this code correct I?
```
std::set my_set = {8, 4, 1, 3, 5};
const auto it1 = std::lower_bound(my_set.begin(), my_set.end(), 6);
const auto it2 = std::upper_bound(my_set.begin(), my_set.end(), 8);
std::cout << *it1 << " " << *it2 << "\n";
```

- use dedicated container methods lower_bound and upper_bound
- it2 == my_set.end() and can not be dereferenced
- ' ' and '\n' are single characters and should by streamed this way

### Is this code correct II?
```
std::unordered_map<std::pair<int, std::string>, bool> my_pair;
my_pair[{42, "C++"}] = true;
std::cout << my_pair.size() << '\n';
```

- only hashable types can be used in unordered containers

### What does this code?
```
int riddle(const char* s) {
    int ret{0};
    if (s == nullptr)
        return 0;
    while (s++)
        ++ret;
    return ret;
}
```

- it will loop for a very long time
- assert(ret * -1 == (int)s)

### Write a function that computes digit length for positive integral

```
static int numlen(const uint64_t n) {
    int len{0}; 
    uint64_t probe{1};
    while (probe <= n) {
        probe *= 10;
        ++len;
    }
    return len;
}
```

- assert(numlen(1) == 1)
- assert(numlen((1LL<<63) - 1) == 19)
- assert(numlen(n) == 1 + (int)std::log10(n))
- it works only for signed types, we need this extra bit for probe