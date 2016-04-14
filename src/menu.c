#include "menu.h"
static int login_state;				//登陆状态：普通用户、游客、管理员、公证员
static char login_ID[MAXSIZE_ID];	//当前登陆账号

//加载用户文件（尾插法），此时ptr_head是一个空的头节点，也是尾节点
void LoadBuyer(buyer *ptr_head)
{
	if(NULL == ptr_head)
	{
		printf("系统故障……链表为空……\n");
		return ;
	}
	FILE *ptr_file = fopen(PATH_BUYER, "rb");	//打开二进制文件
	if(NULL == ptr_file)
	{
		SaveBuyer(ptr_head);					//文件不存在时创建空文件
		return ;
	}
	int flag;
	buyer buyer_backup;
	buyer *ptr_tmp = ptr_head;
	BuyerPointerCpy(&buyer_backup,ptr_head);	//备份指针数据
	while(1)
	{
		flag = fread(ptr_tmp, sizeof(buyer), 1, ptr_file);
		if(flag < 1)
			break;
		BuyerPointerCpy(ptr_tmp,&buyer_backup);
		ptr_tmp = InsertBuyerEnd(ptr_tmp);	//在链表尾插入新节点存放读取的数据
		if(NULL == ptr_tmp)
		{
			printf("系统故障！加载链表失败……\n（输入任意值退出……）\n");
			getchar();
			exit(ERROR_SIGN);
		}
		BuyerPointerCpy(&buyer_backup,ptr_tmp);
	}
	ptr_tmp->prior->next = NULL;
	free(ptr_tmp);			//读取不到新数据时，将最后新建的节点释放
	ptr_tmp = NULL;
	fclose(ptr_file);		//关闭文件
	ptr_file = NULL;
}
//保存用户文件
void SaveBuyer(buyer *ptr_head)
{
	if(NULL == ptr_head)
	{
		printf("系统故障……链表为空……\n");
		return ;
	}
	FILE *ptr_file = fopen(PATH_BUYER, "wb");	//打开二进制文件，文件不存在创建新文件
	if(NULL == ptr_file)
	{
		printf("系统故障！\n");
		return ;
	}
	buyer *ptr_tmp = ptr_head;
	while(NULL != ptr_tmp)
	{
		fwrite(ptr_tmp, sizeof(buyer), 1, ptr_file);
		ptr_tmp = ptr_tmp->next;
	}
	fclose(ptr_file);							//关闭文件
	ptr_file = NULL;
}
//加载发行文件（尾插法），此时ptr_head是一个空的头节点，也是尾节点
void LoadIssue(issue *ptr_head)
{
	if(NULL == ptr_head)
	{
		printf("系统故障……链表为空……\n");
		return ;
	}
	FILE *ptr_file = fopen(PATH_ISSUE, "rb");	//打开二进制文件
	if(NULL == ptr_file)
	{
		SaveIssue(ptr_head);					//文件不存在时创建空文件
		return ;
	}
	int flag;
	issue *ptr_tmp = ptr_head;
	issue *ptr_end = NULL;
	while(1)
	{
		flag = fread(ptr_tmp, sizeof(issue), 1, ptr_file);
		if(flag < 1)
			break;
		ptr_tmp->next  = NULL;				//当前尾节点
		ptr_end = ptr_tmp;					//保存当前尾节点
		ptr_tmp = InsertIssueEnd(ptr_tmp);	//在链表尾插入新节点存放读取的数据
		if(NULL == ptr_tmp)
		{
			printf("系统故障！加载链表失败……\n（输入任意值退出……）\n");
			getchar();
			exit(ERROR_SIGN);
		}
	}
	ptr_end->next = NULL;	//尾节点
	free(ptr_tmp);			//读取不到新数据时，将最后新建的节点释放
	ptr_end = ptr_tmp = NULL;
	fclose(ptr_file);		//关闭文件
	ptr_file = NULL;
}
//保存发行文件
void SaveIssue(issue *ptr_head)
{
	if(NULL == ptr_head)
	{
		printf("系统故障……链表为空……\n");
		return ;
	}
	FILE *ptr_file = fopen(PATH_ISSUE, "wb");	//打开二进制文件，文件不存在创建新文件
	if(NULL == ptr_file)
	{
		printf("系统故障！\n");
		return ;
	}
	issue *ptr_tmp = ptr_head;
	while(NULL != ptr_tmp)
	{
		fwrite(ptr_tmp, sizeof(issue), 1, ptr_file);
		ptr_tmp = ptr_tmp->next;
	}
	fclose(ptr_file);							//关闭文件
	ptr_file = NULL;
}
//加载彩票文件，加载前需要先加载发行文件
void LoadLottery(lottery *ptr_head, issue *ptr_latest)
{
	if(NULL == ptr_head || NULL == ptr_latest)
	{
		printf("系统故障…………\n");
		return ;
	}
	char str_path[MAXSIZE_PATH];
	GetLotteryPath(str_path,ptr_latest);		//存放路径及文件名
	FILE *ptr_file = fopen(str_path, "rb");		//打开二进制文件
	if(NULL == ptr_file)
	{
		SaveLottery(ptr_head,ptr_latest);		//文件不存在时创建空文件
		return ;
	}
	int flag;
	lottery lottery_backup;
	lottery *ptr_tmp = ptr_head;
	LotteryPointerCpy(&lottery_backup,ptr_head);//备份指针数据
	while(1)
	{
		flag = fread(ptr_tmp, sizeof(lottery), 1, ptr_file);
		if(flag < 1)
			break;
		LotteryPointerCpy(ptr_tmp,&lottery_backup);
		ptr_tmp = InsertLotteryPrior(ptr_head);	//在链表头节点之前插入新节点存放读取的数据
		if(NULL == ptr_tmp)
		{
			printf("系统故障！加载链表失败……\n（输入任意值退出……）\n");
			getchar();
			exit(ERROR_SIGN);
		}
		LotteryPointerCpy(&lottery_backup,ptr_tmp);
	}
	ptr_tmp->prior->next = ptr_tmp->next;
	ptr_tmp->next->prior = ptr_tmp->prior;
	free(ptr_tmp);			//读取不到新数据时，将最后新建的节点释放
	ptr_tmp = NULL;
	fclose(ptr_file);		//关闭文件
	ptr_file = NULL;
}
//保存彩票文件
void SaveLottery(lottery *ptr_head, issue *ptr_latest)
{
	if(NULL == ptr_head || NULL == ptr_latest)
	{
		printf("系统故障…………\n");
		return ;
	}
	char str_path[MAXSIZE_PATH];
	GetLotteryPath(str_path,ptr_latest);	//存放路径及文件名
	FILE *ptr_file = fopen(str_path, "wb");	//打开二进制文件，文件不存在创建新文件
	if(NULL == ptr_file)
	{
		printf("系统故障！\n");
		return ;
	}
	lottery *ptr_tmp = ptr_head;
	while(1)	//环形链表回到头节点
	{
		fwrite(ptr_tmp, sizeof(lottery), 1, ptr_file);
		if(ptr_head == ptr_tmp->next)
			break;
		ptr_tmp = ptr_tmp->next;
	}
	ptr_tmp = NULL;
	fclose(ptr_file);						//关闭文件
	ptr_file = NULL;
}
//加载临时文件，如果存在临时文件，说明系统异常退出
lottery* LoadTmp()
{
	FILE *ptr_file = fopen(PATH_TMP, "rb");		//打开二进制文件
	if(NULL == ptr_file)
		return NULL;							//文件不存在时，说明系统正常，放弃加载
	lottery *ptr_head = InitLottery();			//新建链表
	int flag;
	lottery lottery_backup;
	lottery *ptr_tmp = ptr_head;
	LotteryPointerCpy(&lottery_backup,ptr_head);//备份指针数据
	while(1)
	{
		flag = fread(ptr_tmp, sizeof(lottery), 1, ptr_file);
		if(flag < 1)
			break;
		LotteryPointerCpy(ptr_tmp,&lottery_backup);
		ptr_tmp = InsertLotteryPrior(ptr_head);	//在链表头节点之前插入新节点存放读取的数据
		if(NULL == ptr_tmp)
		{
			printf("系统故障！加载临时链表失败……\n（输入任意值退出……）\n");
			getchar();
			exit(ERROR_SIGN);
		}
		LotteryPointerCpy(&lottery_backup,ptr_tmp);
	}
	ptr_tmp->prior->next = ptr_tmp->next;
	ptr_tmp->next->prior = ptr_tmp->prior;
	free(ptr_tmp);			//读取不到新数据时，将最后新建的节点释放
	ptr_tmp = NULL;
	fclose(ptr_file);		//关闭文件
	ptr_file = NULL;
	return ptr_head;
}
//创建临时文件
void SaveTmp(lottery *ptr_head)
{
	if(NULL == ptr_head)
	{
		printf("系统故障…………\n");
		return ;
	}
	FILE *ptr_file = fopen(PATH_TMP, "wb");	//打开二进制文件，文件不存在创建新文件
	if(NULL == ptr_file)
	{
		printf("系统故障！\n");
		return ;
	}
	lottery *ptr_tmp = ptr_head;
	while(1)	//环形链表回到头节点
	{
		fwrite(ptr_tmp, sizeof(lottery), 1, ptr_file);
		if(ptr_head == ptr_tmp->next)
			break;
		ptr_tmp = ptr_tmp->next;
	}
	ptr_tmp = NULL;
	fclose(ptr_file);						//关闭文件
	ptr_file = NULL;
}
//删除临时文件
void DeleteTmp()
{
	char str_path[MAXSIZE_PATH];
	strcpy(str_path,PATH_TMP);
	remove(str_path);
}
//系统初始化，创建链表，按顺序加载链表
int SystemInit(void **ptr)
{
	if(NULL==ptr)
		return ERROR_SIGN;
	ptr[0] = InitBuyer();	//规定第1个指针为用户指针
	if(NULL == ptr[0])
		return ERROR_SIGN;
	buyer *ptr_buyer = (buyer*)ptr[0];
	ptr[1] = InitIssue();	//规定第2个指针为发行指针
	if(NULL == ptr[1])
		return ERROR_SIGN;
	issue *ptr_issue = (issue*)ptr[1];
	ptr[2] = InitLottery();	//规定第3个指针为彩票指针
	if(NULL == ptr[2])
		return ERROR_SIGN;
	lottery *ptr_lottery = (lottery*)ptr[2];
	LoadBuyer(ptr_buyer);					//加载用户信息
	LoadIssue(ptr_issue);					//加载发行信息
	if(HAS_ISSUE == ptr_issue->issue_state)	//如果最新一期彩票已发行
	{
		LoadLottery(ptr_lottery,ptr_issue);	//加载本期彩票信息
		lottery *ptr_tmp = LoadTmp();		//加载临时文件
		if(NULL != ptr_tmp)					//如果临时文件存在，把数据存放到彩票链表
			ptr[2] = ConnectLottery(ptr_lottery,ptr_tmp);
		DeleteTmp();						//删除临时文件
		SaveLottery(ptr[2],ptr_issue);		//保存本期彩票信息
	}
	login_state = GUEST;					//初始化登陆状态
	return NORMAL_SIGN;
}
//系统开始运行，死循环，需要3条链表
int SystemRun(void **ptr)
{
	if(NULL==ptr || NULL==ptr[0] || NULL==ptr[1] || NULL==ptr[2])
		return ERROR_SIGN;
	int exit_flag = NORMAL_SIGN;	//程序正常标志，系统处于循环中
	while(1)
	{
		if(EXIT_SIGN == exit_flag)	//正常退出
			return NORMAL_SIGN;
		switch(login_state)
		{
			case GUEST:			//游客界面
				exit_flag = MenuGuestCommand(ptr);
				break;
			case ADMINISTRATORS://管理员界面
				exit_flag = MenuAdminCommand(ptr);
				break;
			case NOTARY:		//公证员界面
				exit_flag = MenuNotaryCommand(ptr);
				break;
			case GENERAL_USER:	//彩民界面
				UserInit(ptr);	//初始化用户的彩票链表
				exit_flag = MenuBuyerCommand(ptr);
				UserExit(ptr);
				break;
			default:			//程序异常
				exit_flag = ERROR_SIGN;
		}
		if(ERROR_SIGN == exit_flag)
		{
			printf("程序异常！\n（输入任意值退出……）\n");
			EmptyInputBuffer();
			getchar();
			exit(ERROR_SIGN);
		}
	}
}
//用户登陆后，初始化链表，从彩票链表获取用户自己的彩票
void UserInit(void **ptr)
{
	if(NULL==ptr || NULL==ptr[0] || NULL==ptr[1] || NULL==ptr[2])
		return ;
	buyer *ptr_buyer = (buyer*)ptr[0];
	issue *ptr_issue = (issue*)ptr[1];
	lottery *ptr_lottery = (lottery*)ptr[2];
	if(HAS_ISSUE != ptr_issue->issue_state)		//彩票未发行（发行头节点为空，无发行数据）
		return ;
	buyer *ptr_me = FindBuyer(ptr_buyer, login_ID);
	lottery *ptr_lottery_tmp;
	ptr_lottery_tmp = FindBuyerLottery(ptr_lottery,ptr_me->buyer_ID);
	if(NULL == ptr_lottery_tmp)					//找不到彩票
	{
		ptr_me->my_lottery = NULL;
		return ;
	}
	ptr_me->my_lottery = ptr_lottery_tmp;
	do
	{
		ptr[2] = InsertLotteryPriorFromSrc(ptr_me->my_lottery,ptr[2],ptr_lottery_tmp);
		if(ptr_me->my_lottery == ptr[2])
			break;
		ptr_lottery_tmp = FindBuyerLottery(ptr[2],ptr_me->buyer_ID);
	}
	while(NULL != ptr_lottery_tmp);			//找不到彩票时退出循环
	SaveTmp(ptr_me->my_lottery);			//创建用户临时文件
	if(ptr_me->my_lottery == ptr[2])		//如果彩票总链表中没有彩票
	{
		lottery *ptr_null = InitLottery();
		SaveLottery(ptr_null,ptr_issue);	//清空彩票文件
		free(ptr_null);
		ptr_null = NULL;
	}
	else
		SaveLottery(ptr[2],ptr_issue);
}
//用户退出登陆后，归还彩票链表
void UserExit(void **ptr)
{
	if(NULL==ptr || NULL==ptr[0] || NULL==ptr[1] || NULL==ptr[2])
		return ;
	buyer *ptr_buyer = (buyer*)ptr[0];
	issue *ptr_issue = (issue*)ptr[1];
	lottery *ptr_lottery = (lottery*)ptr[2];
	buyer *ptr_me = FindBuyer(ptr_buyer, login_ID);
	if(NULL == ptr_me->my_lottery)			//如果用户彩票链表中没有彩票
		return ;
	if(ptr_me->my_lottery != ptr_lottery)	//如果彩票总链表中有彩票
	{
		if(!strlen(ptr_lottery->lottery_ID))//空彩票节点
		{
			free(ptr_lottery);
			ptr[2] = ptr_me->my_lottery;
		}
		else
			ptr[2] = ConnectLottery(ptr[2],ptr_me->my_lottery);
	}
	ptr_me->my_lottery = NULL;
	SaveLottery(ptr[2],ptr_issue);
	SaveBuyer(ptr_buyer);
	DeleteTmp();							//删除临时文件
}
//系统退出处理，销毁链表
void SystemExit(void **ptr)
{
	if(NULL==ptr || NULL==ptr[0] || NULL==ptr[1] || NULL==ptr[2])
		return ;
	DestroyBuyer((buyer*)ptr[0]);
	ptr[0] = NULL;
	DestroyIssue((issue*)ptr[1]);
	ptr[1] = NULL;
	DestroyLottery((lottery*)ptr[2]);
	ptr[2] = NULL;
	system("clear");
}
//初始界面，游客界面，注册登陆功能
void MenuGuestUI()
{
	system("clear");
	printf("彩票管理系统(%s)\n",VERSION);
	int i;
	for(i = 0; i < MAXSIZE_UI; ++i)
		printf("~");
	printf("\n\t\t1：用户注册\n\t\t2：用户登陆\n\t\t0：退出系统\n");
	for(i = 0; i < MAXSIZE_UI; ++i)
		printf("~");
	printf("\n");
}
//游客命令，需用户链表
int MenuGuestCommand(void **ptr)
{
	if(NULL==ptr || NULL==ptr[0] || NULL==ptr[1] || NULL==ptr[2])
		return ERROR_SIGN;
	int flag;
	buyer *ptr_buyer = (buyer*)ptr[0];
	MenuGuestUI();
	while(1)
	{
		printf("~~~~请输入指令（游客）：");
		while(!scanf("%d",&flag))
		{
			EmptyInputBuffer();		//清空输入缓冲区
			printf("~~~~输入有误！请重新输入\n");
			printf("~~~~请输入指令（游客）：");
		}
		switch(flag)
		{
			case 1 :if(EXIT_SIGN == BuyerRegister(ptr_buyer))
						return NORMAL_SIGN;
					break;
			case 2 :if(EXIT_SIGN == BuyerLogin(ptr_buyer))
						return NORMAL_SIGN;
					break;
			case 0 :return EXIT_SIGN;	//退出标志，表示调用此函数的函数需要退出（退出系统）
			default:printf("~~~~输入有误！请重新输入\n");
		}
	}
}
//管理员界面
void MenuAdminUI()
{
	system("clear");
	printf("彩票管理系统(%s)\n",VERSION);
	int i;
	for(i = 0; i < MAXSIZE_UI; ++i)
		printf("#");
	printf("\n\t1：发行彩票\t2：用户查询\n");
	printf("\t3：发行历史\t4：退出登陆\n");
	printf("\t0：退出系统\n");
	for(i = 0; i < MAXSIZE_UI; ++i)
		printf("#");
	printf("\n");
}
//管理员命令，需用户链表、发行链表、彩票链表
int MenuAdminCommand(void **ptr)
{
	if(NULL==ptr || NULL==ptr[0] || NULL==ptr[1] || NULL==ptr[2])
		return ERROR_SIGN;
	int flag;
	//buyer *ptr_buyer = (buyer*)ptr[0];
	//issue *ptr_issue = (issue*)ptr[1];
	lottery *ptr_lottery = (lottery*)ptr[2];
	MenuAdminUI();
	while(1)
	{
		printf("####请输入指令（管理员）：");
		while(!scanf("%d",&flag))
		{
			EmptyInputBuffer();		//清空输入缓冲区
			printf("####输入有误！请重新输入\n");
			printf("####请输入指令（管理员）：");
		}
		switch(flag)
		{
			case 1 :ptr[1] = IssueUpdate((issue*)ptr[1],ptr_lottery);
					break;
			case 2 :ptr[0] = ShowBuyerInfoAfterSort((buyer*)ptr[0]);	//排序并显示所有用户
					break;
			case 3 :ShowIssueInfoAll((issue*)ptr[1]); 	//显示所有发行信息
					break;
			case 4 :if(EXIT_SIGN == BuyerLogout())
						return NORMAL_SIGN;
					break;
			case 0 :return EXIT_SIGN;	//退出标志，表示调用此函数的函数需要退出（退出系统）
			default:printf("####输入有误！请重新输入\n");
		}
	}
}
//公证员界面
void MenuNotaryUI()
{
	system("clear");
	printf("彩票管理系统(%s)\n",VERSION);
	int i;
	for(i = 0; i < MAXSIZE_UI; ++i)
		printf("*");
	printf("\n\t1：彩票开奖\t2：彩票信息\n");
	printf("\t3：发行信息\t4：退出登陆\n");
	printf("\t0：退出系统\n");
	for(i = 0; i < MAXSIZE_UI; ++i)
		printf("*");
	printf("\n");
}
//公证员命令，需用户链表、发行链表
int MenuNotaryCommand(void **ptr)
{
	if(NULL==ptr || NULL==ptr[0] || NULL==ptr[1] || NULL==ptr[2])
		return ERROR_SIGN;
	int flag;
	buyer *ptr_buyer = (buyer*)ptr[0];
	issue *ptr_issue = (issue*)ptr[1];
	lottery *ptr_lottery = (lottery*)ptr[2];
	MenuNotaryUI();
	while(1)
	{
		printf("****请输入指令（公证员）：");
		while(!scanf("%d",&flag))
		{
			EmptyInputBuffer();		//清空输入缓冲区
			printf("****输入有误！请重新输入\n");
			printf("****请输入指令（公证员）：");
		}
		switch(flag)
		{
			case 1 :RunLottery(ptr_buyer,ptr_issue,ptr_lottery);
					break;
			case 2 :ShowLotteryInfoAll(ptr_lottery);	//显示本期所有彩票
					break;
			case 3 :ShowIssueInfo(ptr_issue);	//显示发行信息
					break;
			case 4 :if(EXIT_SIGN == BuyerLogout())
						return NORMAL_SIGN;
					break;
			case 0 :return EXIT_SIGN;	//退出标志，表示调用此函数的函数需要退出（退出系统）
			default:printf("****输入有误！请重新输入\n");
		}
	}
}
//彩民界面
void MenuBuyerUI()
{
	system("clear");
	printf("彩票管理系统(%s)\n",VERSION);
	int i;
	for(i = 0; i < MAXSIZE_UI; ++i)
		printf("$");
	printf("\n\t1：购买彩票\t2：余额充值\n");
	printf("\t3：我的信息\t4：我的彩票\n");
	printf("\t5：历史记录\t6：发行信息\n");
	printf("\t7：修改姓名\t8：修改密码\n");
	printf("\t9：退出登陆\t0：退出系统\n");
	for(i = 0; i < MAXSIZE_UI; ++i)
		printf("$");
	printf("\n");
}
//彩民命令，需用户链表、发行链表
int MenuBuyerCommand(void **ptr)
{
	if(NULL==ptr || NULL==ptr[0] || NULL==ptr[1] || NULL==ptr[2])
		return ERROR_SIGN;
	int flag;
	buyer *ptr_buyer = (buyer*)ptr[0];
	issue *ptr_issue = (issue*)ptr[1];
	lottery *ptr_lottery = (lottery*)ptr[2];
	MenuBuyerUI();
	while(1)
	{
		printf("$$$$请输入指令（%s）：",login_ID);
		while(!scanf("%d",&flag))
		{
			EmptyInputBuffer();		//清空输入缓冲区
			printf("$$$$输入有误！请重新输入\n");
			printf("$$$$请输入指令（%s）：",login_ID);
		}
		switch(flag)
		{
			case 1 :GetLottery(ptr_buyer,ptr_issue,ptr_lottery);	//下注
					break;
			case 2 :ChargeMoney(ptr_buyer);							//充值
					break;
			case 3 :ShowBuyerInfo(FindBuyer(ptr_buyer,login_ID));	//我的信息
					break;
			case 4 :ShowMyLottery(FindBuyer(ptr_buyer,login_ID));	//我的彩票
					break;
			case 5 :ShowLotteryHistory(FindBuyer(ptr_buyer,login_ID),ptr_issue);//历史记录
					break;
			case 6 :ShowIssueInfo(ptr_issue);	//显示发行信息
					break;
			case 7 :SetNewName(FindBuyer(ptr_buyer,login_ID));		//修改名字
					SaveBuyer(ptr_buyer);
					break;
			case 8 :SetNewPassword(FindBuyer(ptr_buyer,login_ID));	//修改密码
					SaveBuyer(ptr_buyer);
					break;
			case 9 :if(EXIT_SIGN == BuyerLogout())
						return NORMAL_SIGN;
					break;
			case 0 :return EXIT_SIGN;	//退出标志，表示调用此函数的函数需要退出（退出系统）
			default:printf("$$$$输入有误！请重新输入\n");
		}
	}
}





