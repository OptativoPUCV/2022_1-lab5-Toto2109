#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;
    new -> root = NULL;
    new -> current = NULL;
    new ->lower_than = lower_than; 
    //new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {

    TreeNode *new = createTreeNode(key,value);
    if(tree -> root == NULL){
        tree -> root = new;
        tree -> current = new;
    }

    TreeNode * aux = tree -> root;
    while (aux != NULL){
        if (tree -> lower_than(key,aux -> pair -> key) == 1){
            if(aux -> left == NULL) break;
            aux = aux -> left;
        }else{
            if(aux -> right == NULL) break;
            aux = aux -> right;
        }
    }

    if(is_equal(tree,key,aux -> pair -> key)){
        tree -> current = aux;
        return;
    }
    tree -> current = new;
    new -> parent = aux;
    if (tree -> lower_than(key,aux -> pair -> key) == 1){
        aux -> left = new;
        }else{
            aux -> right = new;
        }

}

TreeNode * minimum(TreeNode * x){
    if(x -> left == NULL) return x;
    return minimum(x->left);
}


void removeNode(TreeMap * tree, TreeNode* node) {

    if(node -> left == NULL && node -> right == NULL){
        if(node -> parent -> left == node){
            node -> parent -> left = NULL;
        }else{
            node -> parent -> right = NULL;
        }
        free(node);
        return;
    }

    if(node -> left == NULL){
        TreeNode *aux = node -> right;
        if(node -> parent -> left == node){
            node -> parent -> left = aux;
        }else{
            node -> parent -> right = aux;
        }
        aux -> parent = node -> parent;
        free(node);
        return;
    }

    if(node -> right == NULL){
        TreeNode *aux = node -> left;
        if(node -> parent -> left == node){
            node -> parent -> left = aux;
        }else{
            node -> parent -> right = aux;
        }
        aux -> parent = node -> parent;
        free(node);
        return;
    }

    TreeNode *aux = minimum(tree -> current -> right);
    node -> pair = aux -> pair;
    removeNode(tree, aux);
    return;

    
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key){

    if(tree -> root == NULL) return NULL;
    TreeNode * aux = tree -> root;
    while (aux != NULL){
        if(is_equal(tree,key,aux -> pair -> key)){
            tree -> current = aux;
            return tree -> current -> pair;
        }
        if (tree -> lower_than(key,aux -> pair -> key) == 1){
            aux = aux -> left;
        }else{
            aux = aux -> right;
        }
    }
    return NULL;
}




Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {

    TreeNode *minimo =  minimum(tree -> root);
    tree -> current = minimo;

    return minimo -> pair;
}

Pair * nextTreeMap(TreeMap * tree) {

    if(tree -> current -> right != NULL){
        tree -> current = minimum(tree -> current -> right);
        return tree -> current -> pair;
    }else{
        TreeNode *aux = tree -> current;

        while (tree -> current -> parent != NULL){

            if(tree -> lower_than (aux -> pair -> key,tree -> current -> parent -> pair -> key)){
                tree -> current = tree -> current -> parent;
                return tree -> current -> pair;
            }else{
                tree -> current = tree -> current -> parent;
            }

        }
        return NULL;


    }

}
