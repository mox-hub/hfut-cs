#-*- coding utf-8 -*-
import random 

def generate_random_list(randomlength):
    random_list = []
    base_str = 'ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghigklmnopqrstuvwxyz1234567890'
    length = len(base_str) - 1
    for i in range(randomlength):
        random_list.append( base_str[random.randint(0, length)] )
    return random_list

n = eval(input("please input n:"))
m = eval(input("please input m:"))

result = []
for i in range(n):
    lens = random.randint(1,m)
    tmp_list = generate_random_list(lens)
    print(tmp_list)
    result.append(tmp_list)

print(result)
