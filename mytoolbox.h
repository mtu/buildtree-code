#include "Common.h"
using namespace std;
//void TrimLine(string & line);
void Split(string &s, vector <string> &vs);
void Split(string &s, char c, vector <string> &vs);
void Join(string joiner,vector <string> &vs, string &s);
void Join(string joiner,deque <string> &vs, string &s);
void Erase(string &s, string substr);
void GbkStr2CharVec(string &s, vector<string> &cv);
void Utf8Str2CharVec(string &s, vector<string> &cv);
double s2d(string &s);
float s2f(string &s);
int s2i(string &s);
string i2s(int i);
string f2s(float f);
