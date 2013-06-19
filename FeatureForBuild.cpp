#ifndef FEATUREFORBUILD_CPP_INCLUDED
#define FEATUREFORBUILD_CPP_INCLUDED
#include"FeatureForBuild.h"
#include "HeadWord.h"

void FeatureForBuild::ClearAll()
{
    vector<RTreeNode *>().swap(instanceVec);
    vector<string>().swap(posVec);
    vector<pair<int,int> >().swap(baSpanVec);
    vector<string>().swap(sentenceVec);
    headWordMap.clear();
    vector<Feature *>().swap(posFeatVec);
    posFeatMap.clear();
    vector<Feature>().swap(negFeatVec);
    vector<vector<vector<Feature > > >().swap(testFeatMat);
    vector<vector<EduFeature > >().swap(testEduFeatMat);
    vector<vector<int> >().swap(pathOfConstruct);
    vector<vector<float> >().swap(scoreVec);
    vector<string>().swap(labelVec);
}
int FeatureForBuild::InitTrain(const string& m_pureStr,const string&m_relStr,const string& m_synStr)
{
    mySynTree = new SynTree(m_pureStr);
    mySynTree->ParseTree(m_synStr,0);

    myRelTree = new SynTreeSQ;

    myRelTree->ParseTree(m_relStr,0);
    if(myRelTree->root.Back!=mySynTree->root.Back || myRelTree->root.Back!=myRelTree->sentenceVec.size()-1){
        cerr<<"size unequal: "<<"RelTree: "<<myRelTree->root.Back<<" SynTree: "<<mySynTree->root.Back<<" senVec: "<<myRelTree->sentenceVec.size()-1<<endl;
        exit(1);
    }
    sentenceVec = mySynTree->sentenceVec;
 //   myRelTree->Binarization(&myRelTree->root);
    if(myRelTree->root.children.empty())return -1;
    GetInstance();
  //  cerr<<endl;
    GetManuPOS(myRelTree->sentenceVec,posVec);
    if (instanceVec.empty())return -1;
    AddHeadWordMap(&mySynTree->root);
 //   cerr<<headWordMap.size()<<endl;
    return 0;
}
int FeatureForBuild::InitTest(const string&m_pureStr,const string& m_synStr,const string&segStr)
{
    mySynTree = new SynTree(m_pureStr);
    mySynTree->ParseTree(m_synStr,0);
    sentenceVec = mySynTree->sentenceVec;
    myRelTree = new SynTreeSQ;
//    cerr<<"sentence vector context:"<<sentenceVec.size()<<endl;copy(sentenceVec.begin(),sentenceVec.end(),ostream_iterator<string,char>(cerr," "));
//    cerr<<endl;
    GetBaseSpanForDecode(segStr);
//    for(size_t i(0);i<baSpanVec.size();++i){
//        copy(sentenceVec.begin()+baSpanVec[i].first,sentenceVec.begin()+baSpanVec[i].second+1,ostream_iterator<string,char>(cerr," "));
//        cerr<<endl;
//    }

    if(mySynTree->root.Back!=baSpanVec.back().second || mySynTree->root.Back!=sentenceVec.size()-1){
        cerr<<"size unequal: "<<" SynTree: "<<mySynTree->root.Back<<" basic span: "<<baSpanVec.back().second<<" senVec: "<<sentenceVec.size()-1<<endl;
        exit(1);
    }
    if (baSpanVec.size()==1)return -1;
   // LoadTestFeatMat();
   LoadTestEduFeatMat();
    AddHeadWordMap(&mySynTree->root);
    return 0;
}
void FeatureForBuild::GetInstance()
{
    if(myRelTree->root.children.size()<2)RecursiveEachNode(myRelTree->root.children[0]);
    else{
        myRelTree->root.value="UKN";
        RecursiveEachNode(&myRelTree->root);
    }

}
void FeatureForBuild::RecursiveEachNode(RTreeNode *a)
{
 //   cerr<<a->value<<" "<<a->level<<" " ;
    if(!a->children.empty() ){
       RecursiveEachNode(a->children[0]);
        RecursiveEachNode(a->children[1]);
        instanceVec.push_back(a);
   }
}
string FeatureForBuild::GetPOSTag(TreeNode* a)
{
  if (a->parent!=NULL){return a->parent->value;}
  else return "NULL";
}
void FeatureForBuild::GetManuPOS(vector<string>&senVec,vector<string>& posVec)
{
    size_t pos;
    for(size_t i(0);i<senVec.size();++i)
    {
        pos = senVec[i].find("/");
        if(pos!=string::npos)posVec.push_back(senVec[i].substr(pos+1));
        else posVec.push_back("NULL");
    }
}
void FeatureForBuild::GetBaseSpanForDecode(const string&segStr)
{
    string edu="";
    string word;size_t pos;
    stringstream ss(segStr);int pre(0),post(0);
    while(ss>>word){

        pos = word.find_last_of("/");if (pos==string::npos){cerr<<"test seg wrong\n"<<segStr<<endl;exit(1);}
        if(word.substr(pos+1)=="B" && edu.compare("")!=0){
            baSpanVec.push_back(make_pair(pre,post-1));pre = post;
        }
        else
            edu+=word.substr(0,pos)+" ";
         myRelTree->sentenceVec.push_back(word.substr(0,pos));
        post++;
    }
    if(pre<post){baSpanVec.push_back(make_pair(pre,post-1));}
}
void FeatureForBuild::AddHeadWordMap(TreeNode* p)
{
    if(p->children.empty())return;
    if(p->children.size()==1 && p->children[0]->children.empty()){
        headWordMap[p] = p->children[0]->id;
        return;
    }
    for(size_t i(0);i<p->children.size();++i)
    {
        AddHeadWordMap(p->children[i]);
    }
    headWordMap[p]=headWordMap[p->children[HeadWord::GetHeadWord(p->children,p)]];
}
int FeatureForBuild::GetEduNumers(int beg,int back)
{
  int count(1); RTreeNode *p = myRelTree->nodeIdMap[beg];
  for(size_t i(beg+1);i<=back;++i){
    if(myRelTree->nodeIdMap[i]==p)continue;
    else{
      count++; p=myRelTree->nodeIdMap[i];
    }
  }
  return count;
}
TreeNode *FeatureForBuild::GetHighestNode(TreeNode *node)
{
    TreeNode *p=node;
    while(p->parent != NULL){
     // cerr<<node->parent->value<<" "<<node->parent->Begin<<" "<<node->parent->Back<<" "<<node->parent->level<<endl;
      if(p->parent->children.size()>1)return p;
      else p = p->parent;
    }
    return p;
}
TreeNode* FeatureForBuild::GetHeadNode(int beg,int back)
{//不可能为NULL
    TreeNode *p = mySynTree->GetNodeWithId(beg);
    while(p->Back<back){
        if(p->parent!=NULL)p=p->parent;
        else return &mySynTree->root;
    }
    return GetHighestNode(p);
}
TreeNode *FeatureForBuild::GetDominantNode(int beg,int back)
{//可能为NULL
    TreeNode *p = GetHeadNode(beg,back);
    if(p->parent!=NULL)
    return p->parent;
    else return NULL;
}
TreeNode *FeatureForBuild::GetDominantNode(TreeNode* node)
{
    if(node->parent!=NULL)
    return node->parent;
    else return NULL;
}
int FeatureForBuild::GetLevel(TreeNode*node)
{
    if(node==NULL)return -1;
    return node->level;
}
int FeatureForBuild::DominanceRelation(TreeNode*a,TreeNode*b)
{
    int al=GetLevel(a);int bl=GetLevel(b);
    if(al<bl)return 0;else if(al>bl)return 1;else return 2;
}
string FeatureForBuild::GetNodeHeadWord(TreeNode*node)
{
    return sentenceVec[headWordMap[node]];
}
TreeNode* FeatureForBuild::GetLeftSibling(TreeNode*node)
{//可能为NULL
    if(node==NULL)return NULL;
    return node->leftSibling;
}
TreeNode* FeatureForBuild::GetRightSibling(TreeNode*node)
{//可能为NULL
    if(node==NULL)return NULL;
    return node->rightSibling;
}
int FeatureForBuild::GetDelta(TreeNode*a,TreeNode*b)
{
    int al=GetLevel(a);int bl=GetLevel(b);
    if(al>bl)return al-bl;
    else return bl-al;
}
string FeatureForBuild::GetUniGram(int beg,TreeNode* node)
{
    if(node!=NULL){
        if(node->Begin<0 || node->Begin>=sentenceVec.size())return "NULL";
        return sentenceVec[node->Begin];}
    else{
        if(beg<0 || beg>=sentenceVec.size())return "NULL";
        return sentenceVec[beg];
    }
}
string FeatureForBuild::GetBiGram(int beg,int back,TreeNode* node)
{
    if(node!=NULL){
        if(node->Begin<0 || node->Back>=sentenceVec.size())return "NULL";
        if(node->Back!=node->Begin)return sentenceVec[node->Begin]+"/"+sentenceVec[node->Begin+1];
            //return sentenceVec[node->Begin+1];
        else return GetUniGram(beg,node);}
    else{
        if(beg<0 || beg>=sentenceVec.size())return "NULL";
        if(back!=beg)return sentenceVec[beg]+"/"+sentenceVec[beg+1];
            //return sentenceVec[beg+1];
        else return GetUniGram(beg,node);
    }
}
string FeatureForBuild::GetTriGram(int beg,int back,TreeNode* node)
{
    if(node!=NULL){
        if(node->Begin<0 || node->Back>=sentenceVec.size())return "NULL";
        if(node->Back-node->Begin>1)return sentenceVec[node->Begin]+"/"+sentenceVec[node->Begin+1]+"/"+sentenceVec[node->Begin+2];
           // return sentenceVec[node->Begin+2];
        else return GetBiGram(beg,back,node);}
    else{
        if(beg<0 || beg>=sentenceVec.size())return "NULL";
        if(back>beg+1)return sentenceVec[beg]+"/"+sentenceVec[beg+1]+"/"+sentenceVec[beg+2];
            //return sentenceVec[beg+2];
        else return GetBiGram(beg,back,node);
    }
}
string FeatureForBuild::GetUniGramBack(int back,TreeNode* node)
{
    if(node!=NULL){
        if(node->Back<0 || node->Back>=sentenceVec.size())return "NULL";
        return sentenceVec[node->Back];}
    else{
        if(back<0 || back>=sentenceVec.size())return "NULL";
        return sentenceVec[back];
    }
}
string FeatureForBuild::GetBiGramBack(int beg,int back,TreeNode* node)
{
    if(node!=NULL){
        if(node->Begin<0 || node->Back>=sentenceVec.size())return "NULL";
        if(node->Back!=node->Begin)return sentenceVec[node->Back-1]+"/"+sentenceVec[node->Back];
           // return sentenceVec[node->Back-1];
        else return GetUniGramBack(back,node);}
    else{
        if(back<0 || back>=sentenceVec.size())return "NULL";
        if(back!=beg)return sentenceVec[back-1]+"/"+sentenceVec[back];
            //return sentenceVec[back-1];
        else return GetUniGramBack(back,node);
    }
}
string FeatureForBuild::GetTriGramBack(int beg,int back,TreeNode* node)
{
    if(node!=NULL){
        if(node->Begin<0 || node->Back>=sentenceVec.size())return "NULL";
        if(node->Back-node->Begin>1)return sentenceVec[node->Back-2]+"/"+sentenceVec[node->Back-1]+"/"+sentenceVec[node->Back];
            //return sentenceVec[node->Back-2];
        else return GetBiGramBack(beg,back,node);}
    else{
        if(back<0 || back>=sentenceVec.size())return "NULL";
        if(back>beg+1)return sentenceVec[back-2]+"/"+sentenceVec[back-1]+"/"+sentenceVec[back];
            //return sentenceVec[back-2];
        else return GetBiGramBack(beg,back,node);
    }
}
string FeatureForBuild::GetUniPos(int beg)
{
     if(beg<0 || beg>=sentenceVec.size())return "NULL";
        return mySynTree->GetNodeWithId(beg)->parent->value;
}

