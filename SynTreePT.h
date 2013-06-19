#ifndef SYNTREEPT_H_INCLUDED
#define SYNTREEPT_H_INCLUDED

#include "Common.h"

using namespace std;

struct TreeNode
{
  string value;
  TreeNode *parent;
  TreeNode *rightSibling;
  TreeNode *leftSibling;
  int id;
  int level;
  int Begin;
  int Back;
  vector<TreeNode*> children;
  TreeNode(const string&s, TreeNode* m_parent):value(s),parent(m_parent),id(0),rightSibling(NULL),leftSibling(NULL),level(0){};
};

class SynTree
{
public:
    TreeNode root;
    SynTree(const string&ori_s):root("",NULL){
        root.Begin=0;
        stringstream ss(ori_s);
        copy(istream_iterator<string,char>(ss),istream_iterator<string,char>(),back_insert_iterator<vector<string> >(sentenceVec));
        ss.str("");
    }
    int ParseTree(const string& s, int mode=-1);
    int ClearTree();
    string GetSpanWithNode(TreeNode* node);

    void PrintTree(TreeNode *node);
    TreeNode* GetNodeWithId(int id);
    int GetSpanStartWithNode(TreeNode *node);
    int GetSpanEndWithNode(TreeNode *node);
    void GetStrVec(int begin, int back,vector<string>&result);
    int GetSize(){return nodeIdMap.size();}
    vector<string> sentenceVec;
    map<int, TreeNode*> nodeIdMap;

private:



};

#endif // SYNTREEPT_H_INCLUDED
