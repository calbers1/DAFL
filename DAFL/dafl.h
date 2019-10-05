#pragma once
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

//Filestatus indicators
#define fsCreateFail 0
#define fsSuccess 1
#define fsGetFail 2
#define fsCloseFail 3
#define fsOpenFail 4
#define fsPutFail 5

//Class definition for a dataFile type. This class will give a dataFile object
//the capability to perform basic file I/O operations for a fixed length,
//direct access record file structure.Two methods are missing from this class but 
//could be added at a later time. Edit and delete record functionality are not 
//implemented in this initial version of the class.

class dataFile{
	public:
		void createFile (char*,unsigned int);
		void openFile (char*);
		void closeFile ();
		void putRecord (int, const void*);
		void getRecord (int,const void*);
		int recordCount ();
		void updateRecordCount(int);
		int fileStatus();
	private:
		fstream finOut;
		int recSize;
		int recCount;
		int fs;
};