#include "StdAfx.h"
#include "TransAndroid.h"
#include "afx.h"

#define BUFSIZE		40960

bool TransAndroid::m_bWSADATA = false;

TransAndroid::TransAndroid(void)
: m_usbSocket(INVALID_SOCKET)
, m_nSocketPort(5037)
{
	init();
}

TransAndroid::~TransAndroid(void)
{
	closeTransport();
}

/********************************
* Function:     initWSADATA
* Descripiton:  初始win socket
* Input:		
* Output:		
* Return:		
* other:
* autor:        liyingcai
* data:         2013-02-17
********************************/
void TransAndroid::initWSADATA()
{
	if(false == m_bWSADATA)
	{
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (NO_ERROR == iResult)
		{
			m_bWSADATA = true;
		}
	}
}

/********************************
* Function:     init
* Descripiton:  初始socket连接
* Input:		
* Output:		
* Return:		
* other:
* autor:        liyingcai
* data:         2013-02-17
********************************/
void TransAndroid::init()
{
	CSingleLock pLock(&m_sectTion);

	initWSADATA();
	if(INVALID_SOCKET == m_usbSocket)
	{
		createTransport();
		openTransport();
	}
}

/********************************
* Function:     createTransport
* Descripiton:  创建传输通道
* Input:		
* Output:		
* Return:		传输通道句柄, 0: 创建失败
* other:
* autor:        liyingcai
* data:         2012-11-22
********************************/
HANDLE TransAndroid::createTransport()
{
	m_usbSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_usbSocket == INVALID_SOCKET)
	{
		return NULL;
	}

	m_clientService.sin_family = AF_INET;
	m_clientService.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	m_clientService.sin_port = htons( m_nSocketPort);

	return (HANDLE)m_usbSocket;
}

/********************************
* Function:     openTransport
* Descripiton:  打开传输通道
* Input:		handle: 传输通道句柄
* Output:		
* Return:		false: 打开失败, true: 打开成功
* other:
* autor:        liyingcai
* data:         2012-11-22
********************************/
bool TransAndroid::openTransport()
{
	int nResult = connect( m_usbSocket, (SOCKADDR*)&m_clientService, sizeof(m_clientService));
	if (SOCKET_ERROR == nResult)
	{
		closesocket(m_usbSocket);
		m_usbSocket = INVALID_SOCKET;
		return false;
	}

	return true;
}

/********************************
* Function:     closeTransport
* Descripiton:  关闭传输通道
* Input:		handle: 传输通道句柄
* Output:		
* Return:		false: 关闭失败, true: 关闭成功
* other:
* autor:        liyingcai
* data:         2012-11-22
********************************/
bool TransAndroid::closeTransport()
{
	if (INVALID_SOCKET != m_usbSocket)
	{
		shutdown(m_usbSocket, SD_RECEIVE | SD_SEND);
		closesocket(m_usbSocket);
	}

	return true;
}

/********************************
* Function:     sendData
* Descripiton:  向传输通道写入数据
* Input:		handle: 传输通道句柄, pData: 数据内容, nLen: 数据大小
* Output:		
* Return:		写入数据个数, -1: 写入失败; >=0: 写入字符个数
* other:
* autor:        liyingcai
* data:         2012-11-22
********************************/
int TransAndroid::sendData(const char* pData, int nLen)
{
	CSingleLock pLock(&m_sectTion);
	if (INVALID_SOCKET == m_usbSocket)
	{
		return -1;
	}

	int nSend = 0;
	for( ;nSend >= 0 && nSend < nLen; )
	{
		nSend += send(m_usbSocket, pData + nSend, nLen - nSend, 0);
	}

	int nError = 0;
	if(nSend < 0)
	{
		nError = WSAGetLastError();
	}

	return nSend;
}

/********************************
* Function:     recvData
* Descripiton:  从传输通道读取数据
* Input:		handle: 传输通道句柄, pData: 数据内容, nLen: 数据大小
* Output:		
* Return:		读取数据个数, -1: 写入失败; >=0: 写入字符个数
* other:
* autor:        liyingcai
* data:         2012-11-22
********************************/
int TransAndroid::recvData(char* pData, int nLen)
{
	CSingleLock pLock(&m_sectTion);
	if (INVALID_SOCKET == m_usbSocket)
	{
		return -1;
	}

	int nError;
	int nRecv = recv(m_usbSocket, pData, nLen, 0);
	if(nRecv < 0)
	{
		nError = WSAGetLastError();
	}

	return nRecv;
}

