#include <stdio.h>
#include <stdlib.h>

typedef struct rbNode* NodePtr;
struct rbNode{
    int key;
    int red;
    NodePtr left, right, parent;
};

typedef struct rbtree* TreePtr;
struct rbtree {
    NodePtr root, sentinel;
};

NodePtr nodeinit(int key){
    NodePtr node = (NodePtr)malloc(sizeof(struct rbNode));
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = key;
    return node;
};

TreePtr treeinit(){
    TreePtr self = (TreePtr)malloc(sizeof(struct rbtree));
    //TreePtr tree = NULL;
    //NodePtr node = nodeinit();
    //node->red = 0;
    self->root = NULL;
    self->sentinel = NULL;
    return self;
}

NodePtr rbmin(NodePtr node, NodePtr x){
    while (node->left != x){
        node = node->left;
    }
    return x;
}

NodePtr rbmax(NodePtr node, NodePtr x){
    while (node->right != x){
        node = node->left;
    }
    return node;
}

NodePtr nodemin(NodePtr node, NodePtr sentinel){
    while (node->left != sentinel){
        node = node->left;
    }
    return node;
}

NodePtr treemin(TreePtr tree){
    NodePtr node = tree->root;
    NodePtr sentinel = tree->sentinel;
    if (node == sentinel){
        return NULL;
    } else {
        return nodemin(node, sentinel);
    }
}

void left_Rot(TreePtr root, NodePtr x, NodePtr node){
    NodePtr right = node->right;
    node->right = right->left;
    if(right->left != x){
        right->left->parent = node;
    }
    right->parent = node->parent;
    if (node == root->root){
        root->root = right;
    } else if ( node == node->parent->left){
        node->parent->left = right;
    } else {
        node->parent->right = right;
    }
    right->left = node;
    node->parent = right;
    return;
}

void right_Rot(TreePtr root, NodePtr x, NodePtr node){
    NodePtr left = node->left;
    node->left = left->right;
    
    if(left->right != x){
        left->right->parent = node;
    }
    
    left->parent = node->parent;
    if(node == root->root){
        root->root = left;
    } else if (node == node->parent->right){
        node->parent->right = left;
    } else {
        node->parent->left = left;
    }
    left->right = node;
    node->parent = left;
    return;
}

void InsertFix(TreePtr tree, NodePtr z){
    while (z->parent != NULL && z->parent->red == 1){
        if (z->parent == z->parent->parent->left) {
            NodePtr y = z->parent->parent->right;
            if (y->red){
                z->parent->red = 0;
                y->red = 0;
                z->parent->parent->red = 1;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right){
                    z = z->parent;
                    left_Rot(tree, tree->sentinel, z);
                }
                z->parent->red = 0;
                z->parent->parent->red = 1;
                right_Rot(tree, tree->sentinel, z->parent->parent);
            }
        } else {
            NodePtr y = z->parent->parent->left;
            if (y->red){
                z->parent->red = 0;
                y->red = 0;
                z->parent->parent->red = 1;
                z = z->parent->parent;
            } else {
                if(z== z->parent->left){
                    z = z->parent;
                    left_Rot(tree, tree->sentinel, z);
                }
                z->parent->red = 0;
                z->parent->parent->red = 1;
                right_Rot(tree, tree->sentinel, z->parent->parent);
            }
        }
    }
    tree->root->red = 0;
}

