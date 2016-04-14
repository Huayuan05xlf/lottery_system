#ifndef __LOTTERY_H__
#define __LOTTERY_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define	MAX_NUM_COUNT	7		//彩票号码一组7个数
#define	MAX_NUMBER		100		//彩票号码数字范围1~99
#define	NO_LOTTERY		0		//开奖状态（未开奖）
#define	HAS_LOTTERY		1		//开奖状态（已开奖）
#define	BAD_LOTTERY		-1		//开奖状态（已开奖，未中奖）
#define	WIN_LEVEL_1		1000000	//一等奖金额
#define	WIN_LEVEL_2		80000	//二等奖金额
#define	WIN_LEVEL_3		10000	//三等奖金额
#define	WIN_LEVEL_4		1500 	//四等奖金额
#define	WIN_LEVEL_5		200		//五等奖金额
#define	WIN_LEVEL_6		20		//六等奖金额
#define	WIN_LEVEL_7		2		//七等奖金额
#define	MAXSIZE_ID		20		//最长ID限制
#define	MAXSIZE_UI		50		//界面外框尺寸
typedef struct LinkLottery
{
	int issue_number;			//彩票期号
	char lottery_ID[MAXSIZE_ID];//彩票ID（唯一性）
	int num[MAX_NUM_COUNT];		//彩票号码
	char buyer_ID[MAXSIZE_ID];	//购买者账号
	int multiple;				//购买注数
	int	winning_state;			//中奖状态
	float winning_amount;		//中奖金额
	struct LinkLottery *prior;	//前一彩票
	struct LinkLottery *next;	//后一彩票
}lottery;
/*================|双向环形链表操作|================*/
//初始化双向环形链表（新建节点）
extern lottery* InitLottery();
//销毁双向环形链表
extern void DestroyLottery(lottery *ptr_head);
//清空双向环形链表，保留头节点
extern void ClearLottery(lottery *ptr_head);
//根据彩票购买者账号查询，返回头节点往后第一个找到的节点
extern lottery* FindBuyerLottery(lottery *ptr_head, const char *buyer_ID);
//新增节点并初始化指针成员，插入头节点之前，返回新节点地址
extern lottery* InsertLotteryPrior(lottery *ptr_head);
//将源链表的一个节点移动到目标链表头节点之前，返回值为源链表头节点
extern lottery* InsertLotteryPriorFromSrc(lottery *ptr_dest, lottery *ptr_src, lottery* ptr_ins);
//连接2个环形链表，返回目标节点作为头节点
extern lottery* ConnectLottery(lottery *ptr_dest, lottery *ptr_src);
//结构体成员指针复制，不含数据，返回目标节点
extern lottery* LotteryPointerCpy(lottery *ptr_dest, const lottery *ptr_src);
/*================|结构体数据成员修改|================*/
//存入彩票信息，根据时间自动生成彩票ID，形参：彩票节点、当前购买者账号、当前发行期号、彩票号码数组
extern void SetLotteryInfo(lottery *ptr_tmp, char *buyer_ID, int issue_number, int num[], int multiple);
//中奖信息更新，level表示号码相同个数，返回中奖金额
extern float SetWiningInfo(lottery *ptr_tmp, int level);
/*================|结构体数据成员显示|================*/
//显示彩票信息
extern void ShowLotteryInfo(const lottery *ptr_tmp);
//显示中奖信息
extern void ShowWiningInfo(const lottery *ptr_tmp);
//显示所有彩票信息
extern void ShowLotteryInfoAll(lottery *ptr_head);
/*================|其他操作|================*/
//清空输入缓冲区
extern void EmptyInputBuffer();
//获取当前时间，转化为字符串，用于生成彩票ID
extern void TimeToString(char *str_time);
//数字转字符串，不要传入负数
extern void IntToString(char *str, int num);
//自动选择号码
extern void AutoRandomNum(int *ptr_num, int length);
//手动输入号码
extern void ManualGetNum(int *ptr_num, int length);
//修改购买注数
extern int SetLotteryMultiple();
//数组排序，从小到大，快速排序
extern void NumSort(int *ptr_num, int left, int right);
//数组数据查询，数组中有这个数字，则返回1，否则返回0
extern int NumJudge(const int *ptr_num, int length, int tmp);

#endif	/*lottery.h*/