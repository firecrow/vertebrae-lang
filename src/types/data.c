#include "../gekkota.h"

int crw_ctx_data_id = 0;
struct crw_ctx_data *new_data(){
   struct crw_ctx_data *data = malloc(sizeof(struct crw_ctx_data)); 

   if(data == NULL){
      return NULL;
   }

   memset(data, 0, sizeof(struct crw_ctx_data));
   data->id = crw_ctx_data_id++;

   return data;
}
