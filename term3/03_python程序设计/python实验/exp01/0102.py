#-*- coding utf-8 -*-
maxNumber = int(input("请输入一个大于2的自然数"))
number = list(range(2, maxNumber))
# print(number)

m = int(maxNumber ** 0.5)
for index, value in enumerate(number):

    if value > m:
        break
    # 对该位置之后的元素进行过滤，每次除以value，若余数为0，则淘汰它
    number[index + 1:] = filter(lambda x: x % value != 0, number[index + 1:])
    
print(number)