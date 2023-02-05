#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"


// TODO: function definitions here for ls2
void mode1(char argument[], int indentCounter){
    
    //the directory stream
    DIR *folder;

    //pointer to directory stream
    struct dirent *entry;
    //use lstat with struct stat
    //int lstat(const char *path, struct stat *buf)
	    //- 1st param = path that gives information about its ID
	    //- 2nd param = buff, gives information about address to the stat structure... holds also info about a particular file
    //struct of type stat, stores info about files like their size
    struct stat statStruct;
    
    //stores the amount to indent by
    char newIndent[100];

    //open the dir
    folder = opendir(argument);

    //check to make sure user can open dir
    if(folder == NULL){
        printf("Error opening folder.");
    }

    //read the next directory in the stream, until there are no dir's left in the stream
    while((entry = readdir(folder)) != NULL){
            //navigate into the directory
            chdir(argument);

            //use lstat to get info about file and put it into stat struct
            lstat(entry->d_name, &statStruct);
            
            //is lstat unsuccessful, print error
            if(lstat(entry->d_name, &statStruct) == -1){
                
                printf("Error, %s\n", entry->d_name);
            }

            //if lstat was successful, then check if file is dir
            if(S_ISDIR(statStruct.st_mode)){
                //skip anything that's ".." or "."
                if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0){
                    continue;
                }

                //print the dir name and indent by the necessary amount
                if(indentCounter == 0){
                    printf("%s/ (directory)\n", entry->d_name);
                }
                else{ //calculate the indent amount
                    //clear the indent tracker
                    newIndent[0] = '\0';
                    //extend the indent counter to the appropriate length of spaces
                    for(int i = 0; i < indentCounter; i++){
                        strcat(newIndent, INDENT);
                    }
                    //print dir name
                    printf("%s%s/ (directory)\n", newIndent, entry->d_name);
                }
                
                //recurse on subdirs, increment indent counter
                mode1(entry->d_name, indentCounter + 1);
                
            }
            //check if it is a regular file
            else if(S_ISREG(statStruct.st_mode)){
                //size of file
                int size = statStruct.st_size;
                
                //print the file name and indent by the necessary amount
                if(indentCounter == 0){
                    printf("%s (%d bytes)\n", entry->d_name, size);
                }
                else{ //calculate the indent amount
                    //clear indent tracker
                    newIndent[0] = '\0';
                    //extend the indent counter to the appropriate length of spaces
                    for(int i = 0; i < indentCounter; i++){
                        strcat(newIndent, INDENT);
                    }
                    //print file name
                    printf("%s%s (%d bytes)\n", newIndent, entry->d_name, size);
                }
            }
    }

    //go back up to parent dir once you've reached the end of the contents in a dir
    chdir("..");

    //close the dir
    closedir(folder);
}


int mode2(char argument2[], char fileName[], stack_t *stack, int indentCounter2){
    //the directory stream
    DIR *folder;

    //pointer to directory stream
    struct dirent *entry;

    //struct of type stat, stores info about files like their size
    struct stat statStruct;

    //open the dir
    folder = opendir(argument2);
    
    //keeps track of if file we are looking for is found
    int meow = FALSE;

    //holds the amount to indent by
    char newIndent[100];

    //check to make sure user can open dir
    if(folder == NULL){
        printf("Error opening folder.");
    }

    //read the next directory in the stream, until there are no dir's left in the stream
    while((entry = readdir(folder)) != NULL){
            //navigate into the directory
            chdir(argument2);

            //use lstat to get info about file and put it into stat struct
            lstat(entry->d_name, &statStruct);
            
            //is lstat unsuccessful, print error
            if(lstat(entry->d_name, &statStruct) == -1){
                
                printf("Lstat error, %s\n", entry->d_name);
            }

            //if lstat was successful, then check if file is dir
            if(S_ISDIR(statStruct.st_mode)){
                //skip anything that's ".." or "."
                if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0){
                    continue;
                }
                
                //recurse on subdirs
                int meow2 = mode2(entry->d_name, fileName, stack, indentCounter2 + 1);

                //if the file was found, store it on the stack
                if(meow2 == TRUE){
                    meow = TRUE;
                    //calculate the indent
                    if(indentCounter2 > 0){
                        newIndent[0] = '\0';
                        //extend the indent counter to the appropriate length of spaces
                        for(int i = 0; i < indentCounter2; i++){
                            strcat(newIndent, INDENT);
                        }
                    }

                    //allocate space on the heap
                    char* file = (char*) malloc(strlen(entry->d_name) + strlen("/ (directory)") + strlen(newIndent) + 1);
                    
                    //clear 'file' -- fill with 0
                    memset(file, 0, (strlen(entry->d_name) + strlen("/ (directory)") + strlen(newIndent) + 1));
                    //create the string to push onto the stack
                    strcat(file, newIndent);
                    strcat(file, entry->d_name);
                    strcat(file, "/ (directory)");
                    
                    //push the dir onto the stack
                    push(stack, file);
                }
                
            }
            //check if it is a regular file
            if(S_ISREG(statStruct.st_mode)){
                //if the file we are at is what we are looking for, push to stack
                if(strcmp(entry->d_name, fileName) == 0){
                    //calculate indent 
                    if(indentCounter2 > 0){
                        newIndent[0] = '\0';
                        
                        //extend the indent counter to the appropriate length of spaces
                        for(int i = 0; i < indentCounter2; i++){
                            strcat(newIndent, INDENT);
                        }
                    }

                    //stores string size
                    char stringSize[100];

                    //convert int size in bytes to chars
                    sprintf(stringSize, "%ld", statStruct.st_size);
                    
                    //allocate space on heap
                    char* file = (char*) malloc(strlen(newIndent) + strlen(entry->d_name) + strlen(" (") + strlen(stringSize) + strlen(" bytes)") + 1);
                    
                    //clear 'file' -- fill with 0
                    memset(file, 0, strlen(newIndent) + strlen(entry->d_name) + strlen(" (") + strlen(stringSize) + strlen(" bytes)") + 1);
                    
                    //create string to push onto stack
                    strcat(file, newIndent);
                    strcat(file, entry->d_name);
                    strcat(file, " (");
                    strcat(file, stringSize); //convert size to chars
                    strcat(file, " bytes)");
                    
                    //push file onto stack
                    push(stack, file);
                    
                    //set meow to true, because the file was found
                    meow = TRUE;
                    
                }
            }
    }

    //go back up to parent dir once you've reached the end of the contents in a dir
    chdir("..");

    //close the dir
    closedir(folder);

    //return 0 or 1 depending on if the file was found
    return meow;
}

