ACCEPT = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}
ERROR = 17

DFA = [
    [15,  1, 17,  1,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,  2, 17],
    [3,  1,  1,  1,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, 17],
    [17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 16]    
]


def transform(c):
    if c in ['\n', '\t', ' ']: return 0
    if c.isalpha(): return 1
    if c.isdigit(): return 2
    if c == '_': return 3
    if c == '(': return 4
    if c == ')': return 5
    if c == '{': return 6
    if c == '}': return 7
    if c == '[': return 8
    if c == ']': return 9
    if c == '@': return 10
    if c == ':': return 11
    if c == ';': return 12
    if c == ',': return 13
    if c == '.': return 14
    if c == '-': return 15
    if c == '>': return 16
    return -1