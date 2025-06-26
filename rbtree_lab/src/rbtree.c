#include "rbtree.h"
#include <stdlib.h>


rbtree *new_rbtree(void) {
    rbtree *p = (rbtree *) calloc(1, sizeof(rbtree));
    // TODO: initialize struct if needed
    p->nil = (node_t *) calloc(1, sizeof(node_t));
    p->root = p->nil;

    p->nil->color = RBTREE_BLACK;
    p->nil->parent = p->nil;
    p->nil->left = p->nil;
    p->nil->right = p->nil;
    // nil -> key는 사용하지 않음?
    return p;
}

void free_node(rbtree *t, node_t *node) {
    if (node == t->nil) { return; }

    free_node(t, node->left);
    free_node(t, node->right);
    free(node);
}

void delete_rbtree(rbtree *t) {
    // TODO: reclaim the tree nodes's memory
    free_node(t, t->root);
    free(t->nil);
    free(t);
}

void set_nil(rbtree* t) {
    t -> nil -> parent = t -> nil;
    t -> nil -> right = t -> nil;
    t -> nil -> left = t -> nil;
    t -> nil -> color = RBTREE_BLACK;
}

void rotate(rbtree *t, node_t *gp, node_t *p, node_t *c){
    if (c != t->nil) { c -> parent = gp;}
    if (p != t->nil) { p -> parent = c; }

    // grand parent의 자식 포인터
    if (gp != t->nil) {
        if (gp->left == p) { gp->left = c; }
        else { gp->right = c; }
    }

    // p와 c의 자식 포인터
    if (p -> left == c) {
        p -> left = c -> right;
        c -> right -> parent = p;
        c -> right = p;
    }

    else {
        p->right = c->left;
        c->left -> parent = p;
        c->left = p;
    }
    color_t temp = p -> color;
    p -> color = c -> color;
    c -> color = temp;

    set_nil(t);
    find_root(t);
    fixRootColor(t);
}

void switch_color(rbtree *t, node_t *parent_node, color_t p, color_t c) {
    parent_node -> left -> color = c;
    parent_node -> right -> color = c;

    parent_node -> color = p;

    set_nil(t);
    fixRootColor(t);
}

void fixDoubleRed(rbtree* t, node_t *gp, node_t *p, node_t *c) { // grandparents, parents, child

    // 케이스 별로 나누기.
    // case 1: 레드가 몰려있지 않은 경우
    if (gp->left->color == RBTREE_RED && gp->right->color == RBTREE_RED) {
        switch_color(t, gp, RBTREE_RED, RBTREE_BLACK);

        if (gp->parent->color == RBTREE_RED) {
            fixDoubleRed(t, gp->parent->parent, gp->parent, gp);
        } // gp->parent가 nil인 경우는 생각하지 않아도 됨. 이미 color에서 걸러짐.

    // case2 : 레드가 몰려있고, 꺽인경우
    } else if ((gp->left == p) ^ (p->left == c)) {  // 둘다 red인 경우는 이미 걸러짐
        rotate(t, gp, p, c);    // p를 기준으로 p와 c를 회전
        fixDoubleRed(t, gp, c, p); // case3으로 해결.
    }

    // case3: 안 꺽인 경우
    else if (!((gp->left == p) ^ (p->left == c))) {
        // 수정범위 gp->parent의 child ~ c의 parent
        rotate(t, gp->parent, gp, p);   // gp를 기준으로 gp와 p를 회전
    }
//    set_nil(t);
//    find_root(t);
}


void find_root(rbtree *t) {
    if (t->root->parent == t->nil) {
        return;
    }

    node_t *curr = t->root;
    while (curr->parent != t->nil) {
        curr = curr->parent;
    }

    t->root = curr;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert

    node_t *addNode = calloc(1, sizeof(node_t));
    addNode->left = t->nil;
    addNode->right = t->nil;
    addNode->color = RBTREE_RED;
    addNode->key = key;

    // root에 삽입
    if (t->root == t->nil) {
        t->root = addNode;
        addNode->parent = t->nil;
    } else {
    // 이외에 삽입
        node_t *curr;
        node_t *next = t->root;
        int isLeft = 1;
        while (next != t->nil) {
            curr = next;
            if (key <= curr->key) {
                next = curr->left;
                isLeft = 1;
            } else {
                next = curr->right;
                isLeft = 0;
            }
        }

        // curr -> nil의 부모 / next -> nil(삽입할 자리)
        addNode->parent = curr;

        if (isLeft) { curr->left = addNode; }
        else { curr->right = addNode; }

        set_nil(t);
        find_root(t);
        fixRootColor(t);

        if (curr->color == RBTREE_RED) {
            fixDoubleRed(t, curr->parent, curr, addNode);
        }
    }
    return addNode;
}

