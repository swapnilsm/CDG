#include "cdg.h"

int max(int a, int b) {
  return a > b ? a : b;
}

void pushNodeListToStack(Stack* s, CDGNode* node) {
  assert(NULL != node);
  do {
    stackPush(s, &node);
    node = getNextNode(node);
  } while (node);  
}

void postOrder(CDGNode* root, Stack* s) {
  if ( NULL == root ) return;
  Stack* temp = stackNew(sizeof(CDGNode*));;
  CDGNode* node;  
  CDGNode* listNode;
  //  stackPush(temp, &root);
  pushNodeListToStack(temp, root);
  while(!stackIsEmpty(temp)) {
    stackPop(temp, &node);
    if ( getTrueNodeSet(node) ) {
      pushNodeListToStack(temp, getTrueNodeSet(node));
    }
    if ( getFalseNodeSet(node) ) {
      pushNodeListToStack(temp, getFalseNodeSet(node));      
    }
    stackPush(s, &node);
  }
  stackFree(temp);
}

CDGNode* getTrueNodeSet(CDGNode* node) {
  return node->trueNodeSet;
}

CDGNode* setTrueNodeSet(CDGNode* node, CDGNode* trueNodeSet) {
  node->trueNodeSet = trueNodeSet;
  return node;
}

CDGNode* getFalseNodeSet(CDGNode* node) {
  return node->falseNodeSet;
}

CDGNode* setFalseNodeSet(CDGNode* node, CDGNode* falseNodeSet) {
  node->falseNodeSet = falseNodeSet;
  return node;
}

CDGNode* setNextNode(CDGNode* node, CDGNode* nextNode) {
  node->next = nextNode;
  if ( nextNode ) setParent(nextNode, getParent(node));
  return node;
}

CDGNode* getLastNode(CDGNode* node) {
  assert(NULL != node);
  CDGNode* curr = node;
  while ( getNextNode(curr) ) {
    curr = getNextNode(curr);
  }
  return curr;
}

int isLeaf(CDGNode* node) {
  if ( getTrueNodeSet(node) || getFalseNodeSet(node) ) return 0;
  return 1;
}

int getConditionalNodeSum(CDGNode* node) {
  int sum = 0;
  while (node != NULL) {
    if ( !isLeaf(node) ) {
      sum += getScore(node);
    }
    node = getNextNode(node);
  }
  return sum;  
}

int hasUncoveredChild(CDGNode* node, int branch) {
  CDGNode* temp;
  if ( branch ) 
    temp = getTrueNodeSet(node);
  else
    temp = getFalseNodeSet(node);    
  while (temp) {
    if (isLeaf(temp) && 0 < getScore(temp)) return 1;
    temp = getNextNode(temp);
  } 
  return 0;
}

int hasConditionalChild(CDGNode* node) {
  CDGNode* temp;
  temp = getTrueNodeSet(node);
  while (temp) {
    if (!isLeaf(temp)) return 1;
    temp = getNextNode(temp);
  } 
  temp = getFalseNodeSet(node);
  while (temp) {
    if (!isLeaf(temp)) return 1;
    temp = getNextNode(temp);
  } 
  return 0;
}

int isConditionalLeaf(CDGNode* node) {
  if (isLeaf(node)) return 0;
  if (!hasConditionalChild(node)) return 1;
  if ( 0 < getConditionalNodeSum(getTrueNodeSet(node)))
    return 0;
  if ( 0 < getConditionalNodeSum(getFalseNodeSet(node)))
    return 0;
  return 1;
}

CDGNode* resetExpr(CDGNode* node) {
  if (NULL != node->expr) free(node->expr);
  return node;
}

CDGNode* resetTrueNodeSet(CDGNode* node) {
  node->trueNodeSet = NULL;
  return node;
}

CDGNode* resetFalseNodeSet(CDGNode* node) {
  node->falseNodeSet = NULL;
  return node;
}

CDGNode* resetParent(CDGNode* node) {
  node->parent = NULL;
  return node;
}

CDGNode* resetNextNode(CDGNode* node) {
  node->next = NULL;
  return node;
}



