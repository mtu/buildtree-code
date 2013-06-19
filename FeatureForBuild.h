#ifndef FEATUREFORBUILD_H_INCLUDED
#define FEATUREFORBUILD_H_INCLUDED
#include"SynTreePT.h"
#include"SynTreeSQ.h"
#include "SegScore.h"
typedef RelationTree::TreeNode RTreeNode;

struct EduFeature
{
  TreeNode* nodeHead;
  TreeNode* nodeDomin;
  string nHLabel;
  string nHWord;
  string nDLabel;
  string nDWord;

  int eduNum;
  int nHLevel;

  string uniGram;
  string biGram;
  string triGram;
  string uniGramBack;
  string biGramBack;
  string triGramBack;
  string uniPOS;
  string uniBackPOS;
  string biPOS;
  string biBackPOS;
  string triPOS;
  string triBackPOS;

  int Begin;
  int Back;

  void defaultValue(){
        nDLabel="NULL";nDWord="NULL";uniGram="NULL";biGram="NULL";triGram="NULL";
        nHLabel="NULL";nHWord="NULL";nHLevel=0;
    }
};

struct FullFeature
{
    EduFeature *eduA;
    EduFeature *eduB;
    TreeNode *nodeDomin;
    TreeNode *nodeHead;
    string nDLabel;
    string nDWord;
    string nDLeftSiblingLabel;
    string nDRightSiblingLabel;
    string nHLabel;
    string nHWord;
    int nHLevel;
    int dominRel;
    int delta;
    int AdeltaLh;
    int BdeltaLh;
    int Begin;
    int Back;

    void defaultValue(){
        nDLabel="NULL";nDWord="NULL";nDLeftSiblingLabel="NULL";nDRightSiblingLabel="NULL";
        nHLabel="NULL";nHWord="NULL";nHLevel=0;dominRel=2;delta=0;AdeltaLh=0;BdeltaLh=0;
    }
};
struct Feature
{
    EduFeature left;
    EduFeature right;
    FullFeature both;
    //Feature():left(NULL),right(NULL),both(NULL){}
};

class FeatureForBuild
{
public:

  int InitTrain(const string&m_pureStr,const string&m_relStr,const string& m_synStr);//0:成功 -1：不成功
  int InitTest(const string&m_pureStr,const string& m_synStr,const string&segStr);
  void GetManuPOS(vector<string>&senVec,vector<string>& posVec);
  void GetInstance();
  void GetBaseSpanForDecode(const string& segStr);
  void AddHeadWordMap(TreeNode* p);
  string GetPOSTag(TreeNode* a);
  int GetEduNumers(int beg,int back);
  TreeNode *GetHighestNode(TreeNode *node);
  TreeNode *GetHeadNode(int beg,int back);
  TreeNode *GetDominantNode(int beg,int back);
  TreeNode *GetDominantNode(TreeNode* node);
  int GetLevel(TreeNode*node);
  int DominanceRelation(TreeNode*a,TreeNode*b);
  string GetNodeHeadWord(TreeNode*node);
  TreeNode* GetLeftSibling(TreeNode*node);
  TreeNode* GetRightSibling(TreeNode*node);
  int GetDelta(TreeNode*a,TreeNode*b);
  string GetUniGram(int beg,TreeNode* node=NULL);
  string GetBiGram(int beg, int back,TreeNode* node=NULL);
  string GetTriGram(int beg,int back,TreeNode* node=NULL);
  string GetUniGramBack(int back,TreeNode* node=NULL);
  string GetBiGramBack(int beg,int back,TreeNode* node=NULL);
  string GetTriGramBack(int beg,int back,TreeNode* node=NULL);

  string GetUniPos(int beg);
  string GetBiPos(int beg, int back);
  string GetTriPos(int beg,int back);
  string GetUniBackPos(int back);
  string GetBiBackPos(int beg,int back);
  string GetTriBackPos(int beg,int back);

  void ExtractSingleSpanFeature(int beg,int back,EduFeature&edu);
  void ExtractSingleSpanFeature(FullFeature&fSpan,EduFeature&edu);
  void ExtractSingleSpanFeatureDecode(int beg,int back,EduFeature&edu,int level=0);
  void ExtractFullFeature(EduFeature&eduA,EduFeature&eduB,FullFeature&fSpan);
  void ExtractPositiveInstance(RTreeNode* a,Feature &feat);
  void NegativeInstance(RTreeNode* a,RTreeNode*b);
  void ExtractNegFeature(RTreeNode *node);

  void ExtractTestInstance();
  void LoadTestFeatMat();
  void LoadTestEduFeatMat();

  int TrainingProcess(const string&m_pureStr,const string&m_relStr,const string& m_synStr);
  int TestingProcess(const string&m_pureStr,const string&m_segStr,const string& m_synStr,SegScore &Strm1,SegScore &Lbm2,SegScore &lbm3);
  int TestingLabelForStdStr(const string&m_pureStr,const string&m_relStr,const string& m_synStr,SegScore &Lbm2);

  RTreeNode CYKDecode(SegScore &strModel,SegScore&labModel,SegScore&labModel1); //同时找到最佳路径以及标签
  int CYKDecode_Search(SegScore &strModel,SegScore &lbModel);
  void ConstructDecodeTree(int curLevel,int curIndex,int sibLevel,int sibIndex,int leftorright,SegScore&labModel,RTreeNode *node);
  double ScoreforDeocdeLabel(vector<string> &featVec,SegScore&labModel);
  double ScoreForStructure(vector<string> &featVec,SegScore &strModel);
  string GetDeocdeLabel(vector<string> &featVec,SegScore&labModel);
  void GetStructureFeat(Feature &a,vector<string>& featVec);
  void GetStructureFeat(EduFeature &a,EduFeature&b, FullFeature&c, vector<string>& featVec);
  int GetStructureFeatForEdu(EduFeature&a, int count,vector<string>& featVec);
  int GetStructureFeatForFull(FullFeature&a,int count,vector<string>&featVec);
  void GetLabelFeat(Feature &a, EduFeature&b,int leftorright, vector<string>& featVec);
  void GetLabelFeat(EduFeature &a, EduFeature&b,EduFeature &c,int leftorright, vector<string>& featVec);
  void LabelingForStdStr(SegScore&labModel);


  void PrintStrFeature();
  void PrintLabelFeature();
  void PrintStrFeature(vector<Feature> &vec);


  void RecursiveEachNode(RTreeNode *a);
  void ClearAll();
private:


public:
  SynTree *mySynTree;
  SynTreeSQ *myRelTree;
  vector<RTreeNode *> instanceVec;
  vector<string> posVec;

  vector<pair<int,int> > baSpanVec;//pair:begin,back
  vector<string> sentenceVec;
  map<TreeNode *,int> headWordMap;
  vector<Feature *> posFeatVec;
  map<RTreeNode*, Feature> posFeatMap;
  vector<Feature > negFeatVec;
  vector<vector<vector<Feature > > > testFeatMat;
  vector<vector<EduFeature> > testEduFeatMat;
  vector<vector<int> > pathOfConstruct;
  vector<vector<float> > scoreVec;
  vector<string> labelVec;
  map<RTreeNode*,string> posLabel;


};
void PrintFull(FullFeature&a,int count);
int PrintEdu(EduFeature&a, int count);

#endif // FEATUREFORBUILD_H_INCLUDED
