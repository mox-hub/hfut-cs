#-*- coding utf-8 -*-
import random 

n = eval(input("please input n:"))
m = eval(input("please input m:"))

tmp_list = []

for i in range(n):
    x = random.randint(1,m)
    tmp_list.append(x)

result = tuple(tmp_list)

print(result)

for i in tmp_list:
    if i % 2 == 0:
        tmp_list.remove(i)

result = tuple(tmp_list)
print(result)