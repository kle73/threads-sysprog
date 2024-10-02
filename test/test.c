#include <stdio.h>
#include <setjmp.h>

// Declare the external assembly function
//extern int add(int x, int y);
jmp_buf buf1;
jmp_buf buf2;

void test()
{
    int a = 1;
    if (setjmp(buf1)) return;
    else
    {
        
    }
}

int main() {
    //int x = 5;
    //int y = 3;
    
    //int result = add(x, y);
    
    // Print the result
    //printf("The result of adding %d and %d is: %d\n", x, y, result);
    /* 
    if (setjmp(buf2)) return 0;
    else
    {
        printf("rsp: %p\n", *((long*)buf2+1));
    } */

    test();
    return 0;
}