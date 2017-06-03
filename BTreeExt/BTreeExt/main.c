//
//  main.c
//  BTreeExt
//
//  Created by HeyOnly on 28/04/2017.
//  Copyright © 2017 HeyOnly. All rights reserved.
//

#include <stdio.h>
#include "BTreeExt.h"
#include "LinkNode.h"
#include <stdlib.h>

#define Directory "/Users/heyonly/test"

int main(int argc, const char * argv[]) {
    TOptorBWItem initItem = {0,Directory};
    TOptorBWItem item[11] = {
        {1,"/Users/heyonly/test/test/test.tmp"},
        {2,"/Users/heyonly/test/test1/test.aaa"},
        {2,"/Users/heyonly/test/test1/test/test.aaa"},
        {2,"/Users/heyonly/test/test1/test/aaa/test.aaa"},
        {2,"/Users/heyonly/test/test1/ccc/aaa/test.aaa"},
        {2,"/Users/heyonly/test/test1/ccc/ddd/test.aaa"},
        {2,"/Users/heyonly/test/test1/ccc/eee/test.aaa"},
        {2,"/Users/heyonly/test/test1/ccc/fff/test.aaa"},
        {2,"/Users/heyonly/test/test1/ccc/aaa/test.aaa"},
        {2,"/Users/heyonly/test/test1/ccc/ccc/test.aaa"},
        {2,"/Users/heyonly/test/test1/ccc/gggg/test.aaa"},
    };
    BTNodeExt *treeExt = BTreeExt_Init(initItem.name,0);
    printf("%lu\n",sizeof(item)/sizeof(item[0]));
    for (int i = 0; i < sizeof(item)/sizeof(item[0]); i++) {
        BTreeExt_Insert(&treeExt, &item[i]);
    }
//    BTNodeExt *tree = BTreeExt_Search(treeExt, "/Users/heyonly/test/test1");
//    BTreeExt_Destroy(&tree);
    
    

    printBTree(treeExt);
    
        LinkNode* node = initLinkNode("aaa");
        for (int i = 1; i < 10; i++) {
            char* name = malloc(sizeof(char) * 16);
            memset(name,0,16);
            sprintf(name, "aaa_ %d",i);
            addNodeToHead(&node, name);
            
        }
        printNode(node);
        destroyLinkNode(&node);
        printNode(node);
    
    return 0;
}
