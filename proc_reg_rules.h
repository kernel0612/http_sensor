#ifndef PROC_REG_RULES_H
#define PROC_REG_RULES_H

#include "regex_rules.h"
using namespace std;



class proc_reg_rules
{
public:
	proc_reg_rules(void);
	~proc_reg_rules(void);
	int open_rules_xml(const char* xmlname);
	string excute_regex_rules(char srcContent[],size_t contentLen);

private:
	
	regex_rules* _rules;


};

#endif