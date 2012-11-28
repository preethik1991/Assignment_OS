#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define TRUE 1

int main(void)
{
    char *arg_list[10];

    int status;

    int counter = 0;

    int counter2 = 0;

    pid_t pid;
    char buf[100];

    char inFile[10];
    char outFile[10];

    int fdIn, fdOut;
    while(TRUE)
    {
        printf("> ");
        if (!fgets(buf, 100, stdin))
            return 0;

        pid = fork();
        switch(pid)
        {
            case -1:
                return 1;

            case 0:
                {
                    arg_list[counter] = strtok(buf, " \n");

                    counter = 0;

                    while(arg_list[counter] != NULL)
                    {
                        counter++;
                        arg_list[counter] = strtok(NULL, " \n");
                    }

                    counter2 = 0;

                    while(arg_list[counter2] != NULL)
                    {
                        if(!strcmp(arg_list[counter2], "<"))
                        {
                            if(arg_list[counter2+1] != NULL)
                            {
                                fdIn = open(arg_list[counter2+1], O_RDONLY);
                                dup2(fdIn, STDIN_FILENO);
                            }
                            else
                            {
                                printf("No input file specified");
                            }
                            arg_list[counter2] = 0;
                        }
                        else if(!strcmp(arg_list[counter2], ">"))
                        {
                            if(arg_list[counter2+1] != NULL)
                            {
                                fdOut = open(arg_list[counter2+1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
                                dup2(fdOut, STDOUT_FILENO);
                            }
                            else
                            {
                                printf("No output file specified");
                            }
                            arg_list[counter2] = 0;
                        }
                        counter2++;
                    }

                    execvp(arg_list[0], arg_list);
                    break;
                }

            default:
                waitpid(-1, &status, 0);
                break;
        }
    }

    return 0;
}
