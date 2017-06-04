前段时间，同事问怎么对目录中的文件快速过滤，想来想去，Linux的目录树是个不错的选择。但是同事想让我给他个demo，所以这个额文件目录搜索树就诞生了。还有很多不完善之处，就放上来，希望能有大牛给改一改。



这是模仿文件系统多分支快速搜索树，对文件系统层级搜索
![image](BTreeExtPictire.png)

如图：



最上边是根目录，每一层平行节点之间是一个双向链表，当然，这个链表并没有进行任何排序。节点的定义如下：
```
typedef struct {
    unsigned int flags; 
    char name[256];
} TOptorBWItem;
```
```
typedef struct BTNodeExt {
    TOptorBWItem* item;
    struct BTNodeExt* parrent;
    struct BTNodeExt* childNode;
    struct BTNodeExt* next;
    struct BTNodeExt* pre;
}BTNodeExt;
```


item 中存储了目录/文件

parrent 指向父目录

childNote 指向第一个子节点

next 平行目录中下一个节点

pre 平行目录中上一个节点

目录中主要是查找遍历，查找递归查找


由于在每个节点中只存储了每个文件名。因此在遍历中采用递归算法，使用另一个链表来存储每个节点名存储到链表中，最后遍历链表将整个链表文件名拼接起来，输出。


最后，直接看代码吧

:[https://github.com/heyonly/BTreeExt](https://github.com/heyonly/BTreeExt)