string FeatureForBuild::GetBiPos(int beg, int back)
{
    if(beg<0 || back>=sentenceVec.size())return "NULL";
    if(back!=beg)return mySynTree->GetNodeWithId(beg)->parent->value+"/"+mySynTree->GetNodeWithId(beg+1)->parent->value;
        //return sentenceVec[beg+1];
        else return GetUniPos(beg);
}
string FeatureForBuild::GetTriPos(int beg,int back)
{
 if(beg<0 || back>=sentenceVec.size())return "NULL";
        if(back>beg+1)return mySynTree->GetNodeWithId(beg)->parent->value+"/"+mySynTree->GetNodeWithId(beg+1)->parent->value+"/"+mySynTree->GetNodeWithId(beg+2)->parent->value;
            //return sentenceVec[beg+2];
        else return GetBiPos(beg,back);
}
string FeatureForBuild::GetUniBackPos(int back)
{
if(back<0 || back>=sentenceVec.size())return "NULL";
        return mySynTree->GetNodeWithId(back)->parent->value;
}
string FeatureForBuild::GetBiBackPos(int beg,int back)
{
if(back<0 || back>=sentenceVec.size())return "NULL";
        if(back!=beg)return mySynTree->GetNodeWithId(back-1)->parent->value+"/"+mySynTree->GetNodeWithId(back)->parent->value;
            //return sentenceVec[back-1];
        else return GetUniBackPos(back);
}
string FeatureForBuild::GetTriBackPos(int beg,int back)
{
 if(back<0 || back>=sentenceVec.size())return "NULL";
        if(back>beg+1)return mySynTree->GetNodeWithId(back-2)->parent->value+"/"+mySynTree->GetNodeWithId(back-1)->parent->value+"/"+mySynTree->GetNodeWithId(back)->parent->value;
            //return sentenceVec[back-2];
        else return GetBiBackPos(beg,back);
}

