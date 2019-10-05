/**************************************************************************************
Name: Connor Albers

Date: 10/3/19

Class: dafl(.h|.cpp)

Purpose: This class includes functionality for creating or opening a binary file
specified by the user (dependant on prior existence of file with matching filename),
writing fixed-length data to the file, reading fixed-length data from the file,
and cleanup of said file with content metadata.
***************************************************************************************/

//dafl header
#include "dafl.h"

/*
Method: createFile

Parameters:
char* fn (filename used to create the file)
unsigned int size: the size of the fixed-length data to be entered into the file

Purpose: Creates a file with a user-input filename if it doesn't exist already
*/
void dataFile::createFile(char* fn, unsigned int size)
{
	recSize = size;
	recCount = 0;
	string fileName = fn;

	//create the file
	finOut.open(fileName, ios::out);
	if (!finOut.is_open())
	{
		fs = fsCreateFail;
		finOut.close();
	}
	else
	{
		//close file, reopen with correct usage parameters
		finOut.close();
		finOut.open(fileName, ios::in | ios::out | ios::binary);

		// write record size
		finOut.write((char*)&recSize, sizeof recSize);

		// write current number of records (which will be 0)
		finOut.write((char*)&recCount, sizeof recCount);
		fs = fsSuccess;
	}
}
/*
Method: openFile

Parameters:
char* fn (filename used to open the file)

Purpose: opens a file with a user-input filename if it exists already
*/
void dataFile::openFile(char* fn)
{
	//check if you can open file
		finOut.open(fn, ios::in | ios::binary);
		if (!finOut.is_open())
		{
			fs = fsOpenFail;
			finOut.close();
		}
		else
		{
			//close and reopen with correct usage parameters
			finOut.close();
			finOut.open(fn, ios::in | ios::out | ios::binary);
			finOut.read((char*)&recSize, sizeof recSize);
			finOut.read((char*)&recCount, sizeof recCount);
			fs = fsSuccess;
		}
	

}
/*
Method: closeFile

Parameters:
none

Purpose: records the size of input data and recordcount at beginning of file, then closes
*/
void dataFile::closeFile()
{
	
	finOut.seekp(0);
	finOut.seekg(0);
	finOut.write((char*)&recSize, sizeof recSize );
	finOut.write((char*)&recCount, sizeof recCount );
	finOut.close();
	if (finOut.is_open())
		fs = fsCloseFail;
	else
		fs = fsSuccess;
}
/*
Method: putRecord

Parameters:
int relativeRecordNumber (number used as index for input/output stream pointers)
const void* rec (void container used to hold data to write to file)

Purpose: writes fixed-length data to the file
*/
void dataFile::putRecord(int relativeRecordNumber, const void* rec)
{
	if (relativeRecordNumber < 0 || relativeRecordNumber > recCount)
	{
		fs = fsPutFail;
	}
	else
	{
		//find the correct place to write, then write and flush
		finOut.seekp((relativeRecordNumber * recSize) + 8);
		finOut.write((char*)rec, recSize);
		finOut.flush();
		fs = fsSuccess;
	}
}
/*
Method: getRecord

Parameters:
int relativeRecordNumber (number used as index for input/output stream pointers)
const void* rec (void container used to hold data to read from file)

Purpose: reads fixed-length data from the file
*/
void dataFile::getRecord(int relativeRecordNumber, const void* rec)
{
	if (relativeRecordNumber < 0 || relativeRecordNumber > recCount)
	{
		fs = fsGetFail;
	}
	else
	{
		//find correct index to start read from, then read into rec
		finOut.seekg((relativeRecordNumber * recSize) + 8);
		finOut.read((char*)rec, recSize);
		fs = fsSuccess;
	}
}
/*
Method: recordCount

Parameters:
none

Purpose: returns the number of records contained in the file
*/
int dataFile::recordCount()
{
	return recCount;
}
/*
Method: updateRecordCount

Parameters:
none

Purpose: adds one to the number of records contained in the file
*/
void dataFile::updateRecordCount(int newRecCount)
{
	recCount += newRecCount;
}
/*
Method: fileStatus

Parameters:
none

Purpose: returns the file status after the last operation

possible statuses: 
fsCreateFail 0
fsSuccess 1
fsGetFail 2
fsCloseFail 3
fsOpenFail 4
fsPutFail 5
*/
int dataFile::fileStatus()
{
	return fs;
}