#!/usr/bin/python3
#
# Binary operator
# https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435c44/00000000007ec290

from random import randint

fn_cache = {}
def fn(a, b):
    if (a,b) in fn_cache:
        return fn_cache[(a,b)]

    ret = randint(0, 10**18)
    fn_cache[(a,b)] = ret
    return ret

class Op:
    def __init__(self, state=None):
        self.state = state

    def __radd__(self, other):
        return Op(other) # +A+ takes left as is

    def __add__(self, other):
        return fn(self.state, other)

def solve(tc):
    n = int(input())
    A = Op()
    out = []
    seen = {}
    for i in range(n):
        s = input().replace("#", "+A+")
        r = eval(s)
        if r not in seen:
            seen[r] = len(seen) + 1
        out.append(seen[r])
    print(f"Case #{tc}:", *out)

t = int(input())
for _ in range(1, t + 1):
    solve(_)

"""
Run:
py.exe binary_operator.py < binary_operator.in
"""