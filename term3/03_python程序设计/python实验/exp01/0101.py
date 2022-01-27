import random

#-*- coding utf-8 -*-

number = random.randint(0,100)
FLAG = 0
i = 0
x = eval(input("please input number your guess:"))

while i < 5:
    if x > number:
        print("too large")
    elif x < number:
        print("too small")
    else:
        print("right, you win!!")
        FLAG = 1
        break
    i = i + 1
    x = eval(input("please input number your guess:"))


if FLAG == 0:
    print("game over, the right number is " ,number)