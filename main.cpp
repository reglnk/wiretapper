#include <iostream>
#include <cstring>
#include <fstream>

#define THIS_NAME "gcc"
#define THIS_NAME_EXT "gcc.exe"

#define NEXT_NAME "real-gcc"
#define NEXT_NAME_EXT "real-gcc.exe"

#define LOGS_PATH "C:/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/commands.log"

constexpr unsigned realNameLen = strlen(THIS_NAME);
constexpr unsigned extRealNameLen = realNameLen + 4;
constexpr unsigned progNameLen = strlen(NEXT_NAME);
constexpr unsigned extProgNameLen = progNameLen + 4;

bool contains_char(const char* str, char ch)
{
	unsigned len = strlen(str);
	
	for (unsigned i = 0; i < len; ++i) {
		if (str[i] == ch)
			return true;
	}
	return false;
}

void strmkb(char*& str, char ch)
{
	unsigned n = strlen(str);
	char* nstr = new char[n + 3];
	
	strcpy(nstr + 1, str);
	nstr[0] = nstr[n + 1] = ch;
	nstr[n + 2] = '\0';
	
	delete str;
	str = nstr;
}

void convert(char*& str)
{
	unsigned n = strlen(str);
	
	for (unsigned i = 0; i < n; ++i) {
		if (str[i] == '\\')
			str[i] = '/';
	}
}

int main(int argc, char** argv)
{
	unsigned arglen = strlen(argv[0]);

	unsigned charptr = arglen - extRealNameLen;
	char* newArg;
	
	// Replace input name with given
	if (arglen >= extRealNameLen && std::strcmp(argv[0] + charptr, THIS_NAME_EXT) == 0)
	{
		newArg = new char[charptr + extProgNameLen + 1];
		
		memcpy(newArg, argv[0], charptr);
		strcpy(newArg + charptr, NEXT_NAME_EXT);
	}
	else
	{
		charptr = arglen - realNameLen;
		newArg = new char[charptr + progNameLen + 1];
		
		memcpy(newArg, argv[0], charptr);
		strcpy(newArg + charptr, NEXT_NAME);
	}
	delete argv[0];
	argv[0] = newArg;
	
	unsigned position = 3U;
	
	for (int i = 0; i < argc; ++i) {
		position += strlen(argv[i]) + 3U;
	}
	char* command = new char[position];
	position = 0;
	
	// Protect args from spaces (-I"C:\Program Files\path"...)
	for (int i = 0; i < argc; )
	{
		if (contains_char(argv[i], ' '))
			strmkb(argv[i], '\"');
		
		strcpy(command + position, argv[i]);
		position = strlen(command);
		
		if (++i != argc) command[position++] = ' ';
	}
	
	convert(command);
	
	std::ofstream logfile(LOGS_PATH, std::fstream::app);
	logfile << "<command> " << command << '\n';
	logfile.close();
	
	system(command);
}