#include <stdio.h>
#include "../src/cdg.h"

CDGNode* root;

void tCreateNode();
void tUpdateCDG();
void tTopPath();
void setup();
void tearDown();
void printScores();

int main () {
  setup();
  tCreateNode();
  tUpdateCDG();
  tTopPath();
  tUpdateCDG();
  printf("Hurray... !!! Everything Worked !!!\n");
  return 0;
}

void setup() {
  CDGNode* n[36];
  int i;
  for ( i = 1; i < 36; i++ ) {
    n[i] = newNode(i, 1, 1, NULL, NULL, NULL, NULL, NULL);
  }
  setNextNode(n[1], n[2]);
  setNextNode(n[2], n[3]);

  addTrueNode(n[1],n[4]);
  setNextNode(n[4],n[5]);

  addTrueNode(n[3], n[19]);
  setNextNode(n[19], n[20]);
  setNextNode(n[20], n[21]);
  setNextNode(n[21], n[22]);

  addTrueNode(n[4],n[6]);
  setNextNode(n[6],n[7]);
  setNextNode(n[7],n[8]);

  addTrueNode(n[5],n[9]);
  setNextNode(n[9],n[10]);

  addTrueNode(n[19],n[23]);
  addFalseNode(n[19],n[24]);

  addTrueNode(n[20],n[25]);
  addFalseNode(n[20],n[26]);

  addTrueNode(n[21],n[27]);
  addFalseNode(n[21],n[28]);

  addTrueNode(n[22],n[29]);
  addFalseNode(n[22],n[30]);

  addTrueNode(n[7],n[11]);

  addTrueNode(n[9], n[12]);
  addFalseNode(n[9], n[13]);

  addTrueNode(n[10], n[14]);
  addFalseNode(n[10], n[15]);

  setNextNode(n[15], n[16]);
  setNextNode(n[16], n[17]);

  addTrueNode(n[30], n[31]);
  setNextNode(n[31], n[32]);
  setNextNode(n[32], n[33]);  

  addTrueNode(n[16],n[18]);

  addTrueNode(n[32], n[34]);

  addTrueNode(n[34], n[35]);

  root = n[1];

} 

void tearDown() {

}

void tCreateNode() {
  CDGNode* b = newBlankNode();
  assert(NULL != b);

  CDGNode *n, *p, *t, *f, *nxt;
  p = newBlankNode();
  t = newBlankNode();
  f = newBlankNode();
  nxt = newBlankNode();
  n = newNode(1, 2, 1, "abc", t, f, p, nxt);
  assert(1 == getID(n));
  assert(2 == getScore(n));
  assert(0 == strcmp(getExpr(n), "abc"));
  assert(t == getTrueNodeSet(n));
  assert(f == getFalseNodeSet(n));
  assert(p == getParent(n));
  assert(nxt == getNextNode(n));
}

void tUpdateCDG() {
  updateCDG(root);
  assert(7 == getScore(root));
  assert(8 == getScore(getNextNode(getNextNode(root))));
}

void printScores() {
  printf("=======================================\n");
  Stack* nodeStack = stackNew(sizeof(CDGNode*));
  CDGNode* node;
  postOrder(root, nodeStack);
  while ( !stackIsEmpty(nodeStack) ) {
    stackPop(nodeStack, &node);
    printf ("%d : %d \n", getID(node), getScore(node));
  }
  stackFree(nodeStack);
  printf("=======================================\n");  
}

void printPath (CDGNode* node) {
  while ( node ) {
    printf("ID: %d, Outcome: %d\n", getID(node), getOutcome(node));
    printPath(getTrueNodeSet(node));
    printPath(getFalseNodeSet(node));
    node = getNextNode(node);
  }
}

void tTopPath() {
  CDGPath* path = getTopPaths(root, 2);
  CDGPath* tempPath = path;
  CDGNode* node;

  while ( tempPath ) {
    node = getPathNode(tempPath);
    printPath(node);
    tempPath = getNextPath(tempPath);
    printf("====================================\n");
  }
  deletePaths(path);
}
