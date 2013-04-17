import math
import sys
y = 0

def add(x):
    global y
    y += x
    return y

def sub(x):
    global y
    y -= x
    return y

def mult(x):
    global y
    y *= x
    return y

def div(x):
    global y
    y /= x
    return math.floor(y)


if __name__ == "__main__":


#    s = "7.add(4).sub(-9).div(5).mult(1).div(-9)"


    f = open(sys.argv[1],'r')
    s = f.read()
    s = s.split(".")

    y = int(s[0])

    for i in range(1,len(s)):
        eval(s[i])

    print y
