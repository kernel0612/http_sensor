#ifndef PCRE_H_
#define PCRE_H_
#include <pcre.h>
#include <string>
#include <vector>
#include <string.h>
using namespace std;

const int VECSIZE = 30;

struct MatchResult
{
	string name;
	vector<string> value;
};

class Pcre
{
public:
	Pcre();
	~Pcre();

	//Add a regrex, pass in name and regrex
	int AddRule(const string &name, const string &patten);

	//clear all the regrex
	void ClearRules();

	//match all the regrex, also return all the string match to every regrex
	vector<MatchResult> MatchAllRule(const char content[]);

private:
	const char *error;
	int erroffset;
	int ovector[VECSIZE];
	vector<pcre*> re_arr;
	vector<string> patten_name;
};
#endif