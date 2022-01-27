#-*- coding utf-8 -*-
import numpy as np

i = 0
n = eval(input("please input n:"))
number = np.random.randint(1, 10*n, n)
print(number)

average = sum(number) / len(number)
# print(average)

result = []
for num in number:
    if num > average:
        result.append(num)

# print(result)

print((average,result))