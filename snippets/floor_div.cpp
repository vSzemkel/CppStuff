inline static int64_t floor_div(const int64_t a, const int64_t b) {
    return a / b - ((a ^ b) < 0 && a % b != 0);
}
