 //
//  main.c
//  jagu
//
//  Created by Seung Hyun Kong on 6/6/17.
//  Copyright © 2017 Seung Hyun Kong. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

typedef struct rbNode* NodePtr;
struct rbNode{
    int key;
    int red;
    NodePtr left,
            right,
            parent;
};

NodePtr nil;

typedef struct rbtree* TreePtr;
struct rbtree {
    NodePtr nil, root;
    
};

NodePtr nilinit(){
    nil = (NodePtr)malloc(sizeof(struct rbNode));
    nil->key = -1;
    nil->red = 0;
    nil->left = nil->right = NULL;

    return nil;
}

NodePtr nodeinit(int key){
    NodePtr node = (NodePtr)malloc(sizeof(struct rbNode));
    node->key = key;
    node->left = nil;
    node->right = nil;
    node->parent = nil;
    //if (key == -1){
      //  node->red = 0;
    //}
    return node;
};

TreePtr treeinit(){
    TreePtr tree = (TreePtr)malloc(sizeof(struct rbtree));
    tree->root = nil;
    tree->nil = nil;
    return tree;
}

NodePtr nodemin(NodePtr node, NodePtr nil){
    while (node->left != nil){
        node = node->left;
    }
    return node;
}

NodePtr treemin(TreePtr tree){
    NodePtr node = tree->root;
    NodePtr nil = tree->nil;
    if (node == nil){
        return nil;
    } else {
        return nodemin(node, nil);
    }
}

void left_Rot(TreePtr t, NodePtr x){
    NodePtr y = x->right;
    x->right = y->left;
    if (y->left != t->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->nil) {
        t->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void right_Rot(TreePtr t, NodePtr y){
    NodePtr x = y->left;
    y->left = x->right;
    if (x->right != t->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == t->nil) {
        t->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

void InsertFix(TreePtr t, NodePtr z){
    NodePtr y;
    while (z->parent->red) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y != t->nil && y->red){
                z->parent->red = 0;
                y->red = 0;
                z->parent->parent->red = 1;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_Rot(t, z);
                }
                z->parent->red = 0;
                z->parent->parent->red = 1;
                right_Rot(t, z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y != t->nil && y->red) {
                z->parent->red = 0;
                y->red = 0;
                z->parent->parent->red = 1;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_Rot(t, z);
                }
                z->parent->red = 0;
                z->parent->parent->red = 1;
                left_Rot(t, z->parent->parent);
            }
        }
    }
    t->root->red = 0;
}

void insert(TreePtr t, NodePtr z){
    NodePtr y = t->nil;
    NodePtr x = t->root;
    
    if (x == NULL){
        z->parent = nil;
        z->left = t->nil;
        z->right = t->nil;
        z->red = 0;
        t->root = z;
        return;
    }
    
    while (x != t->nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == t->nil) {
        t->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    //z->left  = t->nil;
    //z->right = t->nil;
    z->red = 1;
    InsertFix(t, z);
}


char choosecol(int red){
    char    r[] = "red",
            b[] = "black";
    if (red){
        return *r;
    } else {
        return *b;
    }
}

NodePtr search(TreePtr t, NodePtr node, int val){
    if (node==t->nil || node->key == val){
        return node;
    }
    if (node->key < val){
        return search(t, node->right, val);
    } else {
        return search(t, node->left, val);
    }
}

void transplant(TreePtr t, NodePtr u, NodePtr v){
    if (u->parent == nil) {
        t->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void deletefix(TreePtr t, NodePtr x) {
    NodePtr w;
    while (x != t->root && x->red == 0) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->red) {
                w->red = 0;
                x->parent->red = 1;
                left_Rot(t, x->parent);
                w = x->parent->right;
            }
            if (w->left->red == 0 && w->right->red == 0) {
                w->red = 1;
                x = x->parent;
            } else {
                if (w->right->red == 0) {
                    w->left->red = 0;
                    w->red = 1;
                    right_Rot(t, w);
                    w = x->parent->right;
                }
            
                w->red = x->parent->red;
                x->parent->red = 0;
                w->right->red = 0;
                left_Rot(t, x->parent);
                x = t->root;
            }
        } else {
            w = x->parent->left;
            if (w->red) {
                w->red = 0;
                x->parent->red = 1;
                right_Rot(t, x->parent);
                w = x->parent->left;
            }
            if (w->right->red == 0 && w->left->red == 0) {
                w->red = 1;
                x = x->parent;
            } else {
                if (w->left->red == 0) {
                    w->right->red = 0;
                    w->red = 1;
                    left_Rot(t, w);
                    w = x->parent->left;
                }
                w->red = x->parent->red;
                x->parent->red = 0;
                w->left->red = 0;
                right_Rot(t, x->parent);
                x = t->root;
            }
        }
    }
    x->red = 0;
}

