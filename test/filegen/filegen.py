#!/usr/bin/env python

if __name__ == "__main__":
    f = open('access.log', 'w');
    for i in range(2048):
        s = "element " + str(i) + " on file\n"
        f.write(s)
    f.close()
