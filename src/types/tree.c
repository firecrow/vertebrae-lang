#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "types.h"


struct tree *new_tree(){
    struct tree *tree = malloc(sizeof(struct tree));
    if(tree == NULL){
        return NULL;
    }
    return tree;
};

static struct tree_entry *new_tree_entry(){
    struct tree_entry *entry = malloc(sizeof(struct tree_entry));
    if(entry == NULL){
        return NULL;
    }
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
    struct order_entry *order_entry = malloc(sizeof(struct order_entry));
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
            if(hash == node->hash){
                if(key->length == node->key->length){
                    if(!strncmp(key->content, node->key->content, key->length)){
                        return node->content;
                    }
                }
            }
            if(hash >= node->hash){
                node = node->right;
            }else {
                node = node->left;
            }
        };
        return NULL;
    }
}

void tree_add(struct tree *tree, struct string *key, struct value_obj *value){
    struct tree_entry *entry = new_tree_entry();
    entry->key = key;
    entry->content = value;
    entry->hash = djb2_hash(key);
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
            if(entry->hash >= node->hash){
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
                _set_entry(tree, &(node->left), entry);
            }
        }
    }
}
