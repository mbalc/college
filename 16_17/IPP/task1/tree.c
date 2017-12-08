#include <stdlib.h>
#include <assert.h>


/**\ TYPE DEFINITION \**/

typedef struct Node {
    int id;
    struct Node* parent; //not guaranteed to be correct for non-outmost children
    struct Node* left;
    struct Node* right;
    struct Node* leftmostChild;
    struct Node* rightmostChild;
} Node;


/**\ ACCESSORS \**/

int getId (Node* toGetFrom) {
    if (toGetFrom == NULL) return -1;
    return (*toGetFrom).id;
}

Node* getLeftmostChild (Node* parent) {
    if (parent == NULL) return NULL;
    return (*parent).leftmostChild;
}
Node* getRightmostChild (Node* parent) {
    if (parent == NULL) return NULL;
    return (*parent).rightmostChild;
}
Node* getRight (Node* toGetFrom) {
    if (toGetFrom == NULL) return NULL;
    return (*toGetFrom).right;
}


/**\ CONSTRUCTOR \**/

Node* newNode (int identifier) {
    Node *out;
    out = (Node*) malloc (sizeof (struct Node));

    (*out).id = identifier;
    (*out).left = NULL;
    (*out).right = NULL;
    (*out).parent = NULL;
    (*out).leftmostChild = NULL;
    (*out).rightmostChild = NULL;
    return out;
}


/**\ MODIFIERS \**/

static void linkNodes (Node* onLeft, Node* onRight) {
    if (onLeft != NULL) (*onLeft).right = onRight;
    if (onRight != NULL) (*onRight).left = onLeft;
}

//adds [newChild] rightmost from current [parent]'s children
void addNewChild (Node* newChild, Node* parent) {
    if ((*parent).rightmostChild == NULL) {
        if ((*parent).leftmostChild != NULL) assert (0);
        (*parent).leftmostChild = newChild;
        (*newChild).left = NULL;
    }
    else linkNodes ((*parent).rightmostChild, newChild);

    (*newChild).parent = parent;
    (*newChild).right = NULL;
    (*parent).rightmostChild = newChild;
}

//assumes [rightEnd] is a rightmost child of its parent (only supported case)
void unlinkList (Node* leftEnd, Node* rightEnd) {
    (*(*leftEnd).left).parent = (*rightEnd).parent;
    (*(*leftEnd).left).right = NULL;
    (*(*rightEnd).parent).rightmostChild = (*leftEnd).left;
    (*leftEnd).left = NULL;
}

void setChildren (Node* parent, Node* newLeftmost, Node* newRightmost) {
    (*parent).leftmostChild = newLeftmost;
    (*parent).rightmostChild = newRightmost;
    (*newLeftmost).parent = parent;
    (*newRightmost).parent = parent;
}


/**\ DECONSTRUCTORS \**/

//prepares parent of [me]'s outmost children values to be correct after
//removing [me] Node
static void outmostChildFix (Node* me, Node** myParentsChild,
                             Node* myChild, Node* myNeighbour) {
    if (me == *myParentsChild) {
        Node* newOutmost;
        if (myChild == NULL) newOutmost = myNeighbour;
        else newOutmost = myChild;

        *myParentsChild = newOutmost;
        if (newOutmost != NULL) (*newOutmost).parent = (*me).parent;
    }
}

//children are put in place of its parent, the parent is deallocated
void replaceParentByChildren (Node* toReplace) {
    if (((*toReplace).parent) != NULL) {
        if ((*toReplace).left == NULL)
            outmostChildFix (toReplace, &(*(*toReplace).parent).leftmostChild,
                             (*toReplace).leftmostChild, (*toReplace).right);
        if ((*toReplace).right == NULL)
            outmostChildFix (toReplace, &(*(*toReplace).parent).rightmostChild,
                             (*toReplace).rightmostChild, (*toReplace).left);
    }

    linkNodes ((*toReplace).left, (*toReplace).right);

    if ((*toReplace).leftmostChild != NULL)
        linkNodes ((*toReplace).left, (*toReplace).leftmostChild);
    if ((*toReplace).rightmostChild != NULL)
        linkNodes ((*toReplace).rightmostChild, (*toReplace).right);

    free (toReplace);
}