CDGNode* getMaxScoreConditionNode(CDGNode* node) {
  CDGNode* out = NULL;
  do {
    if ( !isLeaf(node) && 0 < getScore(node)) {
      if ( (NULL == out) || (getScore(out) < getScore(node)) ) {
        out = node;
      }
    }
    node = getNextNode(node);
  } while ( NULL != node);
  return out;
}

CDGNode* getMaxScoreConditionChildNode(CDGNode* node, int* outcome) {
  CDGNode* maxTrue = NULL;
  CDGNode* maxFalse = NULL;
  if ( getTrueNodeSet(node) ) {
    maxTrue = getMaxScoreConditionNode(getTrueNodeSet(node));
  }
  if ( getFalseNodeSet(node) ) {
    maxFalse = getMaxScoreConditionNode(getFalseNodeSet(node));
  }
  if ( NULL == maxFalse ) {
    *outcome = 1;
    return maxTrue;
  }
  if ( NULL == maxTrue ) {
    *outcome = 0;
    return maxFalse;
  }
  if ( getScore(maxTrue) < getScore(maxFalse) ) {
    *outcome = 0;
    return maxFalse;
  }
  *outcome = 1;
  return maxTrue;
}

CDGNode* newNode(int id, int score, int outcome, const char* expr, CDGNode* trueNodeSet, CDGNode* falseNodeSet, CDGNode* parent, CDGNode* next) {
  CDGNode* node;
  node = (CDGNode*)malloc(sizeof(CDGNode));
  assert(NULL != node);
  setID(node, id);
  setScore(node, score);
  setOutcome(node, outcome);
  setExpr(node, expr);
  setTrueNodeSet(node, trueNodeSet);
  setFalseNodeSet(node, falseNodeSet);  
  setParent(node, parent);
  setNextNode(node, next);
  return node;  
}

CDGNode* newBlankNode() {
  return newNode(0, 0, 1, NULL, NULL, NULL, NULL, NULL);
}

void deleteNode(CDGNode* node) {
  assert(NULL != node);
  resetExpr(node);
  resetTrueNodeSet(node);
  resetFalseNodeSet(node);
  resetParent(node);
  resetNextNode(node);
  free(node);
}

void deleteNodeList(CDGNode* node) {
  assert(NULL != node);
  CDGNode* next;
  do {
    next = getNextNode(node);
    deleteNode(node);
    node = next;
  } while (node);
}

void deleteCDG(CDGNode* root) {
  if ( NULL == root ) return;
  CDGNode* node;
  Stack* nodeStack = stackNew(sizeof(CDGNode*));
  postOrder(root, nodeStack);
  while ( !stackIsEmpty(nodeStack) ) {
    stackPop(nodeStack, &node);
    deleteNode(node);
  }
  stackFree(nodeStack);
}

int getID(CDGNode* node) {
  return node->id;
}

CDGNode* setID(CDGNode* node, int id) {
  node->id = id;
  return node;
}

int getScore(CDGNode* node) {
  return node->score;
}

CDGNode* setScore(CDGNode* node, int score) {
  node->score = score;
  return node;
}

int getOutcome(CDGNode* node) {
  return node->outcome;
}

CDGNode* setOutcome(CDGNode* node, int outcome) {
  node->outcome = outcome;
  return node;
}

char* getExpr(CDGNode* node) {
  return node->expr;
}

CDGNode* setExpr(CDGNode* node, const char* expr) {
  if ( NULL == expr ) {
    node->expr = NULL;
    return node;
  }
  node->expr = (char*)malloc(sizeof(char)*(strlen(expr)+1));
  strcpy(node->expr, expr);
  return node;
}

CDGNode* addTrueNode(CDGNode* node, CDGNode* trueNode) {
  if ( NULL == trueNode ) return node;
  trueNode->next = node->trueNodeSet;
  node->trueNodeSet = trueNode;
  setParent(trueNode, node);
  return node;
}

CDGNode* addFalseNode(CDGNode* node, CDGNode* falseNode) {
  if ( NULL == falseNode ) return node;
  falseNode->next = node->falseNodeSet;
  node->falseNodeSet = falseNode;
  setParent(falseNode, node);  
  return node;
}

