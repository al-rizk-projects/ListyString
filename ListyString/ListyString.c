//Alex Rizk
//NID: al998687
#include <stdio.h>
#include <stdlib.h>
#include "ListyString.h"
#include <string.h>

int processInputFile(char *filename)
{
    //holders for the input
    char word;
    char buffer[1024];

    ListyString *listy;

    FILE *ifp = NULL;

    //Checks if the file is empty
    if ((ifp = fopen(filename, "r")) == NULL)
    {
        return 1;
    }

    fscanf(ifp, "%s", buffer);

    //creates ListyString from the input contained within the buffer
    listy = createListyString(buffer);

    //case statments are checked while looping until the end of the input
    while(fscanf(ifp, "%c", &word) != EOF)
    {
        switch(word)
        {
            case '@':
                fscanf(ifp, " %c %s", &word, buffer);
                replaceChar(listy, word, buffer);
                break;
            case '+':
                fscanf(ifp, " %s", buffer);
                listyCat(listy, buffer);
                break;
            case '-':
                fscanf(ifp, " %c", &word);
                replaceChar(listy, word, "");
                break;
            case '~':
                reverseListyString(listy);
                break;
            case '?':
                printf("%d\n", listy -> length);
                listyLength(listy);
                break;
            case '!':
                printListyString(listy);
                break;
            default:
            ;
        }
    }

    fclose(ifp);
    return 1;
}



ListyString *createListyString(char *str)
{
    int i;
    int length;
    ListyNode *tail;

    //Allocates space for the structs
    ListyString *listy = malloc(sizeof(ListyString));

    listy -> head = NULL;
    tail = listy -> head;

    //Checks if the string is empty
    if (str == NULL || strcmp(str, "") == 0)
    {
        listy -> head = NULL;
        listy -> length = 0;
        return listy;
    }

    length = strlen(str);

    //Loops through list creating nodes for the chars in the string to settle
    for (i = 0; i < length; i++)
    {
        if (tail == NULL)
        {
            listy -> head = malloc(sizeof(ListyNode));
            tail = listy -> head;
            listy -> head -> data = str[i];
            listy -> head -> next = NULL;

        }else{
            tail -> next = malloc(sizeof(ListyNode));
            tail -> next -> data = str[i];
            tail -> next -> next = NULL;
            tail = tail -> next;
        }
    }

    listy -> length = length;
    //return a pointer to the new ListyString
    return listy;
}

ListyString *destroyListyString(ListyString *listy)
{
    //NULL check to IMMEDIATELY DESTROY the array
    if (listy == NULL || listy -> head == NULL)
    {
        free(listy);
        return NULL;
    }

    ListyNode *current = listy -> head;

    //iterates through the listy list and frees each node until it reaches null
    while (current != NULL)
    {
        current = current -> next;
        free(current);
        current = current;
    }

    //completely DESTROYS the list
    free(listy);

    return NULL;
}

ListyString *cloneListyString(ListyString *listy)
{
    int i, length;
    ListyNode *tail2;
    ListyNode *tail;

    //Null Check
    if (listy == NULL)
    {
        return NULL;
    }

    //Creates new ListyString and copys contents of listy into listycopy
    ListyString *listycopy = malloc(sizeof(cloneListyString));
    listycopy -> head = NULL;
    listycopy -> length = listy -> length;
    length = listycopy -> length;

    //if listy is empty return the listycopy, no further copying needs to be done
    if (listy -> head == NULL)
        return listycopy;

    //If the head is not empty create from for the head
    listycopy -> head = malloc(sizeof(ListyNode));

    //set tail to hold listy's head
    tail = listy -> head;

    //set tail2 to hold listycopy's head
    tail2 = listycopy -> head;

    //loop through and copy the additional values of the listy array into listy copy
    for (i = 0; i < length; i++)
    {
        tail2 -> data = tail -> data;

        if (i < (length- 1))
        {
            tail2 -> next = malloc(sizeof(ListyNode));
            tail2 = tail2 -> next;
        }

        tail = tail -> next;
    }

    tail2 -> next = NULL;

    return listycopy;

}

