//
//  LinkTable.h
//  BTreeExt
//
//  Created by HeyOnly on 30/05/2017.
//  Copyright © 2017 HeyOnly. All rights reserved.
//

#ifndef LinkTable_h
#define LinkTable_h

#include <stdio.h>
typedef struct _LinkNode {
    int flags;
    struct _LinkNode *next;
}LinkNode;

LinkNode* initLinkNode(int flags);
int insertNode(LinkNode** pNode,int flags,int position);
int addNodeToEnd(LinkNode** pNode,int flags);

LinkNode* searchNode(LinkNode* pNode,int flags);
int deleteNode(LinkNode** pNode,int flags);
int destroyLinkNode(LinkNode** pNode);
void printNode(LinkNode* pNode);
#endif /* LinkTable_h */