string FilterNodeValue(TreeNode*a)
{
  if(a==NULL||a==0x0)
  {
    return "NULL";
  }
  if(a->value=="")
    return "NULL";
  if(a!=NULL && a!=0x0){
  //    cerr<<a->value<<"AAA"<<endl;
      return a->value;
  }
}
void FeatureForBuild::ExtractSingleSpanFeature(int beg,int back, EduFeature& edu)
{
    edu.Begin = beg;edu.Back = back;
    edu.eduNum = GetEduNumers(beg,back);
    edu.nodeHead = GetHeadNode(beg,back);
    edu.nodeDomin = GetDominantNode(edu.nodeHead);
    edu.nHLabel = FilterNodeValue(edu.nodeHead); //cerr<<edu.nHLabel<<endl; cerr<<headWordMap[edu.nodeHead]<<endl;
    edu.nHWord = sentenceVec[headWordMap[edu.nodeHead]];
    edu.nDLabel = FilterNodeValue(edu.nodeDomin);
    edu.nDWord = sentenceVec[headWordMap[edu.nodeDomin]];
    edu.nHLevel = GetLevel(edu.nodeHead);
    edu.uniGram = GetUniGram(beg);
    edu.biGram = GetBiGram(beg,back);
    edu.triGram = GetTriGram(beg,back);
    edu.uniGramBack = GetUniGramBack(back);
    edu.biGramBack = GetBiGramBack(beg,back);
    edu.triGramBack = GetTriGramBack(beg,back);
    edu.uniPOS  = GetUniPos(beg);
    edu.biPOS = GetBiPos(beg,back);
    edu.triPOS = GetTriPos(beg,back);
    edu.uniBackPOS = GetUniBackPos(back);
    edu.biBackPOS = GetBiBackPos(beg,back);
    edu.triBackPOS = GetTriBackPos(beg,back);

}
void FeatureForBuild::ExtractSingleSpanFeature(FullFeature&pre, EduFeature& edu)
{
 //   cerr<<"print full feature: ";PrintFull(pre,0);
    edu.Begin = pre.Begin;edu.Back = pre.Back;
    edu.eduNum = pre.eduA->eduNum+pre.eduB->eduNum;
    edu.nodeHead = pre.nodeHead;
    edu.nodeDomin = pre.nodeDomin;
    edu.nHLabel = pre.nHLabel;
    edu.nHWord = pre.nHWord;
    edu.nDLabel = pre.nDLabel;
    edu.nDWord = pre.nDWord;
    edu.nHLevel = pre.nHLevel;
    edu.uniGram = GetUniGram(edu.Begin);
    edu.biGram = GetBiGram(edu.Begin,edu.Back);
    edu.triGram = GetTriGram(edu.Begin,edu.Back);
    edu.uniGramBack = GetUniGramBack(edu.Back);
    edu.biGramBack = GetBiGramBack(edu.Begin,edu.Back);
    edu.triGramBack = GetTriGramBack(edu.Begin,edu.Back);
    edu.uniPOS  = GetUniPos(edu.Begin);
    edu.biPOS = GetBiPos(edu.Begin,edu.Back);
    edu.triPOS = GetTriPos(edu.Begin,edu.Back);
    edu.uniBackPOS = GetUniBackPos(edu.Back);
    edu.biBackPOS = GetBiBackPos(edu.Begin,edu.Back);
    edu.triBackPOS = GetTriBackPos(edu.Begin,edu.Back);

}
void FeatureForBuild::ExtractFullFeature(EduFeature&eduA,EduFeature&eduB,FullFeature&fSpan)
{
//    cerr<<"eduA: ";PrintEdu(eduA,0);
//    cerr<<endl;
//    cerr<<"eduB: ";PrintEdu(eduB,0);
//    cerr<<endl;
    fSpan.Begin=(eduA.Begin);fSpan.Back=(eduB.Back);
    fSpan.nodeHead = GetHeadNode(fSpan.Begin,fSpan.Back);
    fSpan.nodeDomin = GetDominantNode(fSpan.nodeHead);
    fSpan.nDLabel = FilterNodeValue(fSpan.nodeDomin);
    fSpan.nDWord = sentenceVec[headWordMap[fSpan.nodeDomin]];

    TreeNode *left = GetLeftSibling(fSpan.nodeDomin);
    fSpan.nDLeftSiblingLabel = FilterNodeValue(left);
    TreeNode *right = GetRightSibling(fSpan.nodeDomin);
    fSpan.nDRightSiblingLabel = FilterNodeValue(right);
    fSpan.nHLabel = FilterNodeValue(fSpan.nodeHead);
    fSpan.nHWord = sentenceVec[headWordMap[fSpan.nodeHead]];
    fSpan.nHLevel = GetLevel(fSpan.nodeHead);
    fSpan.AdeltaLh = GetDelta(eduA.nodeHead,fSpan.nodeHead);
    fSpan.BdeltaLh = GetDelta(eduB.nodeHead,fSpan.nodeHead);
    fSpan.delta = GetDelta(eduA.nodeHead,eduB.nodeHead);
    fSpan.dominRel = DominanceRelation(eduA.nodeDomin,eduB.nodeDomin);

    fSpan.eduA = &eduA; fSpan.eduB = &eduB;
    //PrintFull(fSpan,0);
}
void FeatureForBuild::ExtractSingleSpanFeatureDecode(int beg,int back, EduFeature& edu,int level)
{
    edu.Begin = beg;edu.Back = back;
    edu.eduNum = level+1;
    edu.nodeHead = GetHeadNode(beg,back);
    edu.nodeDomin = GetDominantNode(edu.nodeHead);
    edu.nHLabel = FilterNodeValue(edu.nodeHead); //cerr<<edu.nHLabel<<endl; cerr<<headWordMap[edu.nodeHead]<<endl;
    edu.nHWord = sentenceVec[headWordMap[edu.nodeHead]];
    edu.nDLabel = FilterNodeValue(edu.nodeDomin);
    edu.nDWord = sentenceVec[headWordMap[edu.nodeDomin]];
    edu.nHLevel = GetLevel(edu.nodeHead);
    edu.uniGram = GetUniGram(beg);
    edu.biGram = GetBiGram(beg,back);
    edu.triGram = GetTriGram(beg,back);
    edu.uniGramBack = GetUniGramBack(back);
    edu.biGramBack = GetBiGramBack(beg,back);
    edu.triGramBack = GetTriGramBack(beg,back);
    edu.uniPOS  = GetUniPos(beg);
    edu.biPOS = GetBiPos(beg,back);
    edu.triPOS = GetTriPos(beg,back);
    edu.uniBackPOS = GetUniBackPos(back);
    edu.biBackPOS = GetBiBackPos(beg,back);
    edu.triBackPOS = GetTriBackPos(beg,back);
}
void FeatureForBuild::ExtractPositiveInstance(RTreeNode* node,Feature &nodeFeat )
{
    if(node->children.empty()){
        return;
    }
  //  cerr<<node->children.size()<<endl;
    if(node->children[0]->children.empty()){
        ExtractSingleSpanFeature(node->children[0]->Begin,node->children[0]->Back,nodeFeat.left);

    }
    else{
        Feature p;
        ExtractPositiveInstance(node->children[0],p);
//        cerr<<p.both.Begin<<p.both.Back;
//        cerr<<p.both.nDLabel;
        ExtractSingleSpanFeature(p.both,nodeFeat.left);
    }
    if(node->children.size()==1){
        nodeFeat.right.defaultValue();return;
        }
    else{
    if(node->children[1]->children.empty()){
 //       cerr<<endl<<node->children[1]->Begin<<" "<<node->children[1]->Back<<" "<<node->value<<" "<<node->children[1]->value <<endl;
        ExtractSingleSpanFeature(node->children[1]->Begin,node->children[1]->Back,nodeFeat.right);
    }
    else{
        Feature t;  ExtractPositiveInstance(node->children[1],t);
        ExtractSingleSpanFeature(t.both,nodeFeat.right);
    }
    }
    ExtractFullFeature(nodeFeat.left,nodeFeat.right,nodeFeat.both);
    node->children[0]->LeftOrRight=0; node->children[1]->LeftOrRight=1;
    //posFeatVec.push_back(&nodeFeat);
    posFeatMap[node] = nodeFeat;
  //  PrintFull(nodeFeat.both,0);
  //  return &nodeFeat;
}
void FeatureForBuild::NegativeInstance(RTreeNode* a,RTreeNode*b)
{
    Feature nodeFeat;
    ExtractSingleSpanFeature(a->Begin,a->Back,nodeFeat.left);
    ExtractSingleSpanFeature(b->Begin,b->Back,nodeFeat.right);
    ExtractFullFeature(nodeFeat.left,nodeFeat.right,nodeFeat.both);
    negFeatVec.push_back(nodeFeat);
}
void FeatureForBuild::ExtractNegFeature(RTreeNode *node)
{
    if(node->children.empty())return;

    RTreeNode *left = node->children[0];
    RTreeNode *right = node->children.back();
//    cerr<<left->Begin<<" "<<left->Back<<endl;
//    cerr<<right->Begin<<" "<<right->Back<<endl;

//    if(!left->children.empty()){
//    while(!left->children.empty()){
//    while(!right->children.empty()){
//        right = right->children[0];
//        NegativeInstance(left,right);
//        }
//        left = left->children.back();
//        right = node->children.back();
//    }}
  //  else{
        while(!right->children.empty()){
        right = right->children[0];
        NegativeInstance(left,right);
        }
 //   }
    left = node->children[0];
    right = node->children.back();

    while(!left->children.empty()){
        left = left->children.back();
        NegativeInstance(left,right);
//        if(!right->children.empty())
//            NegativeInstance(left,right->children[0]);
    }
    ExtractNegFeature(node->children[0]);
    if(node->children.size()>1)
    ExtractNegFeature(node->children.back());
}
void FeatureForBuild::LoadTestEduFeatMat()
{
    //initialization;
    testEduFeatMat.resize(baSpanVec.size(),vector<EduFeature>());
    scoreVec.resize(baSpanVec.size(),vector<float>());
    pathOfConstruct.resize(baSpanVec.size(),vector<int>());
    for(size_t i(0);i<baSpanVec.size();++i){//1 to n-1层
        testEduFeatMat[i].resize(baSpanVec.size()-i);
        scoreVec[i].resize(baSpanVec.size()-i,-999);
        pathOfConstruct[i].resize(baSpanVec.size()-i,0);
    }
    for(size_t i(0);i<scoreVec[0].size();++i){scoreVec[0][i]=0;}
     for(size_t i(0); i< baSpanVec.size()-1;++i){//0 to n-1 层,跨度从1 to n
        for (size_t k(0);k<baSpanVec.size()-i;++k){//第[i][k]考虑base[k].first to base[k+i].second
            EduFeature eduFeat;
           // cerr<<baSpanVec[k].first<<" "<<baSpanVec[k+i].second<<endl;
            ExtractSingleSpanFeatureDecode(baSpanVec[k].first,baSpanVec[k+i].second,eduFeat,i);
            testEduFeatMat[i][k]=(eduFeat);

        }
    }

}
void FeatureForBuild::LoadTestFeatMat()
{
}
void FeatureForBuild::ExtractTestInstance()
{

}
int PrintEdu(EduFeature&a, int count)
{
    cout<<count++<<"/"<<a.nHLabel<<" ";
    cout<<count++<<"/"<<a.nHWord<<" ";
    cout<<count++<<"/"<<a.nDLabel<<" ";
    cout<<count++<<"/"<<a.nDWord<<" ";
    cout<<count++<<"/"<<a.eduNum<<" ";cout<<count++<<"/"<<a.nHLevel<<" ";cout<<count++<<"/"<<a.uniGram<<" ";
    cout<<count++<<"/"<<a.biGram<<" ";
    cout<<count++<<"/"<<a.triGram<<" ";
    cout<<count++<<"/"<<a.uniGramBack<<" ";
    cout<<count++<<"/"<<a.biGramBack<<" ";
    cout<<count++<<"/"<<a.triGramBack<<" ";
//    cout<<count++<<"/"<<a.uniPOS<<" ";
//    cout<<count++<<"/"<<a.biPOS<<" ";
//    cout<<count++<<"/"<<a.triPOS<<" ";
//    cout<<count++<<"/"<<a.uniBackPOS<<" ";
//    cout<<count++<<"/"<<a.biBackPOS<<" ";
//    cout<<count++<<"/"<<a.triBackPOS<<" ";

//    cerr<<count++<<"/"<<a.nHLabel<<" "<<count++<<"/"<<a.nHWord<<" "<<count++<<"/"<<a.nDLabel<<" "<<count++<<"/"<<a.nDWord<<" ";
//    cerr<<count++<<"/"<<a.eduNum<<" "<<count++<<"/"<<a.nHLevel<<" "<<count++<<"/"<<a.uniGram<<" ";
//    cerr<<count++<<"/"<<a.biGram<<" "<<count++<<"/"<<a.triGram;
    return count;
}
void PrintFull(FullFeature&a,int count)
{
    cout<<count++<<"/"<<a.dominRel<<" ";
    cout<<count++<<"/"<<a.delta<<" ";
    cout<<count++<<"/"<<a.AdeltaLh<<" ";
    cout<<count++<<"/"<<a.BdeltaLh<<" ";
    cout<<count++<<"/"<<a.nDLabel<<" ";
    cout<<count++<<"/"<<a.nDWord<<" ";
    cout<<count++<<"/"<<a.nDLeftSiblingLabel<<" ";
    cout<<count++<<"/"<<a.nDRightSiblingLabel<<" ";
    cout<<count++<<"/"<<a.nHLabel<<" ";cout<<count++<<"/"<<a.nHWord;
//    cerr<<count++<<"/"<<a.dominRel<<" "<<count++<<"/"<<a.delta<<" "<<count++<<"/"<<a.AdeltaLh<<" "<<count++<<"/"<<a.BdeltaLh<<" "<<count++<<"/"<<a.nDLabel<<" ";
//    cerr<<count++<<"/"<<a.nDWord<<" "<<count++<<"/"<<a.nDLeftSiblingLabel<<" "<<count++<<"/"<<a.nDRightSiblingLabel<<" ";
//    cerr<<count++<<"/"<<a.nHLabel<<" "<<count++<<"/"<<a.nHWord;
}
void FeatureForBuild::PrintStrFeature()
{
    int count;
    map<RTreeNode*,Feature>::iterator it = posFeatMap.begin();
    while(it!=posFeatMap.end())
    {
     //   if(it->first->value.find(":")!=string::npos){it++;continue;}
       // cout<<it->first->value<<endl;
//       cerr<<it->first->value<<" "<<it->first->Begin<<" "<<it->first->Back<<" "<<endl;
//       cerr<<it->first->children[0]->Begin<<" "<<it->first->children[1]->Begin<<endl;
        cout<<"1 ";

        count=0;
        count = PrintEdu(it->second.left,count);cout<<" ";
        count = PrintEdu(it->second.right,count);cout<<" ";
        PrintFull(it->second.both,count);
        cout<<endl;
        it++;
    }
}
void FeatureForBuild::PrintStrFeature(vector<Feature> &vec)
{
    int count;
    vector<Feature>::iterator it = vec.begin();
    while(it!=vec.end())
    {
     //   if(it->first->value.find(":")!=string::npos){it++;continue;}
       // cout<<it->first->value<<endl;
        cout<<"0 ";
        count=0;
        count = PrintEdu((*it).left,count);cout<<" ";
        count = PrintEdu((*it).right,count);cout<<" ";
        PrintFull((*it).both,count);
        cout<<endl;
        it++;
    }
}
void FeatureForBuild::PrintLabelFeature()
{
    int count;
    map<RTreeNode*,Feature>::iterator it = posFeatMap.begin();
    while(it!=posFeatMap.end())
    {
        if(it->first->value.find(":")!=string::npos){cout<<"UKN ";}
        else
            cout<<(*it).first->value<<" ";
        count=0;
        cout<<count++<<"/"<<(*it).first->LeftOrRight<<" ";
//        if(it->second.left.Begin == 0&&it->second.right.Back==sentenceVec.size()-1)cout<<count++<<"/"<<"0 ";
//        else cout<<count++<<"/"<<"1 ";
        count = PrintEdu(it->second.left,count);cout<<' ';
        count = PrintEdu(it->second.right,count);cout<<' ';
        if((*it).first->LeftOrRight ==0)
            PrintEdu((posFeatMap[it->first->parent]).right,count);
        else if((*it).first->LeftOrRight == 1)
            PrintEdu(posFeatMap[it->first->parent].left,count);
        else
            cout<<count++<<"/"<<"S";
        cout<<endl;
        it++;
    }
    cout<<endl;
}

