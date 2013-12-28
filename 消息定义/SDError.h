/*!
!module SDError错误处理组件
!moduletype common
SGDP平台的基础错误处理组件。
主要的功能有：
.输出调试信息和错误信息。
.捕获异常，并以用户设定的方式处理异常。
.提供全自动的异常提交机制。
 
此组件全进程有效，多线程安全。
即在应用程序初始化SDError错误处理组件后，在此进程内发生的所有异常，包括其使用的其它组件发生的异常，都将被捕获并按照用户的设定处理。
SGDP平台其它组件也使用SDError来输出错误信息，对错误处理模式的修改将影响此进程使用的其它组件的错误输出模式。

在此组件中，我们把输出信息分成7种：
.Debug 调试信息：缺省处理模式是在DebugView中输出信息。
.Warn 警告信息：缺省处理模式是在DebugView中输出信息。
.Error 发生未预期的异常：一般是指开发者没有预料到的异常，且没有提供相应的异常处理代码。缺省处理模式是在DebugView中输出信息后抛出异常，由SDError的异常处理机制进行处理。
.FatalError 严重错误：一般指开发者预料到的可能出现的导致程序不能继续运行的错误，比如用户没有安装DirectX。缺省处理模式是使用MessageBox输出信息后直接退出程序，不抛出异常。
.Log 记录信息：需要保留记录的信息。
.RemoteLog 远程记录信息：需要记录到远程服务器上的信息。
.Dump 程序执行状态信息：包含程序执行状态和执行环境的信息。
 
此组件的具体使用示例见!!!link \示例\组件示例\SDError@Demo1@异常处理.htm SDError Demo1 异常处理!!!。

!inc SDError.h
*/ 

#pragma once
#include "SDBase.h"
#include <Winnt.h>


