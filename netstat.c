/*************************************************************************
    > File Name: netstat.c
    > Author: Schorsch
    > Mail: sh19931026@163.com
    > Created Time: So 21 Mär 2021 16:29:35 CET
    > Description: 
 ************************************************************************/
#include<stdio.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
long *my_ipconfig(char *ath0)
{
	int nDevLen = strlen(ath0);
	if (nDevLen < 1 || nDevLen > 100)
	{
		printf("dev length too long\n");
		return NULL;
	} 
	int fd = open("/proc/net/dev", O_RDONLY | O_EXCL);
	if (-1 == fd)
	{
		printf("/proc/net/dev not exists!\n");
		return NULL;
	}
	char buf[1024*2];
	lseek(fd, 0, SEEK_SET);
	int nBytes = read(fd, buf, sizeof(buf)-1);
	if (-1 == nBytes)
	{
		perror("read error");
		close(fd);
		return NULL;
	}
	buf[nBytes] = '\0';//返回第一次指向ath0位置的指针
	char* pDev = strstr(buf, ath0);
	if (NULL == pDev)
	{
		printf("don't find dev %s\n", ath0);
		return NULL;
	}
	char *p;
	char *ifconfig_value;
	int i = 0;
	static long rx2_tx10[2];
	/*去除空格，制表符，换行符等不需要的字段*/
	for (p = strtok(pDev, " \t\r\n"); p; p = strtok(NULL, " \t\r\n"))
	{
		i++;
		ifconfig_value = (char*)malloc(20);
		strcpy(ifconfig_value, p);
		/*得到的字符串中的第二个字段是接收流量*/
		if(i == 2)
		{
			rx2_tx10[0] = atol(ifconfig_value);
		}
		/*得到的字符串中的第十个字段是发送流量*/
		if(i == 10)
		{
			rx2_tx10[1] = atol(ifconfig_value);
			break;
		}
		free(ifconfig_value);
	}
	return rx2_tx10;
}

int main()
{
	long *ifconfig_result;
	double re_mb;
	/*eth0 是博主计算机上的网卡的名字*/
	ifconfig_result = my_ipconfig("enp0s3");
	/*保存在文件中的数值的单位是B，经过计算换算成MB*/
	re_mb = (double)ifconfig_result[0]/(1024*1024);
	printf("接收流量：%0.2f MB\n",re_mb);
	/*保存在文件中的数值的单位是B，经过计算换算成MB*/
	re_mb = (double)ifconfig_result[1]/(1024*1024);
	printf("发送流量：%0.2f MB\n",re_mb);
	return 0;
}
/*
————————————————
版权声明：本文为CSDN博主「最新九点半」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_44919483/article/details/89490017
*/
