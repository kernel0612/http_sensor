#ifndef OUTPUT_TO_FILE
#define OUTPUT_TO_FILE
#include "proc_output_interaction.h"

class output_to_file :
	public proc_output_interaction
{
public:
	output_to_file(void);
	~output_to_file(void);
	int init();
	int fini();
	int process(struct outputSerInfo* srv , struct outputCliInfo* clt);

private:
	ACE_TCHAR _filename[MAXPATHLEN+1];
	ACE_HANDLE _filehandle;
};
#endif
