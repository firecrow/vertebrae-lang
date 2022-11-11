#include "../gekkota.h"

void default_next(struct crw_state *ctx){
    if(ctx->cell){
        ctx->cell = ctx->cell->next;
    }
}


struct cell *get_next(struct cell *cell){
    if(cell){
        return cell->next;
    }
    return NULL;
}
