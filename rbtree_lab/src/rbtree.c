#include "rbtree.h"
#include <stdlib.h>

#include <stdio.h>
#include <string.h>

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
    p -> root = nil;

    return p;
}

void free_node(rbtree *t , node_t *x) {
  if (x == t -> nil) return ;
  free_node(t, x -> left);
  free_node(t, x -> right);
  free(x);
}

void delete_rbtree(rbtree *t) {
    // TODO: reclaim the tree nodes's memory
    if (!t) return ;
    if (t ->root != t -> nil) {
      free_node(t, t -> root -> left);
      free_node(t, t -> root -> right);
      free(t -> root);      
    }
    free(t -> nil);
    free(t);
}

void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x -> right;
  x -> right = y -> left;
  if (y -> left != t -> nil) {
    y -> left -> parent = x;
  }
  y -> parent = x -> parent;
  if (x -> parent == t -> nil) { 
    t -> root = y;
  }
  else if (x -> parent -> left == x) {
    x -> parent -> left = y;
  }
  else {
    x -> parent -> right = y;
  }
  y -> left = x;
  x -> parent = y;
}


void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x -> parent;
  y -> left = x -> right;
  if (x -> right != t -> nil) {
    x -> right -> parent = y;
  }
  x -> parent = y -> parent;
  if (y -> parent == t -> nil) {
    t -> root = x;
  }
  else if(y -> parent -> right == y){
    y -> parent -> right = x;
  }
  else {
    y -> parent -> left = x;
  }
  x -> right = y;
  y -> parent = x;
}

