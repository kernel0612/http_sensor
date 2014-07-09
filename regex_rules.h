#ifndef REGEX_RULES_H
#define REGEX_RULES_H
#include <iostream>
#include <string>
#include <vector>
#include "tinyxml.h"
#include "Pcre.h"
//#pragma comment(lib,"tinyxmlstatic.lib")
using namespace std;
struct regex_node
{
	string name;
	string pattern;
	string seg_or_replace;
};
struct echo_node
{
	string name;
	string context;
	string pattern;
	string seg_or_replace;
};
struct rule_node
{
	string name;
	vector<regex_node> regex_nodes;
    bool echo_switch;   //1 ON  0 OFF
	vector<echo_node>  echo_nodes;
	string log;
	string sql;
};
struct system_node 
{
	string name;
	string group;
	string type;
	string seq_relation;
	vector<rule_node> rule_nodes;
};
struct sort_node
{
	size_t seq_num;
	string name;
};
struct regex_match_result_node
{
	string rulename;
	string type;    //regex or echo
	string name;
	string value;
};
class regex_rules
{
public:
	regex_rules(void);
	~regex_rules(void);
	int open_rules_xml(const char* xmlname);
	string excute_regex_rules(char srcContent[],size_t contentLen);

protected:
	int get_seq_relation(string strsrc);
	vector<system_node> _sys_nodes;
	Pcre* _pcre;
	vector<regex_match_result_node> _regex_results;
	vector<sort_node> _sort_nodes;
};

#endif