namespace SGDP {

// 模块名称
const char SDERROR_MODULENAME[] = "SDError";

// 版本号
const SSGDPVersion	SDERROR_VERSION = {0,3,0,1};


//=============================================================================================================================
/*! 
!macro ST_
异常捕获宏。
一般情况下，用户不必使用这些宏。只有当用户希望以自己的方式处理异常时才需要用到这些异常处理宏。
具体使用示例见!!!link \示例\组件示例\SDError@Demo1@异常处理.htm SDError Demo1 异常处理!!!。
!inc SDError.h
*/
#define STB						try {									//! 异常捕获开始
#define STE						} catch(...) { }						//! 异常捕获结束，如果在此期间发生异常，将吞掉此异常，不做任何处理和报告。
#define STER(s)					} catch(...) { return s; }				//! 异常捕获结束，如果在此期间发生异常，将吞掉此异常，不做任何处理和报告。然后将return s。
#define STEI(s)					} catch(...) { SDErrorCatch(1,s); 		//! 异常捕获结束，如果在此期间发生异常，将引起一个Error事件。
#define STEF(s)					} catch(...) { SDErrorCatch(2,s); }		//! 异常捕获结束，如果在此期间发生异常，将引起一个FatalError事件。


#define SDERROR_ADDRFLAG				"[@P#]"					// 这个常量标识一个地址，此地址可以由转换器自动转换为源代码文件名和行号


/*! 
!macro SDERROR_TYPE_
错误输出类型。
!inc SDError.h
*/
#define SDERROR_TYPE_DEBUG					0				//! 调试
#define SDERROR_TYPE_WARN					1				//! 警告
#define SDERROR_TYPE_FATALERROR				2				//! 严重错误
#define SDERROR_TYPE_ERROR					3				//! 异常
#define SDERROR_TYPE_LOG					4				//! LOG
#define SDERROR_TYPE_REMOTELOG				5				//! 远程LOG
#define SDERROR_TYPE_DUMP					6				//! DUMP
#define SDERROR_TYPE_MAX					7
#define SDERROR_TYPE_UNKNOW					0xFFFF


/*! 
!macro SDERROR_OUTPUT_
错误处理模式。
设定Debug、Warn、Error、FatalError模式下，信息输出和程序运行的方式。
这些宏可以以OR的方式组合起来使用。
具体使用示例见!!!link \示例\组件示例\SDError@Demo1@异常处理.htm SDError Demo1 异常处理!!!。
!inc SDError.h
*/
#define SDERROR_OUTPUT_NOTHROW				0x00000001		//! 输出后不做处理，配置文件中标识为N
#define SDERROR_OUTPUT_THROW				0x00000002		//! 输出后抛出异常，配置文件中标识为T
#define SDERROR_OUTPUT_EXIT					0x00000004		//! 输出后退出，配置文件中标识为E
#define SDERROR_OUTPUT_MESSAGEBOX			0x00000100		//! 以对话框形式输出错误，配置文件中标识为M
#define SDERROR_OUTPUT_DEBUGWND				0x00000200		//! 在DebugView窗口中输出信息，配置文件中标识为D
#define SDERROR_OUTPUT_ERRORDUMP			0x00000400		//! 把错误信息发送给服务器，配置文件中标识为O
#define SDERROR_OUTPUT_USERDUMP				0x00000800		//! 把用户信息发送给服务器，配置文件中标识为U
#define SDERROR_OUTPUT_CRASHDUMP			0x00001000		//! 把崩溃信息发送给服务器，此种模式仅供内部使用，配置文件中标识为C
#define SDERROR_OUTPUT_LOGFILE				0x00010000		//! 以LOG形式输出错误，配置文件中标识为L
#define SDERROR_OUTPUT_REMOTELOG			0x00020000		//! 以LOG形式输出错误信息到远程服务器，配置文件中标识为R


/*! 
!macro SDERROR_DUMPFORMAT_
发送给服务器的DUMP报告包含的内容。
!inc SDError.h
*/
#define SDERROR_DUMPFORMAT_SMALL			0x00000001		//! 最小的崩溃报告，只包含调用堆栈信息，配置文件中标识为S
#define SDERROR_DUMPFORMAT_NORMAL			0x00000002		//! 中等尺寸的崩溃报告，包含完整的堆栈信息，配置文件中标识为N
#define SDERROR_DUMPFORMAT_FULL				0x00000004		//! 完整的崩溃报告，包含整个用户内存，配置文件中标识为F
#define SDERROR_DUMPFORMAT_SYSTEMINFO1		0x00000100		//! 包含客户端环境信息1，为能够快速得到的信息，配置文件中标识为A
#define SDERROR_DUMPFORMAT_SYSTEMINFO2		0x00000200		//! 包含客户端环境信息2，为需要一定时间得到的信息，配置文件中标识为B
#define SDERROR_DUMPFORMAT_SCREENSHOT_HIGH	0x00001000		//! 包含客户端桌面截图，高精度，配置文件中标识为H
#define SDERROR_DUMPFORMAT_SCREENSHOT_MED	0x00002000		//! 包含客户端桌面截图，中等精度，配置文件中标识为M
#define SDERROR_DUMPFORMAT_SCREENSHOT_LOW	0x00004000		//! 包含客户端桌面截图，低精度，配置文件中标识为L
#define SDERROR_DUMPFORMAT_EXTRAINFO		0x00010000		//! 额外的信息，配置文件中标识为E
#define SDERROR_DUMPFORMAT_USERINFO1		0x00020000		//! 包含用户输入的信息1，配置文件中标识为1
#define SDERROR_DUMPFORMAT_USERINFO2		0x00040000		//! 包含用户输入的信息2，配置文件中标识为2
#define SDERROR_DUMPFORMAT_USERINFO3		0x00080000		//! 包含用户输入的信息3，配置文件中标识为3


/*! 
!macro SDERROR_LOGFORMAT_
Log文件组织方式。
!inc SDError.h
*/
#define SDERROR_LOGFORMAT_SINGLE			0x00000001		//! 所有记录存放在一个文件里，配置文件中标识为S
#define SDERROR_LOGFORMAT_DIVIDEBYMODULE	0x00000002		//! 按照模块名分文件存放，配置文件中标识为M
#define SDERROR_LOGFORMAT_DIVIDEBYTIME		0x00000004		//! 按照时间分文件存放，配置文件中标识为T；此模式必须和SDERROR_LOGFORMAT_DIVIDEBYMODULE模式同时使用


/*! 
!macro SDERROR_VERSION_
版本比对结果。
!inc SDError.h
*/
#define SDERROR_VERSION_OLD					-2				//! 请求的组件版本与当前实际组件版本不兼容，当前版本过旧
#define SDERROR_VERSION_NEW					-1				//! 请求的组件版本与当前实际组件版本不兼容，当前版本过新
#define SDERROR_VERSION_COMPATIBLE			1				//! 请求的组件版本与当前实际组件版本兼容
#define SDERROR_VERSION_OK					2				//! 请求的组件版本与当前实际组件版本完全相同


//=============================================================================================================================
/*!typedef
退出回调函数类型定义。
当发生异常后，按照用户设定，错误处理组件关闭应用程序前，将调用此回调函数。
用户可以在此回调函数中做资源释放等工作。
!inc SDError.h
*/
typedef void (*SDERROR_EXIT_CB)();		

/*!typedef
信息发送回调函数类型定义。
当需要把信息提交给服务器时，会调用此回调函数。
参数为一个1024字节长度的字符串，已经存放了对要提交信息的描述。
用户可以把要附加的信息加在此字符串的后面。
!inc SDError.h
*/
typedef void (*SDERROR_SENDERROR_CB)(char *);



//=============================================================================================================================
/*!cls
完成异常捕获、追踪、输出等功能的类。
!inc SDError.h
*/
class ISDError : public ISDBase
{
public:
	/************************************************************************************/
	//!group 初始化
	/*!func
	初始化错误处理组件。
	!return	初始化是否成功。
	*/
	virtual bool 			SDAPI	InitErrorModule		() = 0;