/********************************
* Function:     isOkay
* Descripiton:  
* Input:		
* Output:		
* Return:		
* other:
* autor:        liyingcai
* data:         2013-02-17
********************************/
bool TransAndroid::isOkay(char* reply) 
{
	return reply[0] == (byte)'O' && reply[1] == (byte)'K'
		&& reply[2] == (byte)'A' && reply[3] == (byte)'Y';
}

/********************************
* Function:     formAdbRequest
* Descripiton:  
* Input:		
* Output:		
* Return:		
* other:
* autor:        liyingcai
* data:         2013-02-17
********************************/
char* TransAndroid::formAdbRequest(const char* pIn)
{
	int nLen = strlen(pIn);
	char* pData = new char[nLen + 4 + 1];
	sprintf_s(pData, nLen + 4 + 1, "%04X%s", nLen, pIn);

	return pData;
}

/********************************
* Function:     DetectDev
* Descripiton:  android设备检测
* Input:		
* Output:		
* Return:		
* other:
* autor:        liyingcai
* data:         2013-02-17
********************************/
char* TransAndroid::DetectDev()
{
	std::string strTrans = "host:devices";
	char* pLenData = formAdbRequest(strTrans.c_str());

	int nSend = sendData(pLenData, strlen(pLenData));
	if(-1 == nSend)
	{
		delete[] pLenData;
		return NULL;
	}
	delete[] pLenData;

	char buf[9] = {0};
	unsigned nTmp;
	char *tmp;

	if(-1 == recvData(buf, 8))
	{
		return NULL;
	}

	if(!isOkay(buf))
	{
		return NULL;
	}

	char*	pData = new char[BUFSIZE];
	memset(pData, 0, BUFSIZE);
	int nRecv = recvData(pData, BUFSIZE);
	TRACE("TransAndroid::DetectDev\r\n");
	TRACE(pData);
	if(-1 == nRecv)
	{
		delete[] pData;
		return false;
	}

	return pData;
}

/********************************
* Function:     ForwardDev
* Descripiton:  通过tcp连接5037端口forward 端口
* Input:		
* Output:		
* Return:		
* other:
* autor:        liyingcai
* data:         2013-02-17
********************************/
bool TransAndroid::ForwardDev(const char* pSerNum, int lPort, int rPort)
{
	char bufLocal[256] = {0};
	int nCountLocal = _snprintf(bufLocal, 255, "%d", lPort);
	if(nCountLocal <= 0)
	{
		return false;
	}

	char bufRemote[256] = {0};
	int nCountRemote = _snprintf(bufRemote, 255, "%d", rPort);
	if(nCountRemote <= 0)
	{
		return false;
	}

	std::string strTrans = "host-serial:";
	strTrans += pSerNum;
	strTrans += ":forward:";
	strTrans += "tcp:";
	strTrans += bufLocal;
	strTrans += ";";
	strTrans += "tcp:";
	strTrans += bufRemote;
	char* pTrans = formAdbRequest(strTrans.c_str());

	int nSend = send(m_usbSocket, pTrans, strlen(pTrans), 0);
	if(-1 == nSend)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;

	char pData[1024] = {0};
	int nRecv = recv(m_usbSocket, pData, 1024, 0);
	if(nRecv < 0)
	{
		int nError = WSAGetLastError();
		return false;
	}

	bool bOK = isOkay(pData);
	if(!bOK)
	{
		return false;
	}

	return true;
}

