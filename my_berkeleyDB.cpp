#include "my_berkeleyDB.h"

my_berkeleyDB::my_berkeleyDB(void)
{
	_db=0;
	_env=0;
	_bclose=1;
	_bopen=0;
}

my_berkeleyDB::~my_berkeleyDB(void)
{
	if (_bclose!=0)
	{
		this->close();
	}
}
void my_berkeleyDB::print_error(int ret)
{
	if(ret != 0)
		printf("ERROR: %s ",db_strerror(ret));
}
int my_berkeleyDB::open(const char* dbname)
{
	_bclose=1;
	_bopen=0;
	if (!dbname)
	{
		//err
		return -1;
	}
	int ret=0;
	if ((ret=db_env_create(&_env,0))!=0)
	{
		return ret;
	}
	if (ret=_env->set_cachesize(_env,0,1024*1024*64,0)!=0)
	{
		return ret;
	}

	if (ret=_env->open(_env,NULL, DB_CREATE | DB_INIT_LOCK | DB_INIT_LOG | DB_INIT_MPOOL | DB_INIT_TXN, 0664) != 0)
	{
		return ret;
	}	

	if (ret=db_create(&_db,_env,0)!=0)
	{
		//cout <<db_strerror(ret)<<endl;
		return ret;
	}
	print_error(ret);
	if (_db->set_re_len(_db, 2048) != 0)
		perror("set_re_len");
	if (_db->set_re_pad(_db, (int)0x00) != 0)
		perror("set_re_pad");

	if (_db->set_q_extentsize(_db, 16*1024) != 0)
		perror("set_q_extentsize");
	if (_db->set_pagesize(_db,4096)!=0)
	{
		perror("set_pagesize");
	}
	if (ret=_db->open(_db,NULL,dbname,NULL,DB_QUEUE,DB_CREATE,0664)!=0)  
	{
		//db->err(db,ret,"%s",DATABASE);
		return ret;
	}
	_bopen=1;
	return ret;
}
int my_berkeleyDB::close()
{
	if (_bopen)
	{	
		_db->sync(_db,0);
		_bclose=_db->close(_db,0);
		_env->close(_env,0);
	    _bopen=_bclose;
	}
	return _bclose;
}
int my_berkeleyDB::put(char* key,int keySize,void* value,int valueSize)
{
	if (!key||keySize<=0||!value||valueSize<=0)
	{
		return -1;
	}
	if (!_bopen)
	{
		return -1;
	}
	int ret;
	DBT dbtKey,dbtValue;
	memset(&dbtKey,0,sizeof(DBT));
	memset(&dbtValue,0,sizeof(DBT));
	dbtKey.data=key;
	dbtKey.size=keySize;
	dbtValue.data=value;
	dbtValue.size=valueSize;
	if ((ret=_db->put(_db,NULL,&dbtKey,&dbtValue,0))==0)
	{
		//cout <<"key value stored"<<endl;
	}
	return ret;
}
int my_berkeleyDB::put(void* value,int valueSize)
{
	if (!value||valueSize<=0)
	{
		return -1;
	}
	if (!_bopen)
	{
		return -1;
	}
	int ret;
	DBT dbtKey,dbtValue;
	db_recno_t rec;
	memset(&dbtKey,0,sizeof(DBT));
	memset(&dbtValue,0,sizeof(DBT));
	dbtKey.data=&rec;
	dbtKey.size=sizeof(db_recno_t);
	dbtValue.data=value;
	dbtValue.size=valueSize;
	if ((ret=_db->put(_db,NULL,&dbtKey,&dbtValue,DB_APPEND))==0)
	{
		//cout <<"key value stored"<<endl;
	}
	else
	{
		_db->err(_db,ret,"%s","db->put()");
	}
	return ret;
}
int my_berkeleyDB::get(void** value,int* valueSize)
{
	if (!value||valueSize<=0)
	{
		return -1;
	}
	if (!_bopen)
	{
		return -1;
	}
	int ret=0;
	DBT dbtKey,dbtValue;
	db_recno_t rec;
	memset(&dbtKey,0,sizeof(DBT));
	memset(&dbtValue,0,sizeof(DBT));
	dbtKey.data=&rec;
	dbtKey.size=sizeof(db_recno_t);
	if ((ret=_db->get(_db,NULL,&dbtKey,&dbtValue,DB_CONSUME))==0)
	{
		*value=dbtValue.data;
		*valueSize=dbtValue.size;
	}
	else
	{
		_db->err(_db,ret,"%s","db->get()");
	}
	return ret;
}
int my_berkeleyDB::get(char* key,int keySize,void** value,int* valueSize)
{
	if (!key||keySize<=0||!value||valueSize<=0)
	{
		return -1;
	}
	if (!_bopen)
	{
		return -1;
	}
	int ret=0;
	DBT dbtKey,dbtValue;
	memset(&dbtKey,0,sizeof(DBT));
	memset(&dbtValue,0,sizeof(DBT));
	dbtKey.data=key;
	dbtKey.size=keySize;
	if ((ret=_db->get(_db,NULL,&dbtKey,&dbtValue,0))==0)
	{
	    *value=dbtValue.data;
		*valueSize=dbtValue.size;
	}
	return ret;
}
int my_berkeleyDB::del(char* key,int keySize)
{
	DBT dbtKey;
	int ret=0;
	memset(&dbtKey,0,sizeof(DBT));
	dbtKey.data=key;
	dbtKey.size=keySize;
	return ret=_db->del(_db,NULL,&dbtKey,0);
}
int my_berkeleyDB::update(char* key,int keySize,void* value,int valueSize)
{
	return -1;
}
