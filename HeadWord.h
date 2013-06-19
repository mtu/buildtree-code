#ifndef HEADWORD_H_INCLUDED
#define HEADWORD_H_INCLUDED
#include"SynTreePT.h"
namespace HeadWord{
int SearchHead(vector<TreeNode* >& vec, int mode, const string &key)
{
    if(mode==1)
        for(size_t i(0);i<vec.size();++i)
            if(vec[i]->value==key)return i;
    else
        for(size_t i(vec.size());i>0;i--)
            if(vec[i-1]->value==key)return i-1;
    return -1;
}
int GetHeadWord(vector<TreeNode* >& vec,TreeNode *par)
{
    int pos;
   // if(par->children->size()==1 && par->children[0]->children.empty()) return
    if(par->value=="ADJP")
    {
        pos = SearchHead(vec,0,"ADJP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"JJ");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"AD");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NN");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"CS");if(pos!=-1)return pos;
        return  vec.size()-1;//par->Back;
    }
    else if(par->value=="ADVP")
    {
        pos = SearchHead(vec,0,"ADVP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"AD");if(pos!=-1)return pos;
        return vec.size()-1;//par->Back;
    }
    else if(par->value=="DNP")
    {
        pos = SearchHead(vec,0,"DNP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"DEG");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"DEC");if(pos!=-1)return pos;
        return vec.size()-1;//par->Back;
    }
    else if(par->value=="DP")
    {
        pos = SearchHead(vec,1,"DP");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"DT");if(pos!=-1)return pos;
        return 0;//par->Begin;
    }
    else if(par->value=="DVP")
    {
        pos = SearchHead(vec,0,"DVP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"DEV");if(pos!=-1)return pos;
        return vec.size()-1;//par->Back;
    }
    else if(par->value=="FRAG")
    {
        pos = SearchHead(vec,0,"VV");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NR");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NN");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="CLP")
    {
        pos = SearchHead(vec,0,"CLP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"M");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="CP")
    {
        pos = SearchHead(vec,0,"DEC");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"SP");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"ADVP");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"CS");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"CP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"IP");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="INTJ")
    {
        pos = SearchHead(vec,0,"INTJ");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"IJ");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="IP")
    {
        pos = SearchHead(vec,0,"IP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VV");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="LCP")
    {
        pos = SearchHead(vec,0,"LCP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"LC");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="LST")
    {
        pos = SearchHead(vec,1,"LST");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"CD");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"OD");if(pos!=-1)return pos;
        return 0;//par->Begin;
    }
    else if(par->value=="NP")
    {
        pos = SearchHead(vec,0,"NP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NN");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NT");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NR");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"QP");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="PP")
    {
        pos = SearchHead(vec,1,"PP");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"P");if(pos!=-1)return pos;
        return 0;//par->Begin;
    }
    else if(par->value=="PRN")
    {
        pos = SearchHead(vec,0,"NP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"IP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NT");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NR");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NN");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="QP")
    {
        pos = SearchHead(vec,0,"QP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"CLP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"CD");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"OD");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="UCP")
    {
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="VCD")
    {
        pos = SearchHead(vec,0,"VCD");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VV");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VA");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VC");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VE");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="VCP")
    {
        pos = SearchHead(vec,0,"VCP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VV");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VA");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VC");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VE");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="VNV")
    {
        pos = SearchHead(vec,0,"VNV");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VV");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VA");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VC");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VE");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="VP")
    {
        pos = SearchHead(vec,1,"VP");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"VA");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"VC");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"VE");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"VV");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"BA");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"LB");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"VCD");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"VSB");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"VRD");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"VNV");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"VCP");if(pos!=-1)return pos;
        return 0;//par->Begin;
    }
    else if(par->value=="VPT")
    {
        pos = SearchHead(vec,0,"VNV");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VV");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VA");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VC");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VE");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="VRD")
    {
        pos = SearchHead(vec,0,"VRD");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VV");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VA");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VC");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VE");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="VSB")
    {
        pos = SearchHead(vec,0,"VSB");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VV");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VA");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VC");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"VE");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
     else if(par->value=="WHNP")
    {
        pos = SearchHead(vec,0,"WHNP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NP");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NN");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NT");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"NR");if(pos!=-1)return pos;
        pos = SearchHead(vec,0,"QP");if(pos!=-1)return pos;
         return vec.size()-1;//par->Back;
    }
    else if(par->value=="WHPP")
    {
        pos = SearchHead(vec,1,"WHPP");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"PP");if(pos!=-1)return pos;
        pos = SearchHead(vec,1,"P");if(pos!=-1)return pos;
        return 0;//par->Begin;
    }
    else{
        return 0;//par->Begin;
    }

}
}
#endif // HEADWORD_H_INCLUDED
