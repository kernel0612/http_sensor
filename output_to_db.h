#ifndef OUTPUT_TO_DB
#define OUTPUT_TO_DB

#include "proc_output_interaction.h"
#include "my_berkeleyDB.h"
class output_to_db :
	public proc_output_interaction
{
public:
	output_to_db(void);
	~output_to_db(void);
	int init();
	int fini();
	int process(struct outputSerInfo* srv , struct outputCliInfo* clt);
private:
	my_berkeleyDB _srvdb;
	my_berkeleyDB _cltdb;
};

#endif 