void insert_fix_up(rbtree *t, node_t *x) {
  node_t *cur = x;
  // parent is red
  while (cur -> parent -> color == RBTREE_RED){
    // grandparent's left is x's parent
    if (cur -> parent -> parent ->left == cur -> parent) {
      // y ->  uncle is right
      node_t *y = cur -> parent ->  parent -> right;
      if (y -> color == RBTREE_RED) {
        cur -> parent ->color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        cur -> parent -> parent -> color = RBTREE_RED;
        cur = cur -> parent -> parent;
      }
      // uncle is black
      else {
        // uncle is black and cur is right child
        if (cur -> parent -> right == cur) {
          cur = cur -> parent;
          left_rotate(t, cur);
        }
        // uncle is black and cur is left child
        cur -> parent -> color = RBTREE_BLACK;
        cur -> parent -> parent -> color = RBTREE_RED;
        right_rotate(t, cur -> parent);
      }
    }
    // grandparent's right is x's parent
    else {
      node_t *y = cur -> parent -> parent -> left;
      if (y -> color == RBTREE_RED) {
        cur -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        cur -> parent -> parent -> color = RBTREE_RED;
        cur = cur -> parent -> parent;
      }
      else {
        if (cur -> parent -> left == cur) {
          cur = cur -> parent;
          right_rotate(t, cur -> left);
        }
        cur -> parent -> color = RBTREE_BLACK;
        cur -> parent -> parent -> color = RBTREE_RED;
        left_rotate(t, cur -> parent -> parent);
      }
    }
  }
  t -> root -> color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert

    // new node 
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node -> color = RBTREE_RED;
    new_node -> key = key;
    new_node -> parent = t -> nil;
    new_node -> left = t -> nil;
    new_node -> right = t -> nil;

    node_t *cur = t -> root;
    node_t *prev = t -> nil;
    // move to nil 경계까지 move
    while (cur != t -> nil) {
      prev = cur;
      if (new_node -> key < cur -> key){
        cur = cur ->left;
      }
      else {
        cur = cur ->right;
      }
    }
    // new_node parent 설정
    new_node -> parent = prev;

    // 부모 노드와 new_node 연결
    if(prev == t -> nil) {
      t->root = new_node;
    }
    else if (prev -> key > new_node -> key) {
      prev -> left = new_node;
    }
    else {
      prev -> right = new_node;
    }

    // 규칙 확인
    insert_fix_up(t, new_node);
    
    return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *cur = t -> root;
  
  while (cur != t -> nil) {
    if (cur -> key == key) {
      return cur;
    }
    if (cur -> key < key) {
      cur = cur -> right;
    }
    else {
      cur = cur -> left;
    }
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t -> root;
  node_t *min = t -> nil;
  while (cur != t -> nil) {
    min = cur;
    cur = cur -> left;
  }
  return min;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t -> root;
  node_t *max = t -> nil;
  while (cur != t -> nil) {
    max = cur;
    cur = cur -> right;
  }
  return max;
}

// 서브트리 교체 (삭제 시 필요)
void transplant (rbtree *t, node_t *u, node_t *v) {
  if (u -> parent == t -> nil){
    t -> root = v;
  }
  else if (u -> parent -> left == u) {
    u -> parent -> left = v;
  }
  else {
    u -> parent -> right = v;
  }
  v -> parent = u -> parent;
}

node_t *find_seccesor(rbtree*t, node_t *x) {
  while (x -> left != t -> nil) {
    x = x -> left;
  }
  return x;
}
// delete할 노드가 black이여서 규칙 적용.
void delete_fix_up(rbtree *t, node_t* x){
  while (t -> root != x && x -> color == RBTREE_BLACK){
    // 왼쪽 자식
    if (x -> parent -> left == x){
      // 형제 노드
      node_t *w = x -> parent -> right;
      // case1 : w == red
      if (w -> color == RBTREE_RED) {
        x -> parent -> color = RBTREE_RED;
        w -> color = RBTREE_BLACK;
        left_rotate(t, x -> parent);
        w = x -> parent -> right;
      }
      else {
        // case2 : w == black && w's children is black
        if (w -> right  -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK){
          w -> color = RBTREE_RED;
          x = x -> parent;
        }
        // case3 : w == black && w's left is red and w's right is black
        else { 
          if (w -> left -> color == RBTREE_RED) {
            w -> color = RBTREE_RED;
            w -> left -> color = RBTREE_BLACK;
            right_rotate(t, w -> left);
            w = x -> parent -> right;
          }
          // case4 : w == black && w's right is red
          else {
            w -> color = x -> parent ->color;
            x -> parent -> color = RBTREE_BLACK;
            w -> right ->color = RBTREE_BLACK;
            left_rotate(t, x -> parent);
            x = t -> root;
          }
        }
      }
    }
    // right child
    else {
      // sibling node
      node_t *w = x -> parent -> left;
      // case1 : w == red
      if (w -> color == RBTREE_RED){
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        right_rotate(t, w);
        w = x-> parent -> left;
      }
      else {
        // case2 : w == black && w->left == black and w->right == black
        if (w -> right -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK) {
          w -> color = RBTREE_RED;
          x = x -> parent;
        }
        else {
          // case3 : w == black && w->right == red and w->left ==black
          if (w -> right -> color == RBTREE_RED){
            w -> color = RBTREE_RED;
            w -> right -> color = RBTREE_BLACK;
            left_rotate(t, w);
            w = x -> parent -> left;
          }
          // case4 : w == black && w->left == red
          else {
            w -> color = x -> parent -> color;
            x -> parent -> color = RBTREE_BLACK;
            w -> left -> color = RBTREE_BLACK;
            right_rotate(t, w);
            x = t -> root;
          }
        }
      }
    }
  }
  x -> color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y = p;
  color_t y_original_color = y -> color;
  node_t *x = t -> nil;
  // 자식이 왼쪽에 있는 경우
  if (y -> right == t -> nil) {
    x = y -> left;
    transplant(t, p, p->left);
  }
  // 자식이 오른쪽에만 있는 경우
  else if(y -> left == t -> nil) {
    x = y -> right;
    transplant(t, p, p->right);
  }
  // 자식이 둘다 있는 경우 
  else {
    y = find_seccesor(t, p -> right);
    y_original_color = y -> color;
    // y의 자식은 무조건 오른쪽에만 존재. y의 자리를 채울 노드.
    x = y -> right;
    // 바로 밑의 자식이 아닌 경우
    if (p -> right != y) {
      transplant(t, y, y -> right); // y를 오른쪽 자식과 교체 y의 자리에 올라가는 것
      y -> right = p -> right;      
      y -> right -> parent = y;
    }
    else {
      x -> parent = y;
    }
    // succesor랑 바꿀려는 노드랑 위치를 바꾼거
    transplant(t, p, y);
    y -> left = p -> left;
    y -> left -> parent = y;
    y -> color = p -> color;
  }

  if (y_original_color == RBTREE_BLACK) {
    delete_fix_up(t, x);
  }

  free(p);
  return 0;
}

void inorder(const rbtree *t, node_t *x, key_t *arr, size_t *idx) {
  if (x != t -> nil){
    inorder(t, x -> left, arr, idx);
    arr[(*idx)++] = x -> key;
    inorder(t, x -> right, arr, idx);
  }
  return ;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array

  size_t idx = 0;
  inorder(t, t -> root, arr, &idx);
  return 0;
}

void pretty_dfs(const rbtree *t, const node_t *node, const char *prefix, int is_left) {
    if (node == t->nil) return;

    // 현재 노드 라벨
    const char *col = (node->color == RBTREE_RED) ? "R" : "B";

    // 가지 접두사
    char branch[256];
    snprintf(branch, sizeof(branch), "%s%s", prefix, is_left ? "├─" : "└─");

    // 출력
    if (node->parent == t->nil)
        printf("%s%d(%s)\n", branch, node->key, col);               // root
    else
        printf("%s%d(%s) [p=%d]\n", branch, node->key, col, node->parent->key);

    // 다음 레벨 접두사 준비
    char next_prefix[256];
    snprintf(next_prefix, sizeof(next_prefix), "%s%s", prefix, is_left ? "│ " : "  ");

    // 왼쪽 먼저, 그 다음 오른쪽 (관습적으로)
    // 왼쪽을 'is_left=1', 오른쪽을 'is_left=0'로 내려가게 해서 모양을 가지치기
    if (node->left != t->nil || node->right != t->nil) {
        if (node->left != t->nil)
            pretty_dfs(t, node->left, next_prefix, 1);
        else
            printf("%s├─NIL\n", next_prefix);

        if (node->right != t->nil)
            pretty_dfs(t, node->right, next_prefix, 0);
        else
            printf("%s└─NIL\n", next_prefix);
    }
}

void debug_print_pretty(const rbtree *t) {
    if (t->root == t->nil) { puts("Tree is empty."); return; }
    puts("=== RBTree Pretty ===");
    // 루트는 상위 접두사가 없으므로 약간 트릭으로 호출
    pretty_dfs(t, t->root, "", 0);
    puts("=====================");
}