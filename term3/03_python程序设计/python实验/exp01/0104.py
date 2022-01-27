#-*- coding utf-8 -*-

str = input("please input string:")

if str == str[::-1]:#判断是否相等
    print('{} 是回文数'.format(str))#format方法输出
else:
    print('{} 不是回文数'.format(str))#format方法输出