	/************************************************************************************/
	//!group 错误处理和信息输出的模式
	/*!func
	设置错误处理和信息输出的模式。
	!param [in] 错误错误处理和信息输出的类型，见SDERROR_TYPE_。
	!param [in] 错误错误处理和信息输出的模式，见SDERROR_OUTPUT_。
	!return	操作是否成功。
	*/
	virtual bool			SDAPI	SetOutputMode		(unsigned long dwType, unsigned long dwMode) = 0;

	/*!func
	获取当前错误处理和信息输出的模式。
	!param [in] 错误错误处理和信息输出的类型，见SDERROR_TYPE_。
	!return	如果操作失败，返回0；否则返回错误错误处理和信息输出的模式，见SDERROR_OUTPUT_。
	*/
	virtual unsigned long	SDAPI	GetOutputMode		(unsigned long dwType) = 0;


	/*!func
	设置信息输出对话框的输出模式。
	!param [in] 信息输出对话框的标题。
	*/
	virtual void			SDAPI	SetMessageBoxFormat	(const char * strTitle) = 0;

	/*!func
	获取当前信息输出对话框的输出模式。
	!return	如果操作失败，返回NULL；否则返回信息输出对话框的标题。
	*/
	virtual const char *	SDAPI	GetMessageBoxFormat	() = 0;


	/*!func
	设置退出程序的模式。
	!param [in] 退出前调用的回调函数，见SDERROR_EXIT_CB。
	*/
	virtual void			SDAPI	SetExitFormat		(SDERROR_EXIT_CB pfnExitCB) = 0;

	/*!func
	获取当前退出程序的模式.
	!return	如果操作失败，返回NULL；否则返回退出前调用的回调函数，见SDERROR_EXIT_CB。
	*/
	virtual SDERROR_EXIT_CB	SDAPI	GetExitFormat		() = 0;


