#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>

int check_if_c(char* file_name){
    int length = strlen(file_name);
    if (length<3)
        return 0;
    if (file_name[length-1] == 'c' && file_name[length-2] == '.')
        return 1;
    return 0;
}
int check_if_there_is_c_file(char* path) {
    DIR *inner = opendir(path);
    struct dirent *dirInner;
    if (inner) {
        while ((dirInner = readdir(inner)) != NULL) {
            if (strcmp(dirInner->d_name, ".") == 0 || strcmp(dirInner->d_name, "..") == 0 || strcmp(dirInner->d_name,".DS_Store")==0) {
                continue;
            }
            if (check_if_c(dirInner->d_name))
                return 1;
        }
    }
    return 0;
}

int check_if_out(char* file_name){
    if (strcmp(file_name,"a.out")==0)
        return 1;
    return 0;
}
int check_if_output(char* file_name){
    if (strcmp(file_name,"results.txt")==0)
        return 1;
    return 0;
}

int check_if_file_is_not_empty(char* file_path){
    int index = 0;
    int fdin;
    char ch;
    fdin = open(file_path,O_RDONLY);
    while (read(fdin,&ch,1)>0){
        index++;
    }
    close(fdin);
    if (index > 0){
        return 1;
    }
    return 0;
}

int get_score(char* path){
    char file_path[150];
    strcpy(file_path,path);
    strcat(file_path,"/");
    strcat(file_path,"grade.txt");
    char ch;
    int charsr;
    int fdin;
    char buf[10];
    fdin = open(file_path, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
    if (fdin < 0){
        perror("Error ");   /* text explaining why */
        exit(-1);
    }
    read(fdin,buf,10);

    close(fdin);
    return buf[0]-'0';
}
int check_if_compiled(char* path){
    char file_path[150];
    strcpy(file_path,path);
    strcat(file_path,"/");
    strcat(file_path,"compile_error.txt");

    char ch;
    int charsr;
    int fdin;
    char buf[10];
    fdin = open(file_path, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
    if (fdin < 0){
        perror("Error ");   /* text explaining why */
        exit(-1);
    }
    read(fdin,buf,10);

    close(fdin);
    if ((buf[0]-'0')>0){
        return 0;
}
    return 1;
}


int main(int argc, char*argv[]) {
    char *confFilePath = argv[1];
    char lines[3][150];

    //reading lines
    int flag = 0;
    int fdin;
    char ch;
    int index = 0;
    fdin = open(confFilePath, O_RDONLY);
    while (read(fdin, &ch, 1) > 0) {
        if (ch == '\n') {
            lines[flag][index] = '\0';
            flag += 1;
            index = 0;
            continue;
        }
        lines[flag][index] = ch;
        index++;
    }
    close(fdin);
    char* firstLine = lines[0];
    char* secondLine = lines[1];
    char* thirdLine = lines[2];


    //iterate over folders and compile c files
    DIR *outer;
    struct dirent *dirMid;
    outer = opendir(firstLine);

    if (outer) {
        while ((dirMid = readdir(outer)) != NULL) {
            char fileCompilePath[150];
            DIR *inner;
            struct dirent *dirInner;
            char path[150];
            strcpy(path, firstLine);
            strcat(path, "/");
            strcat(path, dirMid->d_name);
            inner = opendir(path);
            pid_t pid;
            int stat;
            int filedesc;

            if (inner) {
                while ((dirInner = readdir(inner)) != NULL) {
		if (strcmp(dirInner->d_name,".DS_Store")==0)
			continue;
                    //check if c file
                    if (check_if_c(dirInner->d_name) == 1 && dirInner->d_type == DT_REG) {
                        //compile file using fork
                        char argument_input_path[150];
                        char argument_output_path[150];
                        strcpy(argument_input_path, path);
                        strcat(argument_input_path, "/");
                        strcat(argument_input_path, dirInner->d_name);
			

                        strcpy(argument_output_path,path);
                        strcat(argument_output_path,"/a.out");


                        char *arguments[5];
                        char *command = "gcc";
                        char* mode = "-o";
                        arguments[0] = command; //gcc
                        arguments[1] = argument_input_path; // file path

                        arguments[2] = mode; // -o
                        arguments[3] = argument_output_path;
                        arguments[4] = NULL; //null dell


                        strcpy(fileCompilePath, path);
                        strcat(fileCompilePath, "/");
                        strcat(fileCompilePath, "compile_error.txt");

                        //child compiling files
                        if ((pid = fork()) == 0) {
                            // try to compile and write scores
                            int err = open("errors.txt", O_WRONLY | O_CREAT| O_APPEND ,S_IRUSR | S_IWUSR);
                            if (-1 == err) {
                                perror("Error ");   /* text explaining why */
                                exit(-1);
                            }

                            if (-1 == dup2(err, fileno(stderr))) {
                                perror("Error ");   /* text explaining why */
                                exit(-1);
                            }

                            int save_err = dup(fileno(stderr));
                            if (execvp(command, arguments) == -1) {
                                perror("fork failed\n");
                            }
                            fflush(stderr);
                            close(err);
                            dup2(save_err, fileno(stderr));
                            close(save_err);
                        }else {
                            waitpid(pid,&stat,0);
                            int child_status = WEXITSTATUS(stat);
                            char write_comp_output = child_status+'0';
                            filedesc = open(fileCompilePath, O_WRONLY | O_CREAT,S_IRUSR | O_TRUNC | S_IWGRP | S_IWUSR);
                            if (filedesc < 0) {
                                perror("Error ");   /* text explaining why */
                                exit(-1);
                            }
                            write(filedesc, &write_comp_output,1);
                            close(filedesc);

                        }
                    }
                }
            }
            close((long) inner);
        }
    }
    closedir(outer);


    //iterating over compiled files and run them
    outer = opendir(firstLine);
    if (outer) {
        while ((dirMid = readdir(outer)) != NULL) {
            if (strcmp(dirMid->d_name,".") == 0 || strcmp(dirMid->d_name,"..")==0 || strcmp(dirMid->d_name,"DS_Store")==0){
                continue;
            }
            DIR *inner;
            struct dirent *dirInner;
            char path[150];
            strcpy(path, firstLine);
            strcat(path, "/");
            strcat(path, dirMid->d_name);
            inner = opendir(path);
            char first_argument[150];
            char *arguments[2];
            char argument_input_path[150];
            char argument_output_path[150];

            if (inner) {
                while ((dirInner = readdir(inner)) != NULL) {
                    if (strcmp(dirInner->d_name,".") == 0 || strcmp(dirInner->d_name,"..")==0 || strcmp(dirInner->d_name,"DS_Store")==0){
                        continue;
                    }
                    //check if out file
                    if (check_if_out(dirInner->d_name)) {
                        strcpy(argument_input_path, path);
                        strcat(argument_input_path, "/");
                        strcat(argument_input_path, dirInner->d_name);
                        //execute the file using fork
                        pid_t pid;
                        int stat;
                        strcpy(argument_output_path, path);
                        strcat(argument_output_path, "/");
                        strcat(argument_output_path, "results.txt");

                        strcpy(first_argument,argument_input_path);

//                        child running files
                        if ((pid = fork()) == 0) {
                            arguments[0] = first_argument;
                            arguments[1] = NULL;

                            int in,out;
                            in = open(secondLine,O_RDONLY);
                            if (in < 0){
                                perror("Input file not exist\n");
                                exit(-1);
                            }
                            out = open(argument_output_path, O_WRONLY | O_CREAT,S_IRUSR | O_TRUNC | S_IWGRP | S_IWUSR);
                            if (out < 0){
                                perror("Output file not exist\n");
                                exit(-1);
                            }
                            dup2(in,0);
                            dup2(out,1);
                            close(in);
                            close(out);

                            execvp(first_argument, arguments);
                            memset(first_argument,0,150);
                        } else {
                            wait(&stat);
                            memset(first_argument,0,150);
                            memset(argument_input_path,0,150);
                            memset(argument_output_path,0,150);
                            memset(path,0,150);
                        }
                    }
                }
                closedir(inner);
            }
        }
        closedir(outer);
    }



    //going on each output file and compare it using comp.out
    outer = opendir(firstLine);
    if (outer) {
        while ((dirMid = readdir(outer)) != NULL) {
            if (strcmp(dirMid->d_name,".") == 0 || strcmp(dirMid->d_name,"..")==0 || strcmp(dirMid->d_name,"DS_Store")==0){
                continue;
            }
            DIR *inner;
            struct dirent *dirInner;
            char path[150];
            strcpy(path, firstLine);
            strcat(path, "/");
            strcat(path, dirMid->d_name);
            inner = opendir(path);
            char *first_argument = "./comp.out";
            char *arguments[3];
            char argument_input_path[150];
            arguments[0] = first_argument; //comp.out
            //arguments[1] = user output
            arguments[2] = thirdLine; //desire output.txt
            arguments[3] = NULL;
            char argument_output_path[150];
            char student_grade_path[150];
            int filedesc;


            if (inner) {
                while ((dirInner = readdir(inner)) != NULL) {
                    if (strcmp(dirInner->d_name,".") == 0 || strcmp(dirInner->d_name,"..")==0 || strcmp(dirInner->d_name,".DS_Store")==0){
                        continue;
                    }
                    //check if output.txt file
                    if (check_if_output(dirInner->d_name)) {
                        strcpy(argument_input_path, path);
                        strcat(argument_input_path, "/");
                        strcat(argument_input_path, dirInner->d_name);
                        arguments[1] = argument_input_path; //user output.txt


                        strcpy(argument_output_path, path);
                        strcat(argument_output_path, "/");
                        strcpy(student_grade_path, argument_output_path);
                        strcat(student_grade_path,"grade.txt");
                        strcat(argument_output_path, "results.txt");

                        //execute the file using fork
                        pid_t pid;
                        int stat;
//                        child running files
                        if ((pid = fork()) == 0) {
                            arguments[0] = first_argument;
                            execvp(first_argument, arguments);
                        } else {
                            waitpid(pid,&stat,0);
                            int child_status = WEXITSTATUS(stat);
                            char write_comp_output = child_status+'0';
                            filedesc = open(student_grade_path, O_WRONLY | O_CREAT,S_IRUSR | O_TRUNC | S_IWGRP | S_IWUSR);
                            if (filedesc < 0) {
                                perror("Error ");   /* text explaining why */
                                exit(-1);
                            }
                            write(filedesc, &write_comp_output,1);
                            close(filedesc);
                            memset(argument_input_path,0,150);
                            memset(argument_output_path,0,150);
                            memset(path,0,150);
                        }
                    }
                }
                closedir(inner);
            }
        }
        closedir(outer);
    }

///going on each output file and compare it using comp.out
    outer = opendir(firstLine);
    if (outer) {
        int first = 0;
        while ((dirMid = readdir(outer)) != NULL) {
            if (strcmp(dirMid->d_name,".") == 0 || strcmp(dirMid->d_name,"..")==0 || strcmp(dirMid->d_name,".DS_Store")==0){
                continue;
            }
            char* line_to_write = dirMid->d_name;
            DIR *inner;
            struct dirent *dirInner;
            char path[150];
            strcpy(path, firstLine);
            strcat(path, "/");
            strcat(path, dirMid->d_name);
            char copy_path[150];
            strcpy(copy_path,path);
            struct stat sb;
            if (stat(copy_path,&sb) == 0 && S_ISDIR(sb.st_mode))
            {
                char line_to_write[150];
                strcpy(line_to_write,dirMid->d_name);
                if (check_if_there_is_c_file(copy_path) == 0){
                strcat(line_to_write,",");
                strcat(line_to_write,"0,");
                strcat(line_to_write,"NO_C_FILE\n");
                }else if (check_if_compiled(copy_path) == 0){
                strcat(line_to_write,",");
                strcat(line_to_write,"10,");
                strcat(line_to_write,"COMPILATION_ERROR\n");
            }else {
                //check if wrong/similar/excellent
                int score = get_score(copy_path);
                if (score == 1) {
                    strcat(line_to_write, ",");
                    strcat(line_to_write, "100,");
                    strcat(line_to_write, "EXCELLENT\n");
                } else if (score == 3) {
                    strcat(line_to_write, ",");
                    strcat(line_to_write, "75,");
                    strcat(line_to_write, "SIMILAR\n");
                } else if (score == 2) {
                    strcat(line_to_write, ",");
                    strcat(line_to_write, "50,");
                    strcat(line_to_write, "WRONG\n");
                }
            }
            int count = strlen(line_to_write);
            int out;
            if((out=open("results.csv",O_WRONLY | O_CREAT| O_APPEND ,S_IRUSR | S_IWUSR))==-1)
            {
                perror("Error ");   /* text explaining why */
                exit(-1);
            }
            if (write(out,line_to_write,count) == -1){
                perror("Error ");   /* text explaining why */
                exit(-1);
            }
            close(out);
            memset(line_to_write,0,150);
            first=1;
                
            }
        }
      }
        closedir(outer);



    //delete all temp files
    outer = opendir(firstLine);
    if (outer) {
        while ((dirMid = readdir(outer)) != NULL) {
            char fileCompilePath[150];
            DIR *inner;
            struct dirent *dirInner;
            char path[150];
            strcpy(path, firstLine);
            strcat(path, "/");
            strcat(path, dirMid->d_name);
            inner = opendir(path);
            char file_path[150];
            if (inner) {
                while ((dirInner = readdir(inner)) != NULL) {
                    if (strcmp(dirMid->d_name, ".") == 0 || strcmp(dirMid->d_name, "..") == 0) {
                        continue;
                    }
                    if (strcmp(dirInner->d_name,"results.txt")==0 || strcmp(dirInner->d_name,"a.out")==0 ||
                            strcmp(dirInner->d_name,"compile_error.txt")==0  || strcmp(dirInner->d_name,"grade.txt")==0)
                    {
                        strcpy(file_path,path);
                        strcat(file_path,"/");
                        strcat(file_path,dirInner->d_name);
                        remove(file_path);
                        memset(file_path,0,150);
                    }
                }
            }
            closedir(inner);

        }
    }
    closedir(outer);

    return 0;
}
