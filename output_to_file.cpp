#include "output_to_file.h"


output_to_file::output_to_file(void):_filehandle(0)
{
	memset(_filename,0,MAXPATHLEN+1);
}


output_to_file::~output_to_file(void)
{
}
int output_to_file::init()
{
	char* fileName="_output_data.out";
	ACE_Date_Time adt(ACE_OS::gettimeofday());
	adt.year();
	sprintf(_filename,"%d-%d-%d--%d-%d-%d",adt.year(),adt.month(),adt.day(),adt.hour(),adt.minute(),adt.second());
	strcat(_filename,fileName);
    _filehandle=ACE_OS::open(_filename,O_RDWR|O_CREAT|O_TRUNC,ACE_DEFAULT_FILE_PERMS);
	if (_filehandle==ACE_INVALID_HANDLE)
	{
		ACE_ERROR_RETURN((LM_ERROR,ACE_TEXT("open failed for %s\n"),_filename),-1);
	}
	_flag=1;
	return 0;
}
int output_to_file::fini()
{
	if (ACE_OS::close(_filehandle)==0)
	{
		_flag=0;
		return 0;
	}
	return -1;
}
int output_to_file::process(struct outputSerInfo* srv , struct outputCliInfo* clt)
{
	if (!srv||!clt||!_flag)
	{
		return -1;
	}
	int ret=0;
	ret=ACE_OS::write(_filehandle,(void*)srv,sizeof(struct outputSerInfo));
	if (ret<sizeof(struct outputSerInfo))
	{
		ACE_ERROR_RETURN((LM_ERROR,ACE_TEXT("write failed for serverinfo %s\n"),_filename),-1);
	}
	ret=ACE_OS::write(_filehandle,(void*)clt,sizeof(struct outputCliInfo));
	if (ret<sizeof(struct outputCliInfo))
	{
		ACE_ERROR_RETURN((LM_ERROR,ACE_TEXT("write failed for clientinfo %s\n"),_filename),-1);
	}
	return 0;
}