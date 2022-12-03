#include "../gekkota.h"

/* TODO: add stdin support, not sure how to implement not blocking */

void handle_out_line(char *line){
    printf("we got a line '%s'\n", line);
}

void handle_err_line(char *line){
    printf("we got an error line '%s'\n", line);
}

void handle_exit(int exit_code){
    printf("we got an exit code '%d'\n", exit_code);
}

/* loop through the bytes of the file until a new lihne is found, or it
 * non-blockingly returns nothing 
 *
 * return is if the line is complete or not. in the case of a complete line, the
 * index is set back to 0 before this function returns
 */
int read_chunk(FILE *stream, char buff[], int *idxp){
    char c;
    int length;
    char *line;
    while((length = fread(&c, 1, 1, stream)) != -1){
        /* no length indicates a non-blocking empty return */
        if(length){
            buff[*idxp++] = c;
            if(c == '\n'){
                buff[*idxp] = 0;
                *idxp = 0;
                return 1;
            }
            if(*idxp >= BUFF_SIZE-1){
                buff[*idxp] = 0;
                return 0;
            }
        }else{
            return 0;
        }
    }
}

/* 
 * fork and exec and redirect the pipes so that stdout/stderr of the child are
 * managable from the parent
 *
 * loop through the status of hte child and if any content is available from
 * the output streams
 */
int exec(char *cmd_path, char *args[]){
    char err_buff[4096];
    char out_buff[4096];

    int out[2];
    int err[2];
    pipe(out);
    pipe(err);

    int pid = fork();
    int status;
    int w;
    
    struct timespec interval;
    interval.tv_sec = 0;
    interval.tv_nsec = INTERVAL;

    int return_code;

    struct timespec time_remaining;

    if(pid == -1){
        fprintf(stderr, "Error forking");
        exit(1);
    }else if(pid == 0){
        printf("im the child");

        dup2(1, out[0]);
        dup2(2, err[0]);

        execvp(cmd_path, args);
    }else{
        printf("I'm the parent %d", pid);

        fcntl(out[1], F_SETFL, O_NONBLOCK);
        fcntl(err[1], F_SETFL, O_NONBLOCK);

        FILE *fout= fdopen(out[1], "r");
        FILE *ferr= fdopen(err[1], "r");

        /* indexes used to keep track of where in the buffers the different
         * streams are 
         */
        int out_idx = 0;
        int err_idx = 0;

        do {
            w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
            if (w == -1) {
               fprintf(stderr, "Error listening for process status");
               exit(1);
            }

            /* check for a stdout line */
            if(read_chunk(fout, out_buff, &out_idx)){
                handle_out_line(out_buff);
            }
            /* check for a stderr line */
            if(read_chunk(ferr, err_buff, &err_idx)){
                handle_out_line(err_buff);
            }
            /* check if the process has returned */
            int status_int = waitpid(pid, &status, WNOHANG);
            if(status_int != -1){
                if(WIFEXITED(status)){
                    WEXITSTATUS(status);
                }else{
                    break;        
                }
           }
           if(WIFEXITED(status)){
               handle_exit(WEXITSTATUS(status));
               break; 
           }else if(WIFSIGNALED(status)){
               handle_exit(WTERMSIG(status));
               break;
           }else if(WIFSTOPPED(status)){
               handle_exit(WSTOPSIG(status));
               break;
           }
           nanosleep(&interval, &time_remaining);
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}
