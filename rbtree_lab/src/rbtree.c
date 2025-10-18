#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    // TODO: initialize struct if needed
    
    node_t *nil = (node_t *)malloc(sizeof(*nil));
    if (!nil) {
      free(p);
      return NULL;
    }

    nil -> color = RBTREE_BLACK;
    nil -> key = 0;
    nil -> parent = nil;
    nil -> left = nil;
    nil -> right = nil;

    p -> nil = nil;
    p -> root = NULL;

    return p;
}

void delete_rbtree(rbtree *t) {
    // TODO: reclaim the tree nodes's memory
    if (!t) return ;
    if (t ->root != NULL) {
      rbtree *left = (rbtree *)malloc(sizeof(rbtree));
      left -> root = t -> root -> left;
      left -> nil = t ->nil;
      rbtree *right = (rbtree *)malloc(sizeof(rbtree));
      right -> root = t -> root ->right;
      right -> nil = t ->nil;
      delete_rbtree(left);
      delete_rbtree(right);
      free(t -> root);      
    }
    free(t -> nil);
    free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert
    
    return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
