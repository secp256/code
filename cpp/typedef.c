#include <stdio.h>
#include <assert.h>

typedef int (*FP_CALC)(int, int);

int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

FP_CALC calc_func(char op)
{
    switch (op)
    {
    case '+':
        return add;
    case '-':
        return sub;
    default:
        return NULL;
    }
    return NULL;
}

int (*s_calc_func(char op)) (int, int)
{
    return calc_func(op);
}

int calc(int a, int b, char op)  
{  
    FP_CALC fp = calc_func(op);  
    int (*s_fp)(int,int) = s_calc_func(op);//用于测试  

    assert(fp == s_fp);// 可以断言这两个是相等的  

    if(fp)
        return fp(a,b);  
    else
        return -1;  
} 

char (*pFun)(int);
char glFun(int a) { return a; }

int main()
{
    // 1
    pFun = glFun;
    printf("%c\n", (*pFun)(98));

    // 2
    int a = 100, b = 20;  
    printf("calc(%d, %d, %c) = %d\n", a, b, '+', calc(a, b, '+'));  
    printf("calc(%d, %d, %c) = %d\n", a, b, '-', calc(a, b, '-'));     
    return 0;
}