void replaceChar(ListyString *listy, char key, char *str)
{
    int i, length;
    ListyNode *before, *current, *after;

    //NULL check
    if(listy == NULL || listy -> head == NULL)
        return;

    //Set the values for the before, current, and after ListyNode pointers
    before = NULL;
    current = listy -> head;
    after = listy -> head -> next;

    //Fetch the length of the string
    length = strlen(str);

    //Itterates through the list until null
    while(current != NULL)
    {

        i = 1;
        after = current -> next;

        //If the key is found in the array preform one of the three procedures
        if(current -> data == key)
        {
            //If the length of the string is greater than on
            //replace the key and allocate space for the rest of the chars
            //until the if condition is no longer valid by incrementing i
            if (length > 1)
            {
                current -> data = str[0];

                while (i < length)
                {
                    current -> next = malloc(sizeof(ListyNode));
                    current -> next -> next = NULL;
                    current -> next -> data = str[i++];
                    current = current -> next;
                    listy -> length++;
                }

                current -> next = after;

            //If the length of the string happens to be 1 then
            //replace the key with the string at its current location
            }else if (length == 1)
            {
                current -> data = str[0];

            //if the length of the string is zero eliminate the node
            //and set previous nodes to the proper next node
            }else if (length == 0)
            {
                free(current);

                if(before != NULL)
                {
                    before -> next  = after;
                }else{
                    listy -> head = after;
                }

                //continue forces the next iteration of the loop
                current = after;
                listy -> length--;
                continue;
            }
        }

        //iterates through the list
        before = current;
        current = current -> next;
    }
}

void reverseListyString(ListyString *listy)
{
    //Null check
    if (listy == NULL || listy -> head == NULL)
    {
        return;
    }

    //setting up pointers for the reverse
    ListyNode *before = NULL;
    ListyNode *current = listy -> head;
    ListyNode *after = NULL;

    //Runs through array and alters the head and pointers in order to reverse the array
    while (current != NULL)
    {
        after = current -> next;
        current -> next = before;
        before = current;
        current = after;
    }
    listy -> head = before;

}

ListyString *listyCat(ListyString *listy, char *str)
{
    int i,length;
    //test case 21, 20 checking that list is not null and a string is present
    if (listy != NULL && str == NULL)
    {
        return listy;
    }

    length = strlen(str);
    ListyNode *current = listy -> head;

    //If the ListyString is empty create nodes to fit the concatinated word
    if (current == NULL)
    {
        listy -> head = malloc(sizeof(ListyNode));
        listy -> head -> data = str[0];
        listy -> length++;
        listy -> head-> next = NULL;
        current = listy ->head;

        i = 1;
    } else{
    //Otherwise places the word to be concatinated at the end of the list
        while (current -> next != NULL)
        {
            current = current -> next;
        }

        i = 0;

    }

    //Loops all while creating new space for the new string
    for(;i < length; i++)
    {
        current ->  next = malloc(sizeof(ListyNode));
        current -> next -> next = NULL;
        current -> next -> data = str[i];
        current = current -> next;
        listy -> length++;
    }

    return listy;

}

int listyCmp(ListyString *listy1, ListyString *listy2)
{
    //Special conditions check
    if (listy1 == NULL && listy2 == NULL)
    {
        return 0;
    }else if (listy1 == NULL && listy2 != NULL )
    {
        return 1;
    }else if (listy1 != NULL && listy2 == NULL)
    {
        return 1;
    }

    //setting both to head in order to being in the same node
    ListyNode *temp1 = listy1 -> head;
    ListyNode *temp2 = listy2 -> head;

    //if both list are NULL they are identical
    if (listy1 == NULL && listy2 == NULL)
    {
        return 0;
    }

    //Checks the head of list 1 and checks list 2 and vice versa
    if (listy1 -> head == NULL && listy2 == NULL )
    {
        return 1;
    }else if (listy1 == NULL && listy2 -> head == NULL){
        return -1;
    }

    //runs through both lists until it reaches a difference
    while (temp1 && temp2 && temp1 -> data == temp2 -> data)
    {
        temp1 = temp1 -> next;
        temp2 = temp2 -> next;
    }

    //after finding the difference it compairs and returns 1, -1, or 0
    if(temp1 && temp2)
        return (temp1 -> data > temp2 -> data)? 1: -1;

    if (temp1 && !temp2)
        return 1;
    if (temp2 && !temp1)
        return -1;

    return 0;



}

int listyLength(ListyString *listy)
{
    //NULL check and head check otherwize return the length
    if (listy == NULL)
    {
        return -1;
    }else if (listy -> head == NULL)
    {
        return 0;
    }

        return listy -> length;

}

void printListyString(ListyString *listy)
{
    //Checks whether the string being checked is empty
    if (listy == NULL || listy -> head == NULL)
    {
        printf("(empty string)\n");
        return;

    }
    //Otherwise run through each node in the list and print it out
    ListyNode *temp = listy -> head;
    while (temp != NULL)
    {
        printf("%c", temp -> data);
        temp = temp -> next;
    }
    printf("\n");

}

double difficultyRating(void)
{
    return 4.0;
}

double hoursSpent(void)
{
    return 38.0;
}

int main(int argc, char **argv)
{
    //passes the argument to processInputFile
    if (argc > 1)
    {
        processInputFile(argv[1]);
    }

	return 0;
}
