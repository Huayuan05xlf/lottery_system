#ifndef __ISSUE_H__
#define __ISSUE_H__
#include "lottery.h"
#define	HAS_ISSUE		1		//发行状态（已发行）
#define	INIT_ISSUE_NUM	2016001	//第1期彩票期号
#define	INIT_MONEY		100000	//第1期奖池金额
#define	INIT_NUMBER		-1		//初始开奖号码（正常号码为正数）
#define INIT_PRICE		2		//彩票售价
typedef struct LinkIssue
{
	int issue_number;			//彩票期号（唯一性）
	int issue_state;			//发行状态
	int lottery_state;			//开奖状态
	int win_num[MAX_NUM_COUNT];	//开奖号码
	int lottery_price;			//彩票售价
	int purchase_count;			//购买总数
	float winning_amount_all;	//中奖总额
	float jackpot_blance;		//奖池余额
	struct LinkIssue *next;		//下一发行信息（前一期）
}issue;
/*================|单向链表操作|================*/
//初始化单向链表（新建节点）
extern issue* InitIssue();
//销毁单向链表
extern void DestroyIssue(issue *ptr_head);
//根据彩票期号查询，返回找到的节点
extern issue* FindIssue(issue *ptr_head, const int issue_number);
//新增节点并初始化指针成员，插入链表头，返回头节点地址
extern issue* InsertIssueHead(issue *ptr_head);
//新增节点并初始化指针成员，插入链表尾，返回新节点地址
extern issue* InsertIssueEnd(issue *ptr_head);
/*================|结构体数据成员修改|================*/
//发行新一期彩票后，设置期号、彩票售价、奖金余额，初始化开奖状态、开奖号码
extern void SetIssueInfo(issue *ptr_tmp);
//遍历彩票链表，更新中奖金额，奖池余额减少
extern void UpdateIssueMoney(issue *ptr_tmp, lottery *ptr_head, int win_num[]);
//修改开奖状态
extern void SetIssueState(issue *ptr_tmp, int win_num[]);
//购买彩票，奖池余额增加
extern void AddJackpotBlance(issue *ptr_tmp, float money);
/*================|结构体数据成员显示|================*/
//显示发行信息
extern void ShowIssueInfo(const issue *ptr_tmp);
//显示开奖信息
extern void ShowLotteryState(const issue *ptr_tmp);
//显示所有发行信息
extern void ShowIssueInfoAll(const issue *ptr_tmp);
/*================|其他操作|================*/
//两个数组查重（数组本身无重复数据），返回相同数据个数
extern int NumCheck(const int *ptr_num1, const int *ptr_num2, int length);

#endif	/*issue.h*/