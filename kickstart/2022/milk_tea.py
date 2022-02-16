#!/usr/bin/python3
""" Milk Tea """
# https://codingcompetitions.withgoogle.com/kickstart/round/00000000008f4332/0000000000943934

T = int(input())
for cn in range(1, T + 1):
    N, M, P = map(int, input().split())
    orders = [str()] * N
    for i in range(N):
        orders[i] = input()
    bl = [0] * M
    for i in range(M):
        b = 0
        sb = input()
        for c in sb:
            b = (b << 1) + ord(c) - ord('0')
        bl[i] = b
    bad = set(bl)

    ideal = 0
    misses = [0] * P
    biases = [(0, 0)] * P
    for p in range(P):
        zero = 0
        for n in range(N):
            if orders[n][p] == '0':
                zero += 1
        ideal <<= 1
        if 2 * zero < N:
            ideal += 1
            misses[p] = zero
        else:
            misses[p] = N - zero
        biases[p] = (abs(N - 2 * zero), p)

    biases = sorted(biases)
    masks = [0] * P
    for p in range(P):
        masks[p] = 1 << (P - biases[p][1] - 1)
    ans = 1 << 100
    MAX = 1 << min(P, 20)
    for bias in range(MAX):
        can, tmp = ideal, bias
        for m in masks:
            if tmp == 0:
                break
            if tmp & 1:
                can ^= m
            tmp >>= 1
        if can not in bad:
            can, tmp = 0, bias
            for _, b in biases:
                d = misses[b]
                can += N - d if tmp & 1 else d
                tmp >>= 1
            if can < ans:
                ans = can

    print(f"Case #{cn}: {ans}")


# Run:
# py.exe milk_tea.py < milk_tea.in
