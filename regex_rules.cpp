#include "regex_rules.h"


regex_rules::regex_rules(void):_pcre(new Pcre)
{
}


regex_rules::~regex_rules(void)
{
}
int regex_rules::open_rules_xml(const char* xmlname)
{
	if (!xmlname)
	{
		cout<<"please input xml"<<endl;
		return -1;
	}
	TiXmlDocument* pdoc=new TiXmlDocument(xmlname);
	if (pdoc==NULL)
	{
		cout <<"create Xmldoc failed"<<endl;
		return -1;
	}
	if (!pdoc->LoadFile())
	{
		cout <<"open xml failed"<<endl;
		return -1;
	}
	//pdoc->Print();

	TiXmlElement *root=pdoc->RootElement();
	if (root)
	{
		TiXmlElement* sys=root->FirstChildElement();

		for (;sys;sys=sys->NextSiblingElement())
		{
			system_node  onesys;
			TiXmlAttribute* att=sys->FirstAttribute();
			for (;att;att=att->Next())
			{
				//cout <<att->Name()<<":"<<att->Value()<<endl;
				if (strcmp(att->Name(),"name")==0)
				{
					onesys.name=att->Value();
					continue;
				}
				if (strcmp(att->Name(),"type")==0)
				{
					onesys.type=att->Value();
					continue;
				}
				if (strcmp(att->Name(),"seqrel")==0)
				{
					onesys.seq_relation=att->Value();
					continue;
				}

			}
			TiXmlElement* rule=sys->FirstChildElement();

			vector<rule_node> rules;
			for (;rule;rule=rule->NextSiblingElement())
			{
				rule_node  onerule;
				TiXmlAttribute* ruleAtt=rule->FirstAttribute();
				if (ruleAtt)
				{

					//cout <<ruleAtt->Name()<<":"<<ruleAtt->Value()<<endl;
				}
				TiXmlElement* child=rule->FirstChildElement();				
				for (;child;child=child->NextSiblingElement())
				{
					if (strcmp(child->Value(),"regex")==0)
					{
						regex_node reg;
						TiXmlAttribute *childatt=child->FirstAttribute();
						for (;childatt;childatt=childatt->Next())
						{
							if (strcmp(childatt->Name(),"name")==0)
							{
								reg.name=childatt->Value();
								continue;
							}
							if (strcmp(childatt->Name(),"pattern")==0)
							{
								reg.pattern=childatt->Value();
								continue;
							}
							if (strcmp(childatt->Name(),"rep")==0||strcmp(childatt->Name(),"seg")==0)
							{
								reg.seg_or_replace=childatt->Value();
								continue;
							}
							//cout <<childatt->Name()<<" "<<childatt->Value()<<endl;
						}
						onerule.regex_nodes.push_back(reg);
						continue;
					}
					if (strcmp(child->Value(),"echoswi")==0)
					{
						bool swi;
						TiXmlAttribute *childatt=child->FirstAttribute();
						for (;childatt;childatt=childatt->Next())
						{
							if (strcmp(childatt->Name(),"switch")==0)
							{
								swi=childatt->IntValue();
								continue;
							}
							//cout <<childatt->Name()<<" "<<childatt->Value()<<endl;
						}
						onerule.echo_switch=swi;
						continue;
					}
					if (strcmp(child->Value(),"echo")==0)
					{
						echo_node   echo;
						TiXmlAttribute *childatt=child->FirstAttribute();
						for (;childatt;childatt=childatt->Next())
						{
							if (strcmp(childatt->Name(),"name")==0)
							{
								echo.name=childatt->Value();
								continue;
							}
							if (strcmp(childatt->Name(),"context")==0)
							{
								echo.context=childatt->Value();
								continue;
							}
							if (strcmp(childatt->Name(),"pattern")==0)
							{
								echo.pattern=childatt->Value();
								continue;
							}
							if (strcmp(childatt->Name(),"rep")==0||strcmp(childatt->Name(),"seg")==0)
							{
								echo.seg_or_replace=childatt->Value();
								continue;
							}
							//cout <<childatt->Name()<<" "<<childatt->Value()<<endl;
						}
						onerule.echo_nodes.push_back(echo);
						continue;
					}

				}
				rules.push_back(onerule);
			}
			onesys.rule_nodes=rules;
			_sys_nodes.push_back(onesys);
		}

	}
	delete pdoc;
	return 0;
}
string regex_rules::excute_regex_rules(char srcContent[],size_t contentLen)
{
	vector<system_node>::const_iterator co_sys_node_it;
	//string str_match_regex_result;
	//string str_match_echo_result;
	string full_result;
	{
		vector<regex_match_result_node> temp;
		_regex_results.swap(temp);
	}

	for (co_sys_node_it=_sys_nodes.begin();co_sys_node_it!=_sys_nodes.end();++co_sys_node_it)
	{
		full_result.append("sysName=");
		full_result.append(co_sys_node_it->name);
        full_result.append(";");
		full_result.append("group=");
		full_result.append(co_sys_node_it->group);
		full_result.append(";");
		full_result.append("type=");
		full_result.append(co_sys_node_it->type);
		full_result.append(";");

		get_seq_relation(co_sys_node_it->seq_relation);

		co_sys_node_it->seq_relation;
		vector<rule_node> _rule_node=co_sys_node_it->rule_nodes;
		vector<rule_node>::const_iterator co_rule_it;
		for (co_rule_it=_rule_node.begin();co_rule_it!=_rule_node.end();++co_rule_it)
		{
			co_rule_it->name;

			vector<regex_node>::const_iterator co_regex_node_it=
				co_rule_it->regex_nodes.begin();
			for (;co_regex_node_it!=co_rule_it->regex_nodes.end();++co_regex_node_it)
			{
				_pcre->ClearRules();
				_pcre->AddRule(co_regex_node_it->name,co_regex_node_it->pattern);
				vector<MatchResult> match_regex_results=_pcre->MatchAllRule(srcContent);
				for (int i=0;i<match_regex_results.size();i++)
				{
					regex_match_result_node  one_match_node;
					one_match_node.rulename=co_rule_it->name;
					one_match_node.name=match_regex_results[i].name;
					one_match_node.type="regex";
					for (int j=0;j<match_regex_results[i].value.size();j++)
					{
						string  tmpresult=match_regex_results[i].value[j];
						if (co_regex_node_it->seg_or_replace.empty())
						{
							one_match_node.value.append(tmpresult);
							one_match_node.value.append(";");
						}
						else
							one_match_node.value=co_regex_node_it->seg_or_replace;
					}
					_regex_results.push_back(one_match_node);
				}
			}


			if (co_rule_it->echo_switch==1)
			{
				vector<echo_node>::const_iterator co_echo_node_it=
					co_rule_it->echo_nodes.begin();
				
				for (;co_echo_node_it!=co_rule_it->echo_nodes.end();++co_echo_node_it)
				{
					_pcre->ClearRules();
					_pcre->AddRule(co_echo_node_it->name,co_echo_node_it->pattern);
					vector<MatchResult> match_echo_results=_pcre->MatchAllRule(srcContent);
					for (int i=0;i<match_echo_results.size();i++)
					{
						regex_match_result_node  one_match_node;
						one_match_node.rulename=co_rule_it->name;
						one_match_node.name=match_echo_results[i].name;
						one_match_node.type="echo";
						for (int j=0;j<match_echo_results[i].value.size();j++)
						{
							string  tmpresult=match_echo_results[i].value[j];
							if (co_regex_node_it->seg_or_replace.empty())
							{
								one_match_node.value.append(tmpresult);
								one_match_node.value.append(";");
							}
							else
								one_match_node.value=co_echo_node_it->seg_or_replace;
						}
						_regex_results.push_back(one_match_node);
					}
				}
		
			}
			//co_rule_it->log;
			//co_rule_it->sql;
		}
	}

	string subret;
	vector<sort_node>::const_iterator co_sort_node_it;
	vector<regex_match_result_node>::const_iterator co_regex_match_ret_it;
	for (co_sort_node_it=_sort_nodes.begin();co_sort_node_it!=_sort_nodes.end();++co_sort_node_it)
	{

		for (co_regex_match_ret_it=_regex_results.begin();co_regex_match_ret_it
			!=_regex_results.end();++co_regex_match_ret_it)
		{
			if (co_regex_match_ret_it->rulename.compare(co_sort_node_it->name)==0)
			{
				subret.append(co_regex_match_ret_it->rulename);
				subret.append("=");
				subret.append("{");
				subret.append("type");
				subret.append("=");
				subret.append(co_regex_match_ret_it->type);
				subret.append(";");
				subret.append(co_regex_match_ret_it->name);
				subret.append("=");
				subret.append(co_regex_match_ret_it->value);
				subret.append(";");
				subret.append("}");
			}
			else
			{
				continue;
			}
		}
	}
	full_result.append(subret);
	return full_result;
}

int regex_rules::get_seq_relation(string strsrc)
{
	if (strsrc.empty())
	{
		return -1;
	}
	int len=strsrc.length();
	for (size_t i=0,j=0;i<strsrc.length();)
	{
		size_t nindex=strsrc.find(',',i);
		if (nindex!=string::npos)
		{
			string str=strsrc.substr(i,nindex-i);
			int sublen=str.length();
			sort_node  one;
			one.seq_num=j;
			one.name=str;
			_sort_nodes.push_back(one);
			i+=(sublen+1);
			++j;
		}
		else
		{
			sort_node one;
			one.seq_num=j;
			one.name=strsrc.substr(i);
			_sort_nodes.push_back(one);
			break;
		}
	}
	cout <<"sort complete"<<endl;
	return 0;

}