void deletenode(TreePtr t, NodePtr z){
    NodePtr y = z;
    NodePtr x;
    if (z == t->nil) {
        return;
    }
    int y_original = y->red;
    if (z->left == t->nil) {
        x = z->right;
        transplant(t, z, z->right);
    } else if (z->right == t->nil) {
        x = z->left;
        transplant(t, z, z->left);
    } else {
        y = nodemin(z->right, t->nil);
        y_original = y->red;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->red = z->red;
    }
    if (y_original == 0) {
        deletefix(t, x);
    }
    
}

//int height = 0;
void rbt_print(TreePtr t, NodePtr node, int level) {
    if (node->right != NULL){
        //height++;
        rbt_print(t, node->right, level + 1);
    }
    for(int i = 0; i < level; i++)
        printf("    ");
    printf("%d-%s\n", node->key, (node->red ? "r":"b"));
    if (node->left != NULL){
        //height++;
        rbt_print(t, node->left, level + 1);
    }
}

int treeheight(TreePtr t){
    NodePtr x = t->root;
    int height = 0;
    while (x != t->nil) {
        x = x->left;
        if (x->red == 0) {
            height++;
        }
    }
    return height;
}

int nodecount(TreePtr t, NodePtr node) {
    int i = 0;
    if (node == nil)
        return 0;
    else {
        
        i += nodecount(t, node->left);
        i++;
        i += nodecount(t, node->right);
    }
    return i;
}

int isleaf(TreePtr tree, NodePtr node){
    if (node == nil){
        return 0;
    } else if (node->left == tree->nil){
        return 0;
    } else if (node->right == tree->nil){
        return 0;
    } else {
        return 1;
    }
}

int blackcount(TreePtr t, NodePtr node){
    //int i = 0;
    if (node == nil) {
        return 0;
    } else {
        if (node->red == 0) {
            return blackcount(t, node->left) + blackcount(t, node->right);
        } else {
            return blackcount(t, node->left) + blackcount(t, node->right) + 1;
        }
    }
    
}

void inordertrav(TreePtr tree, NodePtr node){
    if (node == tree->nil){
        return;
    } else {
        inordertrav(tree, node->left);
        printf("%3d - %s\n", node->key, node->red ? "r" : "b");
        inordertrav(tree, node->right);
    }
    
}

int main(void){
    //nil = nodeinit(-1);
    nil = nilinit();
    TreePtr t = treeinit();
    FILE *fp;
    //DIR *dp;
    /*struct dirent *ep = NULL;
    dp = opendir("./input/");
    if (dp != NULL) {
        while (ep = readdir(dp)) {
            puts(ep->d_name);
        }
        (void) closedir(dp);
        
    } else {
        perror("Couldn't open Directory\n");
        return 0;
    }*/
    //int i = 0;
    int data, in = 0, del = 0, miss = 0;
    fp = fopen("/Users/seunghkong/desktop/input.txt", "r");
    while(fscanf(fp, "%d", &data)){
        if (data == 0){
            break;
        } else if (data > 0){
            //printf("inserting %d\n",data);
            insert(t, nodeinit(data));
            in++;
            //rbt_print(t, t->root, 0);
        } else if (data < 0){
            //printf("%d is negative\n", data);
            if (search(t, t->root, abs(data)) != t->nil){
                //printf("is in tree %d\n", search(t, t->root, abs(data))->key);
                deletenode(t, search(t, t->root, abs(data)));
                del++;
                //printf("%d is deleted\n", data);
                //rbt_print(t, t->root, 0);
            } else {
                miss++;
                //printf("absolute value of %4d is not already in the rb Tree. Ignoring...\n", data);
            }
        }
    }
    fclose(fp);
    //rbt_print(t, t->root, 0);
    printf("total = %d\n", nodecount(t, t->root));  //all node count
    printf("insert = %d\n", in);                    //number of inserts
    printf("delete = %d\n", del);                   //number of deletes
    printf("miss = %d\n", miss);                    //number of misses
    printf("nb = %d\n", blackcount(t, t->root));    //number of black nodes
    printf("bh = %d\n", treeheight(t));             //black node height
    //rbt_print(t, t->root, 0);
    inordertrav(t, t->root);
    NodePtr x = t->root;
    while (x != t->nil){
        printf("%d %d\n", x->key, x->red);
       x = x->left;
    }
    return 0;
}
