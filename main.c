#include <stdlib.h>
#include <unistd.h> // for close() sys call
#include <fcntl.h> // for some of the open close read write sts calls
#include <stdio.h>
#include <string.h> // for strcmp
#include "string_parser.h"
#include "command.h"

// commander takes the file descriptor as well as the line
int commander(command_line line);

int main(int argc, char** argv) {
    int error;
    if (argc == 1) {
        // interactive mode
        // set the size that it allowed for the buffer
        // devlear line_buffer
        size_t len = 128;
        // TODO
        char* line_buf = (char *) calloc(len, sizeof(char));
        //char* line_buf = malloc(len);
        // Check if line_buf does not allocate and error
        if (line_buf == NULL) {
            fprintf(stderr, "Impliment the cannot allocate the buffer error\n");
        }
        command_line large_token_buffer;
        command_line small_token_buffer;

        // int line_num = 0;

        // take and handle input
        printf(">>> ");
        // loop until the file is over
        while (getline(&line_buf, &len, stdin) != -1)
        {
            // check if the exit command is made
            if (strcmp(line_buf, "exit\n") == 0) {
                free(line_buf);
                return 0;
            }
            //tokenize line buffer
            // large token is serperated by ";"
            large_token_buffer = str_filler(line_buf, ";");
            // iterate through each large token
            for (int i = 0; large_token_buffer.command_list[i] != NULL; i++)
            {
                // tokenize large buffer: smaller token is seperated by " "
                small_token_buffer = str_filler(large_token_buffer.command_list[i], " ");
                // TODO
                if (small_token_buffer.num_token == 0) {
                    free_command_line(&small_token_buffer);
                    memset(&small_token_buffer, 0, 0);
                    break;
                }
                // Send the smaller token to call the right command: 2 for the file descriptor is standard error
                
                error = commander(small_token_buffer);
                if (error) {
                    free_command_line(&small_token_buffer);
                    memset(&small_token_buffer, 0, 0);
                    break;
                }
                // free smaller tokens and reset variable
                free_command_line(&small_token_buffer);
                memset(&small_token_buffer, 0, 0);
            }
        
            free_command_line(&large_token_buffer);
            memset(&large_token_buffer, 0, 0);
            printf(">>> ");
        }
        free(line_buf);

    } else if (argc == 3) {
        // file mode

        // Check for option -f as it is the only option allowed
        int compareForFile = strcmp(argv[1], "-f");
        if (compareForFile != 0) {
            fprintf(stderr, "-f is not specified for a file\n");
            return -1;
        }

        // Open and handle the file and return an error if the file does not exist
        FILE *fp = fopen(argv[2], "r");
        if (fp == NULL) {
            fprintf(stderr, "second argument must be a file\n");
            return -1;
        }
        
        /* TODO delete these lines
        // create the output.txt file
        //FILE *outputFp;
        //outputFp = fopen("output.txt", "w");
        //int fd = creat("output.txt", 0777);
        */

        // write all errors to output.txt
        //freopen("output.txt", "w", stderr);
        // write all std output to output.txt
        freopen("output.txt", "w", stdout);
        
        // devlear line_buffer
        size_t len = 128;
        // TODO
        char* line_buf = (char *) calloc(len, sizeof(char));
        //char* line_buf = malloc(len);

        command_line large_token_buffer;
        command_line small_token_buffer;

        // int line_num = 0;
        // loop until the file is over
        while (getline(&line_buf, &len, fp) != -1)
        {
            //tokenize line buffer
            // large token is serperated by ";"
            large_token_buffer = str_filler(line_buf, ";");
            // iterate through each large token
            for (int i = 0; large_token_buffer.command_list[i] != NULL; i++)
            {
                // tokenize large buffer
                // smaller token is seperated by " "
                small_token_buffer = str_filler(large_token_buffer.command_list[i], " ");
                // TODO
                if (small_token_buffer.num_token == 0) {
                    free_command_line(&small_token_buffer);
                    memset(&small_token_buffer, 0, 0);
                    break;
                }
                error = commander(small_token_buffer);
                if (error) {
                    free_command_line(&small_token_buffer);
                    memset(&small_token_buffer, 0, 0);
                    break;
                }
                // free smaller tokens and reset variable
                free_command_line(&small_token_buffer);
                memset(&small_token_buffer, 0, 0);
            }
        
            free_command_line(&large_token_buffer);
            memset(&large_token_buffer, 0, 0);
        }
        free(line_buf);
        fclose(fp);
        //TODO delete this line: close(fd);
    } else { 
        // Handle the last case which is an error
        fprintf(stderr, "there are more than 3 arguments or only 2\n\n");
    }
    return 0;
}

