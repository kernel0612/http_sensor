#ifndef PROC_REG_RULES_H
#define PROC_REG_RULES_H
#include <iostream>
#include <string>
using namespace std;
class proc_reg_rules
{
public:
	proc_reg_rules(void);
	~proc_reg_rules(void);
	int open_rules_xml(const char* xmlname);
	string excute_regex_rules();

private:

};

#endif