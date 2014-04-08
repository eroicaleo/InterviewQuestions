#!/usr/bin/env python3

def GenTriplet(password, length):
    l = len(password)
    if l <= length:
        yield password
    elif length == 1:
        for c in password:
            yield c
    else:
        for i in range(l-length+1):
            for code in GenTriplet(password[i+1:], length-1):
                yield password[i] + code


for code in GenTriplet('google', 3):
    print(code)
