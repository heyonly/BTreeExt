//
//  LinkTable.c
//  BTreeExt
//
//  Created by HeyOnly on 30/05/2017.
//  Copyright © 2017 HeyOnly. All rights reserved.
//

#include "LinkNode.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

LinkNode* initLinkNode(const char* name) {
    LinkNode* node = malloc(sizeof(LinkNode));
    void* ret = memset(node,0,(unsigned long)sizeof(LinkNode));
    node->name = malloc(sizeof(char) * (strlen(name) + 1));
    memset((void*)node->name, 0, strlen(name) + 1);
    assert(ret);
    node->next = NULL;
    strcpy(node->name, name);
    return node;
}

int addNodeToHead(LinkNode** pNode,const char* name) {
    if (NULL == pNode || NULL == *pNode) {
        return -1;
    }
    LinkNode* newNode = initLinkNode(name);
    newNode->next = *pNode;
    *pNode = newNode;
    return 0;
}

LinkNode* searchNode(LinkNode* pNode,int flags);
int deleteNode(LinkNode** pNode,int flags);
int destroyLinkNode(LinkNode** pNode) {
    LinkNode* node = (*pNode)->next;
    free((void*)node->name);
    node->name = NULL;
    free((*pNode));
    *pNode = NULL;
    while (node) {
        node = node->next;
        free(node);
        node = NULL;
    }
    return 0;
}
void printNode(LinkNode* pNode) {
    LinkNode *node = pNode;
    
    while (node) {
        printf("===  %s\n",node->name);
        node = node->next;
        
    }
}