void fixRootColor(rbtree *t) {
    if (t->root->color != RBTREE_BLACK) {
        t->root->color = RBTREE_BLACK;
    }
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    // TODO: implement find
    node_t *curr = t->root;
    while (curr != t->nil) {
        if (key == curr->key && curr != t->nil) {
            return curr;
        } else if (key < curr->key) {
            curr =  curr->left;
        } else {
            curr = curr->right;
        }
    }
    return NULL;
}

node_t *rbtree_min(const rbtree *t) {
    // TODO: implement find
    node_t *prev = NULL;
    node_t *curr = t->root;
    while (curr != t->nil) {
        prev = curr;
        curr = curr ->left;
    }
    return prev;
}

node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    node_t *prev = NULL;
    node_t *curr = t->root;
    while (curr != t->nil) {
        prev = curr;
        curr = curr -> right;
    }
    return prev;
}

void fixExtraBlack(rbtree *t, node_t *parent_node, node_t *replace_node) {

    if (replace_node == t->root) {
        replace_node -> color = RBTREE_BLACK;
        return;
    }

    // red and black
    if (replace_node -> color == RBTREE_RED) {
        replace_node -> color = RBTREE_BLACK;
        return;
    }

    // doubly black
    node_t *sibling_node, *close_child, *far_child;
    if (replace_node == parent_node -> left){
        sibling_node = parent_node->right;
        close_child = sibling_node -> left;
        far_child = sibling_node -> right;
    }
    else {
        sibling_node = parent_node->left;
        close_child = sibling_node -> right;
        far_child = sibling_node -> left;
    }

    // case1: 형제가 레드(= 형제의 자식이 black일 수 없음) -> 형제를 black으로 만들어줘야함. -> 그리고 아래 케이스로 내려감.
    if (sibling_node -> color == RBTREE_RED) {
        rotate(t, parent_node->parent, parent_node, sibling_node);
        return fixExtraBlack(t, parent_node, replace_node);
    }

    // sibling은 black
    // case2: 자식들도 다 black -> 사용할 red 없으니까 black을 위로 올려보냄.
    if (close_child -> color == RBTREE_BLACK && far_child -> color == RBTREE_BLACK){
        sibling_node -> color = RBTREE_RED;
        return fixExtraBlack(t, parent_node -> parent, parent_node);
    }

    // case 3: close child 가 red -> far child를 red로 위치 이동
    if (close_child -> color == RBTREE_RED && far_child -> color == RBTREE_BLACK) {
        rotate(t, parent_node, sibling_node, close_child);
        return fixExtraBlack(t, parent_node, replace_node);
    }

    // case4:
    if (far_child -> color == RBTREE_RED) {
        far_child -> color = RBTREE_BLACK;
        rotate(t, parent_node -> parent, parent_node, sibling_node);
    }

    set_nil(t);
    find_root(t);
    fixRootColor(t);

}


int rbtree_erase(rbtree *t, node_t *p) {
    // 소멸 노드 찾기
    node_t *disappear_node = p;
    node_t *replace_node;

    // 자식이 2개면 disappear_node가 p가 아닌 다른 노드로 바뀜
    if (disappear_node -> left != t->nil && disappear_node -> right != t->nil) {
        node_t *curr = disappear_node->right;
        while (curr -> left != t->nil) {
            curr = curr -> left;
        }
        disappear_node = curr;
        p -> key = disappear_node -> key;
    }

    // 자식의 0개
    if (disappear_node -> left == t->nil && disappear_node -> right == t->nil) {
        replace_node = t -> nil;
    }
    // 자식이 1개
    else if (disappear_node -> left != t->nil) {
        replace_node = disappear_node -> left;
    }
    else if (disappear_node -> right != t->nil) {
        replace_node = disappear_node -> right;
    }

    // 삭제
    // disapper node의 자식의 처리는 따로해 줄 필요 없음 -> 하나 남은 자식이 대체자거나, 아니면 자식이 없으니까.
    // 부모
    node_t *parent_node = disappear_node -> parent;
    if (replace_node != t-> nil) {replace_node -> parent = parent_node;}

    if (parent_node == t-> nil) { // 진짜로 삭제되는게 root면
        t->root = replace_node;
        t->root->parent = t->nil;
    } else {
        if (parent_node -> left == disappear_node) {
            parent_node -> left = replace_node;
        } else if (parent_node -> right == disappear_node) {
            parent_node -> right = replace_node;
        }
    }

    if (disappear_node -> color == RBTREE_BLACK) {
        fixExtraBlack(t, parent_node, replace_node);
    }

    set_nil(t);
    find_root(t);
    fixRootColor(t);
    free(disappear_node);
    return 0;
}

void inorder_tree(const rbtree *t, key_t *arr, node_t* node, int *idx, const size_t n) {
    if (node == t->nil) {return;}
    inorder_tree(t, arr, node->left,idx, n);
    if (*idx < n) {
        arr[*idx] = node->key;
        (*idx)++;
    }
    inorder_tree(t, arr, node -> right,idx,n);

}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    int idx = 0;
    inorder_tree(t, arr, t -> root, &idx, n);
    return 0;
}
