/*
Description: Project 1 commands file. This file contains all the
funciton code for the commands for the single Pseudo-Shell: a
single-threaded UNIX system command-line interface

Author: JD Paul

Date: 4/3/2021
*/
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

void listDir() 
{
    DIR *dr = opendir(".");
    struct dirent *de;
    size_t siz = 256;
    // TODO
    char *name = calloc(siz, sizeof(char));
    if (dr == NULL) {
        char *message = "could not open directory\n";
        write(1, message, strlen(message));
        return;
    }

    while ((de = readdir(dr)) != NULL) {
        strcat(name, de->d_name);
        strcat(name, " ");
    }
    strcat(name, "\n");
    write(1, name, strlen(name));
    free(name);
    closedir(dr);
} // ls

void showCurrentDir() {
    char *buf = (char *) malloc(sizeof(char) * 1024);
    getcwd(buf, 1024);
    write(1, buf, strlen(buf));
    write(1, "\n", strlen("\n"));
    free(buf);
} // pwd

void makeDir(char *dirName){
    int doesMkdir = mkdir(dirName, 0777);
    if (doesMkdir) {
        char* message = "failed to make directory\n";
        write(1, message, strlen(message));
        return;
    }
} /*for the mkdir command*/

void changeDir(char *dirName){
    int worked = chdir(dirName);
    if (worked != 0) {
        char* message = "The directory path does not exist\n";
        write(1, message, strlen(message));
        return;
    }
} /*for the cd command*/

void copyFile(char *sourcePath, char *destinationPath) {
    
    int fd;
    if ((fd = open(sourcePath, O_RDONLY)) < 0) {
        char* message = "path to source file does not exist\n";
        write(1, message, strlen(message));
        return;
    }

    // case if the destination is a path without the file at the end
    //int worked;
    //int fd2;
    int newFile;
    if ((newFile = open(destinationPath, O_RDWR | O_CREAT, 0666)) < 0) {
        char buf[1500] = " ";
        char* loc = strrchr(sourcePath, '/'); // locate last occurance of '/'
        if (loc == NULL){
            strcat(buf, destinationPath);
            strcat(buf,"/");
            strcat(buf, sourcePath);
        } // if '/' does not exist than we can assume the source is a file, not a path
        else {
            //memmove(buf, buf + 1, strlen(buf)); // get rid of the / at the start of the buf
            strcat(buf, destinationPath);
            strcat(buf, loc);
        } // assume the source is a path, not a file
        newFile = open(buf, O_CREAT);
        if (newFile < 0) {
            char* message = "could not create the file at the given path\n";
            write(1, message, strlen(message));
            return;
        }
    }
    
    // now write the contents of the sourcePath file to the newFile
    char buf[BUFSIZ] = "\0";
    ssize_t amount = 0;
    while ((amount = read(fd, buf, BUFSIZ)) != 0) {
        // TODO
        write(newFile, buf, amount);
    }
    
    close(fd);
    close(newFile);
} /*for the cp command*/

void moveFile(char *sourcePath, char *destinationPath){
    int fd;
    if ((fd = open(sourcePath, O_RDONLY)) < 0) {
        char* message = "path to source file does not exist\n";
        write(1, message, strlen(message));
        return;
    }

    if (fd >= 0) {
        close(fd);
    }

    // case if the destination is a path without the file at the end
    int worked;
    int fd2;
    if ((fd2 = open(destinationPath, O_WRONLY | O_CREAT)) < 0) {
        char buf[1500] = " ";
        char* loc = strrchr(sourcePath, '/'); // locate last occurance of '/'
        if (loc == NULL){
            strcat(buf, destinationPath);
            strcat(buf,"/");
            strcat(buf, sourcePath);
        } // if '/' does not exist than we can assume the source is a file, not a path
        else {
            strcat(buf, destinationPath);
            strcat(buf, loc);
        } // assume the source is a path, not a file
        worked = rename(sourcePath, buf);
    } else {
        worked = rename(sourcePath, destinationPath);
        close(fd2);
    }

    if (worked < 0) {
        char* message = "could not move the specified file path\n";
        write(1, message, strlen(message));
        return;
    }
} /*Use strcat. for the mv command*/

void deleteFile(char *filename) {
    int doesRemove = unlink(filename);
    if (doesRemove < 0) {
        char* message = "The file to remove does not exist in this directory\n";
        write(1, message, strlen(message));
        return;
    }
} /*for the rm command*/

void displayFile(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        char* message = "The file trying to be opened by cat is empty or does not exist\n";
        write(1, message, strlen(message));
        return;
    }
    // TODO
    char* buf = (char *) calloc(BUFSIZ, sizeof(char));
    while (read(fd, buf, BUFSIZ)) {
        write(1, buf, strlen(buf));
    }
    close(fd);
    free(buf);
} /*for the cat command*/
