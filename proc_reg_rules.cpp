#include "proc_reg_rules.h"


proc_reg_rules::proc_reg_rules(void)/*:_pcre(new Pcre)*//*,_rules(new regex_rules)*/
{

}


proc_reg_rules::~proc_reg_rules(void)
{
	//if (_pcre)
	//{
	//	delete _pcre;
	//}	
}
int proc_reg_rules::open_rules_xml(const char* xmlname)
{
	//if (!xmlname)
	//{
	//	return -1;
	//}
	//TiXmlDocument* pdoc=new TiXmlDocument(xmlname);
	//if (pdoc==NULL)
	//{
	//	cout <<"create Xmldoc failed"<<endl;
	//	return -1;
	//}
	//if (!pdoc->LoadFile())
	//{
	//	cout <<"open xml failed"<<endl;
	//	return -1;
	//}
	////pdoc->Print();

	//TiXmlElement *root=pdoc->RootElement();
	//if (root)
	//{
	//	TiXmlElement* sys=root->FirstChildElement();
	//	if (sys)
	//	{
	//		//stsys  onesys;
	//		TiXmlAttribute* att=sys->FirstAttribute();
	//		for (;att;att=att->Next())
	//		{
	//			//cout <<att->Name()<<":"<<att->Value()<<endl;
	//			if (strcmp(att->Name(),"name")==0)
	//			{
	//				//onesys.name=att->Value();
	//				continue;
	//			}
	//			if (strcmp(att->Name(),"type")==0)
	//			{
	//				//onesys.type=att->Value();
	//				continue;
	//			}
	//			if (strcmp(att->Name(),"seqrel")==0)
	//			{
	//				//onesys.seqrel=att->Value();
	//				continue;
	//			}

	//		}
	//		TiXmlElement* rule=sys->FirstChildElement();

	//		//list<strule> rules;
	//		for (;rule;rule=rule->NextSiblingElement())
	//		{
	//			//strule  onerule;
	//			TiXmlAttribute* ruleAtt=rule->FirstAttribute();
	//			if (ruleAtt)
	//			{

	//				//cout <<ruleAtt->Name()<<":"<<ruleAtt->Value()<<endl;
	//			}
	//			TiXmlElement* child=rule->FirstChildElement();				
	//			for (;child;child=child->NextSiblingElement())
	//			{
	//				if (strcmp(child->Value(),"regex")==0)
	//				{
	//					//stregex reg;
	//					TiXmlAttribute *childatt=child->FirstAttribute();
	//					for (;childatt;childatt=childatt->Next())
	//					{
	//						if (strcmp(childatt->Name(),"name")==0)
	//						{
	//							//reg.name=childatt->Value();
	//							continue;
	//						}
	//						if (strcmp(childatt->Name(),"pattern")==0)
	//						{
	//							//reg.pattern=childatt->Value();
	//							continue;
	//						}
	//						if (strcmp(childatt->Name(),"rep")==0||strcmp(childatt->Name(),"seg")==0)
	//						{
	//							//reg.segOrRep=childatt->Value();
	//							continue;
	//						}
	//						//cout <<childatt->Name()<<" "<<childatt->Value()<<endl;
	//					}
	//					//onerule.reg=reg;
	//					continue;
	//				}
	//				if (strcmp(child->Value(),"echoswi")==0)
	//				{
	//					string swi;
	//					TiXmlAttribute *childatt=child->FirstAttribute();
	//					for (;childatt;childatt=childatt->Next())
	//					{
	//						if (strcmp(childatt->Name(),"switch")==0)
	//						{
	//							swi=childatt->Value();
	//							continue;
	//						}
	//						//cout <<childatt->Name()<<" "<<childatt->Value()<<endl;
	//					}
	//					//onerule.swi=swi;
	//					continue;
	//				}
	//				if (strcmp(child->Value(),"echo")==0)
	//				{
	//					//stecho   echo;
	//					TiXmlAttribute *childatt=child->FirstAttribute();
	//					for (;childatt;childatt=childatt->Next())
	//					{
	//						if (strcmp(childatt->Name(),"name")==0)
	//						{
	//							//echo.name=childatt->Value();
	//							continue;
	//						}
	//						if (strcmp(childatt->Name(),"context")==0)
	//						{
	//							//echo.context=childatt->Value();
	//							continue;
	//						}
	//						if (strcmp(childatt->Name(),"pattern")==0)
	//						{
	//							//echo.pattern=childatt->Value();
	//							continue;
	//						}
	//						if (strcmp(childatt->Name(),"rep")==0||strcmp(childatt->Name(),"seg")==0)
	//						{
	//							//echo.segOrRep=childatt->Value();
	//							continue;
	//						}
	//						//cout <<childatt->Name()<<" "<<childatt->Value()<<endl;
	//					}
	//					//onerule.echos.push_back(echo);
	//					continue;
	//				}

	//			}
	//			//rules.push_back(onerule);
	//		}
	//		//onesys.regs=rules;
	//		//_rules.push_back(onesys);

	//	}
	//}
	//delete pdoc;
	return 0;
}
string proc_reg_rules::excute_regex_rules()
{
	return "";
}
