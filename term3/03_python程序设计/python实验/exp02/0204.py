#-*- coding utf-8 -*-
import random

lists = random.sample(range(0,20),10)
print(lists)
# 源字符串
new_list1 = lists
print("原字符串：", new_list1)
# 逆序
new_list2 = lists[::-1]
print("逆字符串：", new_list2)
# 偶数位置
new_list3 = lists[::2]
print("偶数位置字符串：", new_list3)