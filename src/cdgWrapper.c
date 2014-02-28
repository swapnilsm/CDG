#include <stdio.h>
#include "cdg.h"

#define MAX_NODES  500

CDGNode* nodes[MAX_NODES] = {NULL}; //Holds all the nodes in the tree
CDGNode* root = NULL;

void preOrderCDG(CDGNode*);
void addtoCDGnode(int , int , int);
void setArray();
void printArray();

// branch variable tell on which branch of parent the node is to be attached 
void addtoCDGnode(int id, int pid, int branch) {
  CDGNode* node = newBlankNode();
  setID(node, id);
  if ( 0 == id ) {
    root = node;
  } else {
    if (branch) {
      addTrueNode(nodes[pid], node);
    } else {
      addFalseNode(nodes[pid], node);
    }
  }
  nodes[id] = node;
}

void setArray(int id, const char *expr) {
  setExpr(nodes[id], expr);
}

void printArray()
{
  CDGNode* node = root;
  preOrderCDG(node);
}

void preOrderCDG(CDGNode* node) {
  while (node) {
    if ( 0 == getID(node) )
      printf("ID: %d, PID: 0", getID(node));
    else
      printf("ID: %d, PID: %d", getID(node), getID(getParent(node)));
    preOrderCDG(getTrueNodeSet(node));
    preOrderCDG(getFalseNodeSet(node));    
  }
}
