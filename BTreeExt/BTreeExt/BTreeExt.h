//
//  BTreeExtExt.h
//  filetest
//
//  Created by HeyOnly on 22/04/2017.
//  Copyright © 2017 haibing. All rights reserved.
//

#ifndef BTreeExtExt_h
#define BTreeExtExt_h
#include <stdio.h>
typedef struct {
    unsigned int flags; // 0:other  1:sdcard 2: data/data/
    char name[256];
} TOptorBWItem;

typedef struct LeafNode {
    unsigned int flags;
    char key[256];
    struct LeafNode *next;
}LeafNode;

typedef struct BTNodeExt {
    TOptorBWItem* item;
    struct BTNodeExt* parrent;
    struct BTNodeExt* childNode;
    struct BTNodeExt* next;
    struct BTNodeExt* pre;
}BTNodeExt;
BTNodeExt* BTreeExt_Init(const char* root,unsigned int flags);
int BTreeExt_Insert(BTNodeExt** tree,const TOptorBWItem* item);

int BTreeExt_Destroy(BTNodeExt **tree);
void BTreeExt_Remove(BTNodeExt** tree,const char* path);
BTNodeExt* BTreeExt_Search(BTNodeExt* tree,const char *path);
void printBTree(BTNodeExt* tree);
#endif /* BTreeExtExt_h */