string Str(int a)
{
    return boost::lexical_cast<string>(a);
}
int FeatureForBuild::GetStructureFeatForEdu(EduFeature&a,int count,vector<string>& vec)
{
    vec.push_back(Str(count++)+"/"+a.nHLabel);vec.push_back(Str(count++)+"/"+a.nHWord);
    vec.push_back(Str(count++)+"/"+a.nDLabel);vec.push_back(Str(count++)+"/"+a.nDWord);
    vec.push_back(Str(count++)+"/"+Str(a.eduNum));vec.push_back(Str(count++)+"/"+Str(a.nHLevel));
    vec.push_back(Str(count++)+"/"+a.uniGram);vec.push_back(Str(count++)+"/"+a.biGram);
    vec.push_back(Str(count++)+"/"+a.triGram);
    vec.push_back(Str(count++)+"/"+a.uniGramBack);
    vec.push_back(Str(count++)+"/"+a.biGramBack);
    vec.push_back(Str(count++)+"/"+a.triGramBack);
//    vec.push_back(Str(count++)+"/"+a.uniPOS);vec.push_back(Str(count++)+"/"+a.biPOS);
//    vec.push_back(Str(count++)+"/"+a.triPOS);
//    vec.push_back(Str(count++)+"/"+a.uniBackPOS);
//    vec.push_back(Str(count++)+"/"+a.biBackPOS);
//    vec.push_back(Str(count++)+"/"+a.triBackPOS);
    return count;
}
int FeatureForBuild::GetStructureFeatForFull(FullFeature&a,int count,vector<string>&vec)
{
    vec.push_back(Str(count++)+"/"+Str(a.dominRel));vec.push_back(Str(count++)+"/"+Str(a.delta));
    vec.push_back(Str(count++)+"/"+Str(a.AdeltaLh));vec.push_back(Str(count++)+"/"+Str(a.BdeltaLh));
    vec.push_back(Str(count++)+"/"+a.nDLabel);vec.push_back(Str(count++)+"/"+a.nDWord);
    vec.push_back(Str(count++)+"/"+a.nDLeftSiblingLabel);vec.push_back(Str(count++)+"/"+a.nDRightSiblingLabel);
    vec.push_back(Str(count++)+"/"+a.nHLabel);vec.push_back(Str(count++)+"/"+a.nHWord);
    return count;
}
void FeatureForBuild::GetStructureFeat(Feature&a, vector<string>& vec)
{
    int count(0);
    count = GetStructureFeatForEdu(a.left,count,vec);
    count = GetStructureFeatForEdu(a.right,count,vec);
    count = GetStructureFeatForFull(a.both,count,vec);
}
void FeatureForBuild::GetStructureFeat(EduFeature &a,EduFeature&b, FullFeature&c,vector<string>& vec)
{
    int count(0);
    count = GetStructureFeatForEdu(a,count,vec);
    count = GetStructureFeatForEdu(b,count,vec);
    count = GetStructureFeatForFull(c,count,vec);
}
void FeatureForBuild::GetLabelFeat(EduFeature &a, EduFeature&b,EduFeature &c,int leftorright, vector<string>& vec)
{
    int count(0);
  //  vec.push_back(Str(count++)+"/"+Str(leftorright));
    vec.push_back(Str(count++)+"/"+Str(leftorright));
    count = GetStructureFeatForEdu(a,count,vec);
    count = GetStructureFeatForEdu(b,count,vec);
//    if(leftorright==0 || leftorright==1)
//       count = GetStructureFeatForEdu(c,count,vec);
//    else
//        vec.push_back(Str(count)+"/S");
}
void FeatureForBuild::GetLabelFeat(Feature&a, EduFeature&b, int leftorright, vector<string>&vec)
{
    int count(0);
    vec.push_back(Str(count++)+"/"+Str(leftorright));
    count = GetStructureFeatForEdu(a.left,count,vec);
    count = GetStructureFeatForEdu(a.right,count,vec);
  //  count = GetStructureFeatForEdu(b,count,vec);
}
double FeatureForBuild::ScoreForStructure(vector<string> &featVec,SegScore &strModel)
{
//    copy(featVec.begin(),featVec.end(),ostream_iterator<string,char>(cerr," "));
//    cerr<<endl;
    vector<pair<string,double> > featVecMEFomat(featVec.size());
    vector<pair<string,double> > outcome;
  //  cerr<<featVec.size()<<endl;
    for(size_t i(0);i<featVec.size();++i){
        featVecMEFomat[i] = make_pair(featVec[i],1.0);
    }
    strModel.eval_all(featVecMEFomat,outcome);
    for(size_t i(0);i<outcome.size();++i){
        if(outcome[i].first=="1")return outcome[i].second;
    }
    return -999;
}
string FeatureForBuild::GetDeocdeLabel(vector<string> &featVec,SegScore&labModel)
{
    vector<pair<string,double> > featVecMEFomat(featVec.size());
    vector<pair<string,double> > outcome;
  //  cerr<<featVec.size()<<endl;
     for(size_t i(0);i<featVec.size();++i){
        featVecMEFomat[i] = make_pair(featVec[i],1.0);
    }
    labModel.eval_all(featVecMEFomat,outcome);
    string tmp=outcome[0].first; double tmpscore= outcome[0].second;
     for(size_t i(1);i<outcome.size();++i){
        if(outcome[i].second>tmpscore){
            tmp = outcome[i].first;
            tmpscore = outcome[i].second;
        }
    }
    return tmp;
}
double FeatureForBuild::ScoreforDeocdeLabel(vector<string> &featVec,SegScore&labModel)
{
    vector<pair<string,double> > featVecMEFomat(featVec.size());
    vector<pair<string,double> > outcome;
  //  cerr<<featVec.size()<<endl;
     for(size_t i(0);i<featVec.size();++i){
        featVecMEFomat[i] = make_pair(featVec[i],1.0);
    }
    labModel.eval_all(featVecMEFomat,outcome);
    string tmp=outcome[0].first; double tmpscore= outcome[0].second;
     for(size_t i(1);i<outcome.size();++i){
        if(outcome[i].second>tmpscore){
            tmp = outcome[i].first;
            tmpscore = outcome[i].second;
        }
    }
    return tmpscore;
}
int FeatureForBuild::CYKDecode_Search(SegScore &strModel,SegScore &lbModel)
{
    vector<string> strFeatVec;vector<string> labFeatVec;
    for(size_t i(1);i<testEduFeatMat.size();++i){//从跨度2 到 n, 1 层 到 n-1层
       for(size_t j(0);j<scoreVec[i].size();++j){//每一层的方格
           double tmpStrScore,tmpLabScore;
           for(size_t k(0);k<i;++k){ // 从0层到i-1层
               FullFeature full;
               ExtractFullFeature(testEduFeatMat[k][j],testEduFeatMat[i-1-k][j+1+k],full);
               vector<string>().swap(strFeatVec);vector<string>().swap(labFeatVec);
               GetStructureFeat(testEduFeatMat[k][j],testEduFeatMat[i-1-k][j+1+k],full,strFeatVec);
               //预测结构的同时预测标签
               EduFeature c;c.defaultValue(); int isStart=testEduFeatMat[k][j].Begin==0&&testEduFeatMat[i-1-k][j+1+k].Back==sentenceVec.size()-1?0:1;
               GetLabelFeat(testEduFeatMat[k][j],testEduFeatMat[i-1-k][j+1+k],c,isStart,labFeatVec);
                tmpStrScore = scoreVec[k][j]+scoreVec[i-1-k][j+1+k]+ScoreForStructure(strFeatVec,strModel);
               //tmpStrScore = scoreVec[k][j]+scoreVec[i-1-k][j+1+k]+ScoreForStructure(strFeatVec,strModel)+ScoreforDeocdeLabel(labFeatVec,lbModel); //计算后验概率

               if(tmpStrScore > scoreVec[i][j]){
                 scoreVec[i][j]=tmpStrScore;
                 pathOfConstruct[i][j]=k;
               }
           }}}
    return 1;
}
void FeatureForBuild::ConstructDecodeTree(int curLevel,int curIndex,int sibLevel,int sibIndex,int leftorright,SegScore&labModel,RTreeNode *node)
{
    if(curLevel==0){
        node->Begin = baSpanVec[curIndex].first;node->Back=baSpanVec[curIndex].second;
        node->value="0";
        return;
    }
    //判断label
    vector<string> labFeatVec;
    int leftLevel,leftIndex,rightLevel,rightIndex;
    leftLevel = pathOfConstruct[curLevel][curIndex]; leftIndex = curIndex;
    rightLevel = curLevel-1-leftLevel;rightIndex = curIndex+1+leftLevel;

    int isStart = testEduFeatMat[leftLevel][leftIndex].Begin==0&&testEduFeatMat[rightLevel][rightIndex].Back==sentenceVec.size()-1?0:1;
    if(sibLevel>=0 && sibIndex>=0)
      GetLabelFeat(testEduFeatMat[leftLevel][leftIndex],testEduFeatMat[rightLevel][rightIndex],testEduFeatMat[sibLevel][sibIndex],leftorright,labFeatVec);
    else{
      EduFeature a;a.defaultValue();
      GetLabelFeat(testEduFeatMat[leftLevel][leftIndex],testEduFeatMat[rightLevel][rightIndex],a,leftorright,labFeatVec);
    }
    string label = GetDeocdeLabel(labFeatVec,labModel);
    vector<string>().swap(labFeatVec);
    node->value=label;

    RTreeNode *leftnode=new RTreeNode("",node);RTreeNode *rightnode=new RTreeNode("",node);
    //注意leftorright的取值

    ConstructDecodeTree(leftLevel,leftIndex,rightLevel,rightIndex,0,labModel,leftnode);
    ConstructDecodeTree(rightLevel,rightIndex,leftLevel,leftIndex,1,labModel,rightnode);
    node->Begin = leftnode->Begin;node->Back = rightnode->Back;
   // cerr<<"left: "<<leftnode->Begin<<" "<<leftnode->Back<<endl;
   // cerr<<"right "<<rightnode->Begin<<" "<<rightnode->Back<<endl;
    node->children.push_back(leftnode);node->children.push_back(rightnode);

}
RTreeNode FeatureForBuild::CYKDecode(SegScore &strModel,SegScore&labModel,SegScore&labModel1)
{
    CYKDecode_Search(strModel,labModel1);
    RTreeNode testroot("",NULL);
    ConstructDecodeTree(pathOfConstruct.size()-1,0,-1,-1,-1,labModel,&testroot);
    return testroot;
}
void FeatureForBuild::LabelingForStdStr(SegScore &lbModel)
{
    int count;
    map<RTreeNode*,Feature>::iterator it = posFeatMap.begin();
    vector<string> lbFeatVec;
    while(it!=posFeatMap.end())
    {

        if(it->first->LeftOrRight==0)
        GetLabelFeat(it->second.left,it->second.right,posFeatMap[it->first->parent].right,0,lbFeatVec);
        else if(it->first->LeftOrRight==1)
        GetLabelFeat(it->second.left,it->second.right,posFeatMap[it->first->parent].left,1,lbFeatVec);
        else{
        EduFeature a;a.defaultValue();
        GetLabelFeat(it->second.left,it->second.right,a,-1,lbFeatVec);
        }
        string label = GetDeocdeLabel(lbFeatVec,lbModel);
        it->first->value = label;
        vector<string>().swap(lbFeatVec);
        it++;
    }

}

