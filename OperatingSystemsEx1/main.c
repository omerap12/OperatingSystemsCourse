#include <stdio.h>
#include <string.h>
#include<unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void print_history(char history[100][101] , int count)
{
    int i;
    for(i=0;i<=count;i++)
    {
        printf("%s\n",history[i]);
    }

}


int main(int argc, char** argv) {
    if (argc > 1){
        char* current_path = getenv("PATH");
        char* dot = ":";
        strcat(current_path,dot);
        int i;
        for (i = 1; i<argc-1; i++){
            strcat(current_path,argv[i]);
            strcat(current_path,dot);
        }
        strcat(current_path,argv[argc-1]);
        setenv("PATH",current_path,1);
    }
    char prompt = '$';
    char input[101];
    char *token;
    char history[100][101];
    int current_command = 0;
    char back_up_command[101];
    char core_command[101];
    char rest_command[101];
    int first_token;
    char** arguments;
    int arguments_length = 0;
    int command;
    for (command = 0; command<100; command++)
    {
        printf( "%c ",prompt);
        fflush(stdout);
        first_token = 0;
        //get the command
        scanf(" %100[^\n]s",input);
        //copy the command, strtok ruines it.
        strcpy(back_up_command,input);

        //get the command type
        token = strtok(input," ");
        //get the first command - ls,pwd and etc..
        strcpy(core_command,token);
        arguments = (char**)malloc(sizeof(char *));
        arguments[0] = core_command;
        arguments_length++;


        //seperate first command and rest of command
        while (token!=NULL){
            if (first_token)
                strcat(rest_command,token);
            first_token = 1;
            token = strtok(NULL," ");
            if (token == NULL){
                continue;
            }
            //put arguments in
            arguments = (char**) realloc(arguments,sizeof (char*)*(arguments_length+1));
            arguments[arguments_length] = (char*) malloc((sizeof(token)+1)* sizeof(char*));
            strcpy(arguments[arguments_length],token);
            arguments_length++;
        }
        //add to arguments NULL
        arguments = (char**) realloc(arguments,sizeof (char*)*(arguments_length+1));
        arguments[arguments_length] = (char*)0;


        if (strcmp(core_command,"exit") == 0){
            int pid = getpid();
            char commandToAdd[101];
            sprintf(commandToAdd,"%d ",pid);
            strcpy(history[current_command],strcat(commandToAdd, back_up_command));
            int i;
            for (i = 1; i<arguments_length; i++){
                free(arguments[i]);
            }
            exit(0);
        }
        else if (strcmp(core_command,"cd") == 0){
            int pid = getpid();
            char commandToAdd[100];
            sprintf(commandToAdd,"%d ",pid);
            strcpy(history[current_command],strcat(commandToAdd, back_up_command));

            if (chdir(rest_command))
                printf("Error\n");
            memset(rest_command, 0, 100);
            current_command += 1;
        }
        else if (strcmp(core_command,"history") == 0){
            int pid = getpid();
            char commandToAdd[100];
            sprintf(commandToAdd,"%d ",pid);
            strcpy(history[current_command],strcat(commandToAdd, back_up_command));
            print_history(history,current_command);
            current_command += 1;
        }
        else {
            pid_t pid;
            int stat;
            //if I'm the child
            if ((pid = fork()) == 0){
                //start the process
                if(execvp(core_command,arguments) == -1){
                    perror("fork failed");
                    current_command += 1;
                    return 0;
                }
            }else{
                //I'm the parent
                wait(&stat);
                //add the command to the history
                char commandToAdd[100];
                //convert the pid to string
                sprintf(commandToAdd, "%d ", pid);
                strcpy(history[current_command],strcat(commandToAdd, back_up_command));
                current_command += 1;
            }
        }
        //clean memory for next iteration
        int i;
        for (i = 1; i<arguments_length; i++){
            free(arguments[i]);
        }
        free(arguments);
        arguments_length = 0;
    }

}
