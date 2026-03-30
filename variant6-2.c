#include <stdio.h>

int gcd (int a, int b){
    int r;
    while (b !=0){
        r= a % b;
        a = b;
        b = r;
    }
    return a;
}
int main(){
    int a , b;
    scanf("%d %d", &a, &b);
    int result = gcd(a,b);
    printf ("GCD = %d\n", result);

    if (result ==1 ){
        printf ("coprime\n");
    }
    return 0;
}