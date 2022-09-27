//Omer Aplatony 209493667
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void write_to_file(char*pid,char* a, char* b, char* c);

void signal_from_server(int signo){
    /**
 * reading the output
 */
    int my_pid = getpid();
    char file_name[235] = "to_client_";
    char pid[20];
    //converting to string
    sprintf(pid,"%d",my_pid);
    strcat(file_name,pid);
    char buffer[255];
    FILE  *file = fopen(file_name,"r");
    while (fgets(buffer, sizeof(buffer),file)){
        printf("%s\n",buffer);
    }
    if (file == NULL){
        perror("fopen error");
        return;
    }
    fclose(file);
    remove(file_name);
}
void alarm_handler(int signo){
        printf("Client closed because no response was received from the server for 30 seconds");
        char output_file_name[255] = "to_serv";
        remove(output_file_name);
        exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 5){
        printf("ERROR_FROM_EX4\n");
        exit(1);
    }
    signal(SIGTERM,signal_from_server); //sigkill start the signal
    signal(SIGALRM,alarm_handler);
    int server_id = atoi(argv[1]);
    write_to_file(argv[1],argv[2],argv[3],argv[4]);
    //notify server that the file was provided.
    kill(server_id,SIGTERM);
    alarm(30);
    pause();
    return 0;
}

void write_to_file(char* pid,char* a, char* b, char* c){
    FILE *fptr;
    char file_name[255] = "to_serv";
    int i = 0;
    while (i<10){
        if (access(file_name,F_OK) == 0){
            srand(time(NULL));
            int random_number;
            random_number = rand() % 5;
            random_number+=1;
            sleep(random_number);
            i+=1;
        }else{
            fptr = fopen(file_name,"w");
            fprintf(fptr,"%d\n%s\n%s\n%s",getpid(),a,b,c);
            fclose(fptr);
            return;
        }
    }
    printf("ERROR_FROM_EX4\n");
    exit(1);
}