/********************************
* Function:     UninstallApk
* Descripiton:  通过tcp连接5037端口卸载apk
* Input:		
* Output:		
* Return:		
* other:
* autor:        liyingcai
* data:         2013-02-17
********************************/
bool TransAndroid::UninstallApk(char* pPackageName, char* pSerNum, bool isKeepUserData, char* pReason)
{
	std::string strTrans = "host:transport:";
	strTrans += pSerNum;
	char* pTrans = formAdbRequest(strTrans.c_str());

	int nSend = sendData(pTrans, strlen(pTrans));
	if(-1 == nSend)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;

	pTrans = new char[4];
	recvData(pTrans, 4);
	bool bOK = isOkay(pTrans);
	if(false == bOK)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;

	std::string strApk = "shell:pm uninstall ";
	if(isKeepUserData)
	{
		strApk += "-k ";
	}
	strApk += pPackageName;
	char* pForm = formAdbRequest(strApk.c_str());
	nSend = sendData(pForm, strlen(pForm));

	Sleep(100);
	int nError = 0;
	if(nSend < 0)
	{
		nError = WSAGetLastError();
		return false;
	}

	char*	pData = new char[BUFSIZE];
	memset(pData, 0, BUFSIZE);
	int nRecv = recvData(pData, BUFSIZE);
	if(-1 == nRecv)
	{
		delete[] pData;
		return false;
	}

	TRACE("TransAndroid::UninstallApk\r\n");
	TRACE(pData);

	std::string strResult;
	strResult.append(pData);
	delete[] pData;

	if(std::string::npos != strResult.find("Success"))
	{
		return true;
	}
	else
	{
		int nBegin = strResult.find("[");
		int nEnd = strResult.find("]");
		std::string strData = strResult.substr(nBegin + 1, nEnd - nBegin - 1);
		if(pReason != NULL)
		{
			memcpy(pReason, strData.c_str(), nEnd - nBegin);
		}
	}

	return false;
}

/********************************
* Function:     PushApk
* Descripiton:  通过tcp连接5037端口把文件push到设备
* Input:		
* Output:		
* Return:		
* other:
* autor:        liyingcai
* data:         2013-02-17
********************************/
bool TransAndroid::PushApk(wchar_t* pPath, char* pSerNum, bool bSDCard /*= false*/)
{
	std::string strTrans = "host:transport:";
	strTrans += pSerNum;
	char* pTrans = formAdbRequest(strTrans.c_str());

	int nSend = sendData(pTrans, strlen(pTrans));
	if(-1 == nSend)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;

	pTrans = new char[4];
	recvData(pTrans, 4);
	bool bOK = isOkay(pTrans);
	if(false == bOK)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;

	char* pForm = formAdbRequest("sync:");
	nSend = sendData(pForm, strlen(pForm));

	struct _stat st;
	if(0 != _wstat(pPath, &st))
	{
		delete[] pForm;
		return false;
	}

	delete[] pForm;
	pForm = NULL;

	std::string strDest = "/data/local/tmp/TmpMobileGame.apk";
	if(bSDCard)
	{
		strDest = "/sdcard/tmp/TmpMobileGame.apk";
	}

	int len = strlen(strDest.c_str());
	char tmp[64] = {0};
	_snprintf(tmp, sizeof(tmp), ",%d", st.st_mode);
	int r = strlen(tmp);

	syncmsg msg;
	msg.req.id = ID_SEND;
	msg.req.namelen = htoll(len + r);

	nSend = sendData((char*)&(msg.req), sizeof(msg.req));

	int nError = 0;
	if(nSend < 0)
	{
		nError = WSAGetLastError();
		return false;
	}

	nSend = sendData(strDest.c_str(), strlen(strDest.c_str()));

	if(nSend < 0)
	{
		nError = WSAGetLastError();
		return false;
	}

	nSend = sendData(tmp, r);

	if(nSend < 0)
	{
		nError = WSAGetLastError();
		return false;
	}

	HANDLE handle = CreateFile(pPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		0, NULL);
	if(INVALID_HANDLE_VALUE == handle)
	{
		return false;
	}

	int size = SetFilePointer(handle, 0, NULL, FILE_END);
	SetFilePointer(handle, 0, NULL, FILE_BEGIN);

	char* file_buffer = (char*)malloc(size);
	DWORD nRead = 0;
	BOOL bRead = ReadFile(handle, file_buffer, size, &nRead, NULL);
	CloseHandle(handle);

	syncsendbuf sbuf;
	int err = 0;
	int total = 0;

	sbuf.id = ID_DATA;
	while (total < size) {
		int count = size - total;
		if (count > SYNC_DATA_MAX) {
			count = SYNC_DATA_MAX;
		}

		memcpy(sbuf.data, &file_buffer[total], count);
		sbuf.size = htoll(count);
		if(-1 == sendData((char*)&sbuf, sizeof(unsigned) * 2 + count)){
			int aa = ::GetLastError ();
			err = -1;
			nError = WSAGetLastError();
			break;
		}
		total += count;
	}

	if(-1 == err)
	{
		free(file_buffer);
		return false;
	}
	free(file_buffer);

	msg.data.id = ID_DONE;
	msg.data.size = htoll(st.st_mtime);

	nSend = sendData((char*)&(msg.data), sizeof(msg.data));

	if(nSend < 0)
	{
		nError = WSAGetLastError();
		return false;
	}

	if(-1 == recvData((char*)(&msg.status), sizeof(msg.status)))
	{
		return false;
	}

	if(msg.status.id != ID_OKAY)
	{
		return false;
	}

	return true;
}



