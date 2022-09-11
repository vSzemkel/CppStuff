### First things first
- introduce yourself as an interviewer
- explain your role in the company and recruitment process
- give a candidate a chance to ask questions

### What are sanitizers, e.g. address sanitizer
- -fsanitize=address
- -fsanitize=undefined
- -fsanitize=thread

### What is undefined behaviour, name a few
- signed integer overflow
- general pointer addition
- dereferencing nullptr
- use aftre free

### What stands after the abbreviation
- RAII ar-ej-aj-aj
- CTAD
- CRTP
- SSO

### What is smart poiner
- whats wrong with std::auto_ptr<>
- name them, describe diferences

### What is std::array for?
- stack allocated, fast
- std container interface to use with algorithms
- can swap, can lex compare

### Describe Lower Bound algorithm
- is there any difference between using it with std::vector ans std::set?

### Describe Heap datastructure
- what is the cost of initializing the heap datastructure

### Why there exists a specification of std::vector<bool>
- what is special in using it
- how it organizes its memory

### Describe the difference between stack and heap allocated memory

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
- it returns a pointer range size minus initial s pointer location, after overflowing
- assert(ret * -1 == (int)s)

### Write a function that computes digit length for positive integral

```
template <typename T>
static int numlen(T n) {
    int len{0};
    while (n) {
        n /= 10;
        ++len;
    }
    return len;
}
```

- assert(numlen(1) == 1)
- assert(numlen((1LL<<63) - 1) == 19)
- assert(numlen(n) == 1 + (int)std::log10(n))
