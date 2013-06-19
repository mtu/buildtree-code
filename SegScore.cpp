#include "mytoolbox.h"
#include "SegScore.h"

SegScore::SegScore(const char* MeModel)
{
	LoadMEmodel(MeModel);
}

void SegScore::LoadMEmodel(const char* memodelfile)
{
	ifstream fin;
	fin.open(memodelfile);
	if(!fin.is_open())
	{
		cerr<<"open MaxEnt model error!"<<endl;
		return;
	}
	string s;
	getline(fin,s);
	getline(fin,s);
	feature_num = s2i(s);
	for (int i = 0;i<feature_num;i++)
	{
		getline(fin,s);
		boost::trim(s);
		feature2id.insert(pair<string,int>(s,i));
		featureVec.push_back(s);
	}
	getline(fin,s);
	tag_num = s2i(s);
	for (int i = 0; i<tag_num; i++)
	{
		getline(fin,s);
		boost::trim(s);
		tag2id.insert(pair<string,int>(s,i));
		tagVec.push_back(s);
	}
	featureAddrVec.resize(feature_num+1,0);
	for (int i = 0; i<feature_num; i++)
	{
		getline(fin,s);
		boost::trim(s);
		vector <string> vs;
		Split(s,vs);
		featureAddrVec.at(i+1) = featureAddrVec.at(i)+s2i(vs.at(0));
		for(size_t j = 1;j<vs.size();j++)
			lambda2tagVec.push_back(s2i(vs.at(j)));
	}
	getline(fin,s);
	lambda_num = s2i(s);
	for (int i = 0; i<lambda_num;i++)
	{
		getline(fin,s);
		boost::trim(s);
		double lambda = s2d(s);
		lambdaVec.push_back(lambda);
	}
	cerr<<"load MaxEnt model over!\n";
}

void SegScore::eval_all(vector<pair<string, double> >& context, vector<pair<string, double> > & outcome)
{
	outcome.clear();
	outcome.resize(tag_num);

	vector<double> probs;
	probs.resize(tag_num, 0.0);

	for (size_t i = 0; i < context.size(); i ++)
	{
		//cout << context.at(i).first << endl;
		map<string,int>::iterator iter = feature2id.find(context.at(i).first);
		if (iter != feature2id.end())
		{
			int featureid = iter->second;
			int num1 = featureAddrVec.at(featureid);
			int num2 = featureAddrVec.at(featureid+1);
			for (int j = num1; j < num2; j ++)
			{
				probs.at(lambda2tagVec.at(j)) += lambdaVec.at(j) * context.at(i).second;
			}
		}
		else
		{
			continue;
		}
	}

	double sum = 0.0;
	for (size_t j = 0; j < probs.size(); j ++)
	{
		probs.at(j) = exp(probs.at(j));
		sum += probs.at(j);
	}

	for (size_t k = 0; k < probs.size(); k ++)
	{
		probs.at(k) /= sum;

		outcome.at(k).first = tagVec.at(k);
		outcome.at(k).second = log10(probs.at(k));
	}
}

