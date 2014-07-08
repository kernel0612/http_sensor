#ifndef REGEX_RULES_H
#define REGEX_RULES_H
#include <iostream>
#include <vector>
#include <string>
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

class regex_rules
{
public:
	regex_rules(void);
	~regex_rules(void);
	inline	vector<system_node>& get_system_regex_rule()
	{
		return _sys_nodes;
	}


protected:
	vector<system_node> _sys_nodes;
};

#endif