	/*!func
	设置LOG的模式。
	!param [in] Log模式，见SDERROR_LOGFORMAT_。
	!param [in] Log文件的文件名。
	*/
	virtual void			SDAPI	SetLogFormat		(unsigned long dwFormat, const char * strLogFile=0 ) = 0;

	/*!func
	获取当前LOG的模式。
	!param [out] Log文件的文件名。
	!return Log模式，见SDERROR_LOGFORMAT_。
	*/
	virtual unsigned long	SDAPI	GetLogFormat		(const char ** pstrLogFile) = 0;


	/*!func
	设置远程LOG的模式。
	!param [in] LOG服务器，可以时ip地址或者域名地址和端口号，可以用分号分割多个服务器地址。例如：www.snda.com/log.asp;202.100.96.233:1234/remotelog.php
	*/
	virtual void			SDAPI	SetRemoteLogFormat	(const char * strServerAddr) = 0;

	/*!func
	获取当前远程LOG的模式。
	!return LOG服务器。
	*/
	virtual const char *	SDAPI	GetRemoteLogFormat	() = 0;


	/*!func
	设置崩溃DUMP的输出模式。
	!param [in] 崩溃DUMP的输出模式，见SDERROR_DUMPFORMAT_。
	!param [in] 错误输出服务器，可以时ip地址或者域名地址和端口号，可以用分号分割多个服务器地址。例如：www.snda.com/bugreport.asp;202.100.96.233:1234/crash.php
	!param [in] 输出用户信息前的回调函数，见SDERROR_SENDERROR_CB。
	!param [in] 用户信息输入对话框的总体说明，最大长度256个字。
	!param [in] 用户信息输入对话框的第1个输入框的说明，最大长度32个字。
	!param [in] 用户信息输入对话框的第2个输入框的说明，最大长度32个字。
	!param [in] 用户信息输入对话框的第3个输入框的说明，最大长度64个字。
	!return 操作是否成功。
	*/
	virtual bool			SDAPI	SetCrashDumpFormat	(unsigned long dwFormat, const char * strServerAddr, SDERROR_SENDERROR_CB pfnSendErrorCB=0, const char * strTitle=0, const char * strInfo1=0, const char * strInfo2=0, const char * strInfo3=0) = 0;

	/*!func
	获取崩溃DUMP的输出模式。
	!param [out] 崩溃DUMP的输出模式，见SDERROR_DUMPFORMAT_。
	!param [out] 错误输出服务器。
	!param [out] 输出用户信息前的回调函数，见SDERROR_SENDERROR_CB。
	!param [out] 用户信息输入对话框的总体说明。
	!param [out] 用户信息输入对话框的第1个输入框的说明。
	!param [out] 用户信息输入对话框的第2个输入框的说明。
	!param [out] 用户信息输入对话框的第3个输入框的说明。
	!return 操作是否成功。
	*/
	virtual bool			SDAPI	GetCrashDumpFormat	(unsigned long * pdwFormat, const char ** pstrServerAddr, SDERROR_SENDERROR_CB * pfnSendErrorCB=0, const char ** pstrTitle=0, const char ** pstrInfo1=0, const char ** pstrInfo2=0, const char ** pstrInfo3=0) = 0;


	/*!func
	设置错误DUMP的输出模式。
	!param [in] 崩溃DUMP的输出模式，见SDERROR_DUMPFORMAT_。
	!param [in] 错误输出服务器，可以时ip地址或者域名地址和端口号，可以用分号分割多个服务器地址。例如：www.snda.com/bugreport.asp;202.100.96.233:1234/crash.php
	!param [in] 输出用户信息前的回调函数，见SDERROR_SENDERROR_CB。
	!param [in] 用户信息输入对话框的总体说明，最大长度256个字。
	!param [in] 用户信息输入对话框的第1个输入框的说明，最大长度32个字。
	!param [in] 用户信息输入对话框的第2个输入框的说明，最大长度32个字。
	!param [in] 用户信息输入对话框的第3个输入框的说明，最大长度64个字。
	!return 操作是否成功。
	*/
	virtual bool			SDAPI	SetErrorDumpFormat	(unsigned long dwFormat, const char * strServerAddr, SDERROR_SENDERROR_CB pfnSendErrorCB=0, const char * strTitle=0, const char * strInfo1=0, const char * strInfo2=0, const char * strInfo3=0) = 0;