CDGNode* getParent(CDGNode* node) {
  return node->parent;
}

CDGNode* setParent(CDGNode* node, CDGNode* parentNode) {
  node->parent = parentNode;
  return node;
}

CDGNode* getNextNode(CDGNode* node) {
  return node->next;
}

CDGNode* updateScore(CDGNode* node) {
  assert(NULL != node);
  if ( isLeaf(node) ) return node;
  if ( isConditionalLeaf(node)) {
    if ( hasUncoveredChild(node, 1)) {
      setScore(node, 1);
      return setOutcome(node, 1);
    }
    if ( hasUncoveredChild(node, 0)) {
      setScore(node, 1);
      return setOutcome(node, 0);      
    }
    setScore(node, 0);
    return setOutcome(node, 1);    
  }
  int trueScore = getConditionalNodeSum(getTrueNodeSet(node));
  int falseScore = getConditionalNodeSum(getFalseNodeSet(node));
  if ( trueScore >= falseScore ) {
    setScore(node, trueScore + 1);
    setOutcome(node, 1);
  } else {
    setScore(node, falseScore + 1);
    setOutcome(node, 0);
  }
  return node;
}

CDGNode* propagateScoreChange(CDGNode* node) {
  CDGNode* currNode;
  currNode = node;
  while ( currNode ) {
    updateScore(currNode);
    currNode = getParent(currNode);
  }
  return node;
}

CDGNode* visitAnyOneNode(CDGNode* node) {
  assert(NULL != node);
  do {
    if ( isLeaf(node) && 1 == getScore(node) ) {
      setScore(node, 0);
      return node;
    }
    node = getNextNode(node);
  } while (node);
  return NULL;
}

CDGNode* visitAnyOneChild(CDGNode* node) {
  CDGNode* child = NULL;
  if ( getFalseNodeSet(node) ) {
    child = visitAnyOneNode(getFalseNodeSet(node));
  }
  if ( NULL == child && getTrueNodeSet(node) ) {
    child = visitAnyOneNode(getTrueNodeSet(node));
  }

  assert(NULL != child);
  return child;
}

CDGNode* unVisitNode(CDGNode* node) {
  return setScore(node, 1);
}

CDGNode* updateCDG(CDGNode* root) {
  assert(NULL != root);
  Stack* nodeStack = stackNew(sizeof(CDGNode*));
  CDGNode* node;
  postOrder(root, nodeStack);
  while ( !stackIsEmpty(nodeStack) ) {
    stackPop(nodeStack, &node);
    updateScore(node);
  }
  stackFree(nodeStack);
  return root;
}

CDGPath* setPathNode(CDGPath* path, CDGNode* node) {
  assert(NULL != path);
  path->node = node;
  return path;
}

CDGPath* setNextPath(CDGPath* path, CDGPath* nextPath) {
  assert(NULL != path);
  path->next = nextPath;
  return path;
}

CDGPath* newPath() {
  CDGPath* path;
  path = (CDGPath*)malloc(sizeof(CDGPath*));
  assert(NULL != path);
  setPathNode(path, NULL);
  setNextPath(path, NULL);
  return path;
}

CDGNode* getPathNode(CDGPath* path) {
  assert(NULL != path);
  return path->node;
}

CDGPath* getNextPath(CDGPath* path) {
  assert(NULL != path);
  return path->next;
}

CDGNode* copyToPathNode(CDGNode* pathNode, CDGNode* node) {
  assert(NULL != pathNode);
  setID(pathNode, getID(node));
  setExpr(pathNode, getExpr(node));
  setOutcome(pathNode, getOutcome(node));
  return pathNode;
}

