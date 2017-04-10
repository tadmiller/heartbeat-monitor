#include <stdio.h>

int main()
{
    int num = 23;
    char snum[6];

    snum[0] = '2';
    snum[1] = '2';
    snum[2] = '2';
    snum[3] = '2';
    snum[4] = '2';

    sprintf(snum, "%.3d,\n" , num);
    printf("\n\nnum is:%s", snum);

    snum[4] = '\0';

    //sprintf(snum, "%d" , num);
    printf("\n\nnum is:%s", snum);

    return 0;
}