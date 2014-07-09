#include "proc_reg_rules.h"


proc_reg_rules::proc_reg_rules(void):_rules(new regex_rules)
{

}
proc_reg_rules::~proc_reg_rules(void)
{
	if (_rules)
	{
		delete _rules;
	}	
}
int proc_reg_rules::open_rules_xml(const char* xmlname)
{
	return _rules->open_rules_xml(xmlname);
}
string proc_reg_rules::excute_regex_rules(char srcContent[],size_t contentLen)
{

	return _rules->excute_regex_rules(srcContent,contentLen);
}
