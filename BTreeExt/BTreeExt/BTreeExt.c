//
//  BTreeExt.c
//  filetest
//
//  Created by HeyOnly on 22/04/2017.
//  Copyright © 2017 haibing. All rights reserved.
//

#include "BTreeExt.h"
#include <stdlib.h>
#include <string.h>
 #include <assert.h>
#include "LinkNode.h"

static inline char* CopyStr(const char* str){
    char* tPath = malloc(sizeof(char) * strlen(str) + 1);
    memset(tPath, 0, strlen(str) + 1);
    strcpy(tPath, str);
    return tPath;
}
static const char *separatorGetSuffixPath(const char *path);
static void separatorGetPrePath(const char *path,char **sPath);
static const char* separatorSubPath(const char* path,char** sPath);

static BTNodeExt* BTreeExt_Create(const TOptorBWItem* item);
static int BTreeExt_Insert_P(BTNodeExt** pNode,const char* parrentPath,const char* path,unsigned int flags);
static BTNodeExt* BTreeExt_Search_P(BTNodeExt* tree,const char* parrentPath,const char* path);
static BTNodeExt* BTreeExtLeafNode_Search(BTNodeExt* pNode,const char* path);
static BTNodeExt* BTreeExtLeafNode_Insert(BTNodeExt** pNode,TOptorBWItem* item);

static int BTreeExt_Destroy_P(BTNodeExt **tree);

static int BTreeExtLeafNode_Destroy(LeafNode** pNode);
static int stictingPath(char* path,BTNodeExt* pNode);


