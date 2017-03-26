#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
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
            
            printf("You resumed\n");
            //resume();

        }
        if(strcmp(input,p) == 0){

            printf("You paused\n");
            //pause();

        }
        if(strstr(input, "show ") != NULL){
    
            printf("You showed\n");

        }


        else{
            break;
        }

    }
    printf("You have exited the program\n");
}

//void resume();
//void pause();

*/

void resume(){
    printf("you resumed!!\n");
}

void pause(){
    printf("you paused!!\n");
}

void show(){
    int x;
   printf("Input what integer you want to be shown: ");
   scanf("%d", &x);
   
}

void inputCmd(){

    char e[30] = "exit";
    char r[30] = "resume";
    char p[30] = "pause";
    char s[30] = "show";
    char input[30];


    while(1){

        printf("Enter command: ");
        scanf("%s", input);

        if(strcmp(input,r) == 0){

            resume();
            inputCmd();
        
        }
        else if(strcmp(input,p) == 0){
        
            pause();
            inputCmd();
            
        }
        else if(strcmp(input,s) == 0){

            show();
            inputCmd();

        }
        else if(strcmp(input,e) == 0){
            
            break;

        }
        else{

            printf("Invalid input!\n");
            inputCmd();

        }

    }
    printf("You exited the program\n");
   
}


int main(){

    inputCmd();

}