	/*!func
	获取错误DUMP的输出模式。
	!param [out] 崩溃DUMP的输出模式，见SDERROR_DUMPFORMAT_。
	!param [out] 错误输出服务器。
	!param [out] 输出用户信息前的回调函数，见SDERROR_SENDERROR_CB。
	!param [out] 用户信息输入对话框的总体说明。
	!param [out] 用户信息输入对话框的第1个输入框的说明。
	!param [out] 用户信息输入对话框的第2个输入框的说明。
	!param [out] 用户信息输入对话框的第3个输入框的说明。
	!return 操作是否成功。
	*/
	virtual bool			SDAPI	GetErrorDumpFormat	(unsigned long * pdwFormat, const char ** pstrServerAddr, SDERROR_SENDERROR_CB * pfnSendErrorCB=0, const char ** pstrTitle=0, const char ** pstrInfo1=0, const char ** pstrInfo2=0, const char ** pstrInfo3=0) = 0;

	
	/*!func
	用户DUMP的输出模式。
	!param [in] 崩溃DUMP的输出模式，见SDERROR_DUMPFORMAT_。
	!param [in] 错误输出服务器，可以时ip地址或者域名地址和端口号，可以用分号分割多个服务器地址。例如：www.snda.com/bugreport.asp;202.100.96.233:1234/crash.php
	!param [in] 输出用户信息前的回调函数，见SDERROR_SENDERROR_CB。
	!param [in] 用户信息输入对话框的总体说明，最大长度256个字。
	!param [in] 用户信息输入对话框的第1个输入框的说明，最大长度32个字。
	!param [in] 用户信息输入对话框的第2个输入框的说明，最大长度32个字。
	!param [in] 用户信息输入对话框的第3个输入框的说明，最大长度64个字。
	!return 操作是否成功。
	*/
	virtual bool			SDAPI	SetUserDumpFormat	(unsigned long dwFormat, const char * strServerAddr, SDERROR_SENDERROR_CB pfnSendErrorCB=0, const char * strTitle=0, const char * strInfo1=0, const char * strInfo2=0, const char * strInfo3=0) = 0;

	/*!func
	获取用户DUMP的输出模式。
	!param [out] 崩溃DUMP的输出模式，见SDERROR_DUMPFORMAT_。
	!param [out] 错误输出服务器。
	!param [out] 输出用户信息前的回调函数，见SDERROR_SENDERROR_CB。
	!param [out] 用户信息输入对话框的总体说明。
	!param [out] 用户信息输入对话框的第1个输入框的说明。
	!param [out] 用户信息输入对话框的第2个输入框的说明。
	!param [out] 用户信息输入对话框的第3个输入框的说明。
	!return 操作是否成功。
	*/
	virtual bool			SDAPI	GetUserDumpFormat	(unsigned long * pdwFormat, const char ** pstrServerAddr, SDERROR_SENDERROR_CB * pfnSendErrorCB=0, const char ** pstrTitle=0, const char ** pstrInfo1=0, const char ** pstrInfo2=0, const char ** pstrInfo3=0) = 0;


	/*!func
	设置产品信息。
	设置当前产品的产品名称，序列号，版本号。
	!param [in] 产品名称。
	!param [in] 产品序列号。
	!param [in] 产品版本号。
	!return 操作是否成功。
	*/
	virtual bool			SDAPI	SetProductInfo		(const char * strProduct, const char * strSeries, const char * strVersion) = 0;

	/*!func
	设置产品信息。
	使用配置文件中的信息设置当前产品的产品名称，序列号，版本号。
	!param [in] 在Common段中包含Product和Series信息的配置文件。
	!param [in] 在Common段中包含Version信息的配置文件。
	!return 操作是否成功。
	*/
	virtual bool			SDAPI	SetProductInfo		(const char * strConfigFile = 0, const char * strVersionFile = 0) = 0;

