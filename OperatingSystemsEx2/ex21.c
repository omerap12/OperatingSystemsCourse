#include <stdio.h>
#include <sys/fcntl.h>
#include <errno.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
/**
 * check if the text identical , if so return 1 else 0
 * * @param fileOneText
 * @param fileTwoText
 * @return 0/1
 */
int check_if_equal(char* fileOneText, char* fileTwoText){
    if (strcmp(fileOneText,fileTwoText) == 0){
        return 1;
    }
    return 0;
}

/**
 * check if files are look like , return 0 if so
 * @param fileOneText
 * @param fileTwoText
 * @return 1/0
 */
int check_if_look_like(char* fileOneText, char* fileTwoText){
    char* ptr_one = fileOneText;
    char* ptr_two= fileTwoText;
    while (ptr_one[0] != '\0' || ptr_two[0] != '\0'){
        if (ptr_one[0] == '\n' || ptr_one[0] == ' '){
            ptr_one++;
            continue;
        }
        if (ptr_two[0] == '\n' || ptr_two[0] == ' '){
            ptr_two++;
            continue;
        }
        if (toupper(ptr_one[0]) != toupper(ptr_two[0])){
            return 0;
        }
        ptr_one++;
        ptr_two++;
    }
    return 1;
}


int main(int argc, char* argv[]){
    char* fileOnePath = argv[1];
    char* fileTwoPath = argv[2];

    //get the text from the files, put every text in the pointers;
    char* fileOneText = (char*)malloc(sizeof(char));
    char* fileTwoText = (char*)malloc(sizeof(char));

    
    int index = 0;
    int size = 1;
    int fdin;
    char ch;
    fdin = open(fileOnePath,O_RDONLY);
    while (read(fdin,&ch,1)>0){
        fileOneText[index] = ch;
        fileOneText = (char*) realloc(fileOneText,(size+1)* sizeof(char));
        index++;
        size++;
    }
    close(fdin);

    int index2 = 0;
    int size2 = 1;
    int fdin2;
    char ch2;
    fdin2 = open(fileTwoPath,O_RDONLY);
    while (read(fdin2,&ch2,1)>0){
        fileTwoText[index2] = ch2;
        fileTwoText = (char*) realloc(fileTwoText,(size2+1)* sizeof(char));
        index2++;
        size2++;
    }
    close(fdin2);

    if (check_if_equal(fileOneText,fileTwoText) == 1){
        free(fileOneText);
        free(fileTwoText);
        return 1;
    }
    if (check_if_look_like(fileOneText,fileTwoText) == 1){
        free(fileOneText);
        free(fileTwoText);
        return 3;
    }
    free(fileOneText);
    free(fileTwoText);
    return 2;
}
