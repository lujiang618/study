#include <stdio.h>
float fact(int n){
    int i;
    float k=1.0;
    for(i=1;i<=n;i++)
        k=k*i;
    return k;
}

int main(){
    int n = 3;
    int k = fact(n);
    printf("计算结果：%d\n",k);
}