bool TransAndroid::HaveExternalSD(const char* pSerNum)
{
	std::string strTrans = "host:transport:";
	strTrans += pSerNum;
	char* pTrans = formAdbRequest(strTrans.c_str());

	int nSend = sendData(pTrans, strlen(pTrans));
	if(-1 == nSend)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;

	pTrans = new char[4];
	recvData(pTrans, 4);
	bool bOK = isOkay(pTrans);
	if(false == bOK)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;

	std::string strShell = "shell:df";

	char* pForm = formAdbRequest(strShell.c_str());
	nSend = sendData(pForm, strlen(pForm));

	Sleep(100);
	int nError = 0;
	if(nSend < 0)
	{
		nError = WSAGetLastError();

		delete[] pForm;
		return false;
	}

	delete[] pForm;

	char*	pData = new char[BUFSIZE];
	memset(pData, 0, BUFSIZE);
	int nRecv = recvData(pData, BUFSIZE);
	if(-1 == nRecv)
	{
		delete[] pData;
		return false;
	}

	string strDF = pData;
	delete[] pData;

	//	if(string::npos != strDF.find("/mnt/sdcard/external_sd") )
	if(string::npos != strDF.find("/mnt/sdcard") )
	{
		return true;
	}
	else
	{
		return false;
	}
}


/********************************
* Function:     InstallApk
* Descripiton:  通过tcp连接5037端口安装apk到设备
* Input:		
* Output:		
* Return:		
* other:
* autor:        liyingcai
* data:         2013-02-17
********************************/
bool TransAndroid::InstallApk(wchar_t* pPath, char* pSerNum, bool isForce, char* pReason, bool bFileSDCard/* = false*/, bool bInstallSDCard/* = false*/)
{
	std::string strTrans = "host:transport:";
	strTrans += pSerNum;
	char* pTrans = formAdbRequest(strTrans.c_str());

	int nSend = sendData(pTrans, strlen(pTrans));
	if(-1 == nSend)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;

	pTrans = new char[4];
	recvData(pTrans, 4);
	bool bOK = isOkay(pTrans);
	if(false == bOK)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;

	std::string strDest = "/data/local/tmp/TmpMobileGame.apk";
	if(bFileSDCard)
	{
		strDest = "/sdcard/tmp/TmpMobileGame.apk";
	}

	std::string strApk = "shell:pm install ";
	if(isForce)
	{
		strApk += "-r ";
	}

	if (bInstallSDCard)
	{
		strApk += "-s ";
	}

	strApk += strDest;

	char* pForm = formAdbRequest(strApk.c_str());
	nSend = sendData(pForm, strlen(pForm));

	Sleep(1000);
	int nError = 0;
	if(nSend < 0)
	{
		nError = WSAGetLastError();

		delete[] pForm;
		return false;
	}

	delete[] pForm;

	char*	pData = new char[BUFSIZE];
	memset(pData, 0, BUFSIZE);
	int nRecv = recvData(pData, BUFSIZE);
	if(-1 == nRecv)
	{
		delete[] pData;
		return false;
	}

	bOK = isOkay(pData);
	if( strncmp(pData+4, "Error", 5) == 0 )
		bOK = false;
	delete[] pData;

	return bOK;
}

