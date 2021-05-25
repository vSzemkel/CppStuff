
static int balance(const char c) {
    if (c == '(') return 1;
    if (c == ')') return -1;
    return 0;
}

    if (g_expr[L] == '(') {
        int bal{1}, pos{L + 1};
        while (bal > 0) 
            bal += balance(g_expr[pos++]);

    assert(g_expr[pos - 1] == ')' && IS_MATCHED)