	/*!func
	从配置文件获取设置信息。
	!param [in] 配置文件文件名。
	!return 操作是否成功。
	*/
	virtual bool			SDAPI	LoadConfigFromIni	(const char * strIniFile = 0) = 0;


	/************************************************************************************/
	//!group 信息输出
	/*!func
	警告。
	!param [in] 要输出的信息。
	!param [in] 输出信息的组件名。
	*/
	virtual void			SDAPI	Warn				(const char * strInfo, const char * strModuleName = 0) = 0;

	/*!func
	未预期的错误。
	!param [in] 要输出的信息。
	!param [in] 输出信息的组件名。
	*/
	virtual void			SDAPI	Error				(const char * strInfo, const char * strModuleName = 0) = 0;

	/*!func
	预期的错误。
	!param [in] 要输出的信息。
	!param [in] 输出信息的组件名。
	*/
	virtual void			SDAPI	FatalError			(const char * strInfo, const char * strModuleName = 0) = 0;

	/*!func
	调试。
	!param [in] 要输出的信息。
	!param [in] 输出信息的组件名。
	*/
	virtual void			SDAPI	Debug				(const char * strInfo, const char * strModuleName = 0) = 0;	

	/*!func
	记录。
	!param [in] 要输出的信息。
	!param [in] 输出信息的组件名。
	*/
	virtual void			SDAPI	Log					(const char * strInfo, const char * strModuleName = 0) = 0;

	/*!func
	远程记录。
	!param [in] 要输出的信息。
	!param [in] 输出信息的组件名。
	*/
	virtual void			SDAPI	RemoteLog			(const char * strInfo, const char * strModuleName = 0) = 0;

	/*!func
	用户DUMP。
	!param [in] 要输出的信息。
	!param [in] 输出信息的组件名。
	*/
	virtual void			SDAPI	Dump				(const char * strInfo, const char * strModuleName = 0) = 0;


	/************************************************************************************/
	//!group 版本比较
	/*!func
	版本比较，判断组件版本号是否满足用户要求。
	!param [in] 用户要求的组件版本号。
	!param [in] 当前实际组件版本号。
	!return	版本号是否匹配，见SDERROR_VERSION_。
	*/
	virtual int				SDAPI	VerCheck			(const SSGDPVersion * pRequestVer, const SSGDPVersion * pRealVer) = 0;


	/************************************************************************************/
	// DUMP当前程序运行状态，此函数仅供内部使用
	virtual bool			SDAPI	DumpProgram			(int iDumpType, _EXCEPTION_POINTERS * pExceptionInfo=0, const char * strInfo=0) = 0;
};


//=============================================================================================================================
// 提供给STEI,STEF使用的错误处理函数
void SDAPI SDErrorCatch(int iMode,const char * strInfo);	


//=============================================================================================================================
/*!func
此函数返回错误处理组件SDError的实例。
!param [in] 用户要求的组件最低版本号，见SSGDPVersion。
!return 如果成功，返回此组件的实例；如果失败返回空指针。
!inc SDError.h
*/
ISDError * SDAPI SDErrorGetModule(const SSGDPVersion *pstVersion);

/*!typedef
SDErrorGetModule 函数的类型定义。
!inc SDError.h
*/
typedef ISDError * (SDAPI *PFN_SDErrorGetModule)(const SSGDPVersion *pstVersion);


/*!func
此函数返回错误处理组件SDError的实例。不验证版本号。
!return 如果成功，返回此组件的实例；如果失败返回空指针。
!inc SDError.h
*/
ISDError * SDAPI SDErrorInstance();

/*!typedef
SDErrorInstance 函数的类型定义。
!inc SDError.h
*/
typedef ISDError * (SDAPI *PFN_SDErrorInstance)();

}
