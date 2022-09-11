#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "types.h"

int tree_id = 0;
struct tree *new_tree(){
    struct tree *tree = malloc(sizeof(struct tree));
    if(tree == NULL){
        return NULL;
    }
    memset(tree, 0, sizeof(struct tree));
    tree->id = tree_id++;
    return tree;
};

static struct tree_entry *new_tree_entry(){
    struct tree_entry *entry = malloc(sizeof(struct tree_entry));
    if(entry == NULL){
        return NULL;
    }
    memset(entry, 0, sizeof(struct tree_entry));
    return entry;
}

static struct tree_entry *new_order_entry(){
    struct order_entry *entry = malloc(sizeof(struct order_entry));
    if(entry == NULL){
        return NULL;
    }
    memset(entry, 0, sizeof(struct order_entry));
    return entry;
}

static unsigned long djb2_hash(struct string *string){
    unsigned long hash = 5381;
    int c;
    for(int i = 0; i < string->length; i++){
        hash = ((hash << 5) + hash ) + string->content[i];

    }
    return hash;
}

static void _push_order(struct tree *tree, struct tree_entry *new){
    struct order_entry *order_entry = new_order_entry();
    order_entry->entry = new;
    if(order_entry == NULL){
        fprintf(stderr, "Error allocating order entry\n");
        exit(1);
    }
    if(tree->order == NULL){
        tree->order = order_entry;
    } else {
        order_entry->previous = tree->_last_order;
        tree->_last_order->next = order_entry;
    }
    tree->_last_order = order_entry;
}

static void _set_entry(struct tree *tree, struct tree_entry **target, struct tree_entry *new){
    *target = new;
    tree->count++;
    _push_order(tree, new);
}

struct value_obj *tree_get(struct tree *tree, struct string *key){
    unsigned long hash = djb2_hash(key); 
    if(tree->root == NULL){
        return NULL;
    } else {
        struct tree_entry *node = tree->root;
        while(node){
            int cmp = strncmp(key->content, node->key->content, key->length);
            if(key->length == node->key->length){
                if(cmp == 0){
                    return node->content;
                }
            }
            if(cmp > 0){
                node = node->right;
            }else{
                node = node->left;
            }
        };
        return NULL;
    }
}

/* todo make this more comprehensive */
void tree_update(struct tree *tree, struct string *key, struct value_obj *value){
    tree_add(tree, key, value);
}

void tree_add(struct tree *tree, struct string *key, struct value_obj *value){
    struct tree_entry *entry = new_tree_entry();
    entry->key = key;
    entry->content = value;
    if(entry == NULL){
        fprintf(stderr, "Tree add recieved a NULL entry");
        return;
    }
    if(tree->root == NULL){
        _set_entry(tree, &(tree->root), entry);
        return;
    } else {
        struct tree_entry *node = tree->root;
        while(node){
            int cmp = strncmp(entry->key->content, node->key->content, entry->key->length);
            if(cmp == 0){
                node->content = entry->content;
                return;
            }
            if(cmp > 0){
                if(!node->right){
                    _set_entry(tree, &(node->right), entry);
                    return;
                }
                node = node->right;
            }else {
                if(!node->left){
                    _set_entry(tree, &(node->left), entry);
                    return;
                }
                node = node->left;
            }
        }
    }
}
