#ifndef MY_BERKELEYDB_H
#define MY_BERKELEYDB_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
using namespace std;

class my_berkeleyDB
{
public:
	my_berkeleyDB(void);
	~my_berkeleyDB(void);
	int open(const char* dbname);
	int close();
	int put(char* key,int keySize,void* value,int valueSize);
	int put(void* value,int valueSize);
	int get(char* key,int keySize,void** value,int* valueSize);
	int get(void** value,int* valueSize);
	int del(char* key,int keySize);
	int update(char* key,int keySize,void* value,int valueSize);



private:
	void print_error(int ret);
	DB* _db;
	DB_ENV* _env;
	int _bclose;
	int _bopen;

};
#endif