#-*- coding utf-8 -*-
def Fibonacci_Yield_tool(n):
    a, b = 0, 1
    while a + b <= n:
        yield b
        a, b = b, a + b
    
 
def Fibonacci_Yield(n):
    # return [f for i, f in enumerate(Fibonacci_Yield_tool(n))]
    return list(Fibonacci_Yield_tool(n))

x = eval(input("please input max number:"))

print(Fibonacci_Yield(x))