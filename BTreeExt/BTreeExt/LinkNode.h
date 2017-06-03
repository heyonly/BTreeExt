//
//  LinkTable.h
//  BTreeExt
//
//  Created by HeyOnly on 30/05/2017.
//  Copyright © 2017 HeyOnly. All rights reserved.
//

#ifndef LinkNode_h
#define LinkNode_h

#include <stdio.h>
typedef struct _LinkNode {
    char* name;
    struct _LinkNode *next;
}LinkNode;

LinkNode* initLinkNode(const char* name);
int addNodeToHead(LinkNode** pNode,const char* name);

LinkNode* searchNode(LinkNode* pNode,int flags);
int deleteNode(LinkNode** pNode,int flags);
int destroyLinkNode(LinkNode** pNode);
void printNode(LinkNode* pNode);
#endif /* LinkTable_h */
