#include "buyer.h"
#include <termios.h>	//函数mygetch();所需头文件，linux终端接口
#include <unistd.h>		//函数mygetch();所需头文件，linux系统调用
static char mygetch();	//仅在密码输入函数中调用，功能是读取一个字符，但不显示在屏幕上

//初始化双向链表（新建节点）
buyer* InitBuyer()
{
	buyer *ptr_head = (buyer*)calloc(1,sizeof(buyer));
	if (NULL == ptr_head)
		return NULL;
	ptr_head->my_lottery = NULL;
	ptr_head->prior = NULL;
	ptr_head->next  = NULL;
	return ptr_head;
}
//销毁双向链表
void DestroyBuyer(buyer *ptr_head)
{
	if(NULL == ptr_head)
		return ;
	buyer *ptr_free = ptr_head;
	buyer *ptr_next = ptr_head->next;
	while(NULL != ptr_next)		//循环到尾节点
	{
		free(ptr_free);
		ptr_free = ptr_next;
		ptr_next = ptr_next->next;
	}
	free(ptr_free);
	ptr_free = NULL;
}
//根据账号查询用户，返回找到的节点
buyer* FindBuyer(buyer *ptr_head, const char *buyer_ID)
{
	if(NULL == ptr_head || NULL == buyer_ID)
		return NULL;
	buyer *ptr_find = ptr_head;
	while(1)
	{
		if(NULL == ptr_find)
			return NULL;
		else if(!strcmp(buyer_ID, ptr_find->buyer_ID))
			return ptr_find;
		ptr_find = ptr_find->next;
	}
}
//删除用户,返回值为头节点
buyer* DeleteBuyer(buyer *ptr_head, buyer *ptr_del)
{
	if(NULL==ptr_head || NULL==ptr_del)
		return NULL;
	if(ptr_head == ptr_del)			//如果要删除的是链表头节点
		ptr_head = ptr_head->next;	//下一节点改为新的头节点
	else							//删除的不是头节点时
		ptr_del->prior->next = ptr_del->next;
	if(NULL != ptr_del->next)		//删除的不是尾节点时
		ptr_del->next->prior = ptr_del->prior;
	free(ptr_del);
	ptr_del = NULL;
	return ptr_head;
}
//新增节点并初始化指针成员，插入链表尾，返回新节点地址
buyer* InsertBuyerEnd(buyer *ptr_head)
{
	if(NULL == ptr_head)
		return NULL;
	buyer *ptr_new = InitBuyer();	//新建一个节点
	if(NULL == ptr_new)
		return NULL;
	buyer *ptr_tmp = ptr_head;
	while(NULL != ptr_tmp->next)
		ptr_tmp = ptr_tmp->next;	//找到尾节点
	ptr_tmp->next  = ptr_new;
	ptr_new->prior = ptr_tmp;
	return ptr_new;
}
//结构体成员指针复制，不含数据，初始化彩票指针，返回目标节点
buyer* BuyerPointerCpy(buyer *ptr_dest, const buyer *ptr_src)
{
	if(NULL==ptr_dest || NULL==ptr_src)
		return NULL;
	ptr_dest->my_lottery = NULL;	//清空彩票指针
	ptr_dest->prior = ptr_src->prior;
	ptr_dest->next  = ptr_src->next;
	return ptr_dest;
}
//链表节点交换，指针交换，交换成功返回1，否则返回0
int BuyerSwap(buyer *ptr_a, buyer *ptr_b)
{
	if(NULL == ptr_a || NULL == ptr_b)
		return 0;
	if(ptr_a == ptr_b)
		return 0;
	buyer backup;
	BuyerPointerCpy(&backup,ptr_a);
	BuyerPointerCpy(ptr_a,ptr_b);
	BuyerPointerCpy(ptr_b,&backup);
	if(NULL != ptr_a->prior)
	{
		if(ptr_a == ptr_a->prior)
			ptr_a->prior = ptr_b;
		else
			ptr_a->prior->next = ptr_a;
	}
	if(NULL != ptr_a->next)
	{
		if(ptr_a == ptr_a->next)
			ptr_a->next = ptr_b;
		else
			ptr_a->next->prior = ptr_a;
	}
	if(NULL != ptr_b->prior)
	{
		if(ptr_b == ptr_b->prior)
			ptr_b->prior = ptr_a;
		else
			ptr_b->prior->next = ptr_b;
	}
	if(NULL != ptr_b->next)
	{
		if(ptr_b == ptr_b->next)
			ptr_b->next = ptr_a;
		else
			ptr_b->next->prior = ptr_b;
	}
	return 1;
}
//用户链表排序，根据余额降序排序，选择排序，返回新的头节点（余额最大值用户）
buyer* SortBuyerByMoney(buyer *ptr_head)
{
	if(NULL == ptr_head)
		return NULL;
	buyer *ptr_next = ptr_head;
	buyer *ptr_max, *ptr_tmp;
	while(NULL != ptr_next)			//找到链表尾
	{
		ptr_max = ptr_next;
		ptr_tmp = ptr_next->next;
		while(NULL != ptr_tmp)		//找到链表尾
		{
			if(ptr_max->account_balance < ptr_tmp->account_balance)
				ptr_max = ptr_tmp;
			ptr_tmp = ptr_tmp->next;
		}
		if(BuyerSwap(ptr_max,ptr_next))
			ptr_next = ptr_max->next;
		else
			ptr_next = ptr_next->next;
	}
	while(NULL != ptr_head->prior)	//找到链表头
		ptr_head = ptr_head->prior;
	return ptr_head;
}
//注册界面，输入注册信息
void GetRegisterInfo(buyer *ptr_tmp)
{
	if(NULL == ptr_tmp)
		return ;
	int i;
	memset(ptr_tmp->buyer_ID,'\0',MAXSIZE_ID);
	memset(ptr_tmp->password,'\0',MAXSIZE_PSD);
	memset(ptr_tmp->name,'\0',MAXSIZE_NAME);
	char pswd1[MAXSIZE_PSD], pswd2[MAXSIZE_PSD];
	for(i = 0; i < MAXSIZE_UI; ++i)
		printf("=");
	printf("\n===>请输入注册信息……\n");
	printf("===>请输入账号：");
	scanf("%s",ptr_tmp->buyer_ID);
	EmptyInputBuffer();
	while(1)
	{
		printf("===>请输入密码：");
		GetPassword(pswd1,MAXSIZE_PSD);
		if(!strlen(pswd1))
		{
			printf("===>\033[1;31m密码不允许为空！\033[0m\n");
			continue;
		}
		printf("===>请再输入一次：");
		GetPassword(pswd2,MAXSIZE_PSD);
		if(strcmp(pswd1,pswd2))
		{
			printf("===>\033[1;31m两次输入的密码不一致！\033[0m\n");
			pswd1[0] = pswd2[0] = '\0';
		}
		else
			break;
	}
	strcpy(ptr_tmp->password,pswd1);
	printf("===>姓名：");
	scanf("%s",ptr_tmp->name);
	ptr_tmp->account_balance = 0.0;	//初始化账户余额
	for(i = 0; i < MAXSIZE_UI; ++i)
		printf("=");
	printf("\n");
}
//登陆界面，输入账号密码
void GetLoginInfo(buyer *ptr_tmp)
{
	if(NULL == ptr_tmp)
		return ;
	int i;
	for(i = 0; i < MAXSIZE_UI; ++i)
		printf("-");
	printf("\n-->>欢迎登陆……\n");
	printf("-->>账号：");
	scanf("%s",ptr_tmp->buyer_ID);
	EmptyInputBuffer();
	printf("-->>密码：");
	GetPassword(ptr_tmp->password,MAXSIZE_PSD);
	for(i = 0; i < MAXSIZE_UI; ++i)
		printf("-");
	printf("\n");
}
//充值，中奖，账户余额增加，购票，账户余额减少
void AddSubtractMoney(buyer *ptr_tmp, float money, int flag)
{
	if(NULL == ptr_tmp)
		return ;
	if(INCREASE == flag)		//余额增加
		ptr_tmp->account_balance += money;
	else if(REDUCE == flag)		//余额减少
		ptr_tmp->account_balance -= money;
}
//开奖之后，遍历彩票链表，更新用户余额
void UpdateBuyerMoney(buyer *ptr_buyer, lottery *ptr_lottery)
{
	if(NULL == ptr_buyer || NULL == ptr_lottery)
		return ;
	if(!strlen(ptr_lottery->lottery_ID))	//没有彩票
		return;
	float money_tmp;
	buyer *ptr_buyer_tmp;
	lottery *ptr_update = ptr_lottery;
	while(1)		//环形链表回到头节点
	{
		ptr_buyer_tmp = FindBuyer(ptr_buyer,ptr_update->buyer_ID);
		if(NULL != ptr_buyer_tmp)
		{
			money_tmp = ptr_update->winning_amount;
			AddSubtractMoney(ptr_buyer_tmp,money_tmp,INCREASE);
		}
		if(ptr_lottery == ptr_update->next)
			break;
		ptr_update = ptr_update->next;
	}
}
//设置新密码
void SetNewPassword(buyer *ptr_tmp)
{
	if(NULL == ptr_tmp)
		return ;
	char pswd_check[MAXSIZE_PSD];
	char pswd1[MAXSIZE_PSD], pswd2[MAXSIZE_PSD];
	EmptyInputBuffer();
	printf("$$$$请输入原密码：");
	GetPassword(pswd_check,MAXSIZE_PSD);
	if(strcmp(pswd_check,ptr_tmp->password))
	{
		printf("\033[1;31m密码错误！\033[0m\n");
		return ;
	}
	while(1)
	{
		printf("$$$$请输入新密码：");
		GetPassword(pswd1,MAXSIZE_PSD);
		if(!strlen(pswd1))
		{
			printf("$$$$\033[1;31m密码不允许为空！\033[0m\n");
			continue;
		}
		printf("$$$$请再输入一次：");
		GetPassword(pswd2,MAXSIZE_PSD);
		if(strcmp(pswd1,pswd2))
		{
			printf("$$$$\033[1;31m两次输入的密码不一致！\033[0m\n");
			pswd1[0] = pswd2[0] = '\0';
		}
		else
			break;
	}
	strcpy(ptr_tmp->password,pswd1);
	printf("修改成功！\n");
}
//修改姓名
void SetNewName(buyer *ptr_tmp)
{
	if(NULL == ptr_tmp)
		return ;
	char pswd_check[MAXSIZE_PSD];
	EmptyInputBuffer();
	printf("$$$$请输入密码：");
	GetPassword(pswd_check,MAXSIZE_PSD);
	if(strcmp(pswd_check,ptr_tmp->password))
	{
		printf("\033[1;31m密码错误！\033[0m\n");
		return ;
	}
	printf("密码正确！\n");
	printf("$$$$请输入姓名：");
	scanf("%s",ptr_tmp->name);
	printf("修改成功！\n");
}
//用户信息显示
void ShowBuyerInfo(const buyer *ptr_tmp)
{
	if(NULL == ptr_tmp)
		return ;
	printf("$$$$账号：%10s ",ptr_tmp->buyer_ID);
	printf("姓名：%10s ",ptr_tmp->name);
	printf("余额：%6.2f\n",ptr_tmp->account_balance);
}
//显示所有用户信息
void ShowBuyerInfoAll(const buyer *ptr_head)
{
	if(NULL == ptr_head)
		return ;
	int count = 0;
	while(NULL != ptr_head)		//找到链表尾
	{
		ShowBuyerInfo(ptr_head);
		count++;
		ptr_head = ptr_head->next;
	}
	printf("================================>\033[0;36m总注册人数：%d人\033[0m\n",count);
}
//密码输入函数，调用前注意是否需要清空输入缓冲区
void GetPassword(char *password, int length)
{
	if(NULL == password || length < 1)
		return ;
	char ch;
	int i = 0;
	while(1)
	{
		ch = mygetch();		//从终端读取一个字符，但不显示在屏幕上
		if('\n' == ch)		//读取输入的字符，输入回车结束密码输入
			break;
		else if(i > 0 && DEL == ch)	//判断是否输入了“删除”命令
		{
				i--;
				printf("\b \b");	//显示退格
		}
		else if('\n' != ch && '\r' != ch && DEL != ch)
		{ 
			if(i < length-1)		//密码长度限制，保留最后一位放结束符
			{
				ch = CharEncrypt(ch);	//密码加密存储
				password[i++] = ch;
				printf("*");
			}
		}
	}
	printf("\n");
	password[i] = '\0';		//结束符
}
//密码加密，单个字符加密
char CharEncrypt(char ch)
{
	ch = 10 - ch;
	return ch;
}
//密码字符解密
char CharDecrypt(char ch)
{
	ch = 10 - ch;
	return ch;
}
//密码字符串解密
void StringDecrypt(char *str_dest, const char *str_src)
{
	if(NULL == str_dest || NULL == str_src)
		return ;
	int i;
	for(i = 0; str_src[i]!='\0'; ++i)
		str_dest[i] = CharDecrypt(str_src[i]);
	str_dest[i] = '\0';
}
//从终端（标准输入：键盘）读取一个字符，不显示在屏幕上
static char mygetch()
{
	struct termios oldt, newt;			//定义2个结构体，用于保存终端的2种状态（输入回显和不回显）
	char ch;
	tcgetattr(STDIN_FILENO, &oldt);		//获取当前终端状态（标准输入的接口）
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);	//其中一个（新状态）设置为输入不回显并且读取字符不用等待回车
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);//设置终端为新状态
	ch = getchar();							//读取字符，不需要回车
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);//还原终端状态
	return ch;
}