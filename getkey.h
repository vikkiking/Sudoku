#include <termio.h>
#include <stdio.h>
char Getch(void)
{
	struct termios tm, tm_old;
	int fd = 0;
	char ch;
	if (tcgetattr(fd, &tm) < 0)
		return -1;//保存现在的终端设置
	tm_old = tm;
	cfmakeraw(&tm); //更改终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理
	if (tcsetattr(fd, TCSANOW, &tm) < 0)
		return -1;//设置上更改之后的设置
	ch = getchar();
	if (tcsetattr(fd, TCSANOW, &tm_old) < 0)
		return -1;//更改设置为最初的样子
	return ch;
}