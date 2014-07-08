#include "Pcre.h"
#include <stdio.h>

Pcre::Pcre()
{
	re_arr.clear();
	patten_name.clear();
}

Pcre::~Pcre()
{
	for(int i=0; i<re_arr.size(); i++)
        {
                pcre_free(re_arr[i]);
        }
}

int Pcre::AddRule(const string &name, const string &patten)
{
	pcre *re = pcre_compile(patten.c_str(), PCRE_MULTILINE|PCRE_UTF8|PCRE_NO_AUTO_CAPTURE, &error, &erroffset, NULL);
	if(re == NULL)
	{
		printf("pcre compile failed, offset %d: %s\n", erroffset, error);
		return -1;
	}
	else
	{
		re_arr.push_back(re);
		patten_name.push_back(name);
	}
}
void Pcre::ClearRules()
{
	for(int i=0; i<re_arr.size(); i++)
	{
		pcre_free(re_arr[i]); 
	}
	re_arr.clear();
	patten_name.clear();
}

vector<MatchResult> Pcre::MatchAllRule(const char content[])
{
	vector<MatchResult> result_arr;
	int length = strlen(content);
	char buf[2048];
	for(int i=0; i<re_arr.size(); i++)
	{
		MatchResult result;
		result.name = patten_name[i];
		int cur = 0;
		int rc;
		while(cur<length && (rc = pcre_exec(re_arr[i], NULL, content, length, cur, PCRE_NOTEMPTY, ovector, VECSIZE)) >= 0)
		{
			for(int j=0; j<rc; j++)
			{
				memset(buf, 0, sizeof(buf));
				strncpy(buf, content+ovector[2*j], ovector[2*j+1]-ovector[2*j]);
				result.value.push_back(buf);
			}
			cur = ovector[1];
		}
		if(result.value.size() > 0)
			result_arr.push_back(result);
	}
	return result_arr;
}

