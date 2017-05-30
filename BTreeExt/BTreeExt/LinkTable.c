//
//  LinkTable.c
//  BTreeExt
//
//  Created by HeyOnly on 30/05/2017.
//  Copyright © 2017 HeyOnly. All rights reserved.
//

#include "LinkTable.h"
#include <stdlib.h>
#include <assert.h>

LinkNode* initLinkNode(int flags) {
    LinkNode* node = malloc(sizeof(LinkNode));
    void* ret = memset(node,0,(unsigned long)sizeof(LinkNode));
    assert(ret);
    node->next = NULL;
    node->flags = flags;
    return node;
}

int addNodeToEnd(LinkNode** pNode,int flags) {
    if (NULL == pNode || NULL == *pNode) {
        return -1;
    }
    LinkNode* newNode = initLinkNode(flags);
    LinkNode* node = *pNode;
    if (NULL == node->next) {
        node->next = newNode;
        return 0;
    }
    while (node->next) {
        node = node->next;
    }
    node->next = newNode;
    return 0;
}

int insertNode(LinkNode** pNode,int flags,int position) {
    if (NULL == pNode || NULL == *pNode) {
        return -1;
    }
    int pos = 0;
    LinkNode *newNode = initLinkNode(flags);
    LinkNode* node = *pNode;
    while (node) {
        if (pos == position) {
            newNode->next = node->next;
            node->next = newNode;
            return 0;
        }
        pos++;
        node = node->next;
    }
    return -1;
}
LinkNode* searchNode(LinkNode* pNode,int flags);
int deleteNode(LinkNode** pNode,int flags);
int destroyLinkNode(LinkNode** pNode) {
    LinkNode* node = (*pNode)->next;
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
        printf("===  %d\n",node->flags);
        node = node->next;
        
    }
}
