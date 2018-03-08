#ifndef TREE_H_
#define TREE_H_

/**\ TYPE DEFINITION \**/
    typedef struct Node Node;

/**\ ACCESSORS \**/
    int getId (Node* toGetFrom);

    Node* getLeftmostChild (Node* parent);
    Node* getRightmostChild (Node* parent);

    Node* getRight (Node* toGetFrom);

/**\ CONSTRUCTOR \**/
    Node* newNode (int identifier);

/**\ MODIFIERS \**/
    void addNewChild (Node* newChild, Node* parent);

    void unlinkList (Node* leftEnd, Node* rightEnd);
    void setChildren (Node* parent, Node* newLeftmost, Node* newRightmost);

/**\ DECONSTRUCTORS \**/
    void replaceParentByChildren (Node* toReplace);

#endif
