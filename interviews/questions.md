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
- use after free

### What stands after the abbreviation

- RAII ar-ej-aj-aj
- CTAD
- CRTP
- SSO

### What is smart poiner

- whats wrong with std::auto_ptr<>
- name them, describe diferences

### What is SRWLock primitive in Windows

- is it available from C++ ? (std::shared_mutex / std::unique_lock / std::shared_lock)
- is it available from C# ? (System.Threading.ReaderWriterLock)

### What is std::array for?

- stack allocated, fast
- std container interface to use with algorithms
- can swap, can lex compare

### What is socket

- after initialization it abstracts network connection with file-like handler

### Describe Lower Bound algorithm

- is there any difference between using it with std::vector ans std::set?

### Describe Heap datastructure

- find a second largest element in a container
- what is the cost of initializing the heap datastructure

### Why there exists a specification of `std::vector<bool>`

- what is special in using it
- how it organizes its memory

### Given a graph with weighted edges and two vertices, can we always compute the shortest path between them

### Describe the difference between stack and heap allocated memory

### How many pointers a std::shared_ptr can operate on? (aliasing constructor)

### Can all int values (4 bytes) be exactly represented as floats (4 bytes)?

significant is 23+1 bits only: (1 << 24) == 16777216

### Explain the meaning of noexcept keyword

- as a declarator
- as an operator

### Is this code correct I?

```C++
std::set my_set = {8, 4, 1, 3, 5};
const auto it1 = std::lower_bound(my_set.begin(), my_set.end(), 6);
const auto it2 = std::upper_bound(my_set.begin(), my_set.end(), 8);
std::cout << *it1 << " " << *it2 << "\n";
```

- use dedicated container methods lower_bound and upper_bound
- it2 == my_set.end() and can not be dereferenced
- ' ' and '\n' are single characters and should by streamed this way

### Is this code correct II?

```C++
std::unordered_map<std::pair<int, std::string>, bool> my_pair;
my_pair[{42, "C++"}] = true;
std::cout << my_pair.size() << '\n';
```

- only hashable types can be used in unordered containers

### What does this code?

```C
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

### Eliminate single occurence of a pattern from text, minimising allocation count

```C++
const auto pos = text.find(pat);
if (pos != std::string::npos)
    text.erase(pos, pat.size());
```

- this is STL question - use string::erase

### Given a 64 bit number, please invert 42nd bit

```C++
value ^= (uint64_t{1} << 42); // if we assume least significant bit to have number 0
value ^= (uint64_t{1} << 41); // if we assume least significant bit to have number 1
```

### Write 2^42 as hex

hex(1<<42)  =>  0x40000000000

### Check if given set of integers have a subset with sum of its elements being exactly the half of the sum of all elements

1, 3, 4, 5, 8, 9 – counterexample for the naive solution. This is NP problem.\
9 + 4 + 3 != 8 + 5 + 1\
9 + 5 + 1 == 8 + 4 + 3

### Given an array of K numbers, write out all the subsets, line by line

### Ship game: given a board, identify number of ships off all size

### Write a function that computes digit length for positive integral

```C++
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

### How many issues do you see in the snippet below

```C++
class Foo {
     int A;

     Foo() : A = 0
     {}

     int Increment() const {
         ++A;
         return A;
     }
};

int main( void ) {
    Foo test;
    test.Increment();
}
```

- A member initialization
- private constructor
- Increment declared const
- Increment is private

### What are results of lower/upper bounds

```C++
std::vector<int> bound_test = {0, 0, 1, 1, 1, 2, 2};
const auto lb = std::lower_bound(bound_test.begin(), bound_test.end(), 1);
const auto ub = std::upper_bound(bound_test.begin(), bound_test.end(), 1);
```

assert(lb - bound_test.begin() == 2);\
assert(ub - bound_test.begin() == 5);
