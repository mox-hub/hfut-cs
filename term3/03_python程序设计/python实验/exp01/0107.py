def check(str1, str2):
    length1 = len(str1)
    length = min(length1, len(str2))

    k = max(range(0, length+1), key=lambda i: i if str1[length1-i:] == str2[:i]else False)
    return(k,str1+str2[k:])

print("请分别输入两个字符串：")
str1 = str(input())
str2 = str(input())

print("两个字符串分别为：")
print(str1, str2)

print("最大子字符串长度和连接后的字符串分别为：")
print(check(str1, str2))