static const char* rootPath = NULL;
BTNodeExt* BTreeExt_Init(const char* root,unsigned int flags) {
    rootPath = malloc(sizeof(char) * strlen(root));
    memset((void*)rootPath, 0, strlen(root) + 1);
    strcpy((char*)rootPath, root);
    TOptorBWItem item = {flags,'\0'};
    memset(item.name, 0, sizeof(item.name));
    strcpy(item.name, root);
    return BTreeExt_Create(&item);
}
static BTNodeExt* BTreeExt_Create(const TOptorBWItem* item) {
    BTNodeExt* node = malloc(sizeof(BTNodeExt));
    memset(node, 0, sizeof(BTNodeExt));
    node->item = malloc(sizeof(TOptorBWItem));
    memset(node->item, 0, sizeof(TOptorBWItem));
    memset(node->item->name, 0, sizeof(node->item->name));
    strcpy(node->item->name, item->name);
    node->item->flags = item->flags;
    node->childNode = NULL;
    node->parrent = node;
    node->next = NULL;
    node->pre = NULL;
    return  node;
}
int BTreeExt_Insert(BTNodeExt** tree,const TOptorBWItem* item) {
    if (NULL == tree || NULL == *tree) {
        return -1;
    }
    if (strlen(item->name) < strlen(rootPath)) {
        return -1;
    }
    char* tPath = CopyStr(item->name);
    if (0 == memcmp(tPath, rootPath, strlen(rootPath))) {
        tPath = tPath + strlen(rootPath);
    }
    
    int ret = BTreeExt_Insert_P(tree,rootPath,tPath,item->flags);
    return ret;
}
static int BTreeExt_Insert_P(BTNodeExt** pNode,const char* parrentPath,const char* path,unsigned int flags) {
    if (NULL == pNode || NULL == *pNode) {
        return -1;
    }
    //父目录
    char* pPath = CopyStr(parrentPath);
    //sPath 当前目录
    char *sPath = NULL;
    //subPath 次一级目录
    const char *subPath = separatorSubPath(path, &sPath);
    BTNodeExt* parrent = *pNode;
    BTNodeExt* node = NULL;
    while (subPath) {
        node = BTreeExt_Search_P(parrent, pPath, sPath);
        if (NULL == node) {
            break;
        }
        memset(pPath, 0, strlen(sPath));
        pPath = CopyStr(sPath);
        memset(sPath, 0, strlen(sPath));
        subPath = separatorSubPath(subPath,&sPath);
        parrent = node;
        
    }
    if (NULL == sPath) {
        return -1;
    }
    TOptorBWItem item = {flags,""};
    strcpy(item.name, sPath);
    
    BTNodeExt* tNode = BTreeExtLeafNode_Insert(&(parrent->childNode), &item);
    tNode->parrent = parrent;
    
    return BTreeExt_Insert_P(&tNode, sPath, subPath, flags);
}
BTNodeExt* BTreeExt_Search(BTNodeExt* tree,const char *path) {
    if (NULL == tree) {
        return NULL;
    }
    BTNodeExt* node = tree;
    if (0 == strcmp(rootPath, path)) {
        return node;
    }
    if (0 == strncmp(path, rootPath, strlen(rootPath))) {
        node = BTreeExt_Search_P(tree, rootPath, path);
        path = path + strlen(rootPath);
    }
    char* parrentPath = CopyStr(rootPath);
    char* sPath = NULL;
    const char* subPath;
    //path = "/User/heyOnly/test" subPath = heyOnly/test,sPath = User
    subPath = separatorSubPath(path, &sPath);
    while (sPath) {
        node = BTreeExt_Search_P(node,parrentPath,sPath);
        memset(parrentPath, 0, strlen(parrentPath));
        strcpy(parrentPath, sPath);
        subPath = separatorSubPath(subPath, &sPath);
    }
    if (parrentPath != NULL) {
        free((void*)parrentPath);
        parrentPath = NULL;
    }
    return node;
}
static BTNodeExt* BTreeExt_Search_P(BTNodeExt* tree,const char* parrentPath,const char* path) {
    if (NULL == tree) {
        return NULL;
    }
    if (strlen(parrentPath) != strlen(tree->item->name)) {
        return NULL;
    }
    if (0 == strncmp(path, rootPath,strlen(rootPath))) {
        return tree;
    }
    if (0 == memcmp(parrentPath, tree->item->name, strlen(parrentPath))) {
        BTNodeExt* childNode = BTreeExtLeafNode_Search(tree->childNode, path);
        return childNode;
    }
    return NULL;
}
//链表查找
static BTNodeExt* BTreeExtLeafNode_Search(BTNodeExt* pNode,const char* path) {
    if (NULL == pNode) {
        return NULL;
    }
    BTNodeExt* node = pNode;
    while (node) {
        if (strlen(path) != strlen(node->item->name)) {
            node = node->next;
            continue;
        }
        if (0 == memcmp(path, node->item->name, strlen(path))) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}
//链表插入
static BTNodeExt* BTreeExtLeafNode_Insert(BTNodeExt** pNode,TOptorBWItem* item) {
    if (NULL == pNode) {
        pNode = malloc(sizeof(BTNodeExt));
        memset(pNode, 0, sizeof(BTNodeExt));
        
    }
    if (NULL == *pNode) {
//        memset(*pNode, 0, sizeof(BTNodeExt));
        *pNode = BTreeExt_Create(item);
        return *pNode;
    }
    BTNodeExt* node = *pNode;
    while (node->next) {
        node = node->next;
    }
    node->next = BTreeExt_Create(item);
    node->next->pre = node;
    node->next->parrent = node->parrent;
    return node->next;
}
int BTreeExt_Destroy(BTNodeExt **tree){
    if (NULL == tree || NULL == *tree) {
        return -1;
    }
    BTreeExt_Destroy_P(&(*tree)->childNode);
    if (NULL == (*tree)->pre) {
        (*tree)->parrent->childNode = (*tree)->next;
        free((*tree));
        (*tree) = NULL;
        return 0;
    }
    (*tree)->pre->next = (*tree)->next;
    if (NULL != (*tree)->next) {
        (*tree)->next->pre = (*tree)->pre;
    }
    
    free((*tree));
    *tree = NULL;
    return 0;
}

static int BTreeExt_Destroy_P(BTNodeExt **tree) {
    if (NULL == tree || NULL == *tree) {
        return -1;
    }
//    (*tree)->parrent->childNode = (*tree)->next;
    BTreeExt_Destroy_P(&((*tree)->childNode));
    BTNodeExt* pNode = *tree;


    while (pNode) {
        pNode = pNode->next;
        free((void*)pNode);
        pNode = NULL;
    }
    free((*tree));
    *tree = NULL;
    
    return 0;
}

void printBTree(BTNodeExt* tree) {
    if (NULL == tree) {
        return;
    }
    
    BTNodeExt *hNode = tree;
    while (hNode) {
        char* tPath = malloc(sizeof(char) * 1024);
        stictingPath(tPath, hNode);
        printf("%s\n",tPath);
        printBTree(hNode->childNode);
        hNode = hNode->next;
        free(tPath);
        tPath = NULL;
    }

    
   
}

static int stictingPath(char* path,BTNodeExt* pNode) {
    if (NULL == pNode) {
        return -1;
    }
    if (pNode == pNode->parrent) {
        strcat(path, pNode->item->name);
        return 0;
    }
    LinkNode* linkNode = initLinkNode(pNode->item->name);
    
    BTNodeExt *node = pNode->parrent;
    while (node) {
        
        char *tPath = malloc(sizeof(char)* strlen(node->item->name) + 2);
        memset((void*)tPath, 0, strlen(node->item->name) + 2);
        strcat(tPath, node->item->name);
        strcat(tPath, "/");
        addNodeToHead(&linkNode, tPath);
        
        free(tPath);
        tPath = NULL;
        if (node == node->parrent) {
            break;
        }
        
        node = node->parrent;
    }
    memset(path, 0, strlen(path));
    LinkNode *tNode = linkNode;
    while (tNode) {
        strcat(path, tNode->name);
        tNode = tNode->next;
    }
    return 0;
}
static int BTreeExtLeafNode_Destroy(LeafNode** pNode) {
    if (NULL == pNode || NULL == *pNode) {
        return 0;
    }
    LeafNode* node = (*pNode)->next;
    free((*pNode));
    *pNode = NULL;
    while (node) {
        node = node->next;
        free(node);
        node = NULL;
    }
    
    return 0;
}
//获取路径前边
static void separatorGetPrePath(const char *path,char **sPath) {
    
    if (*sPath != NULL) {
        memset(*sPath, 0, strlen(*sPath));
        free(*sPath);
        *sPath = NULL;
    }
    if (path == NULL || 0 == strcmp("", path)) {
        return;
    }
    if (*path == '/') {
        path++;
    }
    int i = 0;
    char c = '\0';
    for (i = 0; i < strlen(path); i++) {
        c = *(path+i);
        if (c == '/') {
            break;
        }
    }
    *sPath = malloc(sizeof(char) * i+1);
    memset(*sPath, 0, sizeof(char) * i+1);
    strncpy(*sPath, path, i);
}
//返回路径后边
static const char *separatorGetSuffixPath(const char *path) {
    if (NULL == path) {
        return NULL;
    }
    if (*path == '/') {
        path++;
    }
    while (*path != '\0') {
        if (*path == '/') {
            break;
        }
        path++;
    }
    if (*path == '\0') {
        return NULL;
    }
    return path + 1;
}
static const char* separatorSubPath(const char* path,char** sPath) {
    separatorGetPrePath(path, sPath);
    return separatorGetSuffixPath(path);
}



