// TestTDBAPI_v2.cpp : Defines the entry point for the console application.
//

#include "Platform.h"
#include "PathHelper.h"

#ifdef __PLATFORM_WINDOWS__
#include "stdafx.h"
#include <windows.h>
#else
#include <stdarg.h>
#endif

#include "TDBAPI.h"
#include <vector>
#include <assert.h>
#include <string>
#include <time.h>
#include <fstream>
using namespace std;
#define ELEMENT_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))


int Print(const char* szFormat, ...);   //������Խ���������������


#define TICK_BEGIN(Topic) \
	int nBefore##Topic##Tick_ = GetTickCount();

#define TICK_END(Topic) \
	int nAfter##Topic##Tick_ = GetTickCount(); \
	Print("Topic:%s time-sep:%d\n", #Topic, nAfter##Topic##Tick_ - nBefore##Topic##Tick_);

void TestLogIn(const char* szIP, const char* szPort, const char*szUser, const char* szPassword );
void TestRetrieveData(THANDLE hTdb);


std::string arr2str(int arr[], int n);



void GetTick(THANDLE hTdb, const std::string& strCode, bool bWithAB, int nStartDay, int nEndDay, int nStartTime =0, int nEndTime = 0);
void GetK(THANDLE hTdb, const std::string& strCode, CYCTYPE nCycType,int nCycDef, REFILLFLAG nFlag,  int nAutoComplete, int nStartDay, int nEndDay, int nStartTime=0, int nEndTime = 0);


void GetTransaction(THANDLE hTdb, const std::string& strCode, int nStartDay, int nEndDay, int nStartTime=0, int nEndTime=0);
void GetOrder(THANDLE hTdb, const std::string& strCode, int nStartDay, int nEndDay, int nStartTime=0, int nEndTime=0);
void GetOrderQueue(THANDLE hTdb, const std::string& strCode, int nStartDay, int nEndDay, int nStartTime=0, int nEndTime=0);
void TestFormula(THANDLE hTdb);

void Momentum(THANDLE hTdb, const std::string& strCode, int momCyc, int& monentum);
void Oscillator(THANDLE hTdb, const std::string& strCode,int momCyc, double& oscillator);
void Volatility(THANDLE hTdb, const std::string& strCode, double& volatility);

void LoadIPFromCin(OPEN_SETTINGS&);

int getLastTradeDate(THANDLE hTdb,const std::string& strCode);
int main(int argc, char* argv[])
{   
	OPEN_SETTINGS settings={"127.0.0.1","10222","user","pwd",30,2,0}; // ��¼ʱ����Ϣ,�������ֵ
	LoadIPFromCin(settings); // �����¼��Ϣ
	TDBDefine_ResLogin loginAnswer={0}; 
	THANDLE hTdb = TDB_Open(&settings, &loginAnswer);
	if (!hTdb)
	{ 
		Print("TDB_Open failed:%s, program exit!\n", loginAnswer.szInfo);
		exit(0);
	}

	string strCode="601857.sh";
	int monentum=0;
	double oscillator=0.0;
	double volatility=0.0;
	//Momentum(hTdb,strCode,12,monentum);
	//Oscillator(hTdb, strCode,1, oscillator);
	Volatility(hTdb, strCode, volatility);
	return 0;
	//������Ϊֹ��û�������
	{
		TDBDefine_Code* pCode = NULL;
		int nCount = 0;
		//��ȡ�����
		TDB_GetCodeTable(hTdb, "SZ", &pCode, &nCount);

		// std::cout << "��Ʊ�ĸ���" << nCount<< std::endl;
		/*for (int i=0; i<nCount; i++) // ��ע�͵�����Ȼ�����
		{
		// std::cout << pCode[i].chWindCode << " " << pCode[i].chMarket << " " << pCode[i].chCode << "  "  << pCode[i].chCNName <<std::endl;
		// ����������ÿ֧��Ʊ����Ϣ  ��ô��롢���������롢�г����롢֤ȯ�������ơ�֤ȯӢ�����ơ�֤ȯ���͡�
		ErrPrint("%s %s\n", pCode[i].chCode, pCode[i].chCNName);
		}*/
		std::cout << "zbz1" << std::endl;
		system("pause");
		TDB_Free(pCode);
	}
	//std::cout << "��Ʊ������Ϣ�������" << std::endl;
	const TDBDefine_Code* pCodeInfo = TDB_GetCodeInfo(hTdb, "ag.shf");
	{
		TDBDefine_Future* pFuture = NULL;
		int nItems = NULL;
		TDBDefine_ReqFuture req = {"ag.shf"};

		TDB_GetFuture(hTdb, &req, &pFuture, &nItems);
		//��ȡ�ڻ�����(���������̿�)����õĽ����Ҫ�û��������� TDB_Free ���ͷ�

		for (int i=0; i<10&&i<nItems; i++ )
		{
			TDBDefine_Future* pCur = pFuture + i;
#ifdef __PLATFORM_WINDOWS__  //�ж���linux�»���windows��
			ErrPrint("code:%s, date:%d, time:%d, price:%d, volume:%I64d, turover:%I64d, nPrice:%d\n", pCur->chWindCode, pCur->nDate, pCur->nTime, pCur->nPrice, pCur->iVolume, pCur->iTurover, pCur->nPrice);
#else
			ErrPrint("code:%s, date:%d, time:%d, price:%d, volume:%lld, turover:%lld, nPrice:%d\n", pCur->chWindCode, pCur->nDate, pCur->nTime, pCur->nPrice, pCur->iVolume, pCur->iTurover, pCur->nPrice);
#endif
		}
		ErrPrint("----------------------\n");
		for (int i=nItems - 20; i>=0 && i<nItems; i++)
		{
			TDBDefine_Future* pCur = pFuture + i;
#ifdef __PLATFORM_WINDOWS__
			ErrPrint("code:%s, date:%d, time:%d, price:%d, volume:%I64d, turover:%I64d, nPrice:%d\n", pCur->chWindCode, pCur->nDate, pCur->nTime, pCur->nPrice, pCur->iVolume, pCur->iTurover, pCur->nPrice);
#else
			ErrPrint("code:%s, date:%d, time:%d, price:%d, volume:%lld, turover:%lld, nPrice:%d\n", pCur->chWindCode, pCur->nDate, pCur->nTime, pCur->nPrice, pCur->iVolume, pCur->iTurover, pCur->nPrice);
#endif

		}
		TDB_Free(pFuture);
	}
	//std::cout << "zbz2" << std::endl;  // ������Ϊֹ��ֻ�����һ�� ��-----------------��
	system("pause");
	{
		TDBDefine_FutureAB* pFuture = NULL;
		int nItems = NULL;
		TDBDefine_ReqFuture req = {"ag.shf", 20131011};

		TDB_GetFutureAB(hTdb, &req, &pFuture, &nItems);
		//��ȡ�ڻ����飬��õĽ����Ҫ�û��������� TDB_Free ���ͷ�

		for (int i=0; i<10 && i<nItems; i++ )
		{
			TDBDefine_FutureAB* pCur = pFuture + i;
#ifdef __PLATFORM_WINDOWS__
			ErrPrint("code:%s, date:%d, time:%d, price:%d, volume:%I64d, turover:%I64d, nPrice:%d, askprice:%s, askvolume:%s, bidprice:%s, bidvolume:%s, accprice:%d, position:%d, preacc:%d, pre-position:%d\n", pCur->chWindCode, pCur->nDate,pCur->nTime, pCur->nPrice, pCur->iVolume, pCur->iTurover, pCur->nPrice, arr2str((int*)pCur->nAskPrice, ELEMENT_COUNT(pCur->nAskPrice)).c_str(), arr2str((int*)pCur->nAskVolume, ELEMENT_COUNT(pCur->nAskVolume)).c_str(), arr2str((int*)pCur->nBidPrice, ELEMENT_COUNT(pCur->nBidPrice)).c_str(),arr2str((int*)pCur->nBidVolume, ELEMENT_COUNT(pCur->nBidVolume)).c_str(), pCur->nSettle, pCur->nPosition, pCur->nPreSettle, pCur->nPrePosition);
#else
			ErrPrint("code:%s, date:%d, time:%d, price:%d, volume:%lld, turover:%lld, nPrice:%d, askprice:%s, askvolume:%s, bidprice:%s, bidvolume:%s, accprice:%d, position:%d, preacc:%d, pre-position:%d\n", pCur->chWindCode, pCur->nDate,pCur->nTime, pCur->nPrice, pCur->iVolume, pCur->iTurover, pCur->nPrice, arr2str((int*)pCur->nAskPrice, ELEMENT_COUNT(pCur->nAskPrice)).c_str(), arr2str((int*)pCur->nAskVolume, ELEMENT_COUNT(pCur->nAskVolume)).c_str(), arr2str((int*)pCur->nBidPrice, ELEMENT_COUNT(pCur->nBidPrice)).c_str(),arr2str((int*)pCur->nBidVolume, ELEMENT_COUNT(pCur->nBidVolume)).c_str(), pCur->nSettle, pCur->nPosition, pCur->nPreSettle, pCur->nPrePosition);

#endif

		}
		//std::cout << "zbz3" << std::endl; 
		//system("pause");
		ErrPrint("----------------------\n");
		//std::cout << "nItems="<< nItems <<std::endl;
		//system("pause");
		for (int i=nItems - 20; i<nItems && i>=0; i++)
		{
			//std::cout << "i=" << i <<std::endl;
			//system("pause");
			TDBDefine_FutureAB* pCur = pFuture + i;
#ifdef __PLATFORM_WINDOWS__
			ErrPrint("code:%s, date:%d, time:%d, price:%d, volume:%I64d, turover:%I64d, nPrice:%d, askprice:%s, askvolume:%s, bidprice:%s, bidvolume:%s, accprice:%d, position:%d, preacc:%d, pre-position:%d\n", pCur->chWindCode, pCur->nDate, pCur->nTime,pCur->nPrice, pCur->iVolume, pCur->iTurover, pCur->nPrice, arr2str((int*)pCur->nAskPrice, ELEMENT_COUNT(pCur->nAskPrice)).c_str(), arr2str((int*)pCur->nAskVolume, ELEMENT_COUNT(pCur->nAskVolume)).c_str(), arr2str((int*)pCur->nBidPrice, ELEMENT_COUNT(pCur->nBidPrice)).c_str(),arr2str((int*)pCur->nBidVolume, ELEMENT_COUNT(pCur->nBidVolume)).c_str() , pCur->nSettle, pCur->nPosition, pCur->nPreSettle, pCur->nPrePosition);
#else
			ErrPrint("code:%s, date:%d, time:%d, price:%d, volume:%lld, turover:%lld, nPrice:%d, askprice:%s, askvolume:%s, bidprice:%s, bidvolume:%s, accprice:%d, position:%d, preacc:%d, pre-position:%d\n", pCur->chWindCode, pCur->nDate, pCur->nTime,pCur->nPrice, pCur->iVolume, pCur->iTurover, pCur->nPrice, arr2str((int*)pCur->nAskPrice, ELEMENT_COUNT(pCur->nAskPrice)).c_str(), arr2str((int*)pCur->nAskVolume, ELEMENT_COUNT(pCur->nAskVolume)).c_str(), arr2str((int*)pCur->nBidPrice, ELEMENT_COUNT(pCur->nBidPrice)).c_str(),arr2str((int*)pCur->nBidVolume, ELEMENT_COUNT(pCur->nBidVolume)).c_str() , pCur->nSettle, pCur->nPosition, pCur->nPreSettle, pCur->nPrePosition);
#endif

		}
		TDB_Free(pFuture);
	}

	//std::cout << "zbz4" << std::endl;
	//system("pause");
	TestFormula(hTdb);  // �����������һЩ���ԣ������һЩ���߰���Ķ������Ȳ�����ô��ɡ�
	std::cout << "zbz5" << std::endl;
	system("pause");

	//���ô���Ĺ�ƱKͼ�������ص�myfile.csv�ļ���
	GetK(hTdb, "000001.sz", CYC_DAY, 1, REFILL_NONE, 0, 20130815, 20130916);
	GetK(hTdb, "000001.sz", CYC_SECOND, 1, REFILL_NONE, 0, 20120101, 20150101);

	system("pause");
	std::cout << "GetK  000300 " << std::endl;
	system("pause");

	char* arrCode[] = {"000001.sz" ,"600000.sh"};
	int arrDays[] = {20130409, 20130816, 20130902};

	for (int i=0; i<sizeof(arrCode)/sizeof(arrCode[0]); i++)
	{
		for (int j=0; j<sizeof(arrDays)/sizeof(arrDays[j]); j++)
		{
			char* pCode = arrCode[i];
			int nDate = arrDays[j];
			GetTick(hTdb, pCode, false, nDate, nDate);
			GetK(hTdb, pCode, CYC_SECOND, 1, REFILL_NONE, 0, nDate, nDate);
			GetK(hTdb, pCode, CYC_SECOND, 1, REFILL_NONE, 1, nDate, nDate);
			GetK(hTdb, pCode, CYC_SECOND, 7, REFILL_NONE, 0, nDate, nDate);
		}
	}

	GetK(hTdb, "000001.sz", CYC_SECOND, 1, REFILL_NONE, 0, 20130409, 20130409);
	GetK(hTdb, "000001.sz", CYC_SECOND, 1, REFILL_NONE, 1, 20130409, 20130409);

	GetK(hTdb, "000001.sz", CYC_SECOND, 3, REFILL_NONE, 0, 20130409, 20130409);
	GetK(hTdb, "000001.sz", CYC_SECOND, 3, REFILL_NONE, 1, 20130409, 20130409);

	//��ȡ��������
	GetTick(hTdb, "000001.sz", false, 20130409, 20130409);

	GetK(hTdb, "000001.sz", CYC_SECOND, 7, REFILL_NONE, 0, 20130409, 20130409);
	GetK(hTdb, "000001.sz", CYC_SECOND, 7, REFILL_NONE, 1, 20130409, 20130409);

	GetTick(hTdb, "000001.sz", false, 20130409, 20130409);

	GetK(hTdb, "600000.sh", CYC_DAY,  1, REFILL_NONE, 1, 20120101, 20131231);
	GetOrderQueue(hTdb, "600000.sh", 20130409, 20131231);
	GetOrder(hTdb, "600000.sh", 20130409, 20131231);
	GetTransaction(hTdb, "600000.sh", 20130409, 20131231);

	GetK(hTdb, "000300.sh", CYC_DAY,  1, REFILL_NONE, 1, 20120101, 20131231);
	GetOrderQueue(hTdb, "000300.sh", 20130409, 20131231);
	GetOrder(hTdb, "000300.sh", 20130409, 20131231);
	GetTransaction(hTdb, "000300.sh", 20130409, 20131231);


	GetOrderQueue(hTdb, "000001.sz", 20130409, 20131231);
	GetOrder(hTdb, "000001.sz", 20130409, 20131231);
	GetTransaction(hTdb, "000001.sz", 20130409, 20131231);


	GetOrderQueue(hTdb, "399300.sz", 20130409, 20131231);
	GetOrder(hTdb, "399300.sz", 20130409, 20131231);
	GetTransaction(hTdb, "399300.sz", 20130409, 20131231);


	GetTick(hTdb, "399300.sz", true, 20130409, 20130410);

	GetTick(hTdb, "000001.sz", false, 20120101, 20131231);

	GetTick(hTdb, "000001.sz", false, 20120101, 20131231);

	GetTick(hTdb, "399300.sz", true, 20120101, 20131231);


	GetTick(hTdb, "399300.sz", true, 20130409, 20130409);

	GetTick(hTdb, "000001.sz", false, 20130409, 20130409);



	GetK(hTdb, "000001.sz", CYC_SECOND, 0, REFILL_NONE, 0, 20130409, 20130409);
	GetK(hTdb, "000001.sz", CYC_SECOND, 4, REFILL_NONE, 0, 20130409, 20130409);

	GetK(hTdb, "000001.sz", CYC_SECOND, 4, REFILL_NONE, 1, 20130409, 20130409);

	GetK(hTdb, "000001.sz", CYC_SECOND, 1, REFILL_NONE, 0, 20130409, 20130409);
	GetK(hTdb, "000001.sz", CYC_SECOND, 1, REFILL_NONE, 1, 20130409, 20130409);
	GetK(hTdb, "000001.sz", CYC_SECOND, 2, REFILL_NONE, 1, 20130409, 20130409);
	GetK(hTdb, "000001.sz", CYC_SECOND, 3, REFILL_NONE, 1, 20130409, 20130409);

	GetK(hTdb, "000001.sz", CYC_MINUTE, 1, REFILL_NONE, 0, 20130409, 20130409);
	GetK(hTdb, "000001.sz", CYC_MINUTE, 1, REFILL_NONE, 1, 20130409, 20130409);
	GetK(hTdb, "000001.sz", CYC_MINUTE, 2, REFILL_NONE, 1, 20130409, 20130409);
	GetK(hTdb, "000001.sz", CYC_MINUTE, 3, REFILL_NONE, 1, 20130409, 20130409);

#if 1   //����Ǹ�ע�ͣ������1 ��ִ�� �����0��ִ��
	{
		GetTick(hTdb, "399300.sz", true, 20130409, 20130410);

		GetTick(hTdb, "000001.sz", false, 20120101, 20131231);

		GetTick(hTdb, "000001.sz", false, 20120101, 20131231);

		GetTick(hTdb, "399300.sz", true, 20120101, 20131231);

	}


	GetK(hTdb, "000001.sz", CYC_SECOND, 1, REFILL_NONE, 0, 20120101, 20131231);
	GetK(hTdb, "000001.sz", CYC_MINUTE,  1, REFILL_NONE, 0, 20120101, 20131231);
	GetK(hTdb, "000001.sz", CYC_DAY,  1, REFILL_NONE, 0, 20120101, 20131231);

	GetK(hTdb, "000001.sz", CYC_SECOND,  1, REFILL_NONE, 1, 20120101, 20131231);
	GetK(hTdb, "000001.sz", CYC_MINUTE,  1, REFILL_NONE, 1, 20120101, 20131231);
	GetK(hTdb, "000001.sz", CYC_DAY,  1, REFILL_NONE, 1, 20120101, 20131231);

	GetK(hTdb, "399300.sz", CYC_SECOND,  1, REFILL_NONE, 0, 20120101, 20131231);
	GetK(hTdb, "399300.sz", CYC_MINUTE,  1, REFILL_NONE, 0, 20120101, 20131231);
	GetK(hTdb, "399300.sz", CYC_DAY,  1, REFILL_NONE, 0, 20120101, 20131231);

	GetK(hTdb, "399300.sz", CYC_SECOND,  1, REFILL_NONE, 1, 20120101, 20131231);
	GetK(hTdb, "399300.sz", CYC_MINUTE,  1, REFILL_NONE, 1, 20120101, 20131231);
	GetK(hTdb, "399300.sz", CYC_DAY,  1, REFILL_NONE, 1, 20120101, 20131231);
	//GetK(THANDLE hTdb, const std::string& strCode, CYCTYPE nCycType, int nCycDef, REFILLFLAG nFlag,  int nAutoComplete, int nStartDay, int nEndDay, int nStartTime/*=0*/, int nEndTime/* = 0*/)
	Print("------------������Ȼ�˳�-----------------\n");

	TDB_Close(hTdb);

#endif

}

//���һ�����������յ�����
int getLastTradeDate(THANDLE hTdb,const std::string& strCode){
	time_t rawtime;
	struct tm *ptm;

	time(&rawtime);
	ptm =gmtime(&rawtime);

	//�����һ��25�ŵ��������������
	int enddate=(ptm->tm_year+1900)*10000 + (ptm->tm_mon+1)*100 + ptm->tm_mday;
	int startdate=(ptm->tm_year+1900)*10000 + (ptm->tm_mon==0?12:ptm->tm_mon)*100 +25;

	TDBDefine_ReqKLine reqK = {"", REFILL_NONE, 0, 0, CYC_DAY, 1, 1, startdate, enddate, 0, 0};
	strncpy(reqK.chCode, strCode.c_str(), sizeof(reqK.chCode)); 

	TDBDefine_KLine* pKLine = NULL;
	int nCount = 0;
	int nReT = TDB_GetKLine(hTdb, &reqK, &pKLine, &nCount);
	TDBDefine_KLine& tdbK = *(pKLine + nCount-1); //��������Ľ��������ڣ��������һ���������Ƿ�����
	int ResDate=tdbK.nDate;
	TDB_Free(pKLine);
	return ResDate;
}

void GetTick(THANDLE hTdb, const std::string& strCode, bool bWithAB, int nStartDay, int nEndDay, int nStartTime/* =0*/, int nEndTime/* = 0*/)
{
	TDBDefine_ReqTick reqTick = {"",nStartDay, nEndDay, nStartTime, nEndTime};
	strncpy(reqTick.chCode, strCode.c_str(), sizeof(reqTick.chCode));

	if (!bWithAB)
	{
		TDBDefine_Tick* pTick = NULL;
		int nCount;
		int nRet = TDB_GetTick(hTdb, &reqTick, &pTick, &nCount);

		Print("---------------------�յ�%d����գ�������:%d -----------------\n", nCount,nRet);
		for (int i=0; i<nCount && i<10; i++)
		{
			TDBDefine_Tick& tdbTick = *(pTick+i);
#ifdef __PLATFORM_WINDOWS__
			Print("code:%s, date:%d, time:%d, nprice:%d, vol:%I64d, turover:%I64d, acc vol:%I64d, acc turover:%I64d\n", tdbTick.chWindCode, tdbTick.nDate, tdbTick.nTime, tdbTick.nPrice, tdbTick.iVolume, tdbTick.iTurover, tdbTick.iAccVolume, tdbTick.iAccTurover);
#else
			Print("code:%s, date:%d, time:%d, nprice:%d, vol:%lld, turover:%lld, acc vol:%lld, acc turover:%lld\n", tdbTick.chWindCode, tdbTick.nDate, tdbTick.nTime, tdbTick.nPrice, tdbTick.iVolume, tdbTick.iTurover, tdbTick.iAccVolume, tdbTick.iAccTurover);
#endif

		}
		TDB_Free(pTick);
	}
	else
	{

		TDBDefine_TickAB* pTick = NULL;
		int nCount;
		int nRet = TDB_GetTickAB(hTdb, &reqTick, &pTick, &nCount);

		Print("---------------------�յ�%d����գ�������:%d -----------------\n", nCount, nRet);
		for (int i=0; i<nCount  && i<10; i++)
		{
			TDBDefine_TickAB& tdbTick = *(pTick+i);
			//if(20130409 != tdbTick.nDate)
#ifdef __PLATFORM_WINDOWS__
			Print("code:%s, date:%d, time:%d, nprice:%d, vol:%I64d, turover:%I64d, acc vol:%I64d, acc turover:%I64d\n", tdbTick.chWindCode, tdbTick.nDate, tdbTick.nTime, tdbTick.nPrice, tdbTick.iVolume, tdbTick.iTurover, tdbTick.iAccVolume, tdbTick.iAccTurover);
#else
			Print("code:%s, date:%d, time:%d, nprice:%d, vol:%lld, turover:%lld, acc vol:%lld, acc turover:%lld\n", tdbTick.chWindCode, tdbTick.nDate, tdbTick.nTime, tdbTick.nPrice, tdbTick.iVolume, tdbTick.iTurover, tdbTick.iAccVolume, tdbTick.iAccTurover);
#endif

		}
		TDB_Free(pTick);
	}

}

//���㶯����momCycΪ6���»���12����
//�������̼�-���յĿ��̼� = һ��ʱ���ڵĶ���
//input�����ݿ⣺htdb  ��Ʊ���ţ�strCode ʱ��Σ�momCyc(1~12 �£��ӽ��쿪ʼ����ʱ��) ����������ֵ):monentum
//output: void
void Momentum(THANDLE hTdb, const std::string& strCode,int momCyc,int& monentum){
	int nStartDay,nEndDay;
	nEndDay=getLastTradeDate(hTdb,strCode);
	nStartDay=nEndDay-10000;//һ���ʱ��
	TDBDefine_ReqKLine reqK = { "", REFILL_NONE, 0, 0, CYC_MONTH, 1, 1, nStartDay,nEndDay, 0, 0};
	//TDBDefine_ReqKLine reqK = {"", REFILL_NONE, 0, 0, CYC_DAY, 1, 1, startdate, enddate, 0, 0};
	strncpy(reqK.chCode, strCode.c_str(), sizeof(reqK.chCode));
	TDBDefine_KLine* pKLine = NULL;
	int nCount = 0;

	int nRet = TDB_GetKLine(hTdb, &reqK, &pKLine, &nCount);


	Print("---------------------�յ�%d��K�ߣ�������:%d -----------------\n", nCount, nRet);
	for (int i=0; i<nCount; i++) // ֻ�����ǰ10��
	{
		TDBDefine_KLine& tdbK = *(pKLine+i);
		cout<<tdbK.nDate<<endl;
	}
	int openLast = (pKLine+ 12 - momCyc)->nOpen; //����6�»���12�� ѡ��ʼ�·���Ϊ���̼�
	int closeToday = (pKLine+nCount-1)->nClose; //��ȡ���һ���µ����̼�
	cout<<openLast<<","<<closeToday;
	monentum = closeToday - openLast;
	system("pause");
	TDB_Free(pKLine);
}

//������ָ��
//��߼�-��ͼ�/���̼�+���̼� =�ö�ʱ�����ָ��
//input: ���ݿ⣺htdb   ��Ʊ���ţ�strCode  ʱ���ȣ�momCyc (1~12 �£��ӽ��쿪ʼ����ʱ��)  ��ָ�꣨����ֵ����oscillator
//output: void
void Oscillator(THANDLE hTdb, const std::string& strCode,int momCyc, double& oscillator){
	int nStartDay, nEndDay;
	nEndDay=getLastTradeDate(hTdb,strCode);
	nStartDay=nEndDay-10000;//һ���ʱ��
	TDBDefine_ReqKLine reqK = { "", REFILL_NONE, 0, 0, CYC_MONTH,momCyc,1, nStartDay, nEndDay,0,0 };
	strncpy(reqK.chCode, strCode.c_str(), sizeof(reqK.chCode));
	TDBDefine_KLine* pKLine = NULL;
	int nCount;
	TDB_GetKLine(hTdb, &reqK, &pKLine, &nCount);
	TDBDefine_KLine* res=pKLine+nCount-1;
	cout<<res->nDate;
	oscillator = double(res->nHigh - res->nLow) / (res->nOpen + res->nClose);
	system("pause");
	TDB_Free(pKLine);
}

//�����Ʊһ��Ĳ�����
//���� log(ÿ�����̼�/ÿ�տ��̼�) ��ƽ���� * һ�꽻����������ƽ����
//input:���ݿ�:hTdb  ��Ʊ����:strCode  һ��Ĳ�����(��Ϊ����ֵ):volatility
//output:void
void Volatility(THANDLE hTdb, const std::string& strCode, double& volatility)
{
	int nStartDay, nEndDay;
	nEndDay=getLastTradeDate(hTdb,strCode);
	nStartDay=nEndDay-10000;//һ���ʱ��
	TDBDefine_ReqKLine reqK = { "", REFILL_NONE, 0, 0, CYC_DAY,1,1, nStartDay, nEndDay,0,0 };
	strncpy(reqK.chCode, strCode.c_str(), sizeof(reqK.chCode));
	TDBDefine_KLine* pKLine = NULL;
	int nCount = 0;
	//TICK_BEGIN(minitue);1
	int nRet = TDB_GetKLine(hTdb, &reqK, &pKLine, &nCount);
	vector<double> tmp(nCount,0.0);
	//TICK_END(minitue);
	Print("---------------------�յ�%d��K�ߣ�������:%d -----------------\n", nCount, nRet);
	double sum = 0.0;
	for (int i = 0; i < nCount; i++) 
	{
		TDBDefine_KLine& tdbK = *(pKLine + i);
		tmp[i] = log((double)tdbK.nClose / tdbK.nOpen);
		sum += tmp[i];
	}
	double average = (double)sum / nCount;
	sum = 0.0;
	for (int i = 0; i < nCount; i++)
	{
		sum += pow(tmp[i]-average,2);
	}
	volatility = sqrt(sum/nCount) * sqrt(double(nCount));
	cout<<volatility;
	system("pause");
	TDB_Free(pKLine);
}

void GetK(THANDLE hTdb, const std::string& strCode, CYCTYPE nCycType, int nCycDef, REFILLFLAG nFlag,  int nAutoComplete, int nStartDay, int nEndDay, int nStartTime/*=0*/, int nEndTime/* = 0*/)
{
	TDBDefine_ReqKLine reqK = {"", nFlag, 0, 0, nCycType, nCycDef, nAutoComplete, nStartDay, nEndDay, nStartTime, nEndTime};
	strncpy(reqK.chCode, strCode.c_str(), sizeof(reqK.chCode));
	TDBDefine_KLine* pKLine = NULL;
	int nCount =0;
	//TICK_BEGIN(minitue);1
	int nRet = TDB_GetKLine(hTdb, &reqK, &pKLine, &nCount);
	//TICK_END(minitue);
	Print("---------------------�յ�%d��K�ߣ�������:%d -----------------\n", nCount, nRet);
	//ofstream outfile;
	//outfile.open("KLinefile.csv", ios::app);
	//outfile.open("GetK.csv");
	//outfile << "��Ʊ����" << "," << "����" << "," << "ʱ��" << "," << "���̼�" << "," << "��߼�" << "," <<"��ͼ�" << "," << "���̼�" << "," <<"�ɽ���" << "," << "iTurover" << endl;
	for (int i=0; i<nCount; i++) // ֻ�����ǰ10��
	{
		if (i % 1000000 == 0)
			cout << i << endl;
		TDBDefine_KLine& tdbK = *(pKLine+i);
#ifdef __PLATFORM_WINDOWS__
		//std::cout << "code:" << tdbK.chWindCode <<", date:" <<  tdbK.nDate <<", nTime:" << tdbK.nTime << std::endl;
		//outfile << tdbK.chWindCode << "," << tdbK.nDate << "," << tdbK.nTime << "," << tdbK.nOpen << "," << tdbK.nHigh << "," << tdbK.nLow << "," << tdbK.nClose << "," << tdbK.iVolume << "," << tdbK.iTurover<< endl;
		Print("code:%s, date:%d, time:%d, open:%d, high:%d, low:%d, close:%d, volume:%I64d, turover:%I64d\n", tdbK.chWindCode, tdbK.nDate, tdbK.nTime, tdbK.nOpen, tdbK.nHigh, tdbK.nLow, tdbK.nClose, tdbK.iVolume, tdbK.iTurover);
#else
		Print("code:%s, date:%d, time:%d, open:%d, high:%d, low:%d, close:%d, volume:%lld, turover:%lld\n", tdbK.chWindCode, tdbK.nDate, tdbK.nTime, tdbK.nOpen, tdbK.nHigh, tdbK.nLow, tdbK.nClose, tdbK.iVolume, tdbK.iTurover);
#endif

	}
	//outfile.close();
	system("pause");
	TDB_Free(pKLine);
}


int Print(const char* szFormat, ...)
{
	const int MAX_OUTPUT_LEN = 65534;
	int nBufSize = MAX_OUTPUT_LEN+1;
	va_list vArgs;
	va_start(vArgs, szFormat);

	char* szBuf = new char [nBufSize];
	vsnprintf_s(szBuf, nBufSize,nBufSize-1 , szFormat, vArgs);
	va_end(vArgs);

	DebugPrint(szBuf);

	printf(szBuf);

	delete[] szBuf;
	return 0;
}

void GetTransaction(THANDLE hTdb, const std::string& strCode, int nStartDay, int nEndDay, int nStartTime/*=0*/, int nEndTime/*=0*/)
{
	TDBDefine_ReqTransaction req = {"", nStartDay, nEndDay, nStartTime, nEndTime};
	strncpy(req.chCode, strCode.c_str(), sizeof(req.chCode));

	TDBDefine_Transaction* pData = NULL;
	int nCount = 0;
	int nRet = TDB_GetTransaction(hTdb, &req, &pData, &nCount);
	Print("--------�յ�%d����ʳɽ���nRet:%d -------\n", nCount, nRet);
	for (int i=0; i<nCount && i<100; i++)
	{
		TDBDefine_Transaction& transaction = pData[i];
		Print("code:%s, date:%d, time:%d, function_code:%c, chBSFlag:%c, price:%d, volume:%d\n",transaction.chWindCode, transaction.nDate, transaction.nTime, transaction.chFunctionCode?transaction.chFunctionCode:' ', transaction.chBSFlag?transaction.chBSFlag:' ', transaction.nTradePrice, transaction.nTradeVolume);
	}
	system("pause");
	TDB_Free(pData);
}

void GetOrder(THANDLE hTdb, const std::string& strCode, int nStartDay, int nEndDay, int nStartTime/*=0*/, int nEndTime/*=0*/)
{
	TDBDefine_ReqOrder req = {"", nStartDay, nEndDay, nStartTime, nEndTime};
	strncpy(req.chCode, strCode.c_str(), sizeof(req.chCode));

	TDBDefine_Order* pData = NULL;
	int nCount = 0;
	int nRet = TDB_GetOrder(hTdb, &req, &pData, &nCount);
	Print("--------�յ�%d�����ί�У�nRet:%d -------\n", nCount, nRet);
	for (int i=0; i<nCount && i<10; i++)
	{
		TDBDefine_Order& order = pData[i];
		Print("code:%s, date:%d, time:%d, chOrderKind:%c, chFunctionCode:%c, price:%d, volume:%d\n",order.chWindCode, order.nDate, order.nTime, order.chOrderKind?order.chOrderKind:' ', order.chFunctionCode?order.chFunctionCode:' ', order.nOrderPrice, order.nOrderVolume);
	}
	TDB_Free(pData);
}
void GetOrderQueue(THANDLE hTdb, const std::string& strCode, int nStartDay, int nEndDay, int nStartTime/*=0*/, int nEndTime/*=0*/)
{
	TDBDefine_ReqOrderQueue req = {"", nStartDay, nEndDay, nStartTime, nEndTime};
	strncpy(req.chCode, strCode.c_str(), sizeof(req.chCode));

	TDBDefine_OrderQueue* pData = NULL;
	int nCount = 0;
	int nRet = TDB_GetOrderQueue(hTdb, &req, &pData, &nCount);
	Print("--------�յ�%d��ί�ж��У�nRet:%d -------\n", nCount, nRet);
	for (int i=0; i<nCount && i<10; i++)
	{
		TDBDefine_OrderQueue& orderQueue = pData[i];
		Print("code:%s, date:%d, time:%d, nSide:%c, price:%d, order items:%d, ab items:%d, ab volume:%s\n", orderQueue.chWindCode, orderQueue.nDate, orderQueue.nTime, orderQueue.nSide?orderQueue.nSide:' ', orderQueue.nPrice, orderQueue.nOrderItems, orderQueue.nABItems, arr2str(orderQueue.nABVolume, orderQueue.nABItems).c_str());
	}
	TDB_Free(pData);
}

std::string arr2str(int arr[], int n)
{
	std::string str;
	str.reserve(32*n);
	for (int i=0; i<n; i++)
	{
		char szBuf[32] = {0};
		_snprintf(szBuf, sizeof(szBuf)-1, "%d", arr[i]);
		str = str + szBuf + " ";
	}
	return str;
}


//�ϴ����ؼ�����й�ʽ
void TestFormula(THANDLE hTdb)
{
	const char* pKDJFormula = 
		"INPUT:N(9), M1(3,1,100,2), M2(3);    "
		"RSV:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100;    "
		"K:SMA(RSV,M1,1);    "
		"D:SMA(K,M2,1);    "
		"J:3*K-2*D;    ";

	const char* pCloseFormula = "CLOSE;    ";

	const char* pSlideAveLine = 
		"INPUT:M1(5,2,999), M2(10,2,999), M3(30,2,999), M4(60,2,999);    "
		"MA1:MA(CLOSE,M1);    "
		"MA2:MA(CLOSE,M2);    "
		"MA3:MA(CLOSE,M3);    "
		"MA4:MA(CLOSE,M4);    ";

	const char* pMACDFormula = 
		"INPUT:SHORT(12,2,200,2), LONG(26,2,200,2), MID(9,2,200,2);    "
		"DIF:EMA(CLOSE,SHORT)-EMA(CLOSE,LONG);    "
		"DEA:EMA(DIF,MID);    "
		"MACD:(DIF-DEA)*2,COLORSTICK;    ";

	struct 
	{
		const char* szFormulaName;
		const char* szFormulaText;
	} arrFomula[] = 
	{
		"KDJ",              pKDJFormula,
		"Close",            pCloseFormula,
		"SlideAveLine",     pSlideAveLine,
		"MACD",             pMACDFormula,
	};

	for (int i=0; i<sizeof(arrFomula)/sizeof(arrFomula[0]); i++)
	{
		//���ع�ʽ
		TDBDefine_AddFormulaRes resFormula = {0};
		int nRet = TDB_AddFormula(hTdb, arrFomula[i].szFormulaName, arrFomula[i].szFormulaText, &resFormula);
		printf("�ϴ���ʽ��nRet:%d, err line:%d, text:%s, info:%s\n", nRet, resFormula.nErrLine, resFormula.chText, resFormula.chInfo);
	}

	//��ȡ�Ѿ����صĹ�ʽ
	TDBDefine_FormulaItem* pFormula = NULL;
	int nFormulaCount = 0;
	int nRet = TDB_GetFormula(hTdb, "", &pFormula, &nFormulaCount);
	for (int i=0; i<nFormulaCount; i++)
	{
		printf("��ȡ��ʽ��nRet:%d, count:%d, name:%s, param:%s\n", nRet, nFormulaCount,pFormula[i].chFormulaName, pFormula[i].chParam);
	}

	//���������
	TDBDefine_CalcFormulaRes calcResult = {0};
	TDBDefine_ReqCalcFormula calcReq = {"","", "000001.sz", CYC_MINUTE, 1, REFILL_NONE, 0, 0, 4000, 400};
	//pFormula[0].chFormulaName,  pFormula[0].chParam
	const int nFormulaIndex = 3;

	//��calcReq ��ȱ��ǰ�����ַ������� �ֱ���Ҫ���㹫ʽ������Ҫ�Ĳ���
	strncpy(calcReq.chFormulaName, pFormula[3].chFormulaName, sizeof(calcReq.chFormulaName));
	strncpy(calcReq.chParam, pFormula[3].chParam, sizeof(calcReq.chParam));

	nRet = TDB_CalcFormula(hTdb, &calcReq, &calcResult);

	printf("�������ָ�깫ʽ�����nRet:%d, record count:%d, filed count:%d\n", nRet, calcResult.nRecordCount, calcResult.nFieldCount);

	ofstream outfile;
	outfile.open("Calcfile.csv", ios::app);
	for (int i=0; i<calcResult.nFieldCount; i++)
	{
		outfile << calcResult.chFieldName[i] << ",";
		printf("%s ", calcResult.chFieldName[i]);
	}
	outfile << endl;
	printf("\n");

	for (int i=0; i<calcResult.nRecordCount; i++)
	{
		printf("--------%d----------\n", i+1);
		for (int j = 0; j<calcResult.nFieldCount; j++)
		{
			outfile << calcResult.dataFileds[j][i] << ",";
			printf("%d ", calcResult.dataFileds[j][i]);
		}
		outfile << endl;
		printf("\n");
	}
	outfile.close();
	TDB_ReleaseCalcFormula(&calcResult);
	TDB_Free(pFormula);

	//ɾ��ָ�깫ʽ
	TDBDefine_DelFormulaRes pDelRes = {0};
	nRet = TDB_DeleteFormula(hTdb, "abc", &pDelRes);//
	printf("ɾ����ʽ%s: nRet:%d, ��Ϣ��%s\n",  pDelRes.chFormularName,nRet, pDelRes.chInfo);
}

void LoadIPFromCin(OPEN_SETTINGS& settings)
{
	strcpy(settings.szIP, "114.80.154.34");
	strcpy(settings.szPort, "10061");
	strcpy(settings.szUser, "TD1033863003");
	strcpy(settings.szPassword, "46005604");
	/*std::string strLine;
	printf("Input IP:");
	std::getline(std::cin, strLine);
	strncpy(settings.szIP, strLine.c_str(), sizeof(settings.szIP)-1);
	settings.szIP[ELEMENT_COUNT(settings.szIP)-1] = 0;

	printf("Input Port:");
	std::getline(std::cin, strLine);
	strncpy(settings.szPort, strLine.c_str(), sizeof(settings.szPort)-1);
	settings.szPort[ELEMENT_COUNT(settings.szPort)-1] = 0;

	printf("Input User:");
	std::getline(std::cin, strLine);
	strncpy(settings.szUser, strLine.c_str(), sizeof(settings.szUser)-1);
	settings.szUser[ELEMENT_COUNT(settings.szUser)-1] = 0;

	printf("Input Password:");
	std::getline(std::cin, strLine);
	strncpy(settings.szPassword, strLine.c_str(), sizeof(settings.szPassword)-1);
	settings.szPassword[ELEMENT_COUNT(settings.szPassword)-1] = 0;*/
}