int commander(command_line line) {
    char *command = line.command_list[0];
    char *lastCommand = line.command_list[line.num_token - 1];
    int numTokens = line.num_token;
    int error = 0;
    //char buf[256];
    //int sizeBuf = sizeof(buf);
    char *message;

    if (command == NULL) {
        error = 1;
        return error;
        // The error that no argument was supplied
    }
    
    // for each case, check that it is used correctly, and if so run the function for the command
    if (strcmp(command, "ls") == 0) 
    {
        // Error check for ls
        if (numTokens > 1 && strcmp(lastCommand, "ls") == 0) {
            message = "Error! Incorrect syntax. No control code found.\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Incorrect syntax. No control code found.\n\n");
            error = 1;
            return error;
        }
        else if (numTokens > 1) {
            message = "Error! Unsupported parameters for command: ls\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Unsupported parameters for command: %s\n\n", command);
            error = 1;
            return error;
        }
        // end error check for ls
        // call the function in command.c for ls
        listDir();
    }
    else if (strcmp(command, "pwd") == 0) 
    {
        // Error check for pwd
        if (numTokens > 1 && strcmp(lastCommand, "pwd") == 0) {
            message = "Error! Incorrect syntax. No control code found.\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Incorrect syntax. No control code found.\n\n");
            error = 1;
            return error;
        }
        else if (numTokens > 1) {
            message = "Error! Unsupported parameters for command: pwd\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Unsupported parameters for command: %s\n\n", command);
            error = 1;
            return error;
        }
        // end error check for pwd
        // call the function in command.c for pwd
        showCurrentDir();    
    }
    else if (strcmp(command, "mkdir") == 0) 
    {
        if (numTokens > 2) {
            message = "Error! Incorrect syntax. No control code found.\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Unrecognized command: %s\n\n", lastCommand);
            error = 1;
            return error;
        }
        if (numTokens < 2) {
            message = "Error! Missing argument for dictionary name\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Missing argument for directory name\n\n");
            error = 1;
            return error;
        }
        makeDir(line.command_list[1]);
    }
    else if (strcmp(command, "cd") == 0) 
    {
        if (numTokens > 2) {
            message = "Error! Unsupported parameters for command: cd\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Unrecognized command: %s\n\n", lastCommand);
            error = 1;
            return error;
        }
        if (numTokens < 2) {
            message = "Error! Missing argument\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Missing argument\n\n");
            error = 1;
            return error;
        }
        changeDir(line.command_list[1]);
    }
    else if (strcmp(command, "cp") == 0) 
    {
        if (numTokens > 3) {
            message = "Error! Unsupported parameters for command: cp\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Unrecognized command: %s\n\n", lastCommand);
            error = 1;
            return error;
        }
        if (numTokens < 3) {
            message = "Error! Missing argument(s)\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Missing argument(s)\n\n");
            error = 1;
            return error;
        }
        copyFile(line.command_list[1], line.command_list[2]);
    }
    else if (strcmp(command, "mv") == 0) 
    {
        if (numTokens > 3) {
            message = "Error! Unsupported parameters for command: mv\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Unrecognized command: %s\n\n", lastCommand);
            error = 1;
            return error;
        }
        if (numTokens < 3) {
            message = "Error! Missing argument(s)\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Missing argument(s)\n\n");
            error = 1;
            return error;
        }
        moveFile(line.command_list[1], line.command_list[2]);
    }
    else if (strcmp(command, "rm") == 0) 
    {
        if (numTokens > 2) {
            message = "Error! Unsupported parameters for command: rm\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Unrecognized command: %s\n\n", lastCommand);
            error = 1;
            return error;
        }
        if (numTokens < 2) {
            message = "Error! Missing argument\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Missing argument(s)\n\n");
            error = 1;
            return error;
        }
        deleteFile(line.command_list[1]);
    }
    else if (strcmp(command, "cat") == 0) 
    {
        if (numTokens > 2) {
            message = "Error! Unsupported parameters for command: cat\n";
            write(1, message, strlen(message));
            //fprintf(stderr, "Error! Unrecognized command: %s\n\n", lastCommand);
            error = 1;
            return error;
        }
        if (numTokens < 2) {
            //fprintf(stderr, "Error! Missing argument(s)\n\n");
            message = "Error! Missing argument\n";
            write(1, message, strlen(message));
            error = 1;
            return error;
        }
        displayFile(line.command_list[1]);

    }
    else 
    {
        //message = strcat("Error! Unrecognized command: ", command);
        char messageBuf[256] = "";
        strcat(messageBuf, "Error! Unrecognized command: ");
        strcat(messageBuf, command);
        strcat(messageBuf, "\n\n");
        write(1, messageBuf, strlen(messageBuf));
        //fprintf(stderr, "Error! Unrecognized command: %s\n\n", command);
        // Is this how you are to break out of this to stop the program, or will this cause leaks?
        error = 1;
        return error;
    }
    return error;
}

