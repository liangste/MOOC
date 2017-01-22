#!/usr/bin/python2.7

import random

if __name__ == "__main__":
    with open('phiX174.a', 'r') as content_file:
        content = content_file.read()

        for i in range(0, 1618):
            s = random.randrange(0, len(content) - 100)
            print content[s:s+100]