void insert(TreePtr tree, NodePtr z){
    NodePtr x = tree->root;
    NodePtr y = tree->sentinel;
    //NodePtr parent, samchon, *p;
    
    if (x == tree->sentinel){
        z->parent = NULL;
        z->left = tree->sentinel;
        z->right = tree->sentinel;
        z->red = 0;
        tree->root = z;
        return;
    }
    
    while (x != tree->sentinel){
        y = x;
        if (z->key < x->key){
            x = x->left;
        } else {
            x = x->right;
        }
    }
    if (y == tree->sentinel){
        tree->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = tree->sentinel;
    z->right = tree->sentinel;
    z->red = 1;
    InsertFix(tree, z);
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

void printtree(NodePtr root){
    NodePtr temp = root;
    if (temp != NULL){
        printtree(temp->left);
        printf("%d-%c ", temp->key, choosecol(temp->red));
        printtree(temp->right);
    }
    return;
}

NodePtr search(NodePtr root, int val){
    if (root==NULL || root->key == val){
        return root;
    }
    if (root->key < val){
        return search(root->right, val);
    } else {
        return search(root->left, val);
    }
}

void transplant(TreePtr self, NodePtr tree, NodePtr v){
    if (tree->parent == NULL)
        self->root= v;
    else if (tree==tree->parent->left)
        tree->parent->left=v;
    else
        tree->parent->right= v;
    if (v!=NULL)
        v->parent = tree->parent;
}

void deletefix(TreePtr tree, NodePtr x){
    NodePtr w;
    while (x != tree->root && x->red == 0){
        if (x == x->parent->left){
            w = x->parent->right;
            if (w->red){
                w->red = 0;
                x->parent->red = 1;
                left_Rot(tree, tree->sentinel, x->parent);
                w = x->parent->right;
            }
            if (w->left->red == 0 && w->right->red == 0){
                w->red = 1;
                x = x->parent;
            } else {
                if (w->right->red == 0){
                    w->left->red = 0;
                    w->red = 1;
                    right_Rot(tree, tree->sentinel, w);
                }
                w->red = x->parent->red;
                x->parent->red = 0;
                w->right->red = 0;
                left_Rot(tree, tree->sentinel, x->parent);
                x = tree->root;
            }
        } else {
            w = x->parent->left;
            if (w->red){
                w->red = 0;
                x->parent->red = 1;
                left_Rot(tree, tree->sentinel, x->parent);
                w = x->parent->left;
            }
            if (w->right->red == 0 && w->left->red == 0){
                w->red = 1;
                x = x->parent;
            } else {
                if (w->left->red == 0){
                    w->right->red = 0;
                    w->red = 1;
                    right_Rot(tree, tree->sentinel, w);
                }
                w->red = x->parent->red;
                x->parent->red = 0;
                w->left->red = 0;
                left_Rot(tree, tree->sentinel, x->parent);
                x = tree->root;
            }
        }
    }
    x->red = 0;
}

void deletenode(TreePtr tree, NodePtr z){
    
    NodePtr y = z;
    NodePtr x;
    int y_original_color = y->red;
    
    if (z == tree->sentinel){
        return;
    } else if (z->left == tree->sentinel){
        x = z->right;
        transplant(tree, z, z->right);
    } else if (z == tree->sentinel){
        return;
    } else if (z->right == tree->sentinel){
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = nodemin(z->right, tree->sentinel);
        y_original_color = y->red;
        x = y->right;
        if (y->parent == z){
            x->parent = y;
        } else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->red = z->red;
    }
    if (y_original_color == 0){
        deletefix(tree, x);
    }
    
}
//int height = 0;
void rbt_print(TreePtr self, NodePtr tree, int level) {
    if (tree->right != NULL){
        //height++;
        rbt_print(self,tree->right, level + 1);
    }
    for(int i = 0; i < level; i++)
        printf("    ");
    printf("%d\n", tree->key);
    if (tree->left != NULL){
        //height++;
        rbt_print(self, tree->left, level + 1);
    }
}

int treeheight(TreePtr tree, NodePtr node){
    if (node == tree->sentinel){
        return 0;
    } else {
        int ldepth = treeheight(tree, node->left);
        int rdepth = treeheight(tree, node->right);
        if (ldepth > rdepth){
            return (ldepth+1);
        } else {
            return (rdepth + 1);
        }
    }
}

int nodecount(TreePtr self, NodePtr tree) {
    int i = 0;
    if (tree == NULL)
        return 1;
    else {
        i += nodecount(self, tree->left);
        
        i += nodecount(self, tree->right);
    }
    return i;
}

int isleaf(TreePtr tree, NodePtr node){
    if (node == NULL){
        return 0;
    } else if (node->left == tree->sentinel){
        return 0;
    } else if (node->right == tree->sentinel){
        return 0;
    } else {
        return 1;
    }
}

int blackcount(TreePtr root, NodePtr tree){
    int i = 0;
    if (tree == NULL){
        return 0;
    } else {
        i = isleaf(root, tree) || tree->red==0 ?  1 : 0;
        return blackcount(root, tree->left) + blackcount(root, tree->right) + i;
    }
    
}

void inordertrav(TreePtr tree, NodePtr node){
    if (node == tree->sentinel){
        return;
    } else {
        inordertrav(tree, node->left);
        printf("%d\n", node->key);
        inordertrav(tree, node->right);
    }
    
}

int main(void){
    TreePtr t = treeinit();
    FILE *fp;
    //int i = 0;
    int data;
    fp = fopen("/Users/seunghkong/desktop/input.txt", "r");
    while(fscanf(fp, "%d", &data)){
        if (data == 0){
            break;
        } else if (data > 0){
            insert(t, nodeinit(data));
        } else if (data < 0){
            if (search(t->root, abs(data)) != NULL){
                deletenode(t, search(t->root, abs(data)));
            } else {
                printf("absolute value of %d is not already in the rb Tree. Ignoring...\n", data);
            }
        }
    }
    fclose(fp);
    //rbt_print(t, t->root, 0);
    printf("total = %d\n", nodecount(t, t->root));
    printf("nb = %d\n", blackcount(t, t->root));
    printf("bh = %d\n", treeheight(t, t->root));
    inordertrav(t, t->root);
    return 0;
}
