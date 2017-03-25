#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inputCmd(){

    char e[30] = "exit";
    char r[30] = "resume";
    char p[30] = "pause";
    char s[30] = "show";
    char *input[30];

    while(1){

        printf("Enter command: ");
        scanf("%s", input);

        if(strcmp(input,r) == 0){
        
            resume();

        }
        if(strcmp(input,p) == 0){

            pause();

        }
        if(strcmp(input,s) == 0){

            if (strstr(request, "show ") != NULL) {
    
            }

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