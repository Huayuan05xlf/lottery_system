#ifndef __BUYER_H__
#define __BUYER_H__
#include "lottery.h"
#define	MAXSIZE_PSD		20		//最长密码限制
#define	MAXSIZE_NAME	20		//最长姓名限制
#define	DEL				127		//‘删除’字符的ASCII码值
#define	INCREASE		1		//余额增加标志
#define	REDUCE			-1		//余额减少标志
#define	ADMIN_ID		"admin"	//管理员ID
#define	ADMIN_PSD		"8888"	//管理员密码
#define	NOTARY_ID		"notary"//公证员ID
#define	NOTARY_PSD		"0000"	//公证员密码
typedef struct LinkBuyer
{
	char buyer_ID[MAXSIZE_ID];	//账号（唯一性）
	char password[MAXSIZE_PSD];	//密码
	char name[MAXSIZE_NAME];	//姓名
	float account_balance;		//账户余额
	struct LinkLottery *my_lottery;	//我的彩票
	struct LinkBuyer *prior;	//前一用户
	struct LinkBuyer *next;		//后一用户
}buyer;
/*================|双向链表操作|================*/
//初始化双向链表（新建节点）
extern buyer* InitBuyer();
//销毁双向链表
extern void DestroyBuyer(buyer *ptr_head);
//根据账号查询用户，返回找到的节点
extern buyer* FindBuyer(buyer *ptr_head, const char *buyer_ID);
//删除用户,返回值为头节点
extern buyer* DeleteBuyer(buyer *ptr_head, buyer *ptr_del);
//新增节点并初始化指针成员，插入链表尾，返回新节点地址
extern buyer* InsertBuyerEnd(buyer *ptr_head);
//结构体成员指针复制，不含数据，初始化彩票指针，返回目标节点
extern buyer* BuyerPointerCpy(buyer *ptr_dest, const buyer *ptr_src);
//链表节点交换，指针交换，交换成功返回1，否则返回0
extern int BuyerSwap(buyer *ptr_a, buyer *ptr_b);
//用户链表排序，根据余额降序排序，选择排序，返回新的头节点（余额最大值用户）
extern buyer* SortBuyerByMoney(buyer *ptr_head);
/*================|结构体数据成员修改|================*/
//注册界面，输入注册信息
extern void GetRegisterInfo(buyer *ptr_tmp);
//登陆界面，输入账号密码
extern void GetLoginInfo(buyer *ptr_tmp);
//充值，中奖，账户余额增加，购票，账户余额减少
extern void AddSubtractMoney(buyer *ptr_tmp, float money, int flag);
//开奖之后，遍历彩票链表，更新用户余额
extern void UpdateBuyerMoney(buyer *ptr_buyer, lottery *ptr_lottery);
//设置新密码
extern void SetNewPassword(buyer *ptr_tmp);
//修改姓名
extern void SetNewName(buyer *ptr_tmp);
/*================|结构体数据成员显示|================*/
//用户信息显示
extern void ShowBuyerInfo(const buyer *ptr_tmp);
//显示所有用户信息
extern void ShowBuyerInfoAll(const buyer *ptr_head);
/*================|其他操作|================*/
//密码输入函数，调用前注意是否需要清空输入缓冲区
extern void GetPassword(char *password, int length);
//密码加密，单个字符加密
extern char CharEncrypt(char ch);
//密码字符解密
extern char CharDecrypt(char ch);
//密码字符串解密
extern void StringDecrypt(char *dest, const char *src);

#endif	/*buyer.h*/