#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "base.h"
#include "run/run.h"
#include "types/types.h"
#include "core/core.h"
#include "operators/operator.h"
#include "library/library.h"
#include "parse/parse.h"

#define BUFF_SIZE 4096
#define STDIN_BLOCK_SIZE 128
#define INTERVAL 100