int FeatureForBuild::TrainingProcess(const string&m_pureStr,const string&m_relStr,const string& m_synStr)
{
    if (InitTrain(m_pureStr,m_relStr,m_synStr)==-1)return -1;
    Feature rootf;
    ExtractPositiveInstance(&myRelTree->root,rootf);
   // cerr<<posFeatMap.size()<<endl;
    ExtractNegFeature(&myRelTree->root);
  //  cerr<<negFeatVec.size()<<endl;
//    PrintStrFeature();
//    PrintStrFeature(negFeatVec);
   PrintLabelFeature();
    return 1;
}
int FeatureForBuild::TestingProcess(const string&m_pureStr,const string&m_segStr,const string& m_synStr, SegScore &Strm1,SegScore &Lbm2,SegScore &Lbm3)
{
    if(InitTest(m_pureStr,m_synStr,m_segStr)==-1)return -1;
    Feature rootf;
    ExtractTestInstance();
    //decoding....
    //
    RTreeNode root = CYKDecode(Strm1,Lbm2,Lbm3);
   // SynTreeSQ sqtree;sqtree.sentenceVec = sentenceVec;
    cout<<"["<<root.value;
    myRelTree->PrintTreeOri(&root);
    cout<<" ]";
    cout<<endl;
    return 1;
}
int FeatureForBuild::TestingLabelForStdStr(const string&m_pureStr,const string&m_relStr,const string& m_synStr,SegScore &Lbm2)
{
    if (InitTrain(m_pureStr,m_relStr,m_synStr)==-1)return -1;
    Feature rootf;
    ExtractPositiveInstance(&myRelTree->root,rootf);
    LabelingForStdStr(Lbm2);
    myRelTree->PrintTreeOri(&myRelTree->root);
    cout<<endl;
    return 1;

}


#endif // FEATUREFORBUILD_CPP_INCLUDED