/********************************
* Function:     InstallApkWithoutCopy
* Descripiton:  通过tcp连接5037端口安装apk到设备
* Input:		
* Output:		
* Return:		
* other:
* autor:        liyingcai
* data:         2013-02-17
********************************/
bool TransAndroid::InstallApkWithoutCopy(wchar_t* pPath, char* pSerNum, bool isForce, char* pReason, bool bInstallSDCard /*= false*/)
{
	bool bFileSDCard = bInstallSDCard;
	bool bRet = PushApk(pPath, pSerNum, bFileSDCard);
	if(bRet)
	{
		closeTransport();
		createTransport();
		openTransport();
		if( (bRet=InstallApk(pPath, pSerNum, isForce, pReason, bFileSDCard, bInstallSDCard)) == false )
		{
			bInstallSDCard = !bInstallSDCard;
			closeTransport();
			createTransport();
			openTransport();
			bRet=InstallApk(pPath, pSerNum, isForce, pReason, bFileSDCard, bInstallSDCard);
		}
	}

	return bRet;
}

/********************************
* Function:     OperateService
* Descripiton:  通过tcp连接5037端口开启或者关闭设备上爱游助手服务
* Input:		
* Output:		
* Return:		
* other:
* autor:        liyingcai
* data:         2013-02-17
********************************/
bool TransAndroid::OperateService(bool bStart, const char* pSerNum)
{
	std::string strTrans = "host:transport:";
	strTrans += pSerNum;
	char* pTrans = formAdbRequest(strTrans.c_str());

	int nSend = sendData(pTrans, strlen(pTrans));
	if(-1 == nSend)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;

	pTrans = new char[4];
	recvData(pTrans, 4);
	bool bOK = isOkay(pTrans);
	if(false == bOK)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;

	std::string strService = "shell:am broadcast -a NotifyServiceStart";
	if(!bStart)
	{
		strService = "shell:am broadcast -a NotifyServiceStop";
	}

	char* pForm = formAdbRequest(strService.c_str());
	nSend = sendData(pForm, strlen(pForm));

	Sleep(100);
	int nError = 0;
	if(nSend < 0)
	{
		nError = WSAGetLastError();

		delete[] pForm;
		return false;
	}

	delete[] pForm;

	char*	pData = new char[BUFSIZE];
	memset(pData, 0, BUFSIZE);
	int nRecv = recvData(pData, BUFSIZE);
	if(-1 == nRecv)
	{
		delete[] pData;
		return false;
	}

	if(NULL == strstr(pData, "completed"))
	{
		return false;
	}

	delete[] pData;

	return true;
}

bool TransAndroid::sendEvent()
{
	return true;
}

bool TransAndroid::sendEventContent(char* pSerNum, const char* pCmd)
{
	TRACE("\r\n");
	TRACE(pCmd);
	std::string strTrans = "host:transport:";
	strTrans += pSerNum;
	char* pTrans = formAdbRequest(strTrans.c_str());

	int nSend = sendData(pTrans, strlen(pTrans));
	if(-1 == nSend)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;

	pTrans = new char[4];
	recvData(pTrans, 4);
	bool bOK = isOkay(pTrans);
	if(false == bOK)
	{
		delete[] pTrans;
		return false;
	}
	delete[] pTrans;
 
	std::string strShell;
	strShell.append(pCmd);

	char* pForm = formAdbRequest(strShell.c_str());
	nSend = sendData(pForm, strlen(pForm));

	Sleep(100);
	int nError = 0;
	if(nSend < 0)
	{
		nError = WSAGetLastError();

		delete[] pForm;
		return false;
	}

	delete[] pForm;

	char*	pData = new char[BUFSIZE];
	memset(pData, 0, BUFSIZE);
	int nRecv = recvData(pData, BUFSIZE);
	if(-1 == nRecv)
	{
		delete[] pData;
		return false;
	}


	TRACE("\r\n");
	TRACE(pData);
	TRACE("\r\n");

	string strDF = pData;
	delete[] pData;

	return true;
}