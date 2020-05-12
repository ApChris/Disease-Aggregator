#ifndef __LIST_H__
#define __LIST_H__

#define FLAG 1400322

typedef struct PathNode
{
    char * path;
    struct PathNode * next;
}PathNode;


typedef struct Node
{
    long item;
    struct Node * next;
}Node;


void PushNode(Node ** head, long data);
bool SearchInList(Node ** head, long data);
long LenOfList(void * head);
void DeleteList(Node ** head);
void DeleteNode(Node ** head, long data);
void PrintList(Node ** head);

void PushNode_Path(PathNode ** head, char * path);
bool SearchInList_Path(PathNode ** head, char * path);
void DeleteList_Path(PathNode ** head);
void DeleteNode_Path(PathNode ** head, char * path);
void PrintList_Path(PathNode ** head);
char * GetValue_Path(PathNode ** head, long i);
#endif
