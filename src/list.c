#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/list.h"

void PushNode(Node ** head, long item)
{
    Node * node = (Node *)malloc(sizeof(Node));

    node -> item = item;

    // The next pointer of new node
    node -> next = (* head);

    // Set head to point to the new node
    (*head) = node;

}

bool SearchInList(Node ** head, long item)
{

    Node * tmp = *head;
    while(tmp != NULL)
    {
        if(tmp -> item == item)
        {
            return true;
        }

        tmp = tmp -> next;
    }
    return false;
}


long LenOfList(void * head)
{
    long counter = 0;
    // Set current pointer to points at head
    Node * current = head;

    // for every list node
    while(current != NULL)
    {
        counter++;
        current = current -> next;
    }
    return counter;
}

void DeleteList(Node ** head)
{
    Node * current = * head;
    Node * next;

    while(current != NULL)
    {
        // change head
        next = current -> next;
        free(current);
        current = next;
    }


    *head = NULL;
}

void DeleteNode(Node ** head, long item)
{
    Node * tmp = *head;
    Node * prev;


    if(tmp != NULL && tmp -> item == item)
    {
        // change head
        *head = tmp -> next;
        free(tmp);
        return;
    }

    // Search for the item to be deleted
    while(tmp != NULL && tmp -> item != item)
    {
        prev = tmp;
        tmp = tmp -> next;
    }


    // if data doesn't exist in ll
    if(tmp == NULL)
    {
        return;
    }
    // unlink the node from ll
    prev -> next = tmp -> next;

    free(tmp);
}




void PrintList(Node ** head)
{
    Node * tmp = *head;
    while(tmp != NULL)
    {

        printf("%ld ", tmp -> item);

        tmp = tmp -> next;
    }
    printf("\n");
}

/////////////////////////////// Path ///////////////////////////////


void PushNode_Path(PathNode ** head, char * path)
{
    PathNode * node = (PathNode *)malloc(sizeof(PathNode));


    node -> path = ( char *)malloc(1 + sizeof(char) * strlen(path));
    strcpy(node -> path,(const  char *)path);

    // The next pointer of new node
    node -> next = (* head);

    // Set head to point to the new node
    (*head) = node;

}

bool SearchInList_Path(PathNode ** head, char * path)
{

    PathNode * tmp = *head;
    while(tmp != NULL)
    {
        if(!strcmp(tmp -> path, path))
        {
            return true;
        }
        tmp = tmp -> next;
    }
    return false;
}

char * GetValue_Path(PathNode ** head, long i)
{

    PathNode * tmp = *head;
    long j = 0;
    while(tmp != NULL)
    {
        if(j == i)
        {
            return tmp -> path;
        }
        tmp = tmp -> next;
    }
    return NULL;
}


void DeleteList_Path(PathNode ** head)
{
    PathNode * current = * head;
    PathNode * next;

    while(current != NULL)
    {
        // change head
        next = current -> next;
        free(current -> path);
        free(current);
        current = next;
    }


    *head = NULL;
}

void DeleteNode_Path(PathNode ** head, char * path)
{
    PathNode * tmp = *head;
    PathNode * prev;


    if(tmp != NULL && !strcmp(tmp -> path, path))
    {
        // change head
        *head = tmp -> next;
        free(tmp -> path);
        free(tmp);
        return;
    }

    // Search for the item to be deleted
    while(tmp != NULL && strcmp(tmp -> path, path))
    {
        prev = tmp;
        tmp = tmp -> next;
    }




    // if data doesn't exist in ll
    if(tmp == NULL)
    {
        return;
    }
    // unlink the node from ll
    prev -> next = tmp -> next;
    free(tmp -> path);
    free(tmp);
}



void PrintList_Path(PathNode ** head)
{
    PathNode * tmp = *head;
    while(tmp != NULL)
    {
        printf("%s\n",tmp -> path);
        tmp = tmp -> next;
    }
    printf("\n");
}
