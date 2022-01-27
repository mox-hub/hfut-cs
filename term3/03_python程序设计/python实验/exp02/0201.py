#-*- coding utf-8 -*-
import random

def generate_random_str(randomlength = 16):
    random_str = ''
    base_str = 'ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghigklmnopqrstuvwxyz'
    length = len(base_str) - 1
    for i in range(randomlength):
        random_str += base_str[random.randint(0, length)]
    return random_str

str1 = generate_random_str(1000)
print("str:", str1)


result = {}
for i in str1:
    result[i] = str1.count(i)
for i in sorted (result) : 
    print (i, " : ", result[i], end ="\n") 

    
