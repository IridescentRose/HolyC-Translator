#include <stdio.h>
#include <stdlib.h>

int main(){

    int totalTests = 0;
    int testsSuccess = 0;

    int i = 1;
    while(1){
        char buffer1[64];
        char buffer2[128];
        sprintf(buffer1, "tests/test%d.hc", i);
        sprintf(buffer2, "bin/hc2c %s out/result%d.c", buffer1, i);
        
        FILE* fp = fopen(buffer1, "r");

        if(!fp){
            break;
        }

        totalTests++;
        fclose(fp);

        int res = system(buffer2);

        res ? printf("Test %d failed!\n", i) : testsSuccess++;
        i++;
    }

    printf("[%d/%d] Tests Completed Successfully.\n", testsSuccess, totalTests);

    return 0;
}