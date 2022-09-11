#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "../operators/operator.h"
#include "run.h"

struct cell *get_next(struct cell *cell){
    if(cell){
        return cell->next;
    }
    return NULL;
}
