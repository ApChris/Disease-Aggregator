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




void PushNode(Node ** head, long item);
bool SearchInList(Node ** head, long item);
long LenOfList(void * head);
void DeleteList(Node ** head);
void DeleteNode(Node ** head, long item);
void PrintList(Node ** head);
void Reverse(Node ** head);
long GetValue(Node ** head, long i);

void PushNode_Path(PathNode ** head, char * path);
bool SearchInList_Path(PathNode ** head, char * path);
void DeleteList_Path(PathNode ** head);
void DeleteNode_Path(PathNode ** head, char * path);
void PrintList_Path(PathNode ** head);
char * GetValue_Path(PathNode ** head, long i);
void Reverse_Path(PathNode ** head);
#endif