//注册
int BuyerRegister(buyer *ptr_head)
{
	if(NULL == ptr_head)
	{
		printf("无法注册……链表为空……\n");
		return ERROR_SIGN;
	}
	buyer buyer_tmp,buyer_backup;
	GetRegisterInfo(&buyer_tmp);			//获取注册信息
	if(!strcmp(ADMIN_ID,buyer_tmp.buyer_ID) \
		|| !strcmp(NOTARY_ID,buyer_tmp.buyer_ID) \
			|| NULL != FindBuyer(ptr_head,buyer_tmp.buyer_ID))
	{
		printf("\033[1;31m该用户已存在！\033[0m\n");
		return NORMAL_SIGN;	//正常退出，返回游客界面
	}
	buyer *ptr_new;
	if(strlen(ptr_head->buyer_ID))			//不是空节点，头节点有用户信息
	{
		ptr_new = InsertBuyerEnd(ptr_head);	//新建节点存储新用户
		if(NULL == ptr_new)
			return ERROR_SIGN;
	}
	else
		ptr_new = ptr_head;
	BuyerPointerCpy(&buyer_backup,ptr_new);	//备份指针数据
	*ptr_new = buyer_tmp;					//复制注册信息
	BuyerPointerCpy(ptr_new,&buyer_backup);	//还原指针数据
	SaveBuyer(ptr_head);					//注册成功后保存文件
	printf("\t注册成功！是否登陆系统？（输入y确定）\n");//注册成功询问是否登陆
	EmptyInputBuffer();
	char flag = getchar();
	if('y' != flag)
		return NORMAL_SIGN;				//正常退出，返回游客界面
	login_state = GENERAL_USER;			//注册成功自动改为登陆状态
	strcpy(login_ID,ptr_new->buyer_ID);	//修改当前登陆的ID
	return EXIT_SIGN;	//退出标志，表示调用此函数的函数需要退出（退出游客界面）
}
//登陆
int BuyerLogin(buyer *ptr_head)
{
	if(NULL == ptr_head)
	{
		printf("无法登陆……链表为空……\n");
		return ERROR_SIGN;
	}
	buyer buyer_tmp;
	char psd_tmp[MAXSIZE_PSD];
	GetLoginInfo(&buyer_tmp);		//获取账号密码
	StringDecrypt(psd_tmp,buyer_tmp.password);//密码解密
	if(!strcmp(ADMIN_ID,buyer_tmp.buyer_ID))
	{								//管理员登陆
		if(!strcmp(ADMIN_PSD,psd_tmp))
		{
			login_state = ADMINISTRATORS;
			strcpy(login_ID,ADMIN_ID);
			return EXIT_SIGN;
		}
		else
			printf("\033[1;31m密码输入错误！\033[0m\n");
	}
	else if(!strcmp(NOTARY_ID,buyer_tmp.buyer_ID))
	{								//公证员登陆
		if(!strcmp(NOTARY_PSD,psd_tmp))
		{
			login_state = NOTARY;
			strcpy(login_ID,NOTARY_ID);
			return EXIT_SIGN;
		}
		else
			printf("\033[1;31m密码输入错误！\033[0m\n");
	}
	else
	{								//普通用户登陆
		buyer *ptr_find = FindBuyer(ptr_head,buyer_tmp.buyer_ID);
		if(NULL != ptr_find)
		{
			if(!strcmp(ptr_find->password,buyer_tmp.password))
			{
				login_state = GENERAL_USER;
				strcpy(login_ID,ptr_find->buyer_ID);
				return EXIT_SIGN;
			}
			else
				printf("\033[1;31m密码输入错误！\033[0m\n");
		}
		else
			printf("\033[1;31m用户不存在！\033[0m\n");
	}
	return NORMAL_SIGN;
}
//注销
int BuyerLogout()
{
	char flag;
	EmptyInputBuffer();
	printf("\t是否退出登陆？（输入y退出）");
	flag = getchar();
	if('y' != flag)
		return NORMAL_SIGN;
	login_state = GUEST;		//登陆状态改为游客
	return EXIT_SIGN;
}
//发行
issue* IssueUpdate(issue *ptr_issue, lottery *ptr_lottery)
{
	if(NULL == ptr_issue || NULL == ptr_lottery)
	{
		printf("发行失败……链表为空……\n");
		return NULL;
	}
	issue *ptr_new;
	if(HAS_ISSUE == ptr_issue->issue_state)	//如果最新一期彩票已发行
	{
		if(HAS_LOTTERY == ptr_issue->lottery_state)	//最新一期彩票已开奖
			ptr_new = InsertIssueHead(ptr_issue);	//新增节点存放新一期发行信息
		else
		{
			printf("\033[1;31m尚未开奖，不能发行！\033[0m\n");
			return ptr_issue;
		}
	}
	else
		ptr_new = ptr_issue;
	SetIssueInfo(ptr_new);		//设置发行信息
	printf("####发行成功！\n");
	SaveIssue(ptr_new);			//发行成功保存发行文件
	ClearLottery(ptr_lottery);	//清空彩票链表
	SaveLottery(ptr_lottery,ptr_new);//新建彩票文件
	ShowIssueInfo(ptr_new);		//显示发行信息
	return ptr_new;
}
//开奖
void RunLottery(buyer *ptr_buyer, issue *ptr_issue, lottery *ptr_lottery)
{
	if(NULL == ptr_issue || NULL == ptr_lottery)
	{
		printf("开奖失败……链表为空……\n");
		return ;
	}
	if(HAS_ISSUE == ptr_issue->issue_state)			//如果最新一期彩票已发行
	{
		if(NO_LOTTERY == ptr_issue->lottery_state)	//最新一期彩票未开奖
		{
			int win_num[MAX_NUM_COUNT];
			printf("****正在开奖中……请稍候……\n");
			AutoRandomNum(win_num,MAX_NUM_COUNT);	//自动生成中奖号码
			UpdateIssueMoney(ptr_issue,ptr_lottery,win_num);//更新彩票
			UpdateBuyerMoney(ptr_buyer,ptr_lottery);//更新用户
			SetIssueState(ptr_issue,win_num);		//设置开奖信息
			printf("****开奖成功！\n");
			SaveIssue(ptr_issue);					//开奖后保存发行文件
			SaveLottery(ptr_lottery,ptr_issue);		//保存彩票文件
			ShowLotteryState(ptr_issue);
		}
		else
			printf("\033[1;31m本期已开奖！\033[0m\n");
	}
	else
		printf("\033[1;31m还未发行，无法开奖！\033[0m\n");
}
//下注
void GetLottery(buyer *ptr_buyer, issue *ptr_issue, lottery *ptr_lottery)
{
	if(NULL == ptr_buyer || NULL == ptr_issue || NULL == ptr_lottery)
	{
		printf("无法下注……链表为空……\n");
		return ;
	}
	if(HAS_ISSUE == ptr_issue->issue_state)			//如果最新一期彩票已发行
	{
		if(NO_LOTTERY == ptr_issue->lottery_state)	//最新一期彩票未开奖
		{
			buyer *ptr_me = FindBuyer(ptr_buyer, login_ID);
			char flag;
			int num[MAX_NUM_COUNT];
			int multiple, money;
			EmptyInputBuffer();
			printf("\t系统默认自动选号，是否手动选号？（输入y确定）");
			flag = getchar();
			if('y' != flag)
			{
				printf("自动生成中奖号……\n");
				AutoRandomNum(num,MAX_NUM_COUNT);
			}
			else
			{
				printf("手动输入中奖号……（请不要重复输入……）\n");
				ManualGetNum(num,MAX_NUM_COUNT);
			}
			multiple = SetLotteryMultiple();
			money = (ptr_issue->lottery_price)*multiple;
			if(ptr_me->account_balance < money)
			{
				printf("\033[1;31m余额不足，请充值！\033[0m\n");
				return ;
			}
			lottery *ptr_lottery_new;
			if(NULL == ptr_me->my_lottery)
			{
				ptr_me->my_lottery = InitLottery();
				ptr_lottery_new = ptr_me->my_lottery;
			}
			else
			{
				ptr_lottery_new = InsertLotteryPrior(ptr_me->my_lottery);
			}
			SetLotteryInfo(ptr_lottery_new,ptr_me->buyer_ID,ptr_issue->issue_number,num,multiple);
			AddSubtractMoney(ptr_me,money,REDUCE);		//修改用户余额
			AddJackpotBlance(ptr_issue,money);			//修改奖池余额
			SaveTmp(ptr_me->my_lottery);				//保存用户临时文件
			SaveIssue(ptr_issue);						//保存发行文件
			printf("$$$$下注成功！\n");
		}
		else
			printf("\033[1;31m本期已开奖！请等待新一期彩票发行！\033[0m\n");
	}
	else
		printf("\033[1;31m还未发行，无法下注！\033[0m\n");
}
//充值
void ChargeMoney(buyer *ptr_buyer)
{
	if(NULL == ptr_buyer)
		return ;
	int money;
	buyer *ptr_me = FindBuyer(ptr_buyer, login_ID);
	printf("$$$$请输入充值金额:");
	do
	{
		while(!scanf("%d",&money))
		{
			EmptyInputBuffer();
			printf("\033[1;31m请输入数字！\033[0m\n$$$$请输入充值金额:");
		}
		if(money < 0)
			printf("\033[1;31m请勿输入负数！\033[0m\n$$$$请输入充值金额:");
	}
	while(money < 0);
	AddSubtractMoney(ptr_me,money,INCREASE);
	if(0 == money)
		printf("$$$$取消充值……\n");
	else
		printf("$$$$充值成功！\n");
	SaveBuyer(ptr_buyer);
}
//显示我的彩票
void ShowMyLottery(buyer *ptr_tmp)
{
	if(NULL == ptr_tmp)
		return ;
	if(NULL == ptr_tmp->my_lottery)
	{
		printf("$$$$没有彩票……\n");
		return ;	
	}
	lottery *ptr_show = ptr_tmp->my_lottery;
	printf("=========================\033[0;36m已购买的彩票信息如下\033[0m=========================\n");
	while(1)	//环形链表回到头节点
	{
		ShowLotteryInfo(ptr_show);
		if(ptr_tmp->my_lottery == ptr_show->next)
			break;
		ptr_show = ptr_show->next;
	}
}
//显示购票历史记录
void ShowLotteryHistory(buyer *ptr_tmp, issue *ptr_issue)
{
	if(NULL == ptr_tmp || NULL == ptr_issue)
		return ;
	int issue_number_find, count = 0;
	float money = 0.0;
	if(INIT_ISSUE_NUM > ptr_issue->issue_number-1)
	{
		printf("$$$$没有历史记录……\n");
		return ;
	}
	printf("$$$$查询期号范围( %d ~ %d )\n",INIT_ISSUE_NUM,ptr_issue->issue_number-1);
	printf("$$$$请输入要查询的期号：");
	scanf("%d",&issue_number_find);
	if(issue_number_find < INIT_ISSUE_NUM || issue_number_find >= ptr_issue->issue_number)
	{
		printf("\033[1;31m输入有误！\033[0m\n");
		return ;
	}
	issue *ptr_find = FindIssue(ptr_issue,issue_number_find);
	lottery *ptr_lottery = InitLottery();
	LoadLottery(ptr_lottery,ptr_find);
	if(NULL == FindBuyerLottery(ptr_lottery,ptr_tmp->buyer_ID))
	{
		printf("$$$$无记录……！\n");
		DestroyLottery(ptr_lottery);
		return ;
	}
	lottery *ptr_show = ptr_lottery;
	while(1)	//环形链表回到头节点
	{
		if(!strcmp(ptr_tmp->buyer_ID,ptr_show->buyer_ID))
		{
			ShowLotteryInfo(ptr_show);
			count++;
			money += ptr_show->winning_amount;
		}
		if(ptr_lottery == ptr_show->next)
			break;
		ptr_show = ptr_show->next;
	}
	printf("============================>\033[0;36m本期共购买%d张彩票，",count);
	if(money > 0)
		printf("中奖金额：%.2f\033[0m\n",money);
	else
		printf("没有中奖……\033[0m\n");
	DestroyLottery(ptr_lottery);
}
//显示排序后的用户列表，返回用户头节点
buyer* ShowBuyerInfoAfterSort(buyer *ptr_head)
{
	if(NULL == ptr_head)
		return NULL;
	printf("===========\033[0;36m用户信息如下（按余额降序排列）\033[0m===========\n");
	ptr_head = SortBuyerByMoney(ptr_head);
	ShowBuyerInfoAll(ptr_head);
	SaveBuyer(ptr_head);	//保存用户文件
	return ptr_head;
}
//根据期号生成存放彩票文件的路径
void GetLotteryPath(char *path, issue *ptr_latest)
{
	char str_tmp[MAXSIZE_PATH];
	strcpy(path,PATH_LOTTERY);		//路径
	strcat(path,"lottery");			//文件名
	IntToString(str_tmp,ptr_latest->issue_number);//数字转字符串
	strcat(path,str_tmp);			//彩票期号做文件名
	strcat(path,".info");
}