/* CDGPath* getTopPathOld(CDGNode* root, CDGNode** lastNode) {
 *   assert(NULL != root);
 *   CDGNode* orgMaxScoreNode = getMaxScoreConditionNode(root);
 * 
 *   if ( NULL == orgMaxScoreNode ) return NULL;
 * 
 *   CDGPath* path = newPath();
 *   CDGNode* node = newBlankNode();
 *   int lastOutcome = 1;
 *   *lastNode = orgMaxScoreNode;
 * 
 *   setPathNode(path, setOutcome(copyToPathNode(node, orgMaxScoreNode), 1));
 *   do {
 *     orgMaxScoreNode = getMaxScoreConditionChildNode(orgMaxScoreNode, &lastOutcome);
 *     if ( orgMaxScoreNode ) {
 *       *lastNode = orgMaxScoreNode;
 *       setNextNode(setOutcome(node, lastOutcome), copyToPathNode(newBlankNode(), orgMaxScoreNode));
 *       node = getNextNode(node);
 *     }
 *   } while (orgMaxScoreNode);
 *   return path;
 * }
 * 
 * CDGPath* getTopPathsOld(CDGNode* root, int numberOfPaths) {
 *   assert(NULL != root);
 *   CDGPath* path = NULL;
 *   CDGPath* currPath = NULL;
 *   CDGNode* lastNode = NULL;
 *   CDGNode* changedNode = NULL;
 *   Stack* visitedNodes = stackNew(sizeof(CDGNode*));
 *   while ( numberOfPaths-- ) {
 *     if ( NULL == path ) {
 *       path = getTopPath(root, &lastNode);
 *       currPath = path;
 *     } else {
 *       setNextPath(currPath, getTopPath(root, &lastNode));
 *       currPath = getNextPath(currPath);
 *     }
 *     if ( NULL == currPath ) break;
 *     changedNode = visitAnyOneChild(lastNode);
 *     stackPush(visitedNodes, &changedNode);
 *     propagateScoreChange(changedNode);
 *   }
 *   while( !stackIsEmpty(visitedNodes) ) {
 *     stackPop(visitedNodes, &changedNode);
 *     unVisitNode(changedNode);
 *     propagateScoreChange(changedNode);
 *   }
 *   stackFree(visitedNodes);
 *   return path;
 * } */

CDGNode* getTopPath(CDGNode* node, Stack* changedNodes) {
  CDGNode* pathNode = newBlankNode();
  CDGNode* temp = pathNode;
  while (node) {
    if ( 0 != getScore(node) ) {
      if ( isLeaf(node) ) {
        setScore(node, 0);
        stackPush(changedNodes, &node);
      } else {
        setNextNode(temp, copyToPathNode(newBlankNode(), node));
        temp = getNextNode(temp);        
        if (getOutcome(node)) {
          setTrueNodeSet(temp, getTopPath(getTrueNodeSet(node), changedNodes));
        } else {
          setFalseNodeSet(temp, getTopPath(getFalseNodeSet(node), changedNodes));
        }
      }
    }
    node = getNextNode(node);
  }
  if ( temp == pathNode ) {
    deleteNode(pathNode);
    pathNode = NULL;
  } else {
    temp = pathNode;
    pathNode = getNextNode(pathNode);
    deleteNode(temp);
  }
  return pathNode;
}

CDGPath* getTopPaths(CDGNode* root, int numberOfPaths) {
  CDGPath* pathHead = NULL;
  CDGNode* path;
  CDGPath* currPath;
  CDGNode* node;
  Stack* changedNodes = stackNew(sizeof(CDGNode*));
  while ( numberOfPaths-- ) {
    path = getTopPath(root, changedNodes);
    if ( NULL == path ) break;
    if ( NULL == pathHead ) {
      pathHead = setPathNode(newPath(), path);
      currPath = pathHead;
    } else {
      setNextPath(currPath, setPathNode(newPath(), path));
      currPath = getNextPath(currPath);
    }
    updateCDG(root);
  }
  while ( !stackIsEmpty(changedNodes) ) {
    stackPop(changedNodes, &node);    
    setScore(node, 1);
  }
  updateCDG(root);
  stackFree(changedNodes);
  return pathHead;
}

void deletePaths(CDGPath* path) {
  assert(NULL != path);
  CDGPath* next;
  do {
    next = getNextPath(path);
    deleteNodeList(getPathNode(path));
    setNextPath(path, NULL);
    free(path);
    path = next;
  } while (path);
}

