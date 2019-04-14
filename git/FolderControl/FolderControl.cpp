// FolderControl.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include "pch.h"
#include <iostream>
#include <direct.h>
#include "stdio.h"
#include <ctime>
#include <io.h>
#include <string>
#include <afx.h>
#define YEAR_SET 1900
#define MON_SET 1
using namespace std;


bool DeleteEntireDir(TCHAR* sPath, bool bDelDir = true);
bool IsOutofDate(int y, int m, int d);


CString dir = "C:\\photo";
TCHAR* szMsg = new TCHAR[100];

int year = 0;
int month = 0;
int day = 0;
int deleteFileCount = 0;
int deleteFolderCount = 0;

int daylimit = 7;// the files created 7 days ago will be deleted
int main(int argc, char *argv[])
{

	//_mkdir("C:\\photo");

	if (argc >1)
	{
		
		dir = (argv[1]);
		daylimit = atoi(argv[2]);
		cout << "Target Folder : " << (argv[1]) << endl;
	}
	else
	{
		cout << "Target Folder : " << "C:\\photo" << endl;
	}
		
	cout << "Delete the files and folder before " << daylimit << " days " << endl << endl;
	

	time_t now = time(0);
	struct tm ltm;
	localtime_s(&ltm, &now);
	cout << "#Local time of this computer: " << endl;
	cout << " " << YEAR_SET + ltm.tm_year;
		cout << " / " << MON_SET + ltm.tm_mon;
		cout << " / " << ltm.tm_mday << endl << endl;;

	year = YEAR_SET+ltm.tm_year;
	month = MON_SET+ltm.tm_mon;
	day = ltm.tm_mday;

	cout << "Working..." << endl << endl;

	szMsg = dir.GetBuffer(dir.GetLength());

	DeleteEntireDir(szMsg);

	szMsg = NULL;



	delete[]szMsg;
	szMsg = NULL;

	cout << "delete : " << deleteFileCount << " files and " << deleteFolderCount << " folders " << endl << endl;
	
	cin.get();
	return 0;
}

bool DeleteEntireDir(TCHAR* sPath, bool bDelDir)
{
	CFileFind ff;
	BOOL bFound;
	bFound = ff.FindFile(CString(sPath) + "\\*.*");
	while (bFound)
	{
		bFound = ff.FindNextFile();
		CString sFilePath = ff.GetFilePath();
		CTime time;
		ff.GetCreationTime(time);
		//printf("Creation time : %i,%i,%i", time.GetYear(), time.GetMonth(), time.GetDay());
		if (IsOutofDate(time.GetYear(), time.GetMonth(), time.GetDay()))
		{
			if (ff.IsDirectory())
			{

				if (!ff.IsDots())
				{

					szMsg = sFilePath.GetBuffer(sFilePath.GetLength());

						DeleteEntireDir(szMsg);

						sFilePath.ReleaseBuffer();
				}
			}
			else
			{
				if (ff.IsReadOnly())
				{
					SetFileAttributes(sFilePath, FILE_ATTRIBUTE_NORMAL);
				}

				DeleteFile(sFilePath);
				deleteFileCount++;
				
			}
		}
	}
	ff.Close();

	if (bDelDir)
	{
		SetFileAttributes(sPath, FILE_ATTRIBUTE_NORMAL);  //Set the folder attributes
		if (sPath != dir)  /// Don't delete the root folder of the selection
		{
			RemoveDirectory(sPath);
			deleteFolderCount++;
		}
	}


	return true;

}

bool IsOutofDate(int y, int m, int d)
{
	int passyear = year - y;
	if (passyear > 0)
	{
		return true;
	}
	else
	{
		int passmonth = month - m + passyear*12;
		if(passmonth >0)
		{
			return true;
		}
		else
		{
			int passday = day - d + passmonth*30;
			if (passday > daylimit)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}