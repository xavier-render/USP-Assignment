    /**
 * @file pipe.c
 * @author Xavier Render 
 * @date 2022-05-29
 * 
 * PURPOSE: Main program demonstrating how a pipe in Unix is used to facilate inter-process communicaton, through the use of system calls and linked list.
 * 
 * REFERENCES: 
 * Splitting a string using strtok() in C - https://www.educative.io/edpresso/splitting-a-string-using-strtok-in-c
 * USP Lecture 05 - C system calls 
 *  
 */
    
    #include <stdlib.h>
    #include <stdio.h>
    #include <assert.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <sys/wait.h>
    #include "linkedList.h"

    #define BUF_SIZE 1000
    
    /*
    Splits a line by space character and puts the splitted
    parts into parts array.
    */
    void split_line(char **parts, char *line)
    {
        int i = 0;
        char *part;

        // extract the first token 
        part = strtok(line, " ");

        // using while loop to extract all other tokens 
        while(part != NULL) {
            parts[i++] = part;
            part = strtok(NULL, " ");
        }
        parts[i] = NULL;
    }

    int main(int argc, char *argv[])
    {
        // error checking, if there's no input file given when running program
        if (argc != 2)
        {
            printf("Usage: ./pipesim <input_file>\n");
            exit(EXIT_FAILURE);
        }

        // Initialize linked list.
        linked_list_t ll;
        ll.head = NULL;
        ll.tail = NULL;

        // buffer line storing lines from file one by one.
        char line[BUF_SIZE] = {0};
        char *filename = argv[1];

        // System call - open in read only mode.
        int fd = open(filename, O_RDONLY);

        // error checking 
        if (fd == -1)
        {
            fprintf(stderr, "failed to open file %s for reading\n", filename);
            exit(EXIT_FAILURE);
        }

        int i = 0;

        // Keep reading character by character and append those characters into line buffer.
        while(read(fd, &line[i], 1) == 1)
        {
            // Reached end of a line.
            if(line[i] == '\n')
            {
                // Terminate the line and insert into linked list.
                line[i] = '\0';
                // call the function insert
                insert(strlen(line) + 1, line, &ll);
                // Reset buffer position.
                i = 0;
                continue;
            }
            i++;
        }
        if(i > 0)
        {
            // Last line, ending without \n.
            line[i] = '\0';
            // call the insert function
            insert(strlen(line), line, &ll);
        }

        i = 1;
        node_ptr_t tmp = ll.head;
        // Traverse the linked list and run the processes.
        while(tmp != NULL)
        {
            // Entire program name.
            char *program = (char*)tmp->data;
            char *args[100];
            
            // Split the arguments of the program.
            split_line(args, program);

            // duplicate processes
            pid_t pid = fork();
            if (pid == 0)
            {
                // Child process.
                if(i == 1)
                {
                    // No input to the command from before
                }
                else
                {
                    // Read input from file output<i-1>.txt
                    char filename[100];
                    sprintf(filename, "output%d.txt", i-1);
                    int fd2 = open(filename, O_RDONLY, 0777);
                    if(fd2 < 0) {
                        perror("open");
                    }
                    // error checking
                    if(dup2(fd2, STDIN_FILENO) < 0) {
                        perror("Unable to duplicate file descriptor.");
                        exit(EXIT_FAILURE);
                    }
                    if(close(fd2) < 0) {
                        perror("close");
                    }
                }
                if (tmp->next == NULL)
                {
                    // Last command, write output to terminal.
                }
                else
                {
                    // Write output to file output<i>.txt
                    char filename[100];
                    // instead of printing output in a console, sprintf stores the output on char buffer (filename)
                    sprintf(filename, "output%d.txt", i);
                    int fd2 = open(filename, O_RDWR|O_CREAT|O_TRUNC, 0777);
                    if(fd2 < 0) {
                        perror("open");
                    }
                    if(dup2(fd2, STDOUT_FILENO) < 0) {
                        perror("Unable to duplicate file descriptor.");
                        exit(EXIT_FAILURE);
                    }
                    if(close(fd2) < 0) {
                        perror("close");
                    }
                }
                // empty the buffer using fflush 
                fflush(stdout);
                
                // error checking 
                if (execvp(args[0], args) < 0) {
                    perror("execv");
                }
                exit(0);
            }
            // Parent Process
            else if(pid > 0)
            {
                if (waitpid(pid, NULL, 0) < 0) {
                    perror("waitpid");
                }
            }
            i++;
            tmp = tmp->next;
        }
    }