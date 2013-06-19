#include"SynTreeSQ.h"
#include"FeatureForBuild.h"

using namespace std;

void test(char *filename)
{
  cerr<<"used for get segmentation file\n";
//  string teststr="[zj-XX [fj-YG [dj-ZW [np-DZ 家用/b  [np-DZ 电脑/n  游戏/n  ] ] [vp-XX 由于/c  [vp-LW [vp-PO 使用/v  [np-DZ [np-AD [np-LH 键盘/n  、/、  鼠标/n  ] 等/u  ] [np-DZ 非/b  [np-DZ 游戏/n  [np-DZ 专用/vN  设备/n  ] ] ] ] ] 操作/v  ] ] ] ，/，  [dj-ZW 手感/n  [vp-PO 不如/vC  家用机/n  ] ] ] 。/。  ] ";
  ifstream in(filename);
  if(!in){cerr<<"wrong to open\n";exit(1);}
  string line;
  InitKeySet();
  while(getline(in,line))
  {
    line = line.substr(line.find(" "));
    SynTreeSQ obj;
    obj.ParseTree(line,0);
  }
}
void test()
{
  string line="[UKN [0: 据/p 研究/vN 资料/n 表明/v ，/， ]\
   [1: [ZE [0: [ZE [0: 儿童/n 头脑/n 里/f 有/v 一/m 种/qN 迅速/aD 掌握/v 语言/n 的/u 前定/b 能力/n ]\
    [1: ，/， ] [2: 但/c 它/rN 会/vM 随/p 年龄/n 的/u 增长/vN （/（ 大多/d 到/p 小学/n ３/m 年级/qN 以后/f ）/） 逐渐/d 消退/v ] ] ]\
     [1: ，/， ] [2: 而/c 我们/rN 的/u 小学/n 往往/d 从/p ４/m 年级/qN 才/d 开始/v 设置/v 英语课/n ] ] ] [2: 。/。 ] ]";

  InitKeySet();

    SynTreeSQ obj;
    obj.ParseTree(line,0);
}
void test1(const char*purefile,const char* synfile, const char* relfile)
{
    ifstream in(purefile); ifstream in1(synfile);ifstream in2(relfile);
    if(!in || !in1 || !in2){cerr<<"wrong to open\n";exit(1);}



    string l,l1,l2; int count(0);
    InitKeySet();

    while(getline(in,l) && getline(in1,l1) && getline(in2,l2) )
    {
        if(count++<0)continue;
        cerr<<count<<endl;
        FeatureForBuild obj;
        if(obj.TrainingProcess(l,l2,l1)==-1)continue;
       // else {}
        obj.ClearAll();

    }
}
void binarization(const char* relfile)
{
    ifstream in(relfile);
    string line;
    InitKeySet();int count(0);
    while(getline(in,line))
    {
        if(count++<0)continue;
        SynTreeSQ obj;
        obj.ParseTree(line,0);
        obj.Binarization(&obj.root);
        if(obj.root.children.empty())cout<<line;
        else
        obj.PrintRelTree_1(&obj.root,0);
        cout<<endl;
    }
}
void TestPro(const char*purefile,const char* synfile, const char* segfile,const char *strMEModel,const char*lbMEModel,const char*lbMEModel1)
{
    ifstream in(purefile); ifstream in1(synfile);ifstream in2(segfile);
    if(!in || !in1 || !in2){cerr<<"wrong to open\n";exit(1);}

    string l,l1,l2; int count(0);
    //InitKeySet();
     SegScore strObj(strMEModel);
    SegScore lbObj(lbMEModel);
    SegScore lbObj1(lbMEModel1);
    while(getline(in,l) && getline(in1,l1) && getline(in2,l2) )
    {
        if(count++<0)continue;
        FeatureForBuild obj;
        if(obj.TestingProcess(l,l2,l1,strObj,lbObj,lbObj1)==-1){
            copy(obj.myRelTree->sentenceVec.begin(),obj.myRelTree->sentenceVec.end(),ostream_iterator<string,char>(cout," "));
            cout<<endl;continue;}
       // else {}
        obj.ClearAll();
        cerr<<count<<endl;
    }
}
void evaluationforstr(const char* canfile,const char*reffile)
{
    ifstream in(canfile); ifstream in1(reffile);
    if(!in || !in1 ){cerr<<"wrong to open\n";exit(1);}

    string l,l1; int match(0);int can(0);int ref(0);
    set<string> refset,canset;
    while(getline(in,l)&&getline(in1,l1)){
        SynTreeSQ canobj;
        SynTreeSQ refobj;
        canobj.ParseTree(l,0);
        refobj.ParseTree(l1,0);
        if(canobj.root.children.empty())continue;
        refset.clear();canset.clear();
        canobj.Convert2Tuple(&canobj.root,canset);
        refobj.Convert2Tuple(&refobj.root,refset);

        can += canset.size();
        ref += refset.size();

        float tmpm = refobj.EvaluationForStr(canset,refset);
        match += tmpm;
        cerr<<tmpm<<" "<<canset.size()<<endl;
    }
     cerr<< "matched= "<<match<<endl;
    cerr<<"all = "<<can<<endl;
    cerr<<"precision= "<<float(match)/can<<endl;
}
void TestLabelStd(const char*purefile,const char* synfile, const char* relfile,const char*lbMEModel)
{
    ifstream in(purefile); ifstream in1(synfile);ifstream in2(relfile);
    if(!in || !in1 || !in2){cerr<<"wrong to open\n";exit(1);}

    string l,l1,l2; int count(0);
    //InitKeySet();
    SegScore lbObj(lbMEModel);

    while(getline(in,l) && getline(in1,l1) && getline(in2,l2) )
    {
        if(count++<0)continue;
        FeatureForBuild obj;
        if(obj.TestingLabelForStdStr(l,l2,l1,lbObj)==-1){
            copy(obj.myRelTree->sentenceVec.begin(),obj.myRelTree->sentenceVec.end(),ostream_iterator<string,char>(cout," "));
            cout<<endl;continue;}
       // else {}
        obj.ClearAll();
        cerr<<count<<endl;
    }
}
void evaluationforLabel(const char* canfile,const char*reffile)
{
    ifstream in(canfile); ifstream in1(reffile);
    if(!in || !in1 ){cerr<<"wrong to open\n";exit(1);}
    int count(0);
    string l,l1; int match(0);int can(0);int ref(0);
    set<string> refset,canset;
    EvaluateStruct evs;
    while(getline(in,l)&&getline(in1,l1)){
        if(count++<0)continue;
        SynTreeSQ canobj;
        SynTreeSQ refobj;
        canobj.ParseTree(l,0);
        refobj.ParseTree(l1,0);
        refset.clear();canset.clear();
        if(canobj.root.children.empty())continue;

//        canobj.Convert2TupleComplete(&canobj.root,canset);
//        refobj.Convert2TupleComplete(&refobj.root,refset);
        canobj.Convert2TupleCompleteNonBin(&canobj.root,canset);
        refobj.Convert2TupleCompleteNonBin(&refobj.root,refset);
        can += canset.size();
        ref += refset.size();

        float tmpm = refobj.EvaluationForStr(canset,refset,evs);
        match += tmpm;
        cerr<<count<<" "<<tmpm<<" "<<canset.size()<<" "<<refset.size()<<endl;
    }
     cerr<< "matched= "<<match<<endl;
    cerr<<"candi all = "<<can<<endl;
    cerr<<"ref all = "<<ref<<endl;
    cerr<<"precision= "<<float(match)/can<<endl;
    cerr<<"recall= "<<float(match)/ref<<endl;
    cerr<<"F1= "<<2*float(match)/can*float(match)/ref/(float(match)/can+float(match)/ref)<<endl;
    map<string,pair<int,int> >::iterator iter=evs.relCountMap.begin();
    while(iter!=evs.relCountMap.end()){
      cerr<<(*iter).first << " "<< (*iter).second.first<<" "<< (*iter).second.second<<" "<< float((*iter).second.first)/(*iter).second.second<<endl;
      iter++;
    }

}
void binary2Nonbinary(const char*relfile)
{
    ifstream in(relfile);
    string line;
    InitKeySet();int count(0);
    while(getline(in,line))
    {
        if(count++<0)continue;
        SynTreeSQ obj;
        obj.ParseTree_1(line,0);
        obj.Bin2NonBin(&obj.root);
        if(obj.root.children.empty())cout<<line;
        else
        obj.PrintTreeOri(&obj.root);
        cout<<endl;
    }
}
int main(int argc, char*argv[])
{
  //test(argv[1]);
//  argv[1]="/home/mtu/buildtree/buildtree/bin/Debug/exp1/News.test.pure";
//  argv[2]="/home/mtu/buildtree/buildtree/bin/Debug/exp1/News.test.ber";
//  argv[3]="/home/mtu/buildtree/buildtree/bin/Debug/exp1/news.posfile-test"; //[BL [0: abc/pos ddd/pos ]]
 //argv[3]="/home/mtu/buildtree/buildtree/bin/Debug/exp1/News.trainout.2";
// argv[4]="/home/mtu/buildtree/buildtree/bin/Debug/exp1/str.m8";
// argv[5]="/home/mtu/buildtree/buildtree/bin/Debug/exp1/lb.m1";
// argv[6]="/home/mtu/buildtree/buildtree/bin/Debug/exp1/lb.m8";
//    argv[2]="/home/mtu/buildtree/buildtree/bin/Debug/exp1/News.testout.2";
//    argv[1]="/home/mtu/buildtree/buildtree/bin/Debug/exp1/test.out.2";
//    argv[2]="/home/mtu/buildtree/buildtree/bin/Debug/exp1/News.testout.1";
//    argv[1]="/home/mtu/buildtree/buildtree/bin/Debug/exp1/non-bin.test";
    binary2Nonbinary(argv[1]);

 // test1(argv[1],argv[2],argv[3]);
 //TestPro(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]);
  //binarization(argv[1]);
  //test();
 // evaluationforstr(argv[1],argv[2]);
 //TestLabelStd(argv[1],argv[2],argv[3],argv[4]);
 // evaluationforLabel(argv[1],argv[2]);
  return 0;

}
