#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"

static void giveOK () {
    printf ("OK\n");
}

void addNode (Node* parent, int* id, Node** refPointer) {
    Node* child = newNode (*id);
    addNewChild (child, parent);
    *refPointer = child;
    ++(*id);
    giveOK ();
}

void deleteNode (Node** toRemove) {
    replaceParentByChildren (*toRemove);
    (*toRemove) = NULL;
    giveOK ();
}

void deleteSubtree (Node* toRemove, int* counter, Node** ref) {
    Node* next;
    for (Node* i = getLeftmostChild(toRemove); i != NULL; i = next) {
        next = getRight (i);
        deleteSubtree (i, counter, ref);
    }
    ref[getId (toRemove)] = NULL;
    replaceParentByChildren (toRemove);
    --(*counter);
}

void splitNode (Node* parent, Node* splitPoint, int* id, Node** refPointer) {
    Node* newLeftEnd = getRight (splitPoint);
    Node* newRightEnd = getRightmostChild (parent);
    if (newLeftEnd != NULL) unlinkList (newLeftEnd, newRightEnd);
    addNode (parent, id, refPointer);
    if (newLeftEnd != NULL) setChildren (*refPointer, newLeftEnd, newRightEnd);
}

void rightmostChild (Node* parent) {
    printf ("%d\n", getId (getRightmostChild (parent)));
}

int main (int argc, char** argv) {
    short int debug = 0;
    if (argc == 2 && strcmp (argv[1], "-v") == 0) {
        debug = 1;
    }
    if (argc > 2 || (debug == 0 && argc == 2)) {
        printf ("ERROR");
        return 1;
    }
    char input[100];
    int argument, secondArg, iterator = 1, nodeCounter = 1;

    Node** ref = calloc (1000000, sizeof (Node*));

    ref[0] = newNode (0);

    while (scanf ("%s", input) == 1) {
        if (scanf ("%d", &argument) == 0) assert (0);

        if (strcmp (input, "ADD_NODE") == 0) {
            addNode (ref[argument], &iterator, &ref[iterator]);
            ++nodeCounter;
        }

        else if (strcmp (input, "RIGHTMOST_CHILD") == 0) {
            rightmostChild (ref[argument]);
        }

        else if (strcmp (input, "DELETE_NODE") == 0) {
            deleteNode (&ref[argument]);
            --nodeCounter;
        }

        else if (strcmp (input, "DELETE_SUBTREE") == 0) {
            deleteSubtree (ref[argument], &nodeCounter, ref);
            giveOK ();
        }

        else if (strcmp (input, "SPLIT_NODE") == 0) {
            if (scanf ("%d", &secondArg) == 0) assert (0);
            splitNode (ref[argument], ref[secondArg], &iterator, &ref[iterator]);
            ++nodeCounter;
        }

        if (debug == 1) fprintf (stderr, "NODES: %d\n", nodeCounter);
        if (scanf ("%c", input) == 0) break;
    }

    deleteSubtree (ref[0], &nodeCounter, ref);
    free (ref);

    return 0;
}
