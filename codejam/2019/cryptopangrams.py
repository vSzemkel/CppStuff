#!/usr/bin/python3
#
# Cryptopangrams
# https://codingcompetitions.withgoogle.com/codejam/round/0000000000051705/000000000008830b#problem

from math import gcd

T = int(input())
for cn in range(1, T + 1):
    N, L = map(int, input().split())
    A = list(map(int, input().split()))
    assert len(A) == L

    sentence = [None] * (L + 1)
    for i in range(L - 1):
        g = gcd(A[i], A[i + 1])
        if 1 < g < A[i]:
            # it's the vanila prime
            sentence[i + 1] = g
            break
    else:
        assert False

    for j in range(i, -1, -1):
        sentence[j] = A[j] // sentence[j + 1]
    for j in range(i + 2, L + 1):
        sentence[j] = A[j - 1] // sentence[j - 1]

    primdict = {prim:chr(ord('A') + ind) for ind, prim in enumerate(sorted(set(sentence)))}
    #assert len(primdict) == 26

    ANS = "".join(primdict[v] for v in sentence)

    print("Case #{0}: {1}".format(cn, ANS))
    
"""
Run:
py.exe cryptopangrams.py < cryptopangrams.in
"""