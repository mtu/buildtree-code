#include "Common.h"
class SegScore
{
	public:
		SegScore(const char* MeModel);
		void eval_all(vector<pair<string, double> >& context, vector<pair<string, double> > & outcome);
	private:
		void LoadMEmodel(const char* memodelfile);
	private:
		int feature_num;
		int tag_num;
		int lambda_num;
		map <string,int> feature2id;
		vector <string> featureVec;
		map <string,int> tag2id;
		vector <string> tagVec;
		vector <double> lambdaVec;
		vector <int> lambda2tagVec;
		vector <int> featureAddrVec;
};
