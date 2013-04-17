#!/usr/bin/python
#
#Author: Harsh Patel
#E-Mail: harsh@nyu.edu
#Problem : DaVyncy challenge
#
#
#
#

import sys,os



def search(s1,s2,mx):
    t=""
    overlap=0
    #Underlap
    if s1.find(s2) > 0:
        t = s1
        overlap = len(s2)
        return t,overlap
        #Strip off one character at a time from s2(end) and match
    for k in range(1,len(s2)):
        if s1.endswith(s2[:-k]):
            overlap = len(s2[:-k])
        if overlap > mx:
            mx = overlap
            t = s1 + s2[len(s2)-k:]
    return (t,mx)

def compute(l):

    while len(l) > 1:
        t=""
        mx = 0
        ind = -1

        # Maximal overlap matching
        for j in range(1,len(l)):

            tu1 = search(l[0],l[j],mx)
            t1 = tu1[0]
            mx1 = tu1[1]
            tu2 = search(l[j],l[0],mx)
            t2 = tu2[0]
            mx2 = tu2[1]

            if mx1 > mx and mx1 > 0:
                t = t1
                mx = mx1
                ind = j
            if mx2 > 0 and mx2 > mx:
                t = t2
                mx = mx2
                ind = j
        if mx > 0 and ind != -1:
            l[0] = t
            l.pop(ind)
    print l[0]


if __name__ == "__main__":

    test_cases = open(sys.argv[1], 'r')
    for test in test_cases:
        #print test
        test = test.rstrip('\n')
        l = test.split(';')

        compute(l)
    test_cases.close()
