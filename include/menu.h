#ifndef __MENU_H__
#define __MENU_H__
#include "buyer.h"
#include "issue.h"
#define	PATH_BUYER		"./lottery_files/buyer.info"	//用户文件存放路径及文件名
#define	PATH_ISSUE		"./lottery_files/issue.info"	//发行文件存放路径及文件名
#define	PATH_TMP		"./lottery_files/mylottery.tmp"	//用户彩票临时文件存放路径及文件名
#define	PATH_LOTTERY	"./lottery_files/lottery_info/"	//每期彩票文件存放路径
#define	VERSION			"Ver1.0_beta"					//版本号
#define	MAXSIZE_PATH	100		//存放路径及文件名的字符串大小
#define	NORMAL_SIGN		0		//程序正常标志
#define	ERROR_SIGN		-1		//程序错误标志
#define	EXIT_SIGN		1		//程序退出标志
#define	GENERAL_USER	1		//普通用户
#define	GUEST			0		//游客、未登陆
#define	ADMINISTRATORS	-1		//管理员
#define	NOTARY			-2		//公证员
/*================|文件操作|================*/
//加载用户文件（尾插法），此时ptr_head是一个空的头节点，也是尾节点，返回头节点
extern void LoadBuyer(buyer *ptr_head);
//保存用户文件
extern void SaveBuyer(buyer *ptr_head);
//加载发行文件（尾插法），此时ptr_head是一个空的头节点，也是尾节点，返回头节点
extern void LoadIssue(issue *ptr_head);
//保存发行文件
extern void SaveIssue(issue *ptr_head);
//加载彩票文件，加载前需要先加载发行文件
extern void LoadLottery(lottery *ptr_head, issue *ptr_latest);
//保存彩票文件
extern void SaveLottery(lottery *ptr_head, issue *ptr_latest);
//加载临时文件，如果存在临时文件，说明系统异常退出
extern lottery* LoadTmp();
//创建临时文件
extern void SaveTmp(lottery *ptr_head);
//删除临时文件
extern void DeleteTmp();
/*================|功能调用|================*/
//系统初始化，创建链表，按顺序加载链表
extern int SystemInit(void **ptr);
//系统开始运行，死循环，需要3条链表
extern int SystemRun(void **ptr);
//用户登陆后，初始化链表，从彩票链表获取用户自己的彩票
extern void UserInit(void **ptr);
//用户退出登陆后，归还彩票链表
extern void UserExit(void **ptr);
//系统退出处理，销毁链表
extern void SystemExit(void **ptr);
/*================|菜单界面|================*/
//初始界面，游客界面，注册登陆功能
extern void MenuGuestUI();
//游客命令，需用户链表
extern int MenuGuestCommand(void **ptr);
//管理员界面
extern void MenuAdminUI();
//管理员命令，需用户链表、发行链表、彩票链表
extern int MenuAdminCommand(void **ptr);
//公证员界面
extern void MenuNotaryUI();
//公证员命令，需用户链表、发行链表
extern int MenuNotaryCommand(void **ptr);
//彩民界面
extern void MenuBuyerUI();
//彩民命令，需用户链表、发行链表
extern int MenuBuyerCommand(void **ptr);
/*================|用户操作|================*/
//注册
extern int BuyerRegister(buyer *ptr_head);
//登陆
extern int BuyerLogin(buyer *ptr_head);
//注销
extern int BuyerLogout();
//发行
extern issue* IssueUpdate(issue *ptr_issue, lottery *ptr_lottery);
//开奖
extern void RunLottery(buyer *ptr_buyer, issue *ptr_issue, lottery *ptr_lottery);
//下注
extern void GetLottery(buyer *ptr_buyer, issue *ptr_issue, lottery *ptr_lottery);
//充值
extern void ChargeMoney(buyer *ptr_buyer);
/*================|查询操作|================*/
//显示我的彩票
extern void ShowMyLottery(buyer *ptr_tmp);
//显示购票历史记录
extern void ShowLotteryHistory(buyer *ptr_tmp, issue *ptr_issue);
//显示排序后的用户列表，返回用户头节点
extern buyer* ShowBuyerInfoAfterSort(buyer *ptr_head);
/*================|其他操作|================*/
//根据期号生成存放彩票文件的路径
extern void GetLotteryPath(char *path, issue *ptr_latest);

#endif	/*menu.h*/