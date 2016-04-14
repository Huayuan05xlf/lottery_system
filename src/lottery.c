#include "lottery.h"

//初始化双向环形链表（新建节点）
lottery* InitLottery()
{
	lottery *ptr_head = (lottery*)calloc(1,sizeof(lottery));
	if (NULL == ptr_head)
		return NULL;
	ptr_head->prior = ptr_head;
	ptr_head->next  = ptr_head;
	return ptr_head;
}
//销毁双向环形链表
void DestroyLottery(lottery *ptr_head)
{
	if (NULL == ptr_head)
		return ;
	lottery *ptr_free = ptr_head;
	lottery *ptr_next  = ptr_head->next;
	while(ptr_head != ptr_next)		//环形链表回到头节点
	{
		free(ptr_free);
		ptr_free = ptr_next;
		ptr_next = ptr_next->next;
	}
	free(ptr_free);		//释放最后一个节点，即头节点之前的那个节点
	ptr_free = ptr_next = NULL;
}
//清空双向环形链表，保留头节点
void ClearLottery(lottery *ptr_head)
{
	if (NULL == ptr_head)
		return ;
	lottery *ptr_free = ptr_head->next;
	ptr_head->prior->next = ptr_head->next;
	ptr_head->next->prior = ptr_head->prior;	//将头节点从链表中取出
	memset(ptr_head,'\0',sizeof(lottery));
	ptr_head->prior = ptr_head;
	ptr_head->next  = ptr_head;		//初始化头节点
	if(ptr_free != ptr_head)		//链表中不止一个节点时，释放除头节点外所有节点
		DestroyLottery(ptr_free);
	ptr_free = NULL;
}
//根据彩票购买者账号查询，返回头节点往后第一个找到的节点
lottery* FindBuyerLottery(lottery *ptr_head, const char *buyer_ID)
{
	if(NULL == ptr_head || NULL == buyer_ID)
		return NULL;
	lottery *ptr_find = ptr_head;
	while(1)
	{
		if(!strcmp(buyer_ID, ptr_find->buyer_ID))
			return ptr_find;
		else if(ptr_head == ptr_find->next)
			return NULL;
		ptr_find = ptr_find->next;
	}
}
//新增节点并初始化指针成员，插入头节点之前，返回新节点地址
lottery* InsertLotteryPrior(lottery *ptr_head)
{
	if(NULL == ptr_head)
		return NULL;
	lottery *ptr_new = InitLottery();	//新建一个节点
	if(NULL == ptr_new)
		return NULL;
	ptr_new->prior = ptr_head->prior;
	ptr_new->next  = ptr_head;
	ptr_head->prior->next = ptr_new;
	ptr_head->prior = ptr_new; 
	return ptr_new;
}
//将源链表的一个节点移动到目标链表头节点之前，返回值为源链表头节点
lottery* InsertLotteryPriorFromSrc(lottery *ptr_dest, lottery *ptr_src, lottery* ptr_ins)
{
	if(NULL==ptr_dest || NULL==ptr_src || NULL==ptr_ins)
		return NULL;
	if(ptr_src == ptr_ins)		//如果要移动的是源链表头节点
		ptr_src = ptr_src->next;//下一节点改为新的头节点
	if(ptr_src == ptr_src->next)//源链表只有这一个节点
		ptr_src = ptr_dest;		//源链表指向目标链表
	ptr_ins->prior->next = ptr_ins->next;
	ptr_ins->next->prior = ptr_ins->prior;
	if(ptr_dest == ptr_ins)					//从源链表中单独取出一个节点，作为新的链表
		ptr_ins->prior = ptr_ins->next = ptr_ins;
	else									//如果目标链表是另一个链表，将取出的节点插入目标链表
	{
		ptr_ins->prior = ptr_dest->prior;
		ptr_ins->next  = ptr_dest;
		ptr_dest->prior->next = ptr_ins;
		ptr_dest->prior = ptr_ins;			//插入目标链表
	}
	return ptr_src;
}
//连接2个环形链表，返回目标节点作为头节点
lottery* ConnectLottery(lottery *ptr_dest, lottery *ptr_src)
{
	if(NULL==ptr_dest || NULL==ptr_src)
		return NULL;
	if(ptr_src == ptr_dest)
		return ptr_dest;
	lottery *ptr_tmp;
	ptr_dest->prior->next = ptr_src;
	ptr_src->prior->next  = ptr_dest;
	ptr_tmp = ptr_dest->prior;
	ptr_dest->prior = ptr_src->prior;
	ptr_src->prior  = ptr_tmp;
	return ptr_dest;
}
//结构体成员指针复制，不含数据，返回目标节点
lottery* LotteryPointerCpy(lottery *ptr_dest, const lottery *ptr_src)
{
	if(NULL==ptr_dest || NULL==ptr_src)
		return NULL;
	ptr_dest->prior = ptr_src->prior;
	ptr_dest->next  = ptr_src->next;
	return ptr_dest;
}
//存入彩票信息，根据时间自动生成彩票ID，形参：彩票节点、当前购买者账号、当前发行期号、彩票号码数组
void SetLotteryInfo(lottery *ptr_tmp, char *buyer_ID, int issue_number, int num[], int multiple)
{
	if(NULL == ptr_tmp || NULL == buyer_ID)
		return ;
	int i;
	ptr_tmp->issue_number = issue_number;	//设置期号
	strcpy(ptr_tmp->buyer_ID,buyer_ID);		//购买者账号
	TimeToString(ptr_tmp->lottery_ID);		//彩票ID
	for(i = 0; i < MAX_NUM_COUNT; ++i)
		ptr_tmp->num[i] = num[i];			//彩票号码
	ptr_tmp->multiple = multiple;			//彩票注数
	ptr_tmp->winning_state = NO_LOTTERY;	//开奖状态
	ptr_tmp->winning_amount = 0.0;			//中奖金额
}
//中奖信息更新，level表示号码相同个数，返回中奖金额
float SetWiningInfo(lottery *ptr_tmp, int level)
{
	if(NULL == ptr_tmp)
		return 0;
	ptr_tmp->winning_state = MAX_NUM_COUNT - level + 1;
	switch(level)
	{
		case 0 :ptr_tmp->winning_state = BAD_LOTTERY;
				break;
		case 1 :ptr_tmp->winning_amount = WIN_LEVEL_7;
				break;
		case 2 :ptr_tmp->winning_amount = WIN_LEVEL_6;
				break;
		case 3 :ptr_tmp->winning_amount = WIN_LEVEL_5;
				break;
		case 4 :ptr_tmp->winning_amount = WIN_LEVEL_4;
				break;
		case 5 :ptr_tmp->winning_amount = WIN_LEVEL_3;
				break;
		case 6 :ptr_tmp->winning_amount = WIN_LEVEL_2;
				break;
		case 7 :ptr_tmp->winning_amount = WIN_LEVEL_1;
				break;
		default:ptr_tmp->winning_state = NO_LOTTERY;
				return 0;
	}
	ptr_tmp->winning_amount *= ptr_tmp->multiple;
	return ptr_tmp->winning_amount;
}
//显示彩票信息
void ShowLotteryInfo(const lottery *ptr_tmp)
{
	if(NULL == ptr_tmp)
		return ;
	int i;
	printf("$$$$ID：\033[1;34m%20s\033[0m ",ptr_tmp->lottery_ID);
	printf("期号：%7d ",ptr_tmp->issue_number);
	printf("号码：");
	for(i = 0; i < MAX_NUM_COUNT; ++i)
		printf("%2d ",ptr_tmp->num[i]);
	printf("\n$$$$购买者账号：%12s ",ptr_tmp->buyer_ID);
	printf("购买注数：%2d ",ptr_tmp->multiple);
	ShowWiningInfo(ptr_tmp);
}
//显示中奖信息
void ShowWiningInfo(const lottery *ptr_tmp)
{
	if(NULL == ptr_tmp)
		return ;
	if(ptr_tmp->winning_state > NO_LOTTERY)
		printf("\033[1;32m恭喜你中了%d等奖！金额：%6.2f\033[0m\n",ptr_tmp->winning_state,ptr_tmp->winning_amount);
	else if(ptr_tmp->winning_state < NO_LOTTERY)
		printf("很遗憾……没有中奖……\n");
	else
		printf("还没有开奖!\n");
}
//显示所有彩票信息
void ShowLotteryInfoAll(lottery *ptr_head)
{
	if(NULL == ptr_head)
		return ;
	if(!strlen(ptr_head->lottery_ID))
	{
		printf("\033[0;36m本期还没有人购买彩票……\033[0m\n");
		return ;
	}
	lottery *ptr_show  = ptr_head;
	printf("=========================\033[0;36m本期所有彩票信息如下\033[0m=========================\n");
	while(1)
	{
		ShowLotteryInfo(ptr_show);
		if(ptr_head == ptr_show->next)
			break;
		ptr_show = ptr_show->next;
	}
}
//清空输入缓冲区
void EmptyInputBuffer()
{
	//int c;
	//while('\n' != (c = getchar()) && EOF != c);
	scanf("%*[^\n]");	//将输入缓冲区中所有不是 \n 的字符读入并抛弃
	scanf("%*c");		//读入一个字符并抛弃
}
//获取当前时间，转化为字符串，用于生成彩票ID
void TimeToString(char *str_time)
{
	if(NULL == str_time)
		return ;
	time_t now;
	time(&now);		//获取时间
	struct tm *ptr_time = localtime(&now);
	char str_tmp[10];//时间字符串保存格式(yyyy_MM_dd_HH_mm_ss)
	int i;
	str_time[0] = '\0';		//字符串置为空
	for(i = 0; i < 6; ++i)	//总共6个数据写入字符串
	{
		switch(i)
		{
			case 0 :IntToString(str_tmp,ptr_time->tm_year+1900);//年
					break;
			case 1 :IntToString(str_tmp,ptr_time->tm_mon+1);	//月
					break;
			case 2 :IntToString(str_tmp,ptr_time->tm_mday);		//日
					break;
			case 3 :IntToString(str_tmp,ptr_time->tm_hour);		//时
					break;
			case 4 :IntToString(str_tmp,ptr_time->tm_min);		//分
					break;
			case 5 :IntToString(str_tmp,ptr_time->tm_sec);		//秒
					break;
			default:return;
		}
		strcat(str_time,str_tmp);
		if(i < 5)
			strcat(str_time,"_");	//插入下划线
	}
}
//数字转字符串，不要传入负数
void IntToString(char *str, int num)
{
	if(NULL == str)
		return ;
	if(num < 10)		//个位数输入，存放2位，前面补0
	{
		str[0] = '0';
		str[1] = num + 48;
		str[2] = '\0';
		return;
	}
	char str_tmp[20];	//int型整数最大只有10位数
	int i = 0;
	do
	{
		str_tmp[i++] = (num%10)+48;	//数字字符ascii码值与数字相差48
		num /= 10;
	}while(num);		//个位数放在字符数组低位
	str_tmp[i] = '\0';
	int length = (int)strlen(str_tmp);
	for (i = 0; i < length; ++i)
		str[i] = str_tmp[length-i-1];
	str[length] = '\0';	//字符串反序复制
}
//自动选择号码
void AutoRandomNum(int *ptr_num, int length)
{
	if(NULL == ptr_num)
		return ;
	int tmp, i;
	for(i = 0; i < length; ++i)
		ptr_num[i] = 0;
	for(i = 0; i < length; ++i)
	{
		srand(time(NULL));
		tmp = rand()%(MAX_NUMBER-1)+1;	//随机数范围1~99
		if(NumJudge(ptr_num,length,tmp))
			--i;						//去重
		else
		{
			ptr_num[i] = tmp;
			printf("    生成第%d个号码：%d\n",i+1,ptr_num[i]);
		}
	}
	NumSort(ptr_num,0,length-1);		//排序，从小到大
}
//手动输入号码
void ManualGetNum(int *ptr_num, int length)
{
	if(NULL == ptr_num)
		return ;
	int tmp, i;
	for(i = 0; i < length; ++i)
		ptr_num[i] = 0;
	for(i = 0; i < length; ++i)
	{
		printf("$$$$请输入第%d个号码（1～99）：",i+1);
		do
		{
			while(!scanf("%d",&tmp))
			{
				EmptyInputBuffer();
				printf("\033[1;31m输入有误！\033[0m\n请重新输入号码（1～99）：");
			}
			if(tmp < 1 || tmp > 99)
				printf("\033[1;31m请按规定输入！\033[0m\n请重新输入号码（1～99）：");
			else
			{
				if(NumJudge(ptr_num,length,tmp))
				{
					printf("\033[1;31m请不要重复输入！\033[0m\n请重新输入号码（1～99）：");
					tmp = 0;
				}
			}
		}
		while(tmp < 1 || tmp > 99);
		ptr_num[i] = tmp;
	}
	NumSort(ptr_num,0,length-1);		//排序，从小到大
}
//修改购买注数
int SetLotteryMultiple()
{
	char flag;
	EmptyInputBuffer();
	printf("\t是否修改注数？（输入y确定）");
	flag = getchar();
	if('y' != flag)
		return 1;	//默认注数为1
	int tmp;
	printf("请输入注数（最大注数为5）：");
	do
	{
		while(!scanf("%d",&tmp))
		{
			EmptyInputBuffer();
			printf("\033[1;31m输入有误！\033[0m\n请重新输入注数（最大注数为5）:");
		}
		if(tmp <= 0 || tmp > 5)
			printf("\033[1;31m请按规定输入！\033[0m\n请重新输入注数（最大注数为5）：");
	}
	while(tmp <= 0 || tmp > 5);
	return tmp;
}
//数组排序，从小到大，快速排序
void NumSort(int *ptr_num, int left, int right)
{
	if(NULL == ptr_num)
		return ;
	if(left >= right)
		return ;
	int i = left;
	int j = right;
	int key = ptr_num[left];	//本次排序的中间值，将小于key的数放左边，大于key的数放右边，此时ptr_num[left]这个位置的值可被覆盖，因为这个值已经被取出
	while(i < j)
	{
		while(i<j && ptr_num[j]>=key)	//从右往左查，遇到小于key的值停止循环
			j--;
		ptr_num[i] = ptr_num[j];		//将较小值ptr_num[j]赋值给原来key值的位置(第一次执行时i下标为left)，此时ptr_num[j]这个位置的值可被覆盖
		while(i<j && ptr_num[i]<=key)	//从左往右查，遇到大于key的值停止循环
			i++;
		ptr_num[j] = ptr_num[i];		//将较大值ptr_num[i]赋值给原来ptr_num[j]值的位置，此时ptr_num[i]这个位置的值可被覆盖
	}
	ptr_num[i] = key;			//将key值放回数组，覆盖掉之前ptr_num[i]上的值
	NumSort(ptr_num,left,i-1);	//递归排序左边的数
	NumSort(ptr_num,i+1,right);	//递归排序右边的数
}
//数组数据查询，数组中有这个数字，则返回1，否则返回0
int NumJudge(const int *ptr_num, int length, int tmp)
{
	if(NULL == ptr_num)
		return 0;
	int i;
	for(i = 0; i < length; ++i)
		if(tmp == ptr_num[i])
			return 1;
	return 0;
}