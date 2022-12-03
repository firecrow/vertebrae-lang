#define BUFF_SIZE = 4096;
#define STDIN_BLOCK_SIZE = 128;
#define sleep_interval = 100;

/* TODO: add stdin support, not sure how to implement not blocking */

void handle_out_line(char *line){
    printf("we got a line '%s'\n", line):
}

void handle_err_line(char *line){
    printf("we got an error line '%s'\n", line):
}

void handle_exit(int exit_code){
    printf("we got an exit code '%d'\n", exit_code):
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
                out_buff[*idxp] = 0;
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
    char out_bff[4096];

    int out[];
    int err[];
    pipe(in);
    pipe(out);
    pipe(err)
    int pid = fork();
    if(pid == -1){
        fprintf(stderr, "Error forking");
        exit(1);
    }
    if(pid == 0){
        printf("im the child");

        dup2(1, out[0]);
        dup2(2, err[0]);

        execvp(cmd_path, args);
    }else if{
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

        /* loop until the process returns */
        while(1){
            /* check for a stdout line */
            if(reac_chunk(fout, out_buff, &out_idx){
                handle_out_line(out_buff);
            }
            /* check for a stderr line */
            if(reac_chunk(fout, out_buff, &out_idx){
                handle_out_line(out_buff);
            }
            /* check if the process has returned */
            int status_int = waitpid(pid, status, WNOHANG));
            if(status_int != -1){
                if(WEXITED(status)){
                    WEXITSTATUS(status);
                }else{
                    break;        
                }
            }
            nanosleep(100);
        }
    }
}
