#-*- coding utf-8 -*-

text = input("please input string:")

text = text.lower().split()

dic = {}

for word in text:
    if word not in dic:
        dic[word] = 1
    else:
        dic[word] += 1 

result = {}

for i in sorted (dic) : 
    print (i, " : ", dic[i], end ="\n") 