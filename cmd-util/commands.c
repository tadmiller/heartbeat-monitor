#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inputCmd(){

    char e[30] = "exit";
    char input[30];

    while(1){

        if(strcmp(input,e) != 0){

        //char input[30];
        printf("Enter command: ");
        scanf("%s", input);
        //printf("%s", str);

        }

        else{
            break;
        }

    }
    printf("You have exited the program\n");
}

int main(){

    inputCmd();

}