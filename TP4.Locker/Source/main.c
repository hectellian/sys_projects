#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
        if (argc != 2) {
            fprintf(stderr, "Not enough arguments: Should be './locker filename'\n");
            exit(EXIT_FAILURE);
        }

        struct flock fl;

        // variables
        int cmd, start, length;
        char input[20];
        char input_cmd;
        char input_type;
        char input_w = 's';
        char* EXIT_PROG = "exit\n";
        char* HELP = "?\n";

        //opening file and testing if there's a problem
        int fd = open(argv[1], O_RDWR);
        if (fd < 0) {
            fprintf(stderr, "Couldn't open file %s: %s\n", argv[1], strerror(errno));
            exit(EXIT_FAILURE);
        }

    for  (;;) { /* Prompt for  locking command  and  carry it out */

        // GUI
        printf("Enter ? for help\n");
        printf("PID=%ld> ", (long)  getpid());
        fflush(stdout);

        // gets input from standard input
        fgets(input, sizeof(input), stdin);

        // help gui
        if (strcmp(input, HELP) == 0) {
            printf("\n");
            printf("    Format: cmd l_type start length [whence(optional)]\n");
            printf("    ’cmd’ --- ’g’ (F_GETLK), ’s’ (F_SETLK), or ’w’ (F_SETLKW)\n");
            printf("    ’l_type ’ --- ’r’ (F_RDLCK), ’w’ (F_WRLCK), or ’u’ (F_UNLCK)\n");
            printf("    ’start ’ --- lock starting offset\n");
            printf("    ’length ’ --- number of bytes to lock\n");
            printf("    ’whence ’ --- ’s’ (SEEK_SET , default), ’c’ (SEEK_CUR), or ’e’ (SEEK_END)\n");
            printf("    'exit' --- Type exit to quit the program\n");
            printf("\n");
        }
        if (strcmp(input, EXIT_PROG) == 0) {
            close(fd);
            exit(EXIT_SUCCESS);
        }


        // Parsing options
        int statcmd = sscanf(input, "%c %c %d %d %c\n", &input_cmd, &input_type, &start, &length, &input_w);

        // Comparing options to give commands
        switch(input_cmd) {
            case 'g':
                cmd = F_GETLK;
                break;
            case 's':
                cmd = F_SETLK;
                break;
            case 'w':
                cmd = F_SETLKW;
                break;
        }
        switch(input_type) {
            case 'r':
                fl.l_type = F_RDLCK;
                break;
            case 'w':
                fl.l_type = F_WRLCK;
                break;
            case 'u':
                fl.l_type = F_UNLCK;
                break;
        }

        fl.l_start = start;
        fl.l_len = length;
        
        switch(input_w) {
            case 's':
                fl.l_whence = SEEK_SET;
                break;
            case 'c':
                fl.l_whence = SEEK_CUR;
                break;
            case 'e':
                fl.l_whence = SEEK_END;
                break;
        }

    // process user unput into the 'cmd'  variable  and  the various  elements  of 'fl'  struct
        int status = fcntl(fd, cmd, &fl); /* Perform request... */
    // interpret results of request and  inform user
        if (cmd == F_GETLK)  { /* F_GETLK*/
    // check status and handle  errors  (look  at manual  for  possible errors)
            if  (status == 0 ) {
    // process results and  print informative  text
                if (fl.l_type == F_UNLCK) {
                    printf("[PID=%ld] got lock\n", (long)  getpid());
                }
                else {
                    switch(fl.l_type) {
                        case F_RDLCK:
                            printf("[PID=%ld] Denied by READ lock on %lu:%lu (held by PID %d)\n", (long)  getpid(), fl.l_start, fl.l_len, fl.l_pid);
                            break;
                        case F_WRLCK:
                            printf("[PID=%ld] Denied by WRITE lock on %lu:%lu (held by PID %d)\n", (long)  getpid(), fl.l_start, fl.l_len, fl.l_pid);
                            break;
                    }
                }
            } else if (errno == EINVAL) {
    // process results and  print informative  text
                fprintf(stderr, "Cannot get lock: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        } else { /* F_SETLK,  F_SETLKW  */
    // check status and handle  errors  (look  at manual  for  possible errors)
            if  (status == 0 ) {
    // process results and  print informative  text
                if (fl.l_type == F_UNLCK) {
                    printf("[PID=%ld] unlocked %s\n", (long)  getpid(), argv[1]);
                }
                else {
                    printf("[PID=%ld] locked %s\n", (long)  getpid(), argv[1]);
                }
            } else if (errno == EACCES || errno == EAGAIN) {
                fprintf(stderr, "Couldn't access lock: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        } 
    }
    
    return 0;
}