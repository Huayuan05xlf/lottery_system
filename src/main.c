#include "menu.h"

int main(int argc, char const *argv[])
{
	void *pointers[3];			//通用指针数组，长度规定为3
	SystemInit(pointers);		//系统初始化
	SystemRun(pointers);		//系统运行
	SystemExit(pointers);		//系统退出
	return 0;
}