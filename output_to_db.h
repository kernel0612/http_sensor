#ifndef OUTPUT_TO_DB
#define OUTPUT_TO_DB

#include "proc_output_interaction.h"
#include <string>
#include <stdio.h>
#include "PQ.h"
using namespace std;
class output_to_db :
	public proc_output_interaction
{
public:
	output_to_db(const char *ip, uint16_t pport, const char *pdbname, 
		const char *puser, const char *ppasswd, int pconnect_timeout = 0);
	~output_to_db(void);
	int init();
	int fini();
	int process(struct outputSerInfo* srv , struct outputCliInfo* clt);
private:

	PQ _pqDB;
};

#endif 