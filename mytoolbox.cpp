#include "mytoolbox.h"
//void TrimLine(string & line)
//{
//	line.erase(0,line.find_first_not_of(" \t\r\n"));
//	line.erase(line.find_last_not_of(" \t\r\n")+1);
//}

void Split(string &s, vector <string> &vs)
{
	vs.clear();
	stringstream ss;
	string e;
	ss << s;
	while(ss >> e)
		vs.push_back(e);
}

void Split(string &s,char c, vector <string> &vs)
{
	int cur = 0,next;
	next = s.find(c);
	while(next != string::npos)
	{
		if(s.substr(cur,next-cur) !="")
			vs.push_back(s.substr(cur,next-cur));
		cur = next+1;
		next = s.find(c,cur);
	}
	vs.push_back(s.substr(cur));
}

void Join(string joiner,vector <string> &vs, string &s)
{
	s = "";
	for(size_t i=0;i<vs.size()-1;i++)
		s = s+vs[i]+joiner;
	s += vs[vs.size()-1];
}

void Join(string joiner,deque <string> &vs, string &s)
{
	s = "";
	for(size_t i=0;i<vs.size()-1;i++)
		s = s+vs[i]+joiner;
	s += vs[vs.size()-1];
}

void Erase(string &s, string substr)
{
	while(s.find(substr) != string::npos)
		s.erase(s.find(substr),substr.size());
}

void GbkStr2CharVec(string &s, vector<string> &cv)
{
	cv.clear();
	for (size_t i = 0; i < s.size(); i++)
	{
		string c;
		if (s[i] >= 0)
			c = s.substr(i,1);
		else
			c = s.substr(i++,2);
		cv.push_back(c);
	}
}

void Utf8Str2CharVec(string &s, vector<string> &cv)
{
	cv.clear();
	for (size_t i = 0; i < s.size(); i++)
	{
		string c;
		unsigned char x = (unsigned char)s[i];
		if (x < 128)
			c = s.substr(i,1);
		else if (x < 224)
			c = s.substr(i++,2);
		else if (x < 240)
		{
			c = s.substr(i,3);
			i += 2;
		}
		else if (x < 248)
		{
			c = s.substr(i,4);
			i += 3;
		}
		else
		{
			cout<<"bad char!\n";
			return;
		}
		cv.push_back(c);
	}
}

double s2d(string &s)
{
	double d;
	stringstream ss;
	ss<<s;
	ss>>d;
	return d;
}

float s2f(string &s)
{
	float f;
	stringstream ss;
	ss<<s;
	ss>>f;
	return f;
}

int s2i(string &s)
{
	int i;
	stringstream ss;
	ss<<s;
	ss>>i;
	return i;
}

string i2s(int i)
{
	string s;
	stringstream ss;
	ss<<i;
	ss>>s;
	return s;
}

string f2s(float f)
{
	string s;
	stringstream ss;
	ss<<f;
	ss>>s;
	return s;
}
