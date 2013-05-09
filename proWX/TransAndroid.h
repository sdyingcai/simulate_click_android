#pragma once
#include <afxmt.h>

#include <Winsock2.h>
#include <string>
using namespace std;

#ifdef __ppc__
static inline unsigned __swap_uint32(unsigned x)
{
    return (((x) & 0xFF000000) >> 24)
        | (((x) & 0x00FF0000) >> 8)
        | (((x) & 0x0000FF00) << 8)
        | (((x) & 0x000000FF) << 24);
}
#define htoll(x) __swap_uint32(x)
#define ltohl(x) __swap_uint32(x)
#define MKID(a,b,c,d) ((d) | ((c) << 8) | ((b) << 16) | ((a) << 24))
#else
#define htoll(x) (x)
#define ltohl(x) (x)
#define MKID(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))
#endif

#define SYNC_DATA_MAX (64*1024)
#define ID_SEND MKID('S','E','N','D')
#define ID_DATA MKID('D','A','T','A')
#define ID_DONE MKID('D','O','N','E')
#define ID_OKAY MKID('O','K','A','Y')

typedef union {
    unsigned id;
    struct {
        unsigned id;
        unsigned namelen;
    } req;
    struct {
        unsigned id;
        unsigned mode;
        unsigned size;
        unsigned time;
    } stat;
    struct {
        unsigned id;
        unsigned mode;
        unsigned size;
        unsigned time;
        unsigned namelen;
    } dent;
    struct {
        unsigned id;
        unsigned size;
    } data;
    struct {
        unsigned id;
        unsigned msglen;
    } status;
} syncmsg;

struct syncsendbuf {
    unsigned id;
    unsigned size;
    char data[SYNC_DATA_MAX];
};

class TransAndroid
{
public:
	TransAndroid(void);
	~TransAndroid(void);

public:
	void init();
	HANDLE createTransport();
	bool openTransport();
	bool closeTransport();
	int sendData(const char* pData, int nLen);
	int recvData(char* pData, int nLen);
	bool isOkay(char* reply);
	char* formAdbRequest(const char* pIn);

	char* DetectDev();
	bool ForwardDev(const char* pSerNum, int lPort, int rPort);
	bool InstallApkWithoutCopy(wchar_t* pPath, char* pSerNum, bool isForce, char* pReason, bool bSDCard = false);
	bool InstallApk(wchar_t* pPath, char* pSerNum, bool isForce, char* pReason, bool bFileSDCard = false, bool bInstallSDCard = false);
	bool PushApk(wchar_t* pPath, char* pSerNum, bool bSDCard = false);
	bool UninstallApk(char* pPackageName, char* pSerNum, bool isKeepUserData, char* pReason);
	bool OperateService(bool bStart, const char* pSerNum);

	bool HaveExternalSD(const char* pSerNum);

	bool sendEvent();
	bool sendEventContent(char* pData, const char* pCmd);

	static void initWSADATA();

private:
	static bool m_bWSADATA;
	SOCKET m_usbSocket;
	int m_nSocketPort;
	struct sockaddr_in m_clientService;
	CCriticalSection m_sectTion;
};
