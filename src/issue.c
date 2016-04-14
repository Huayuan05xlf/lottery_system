#include "issue.h"

//初始化单向链表（新建节点）
issue* InitIssue()
{
	issue *ptr_head = (issue*)calloc(1,sizeof(issue));
	if (NULL == ptr_head)
		return NULL;
	ptr_head->next  = NULL;
	return ptr_head;
}
//销毁单向链表
void DestroyIssue(issue *ptr_head)
{
	if(NULL == ptr_head)
		return ;
	issue *ptr_free = ptr_head;
	issue *ptr_next = ptr_head->next;
	while(NULL != ptr_next)		//循环到尾节点
	{
		free(ptr_free);
		ptr_free = ptr_next;
		ptr_next = ptr_next->next;
	}
	free(ptr_free);
	ptr_free = NULL;
}
//根据彩票期号查询，返回找到的节点
issue* FindIssue(issue *ptr_head, const int issue_number)
{
	if(NULL == ptr_head)
		return NULL;
	issue *ptr_find = ptr_head;
	while(1)
	{
		if(NULL == ptr_find)
			return NULL;
		else if(issue_number == ptr_find->issue_number)
			return ptr_find;
		ptr_find = ptr_find->next;
	}
}
//新增节点并初始化指针成员，插入链表头，返回头节点地址
issue* InsertIssueHead(issue *ptr_head)
{
	if(NULL == ptr_head)
		return NULL;
	issue *ptr_new = InitIssue();	//新建一个节点
	if(NULL == ptr_new)
		return NULL;
	ptr_new->next = ptr_head;
	return ptr_new;
}
//新增节点并初始化指针成员，插入链表尾，返回新节点地址
issue* InsertIssueEnd(issue *ptr_head)
{
	if(NULL == ptr_head)
		return NULL;
	issue *ptr_new = InitIssue();	//新建一个节点
	if(NULL == ptr_new)
		return NULL;
	issue *ptr_tmp = ptr_head;
	while(NULL != ptr_tmp->next)
		ptr_tmp = ptr_tmp->next;	//找到尾节点
	ptr_tmp->next = ptr_new;
	return ptr_new;
}
//发行新一期彩票后，设置期号、彩票售价、奖金余额，初始化开奖状态、开奖号码
void SetIssueInfo(issue *ptr_tmp)
{
	if(NULL == ptr_tmp)
		return ;
	if(HAS_ISSUE == ptr_tmp->issue_state)
		return ;		//已经发行过的不进行操作
	int i;
	if(NULL == ptr_tmp->next)
	{
		ptr_tmp->issue_number = INIT_ISSUE_NUM;	//第一期彩票期号
		ptr_tmp->jackpot_blance = INIT_MONEY;	//第一期奖池金额
	}
	else
	{
		ptr_tmp->issue_number = 1+ptr_tmp->next->issue_number;	//前一期期号加一
		ptr_tmp->jackpot_blance = ptr_tmp->next->jackpot_blance;//继承前一期剩余金额
	}
	ptr_tmp->issue_state = HAS_ISSUE;			//初始化发行状态
	ptr_tmp->lottery_state = NO_LOTTERY;		//初始化开奖状态
	for(i = 0; i < MAX_NUM_COUNT; ++i)
		ptr_tmp->win_num[i] = INIT_NUMBER;		//初始化开奖号码
	ptr_tmp->lottery_price = INIT_PRICE;		//彩票售价
	ptr_tmp->purchase_count = 0;				//购买彩票总数
	ptr_tmp->winning_amount_all = 0.0;			//中奖金额
}
//遍历彩票链表，更新中奖金额，奖池余额减少
void UpdateIssueMoney(issue *ptr_tmp, lottery *ptr_head, int win_num[])
{
	if(NULL == ptr_tmp || NULL == ptr_head)
		return ;
	if(!strlen(ptr_head->lottery_ID))	//没有彩票
		return;
	int level;
	float money_tmp;
	lottery *ptr_update = ptr_head;
	while(1)		//环形链表回到头节点
	{
		level = NumCheck(win_num,ptr_update->num,MAX_NUM_COUNT);
		money_tmp = SetWiningInfo(ptr_update,level);
		ptr_tmp->winning_amount_all += money_tmp;
		if(ptr_head == ptr_update->next)
			break;
		ptr_update = ptr_update->next;
	}
}
//修改开奖状态
void SetIssueState(issue *ptr_tmp, int win_num[])
{
	if(NULL == ptr_tmp)
		return ;
	if(HAS_ISSUE != ptr_tmp->issue_state)
		return ;		//尚未发行过的不进行操作
	if(HAS_LOTTERY == ptr_tmp->lottery_state)
		return ;		//已经开过奖的不进行操作
	int i;
	ptr_tmp->lottery_state = HAS_LOTTERY;
	for(i = 0; i < MAX_NUM_COUNT; ++i)
		ptr_tmp->win_num[i] = win_num[i];		//修改开奖号码
	ptr_tmp->jackpot_blance -= ptr_tmp->winning_amount_all;	//修改奖池余额
}
//购买彩票，奖池余额增加
void AddJackpotBlance(issue *ptr_tmp, float money)
{
	if(NULL == ptr_tmp)
		return ;
	ptr_tmp->jackpot_blance += money;
	ptr_tmp->purchase_count++;
}
//显示发行信息
void ShowIssueInfo(const issue *ptr_tmp)
{
	if(NULL == ptr_tmp)
		return ;
	if(0 == ptr_tmp->issue_number)
	{
		printf("$$$$彩票没有发行……\033[0m\n");
		return ;
	}
	printf("发行期号：%7d ",ptr_tmp->issue_number);
	printf("彩票售价：%2d元 ",ptr_tmp->lottery_price);
	printf("已购买彩票数：%3d ",ptr_tmp->purchase_count);
	printf("奖池余额：%6.2f ",ptr_tmp->jackpot_blance);
	ShowLotteryState(ptr_tmp);
}
//显示开奖信息
void ShowLotteryState(const issue *ptr_tmp)
{
	if(NULL == ptr_tmp)
		return ;
	int i;
	if(HAS_LOTTERY == ptr_tmp->lottery_state)
	{
		printf("开奖号码：");
		for(i = 0; i < MAX_NUM_COUNT; ++i)
			printf("%2d ",ptr_tmp->win_num[i]);
		printf("中奖总额：%6.2f  \n",ptr_tmp->winning_amount_all);
	}
	else
		printf("尚未开奖！\n");
}
//显示所有发行信息
void ShowIssueInfoAll(const issue *ptr_head)
{
	if(NULL == ptr_head)
		return ;
	if(0 == ptr_head->issue_number)
	{
		printf("\033[0;36m没有发行记录……\033[0m\n");
		return ;
	}
	int count = 0;
	printf("=================================\033[0;36m所有发行信息如下\033[0m=================================\n");
	while(NULL != ptr_head)		//找到链表尾
	{
		ShowIssueInfo(ptr_head);
		count++;
		ptr_head = ptr_head->next;
	}
	printf("================================>\033[0;36m已经发行了%d期彩票\033[0m\n",count);
}
//两个数组查重（数组本身无重复数据），返回相同数据个数
int NumCheck(const int *ptr_num1, const int *ptr_num2, int length)
{
	if(NULL == ptr_num1 || NULL == ptr_num2 || length < 0)
		return 0;
	int i, count = 0;
	for(i = 0; i < length; ++i)
		if(NumJudge(ptr_num1,length,ptr_num2[i]))
			count++;
	return count;
}