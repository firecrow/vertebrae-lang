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
}

struct tree_entry *new_tree_entry(){
    struct tree_entry *entry = malloc(sizeof(struct tree_entry));
    if(entry == NULL){
        return NULL;
    }
    return entry;
}

unsigned long djb2_hash(struct string *string){
    unsigned long hash = 5381;
    int c;
    for(int i = 0; i < string->length; i++){
        hash = ((hash << 5) + hash ) + string->content[i];

    }
    return hash;
}

void *tree_get(struct tree *tree, struct string *key){
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

void tree_add(struct tree *tree, struct string *key, void *value){
    struct tree_entry *entry = new_tree_entry();
    entry->content = value;
    entry->hash = djb2_hash(key);
    if(entry == NULL){
        fprintf(stderr, "Tree add recieved a NULL entry");
        return;
    }
    if(tree->root == NULL){
        tree->root = entry;
        return;
    } else {
        struct tree_entry *node = tree->root;
        while(node){
            if(entry->hash >= node->hash){
                if(!node->right){
                    node->right = entry;
                    return;
                }
                node = node->right;
            }else {
                if(!node->left){
                    node->left = entry;
                    return;
                }
                node = node->left;
            }
        }
    }
}
