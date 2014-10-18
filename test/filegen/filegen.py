#!/usr/bin/env python

if __name__ == "__main__":
    f = open('access.log', 'w');
    for i in range(2067):
        s = "element " + str(i) + " asadaddd\n"
        f.write(s)
    f.close()
