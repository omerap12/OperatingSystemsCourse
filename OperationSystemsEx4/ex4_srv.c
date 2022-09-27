//Omer Aplatony 209493667
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

void finish_server(int signo);
void signal_first_client(int signo){
	signal(SIGCHLD,SIG_IGN);
    /**
     * creating fork to handle file
     */
    pid_t pid;
    int stat;
    //child process
    if ((pid = fork() == 0)){
        //index of the line
        int index = 0;
        //reading line by line
        char line[255] = {0};
        //getting the math problem

        //pid number
        char pid[255] = {0};

        //math
        char a[255] = {0};
        char b[255] = {0};
        char c[255] = {0};
        FILE *fptr;
        char file_name[255] = "to_serv";
        fptr = fopen(file_name,"r");
        if (!fptr){
            perror("File error");
            return;
        }

        int first_number;
        int second_number;
        int operation;
        int client_pid;

         //TODO: implement !file->error
         while (fgets(line, sizeof(line),fptr)){
             if (index == 0)
                 strcpy(pid,line);
             else if (index == 1)
                 strcpy(a,line);
             else if (index == 2)
                 strcpy(b,line);
             else
                 strcpy(c,line);
             index++;
         }
        fclose(fptr);

         /**
          * Getting the numbers to atoi
          */
         first_number = atoi(a);
         second_number = atoi(c);
         operation = atoi(b);
         client_pid = atoi(pid);


        /**
         * Deleting the to_serv file
         */
        remove("to_serv");

        /**
         * If dividing be zero
         */
        if (operation == 4 && second_number == 0){
            char* text = "CANNOT_DIVIDE_BY_ZERO";
            FILE *file;
            char output_file_name[255] = "to_client_";
            char p[20];
            sprintf(p,"%d",client_pid);
            strcat(output_file_name,p);
            file = fopen(output_file_name,"w");
            fprintf(file,"%s",text);
            fclose(file);
            kill(client_pid,SIGTERM);
        }
        else{
	    signal(SIGCHLD,SIG_IGN);
            int output;
            /**
             * calculate the output
             */
            switch (operation) {
                case 1:
                    output = first_number + second_number;
                    break;
                case 2:
                    output = first_number - second_number;
                    break;
                case 3:
                    output = first_number * second_number;
                    break;
                case 4:
                    output = first_number / second_number;
                    break;
            }

            /**
             * writing to file
             */
            FILE *file;
            char output_file_name[255] = "to_client_";
            char p[20];
            sprintf(p,"%d",client_pid);
            strcat(output_file_name,p);
            file = fopen(output_file_name,"w");
            fprintf(file,"%d",output);
            fclose(file);
            kill(client_pid,SIGTERM);
	    exit(1);
        }
    }
}
void alarm_handler(int signo){
    printf("The server was closed because no server request was received for the last 60 seconds\n");
    while (wait(NULL) != -1);
    exit(1);
}

void finish_server(int signo){
    while (wait(NULL) != -1);
    exit(0);
}

int main() {
    remove("to_srv");
    signal(SIGTERM,signal_first_client); //sigkill start the signal
    signal(SIGINT,finish_server);
    signal(SIGALRM,alarm_handler);
    signal(SIGCHLD,SIG_IGN);
    while (1){
	alarm(60);
	signal(SIGCHLD,SIG_IGN);
        pause();
	alarm(0);
    }
}
