#include "TServerSystem.h"

#define LOGGING bool(false)

TServerSystem::TServerSystem(string _serverName, string _serverVersion)
	:ServerName(_serverName), ServerNameOriginal(_serverName), ServerVersion(_serverVersion), hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
{
	transform(ServerName.begin(), ServerName.end(), ServerName.begin(), (int(*)(int))toupper);
	ConsoleTitle = ServerName + " Server";

	string iniLocation = ".\\Configurations\\" + ServerNameOriginal + "Svr.ini";
	cSimpleIni.SetUnicode();
	cSimpleIni.LoadFile(iniLocation.c_str());

	SetConsoleTitle(ConsoleTitle.c_str());

	static bool bPresent = true;
	if (bPresent)
	{
		DisplayPresentation();
	}
	bPresent = false;
}

TServerSystem::~TServerSystem()
{
}

void TServerSystem::SSOutputTextFile(char* text, int color)
{
	char buffer[256];
	sprintf(buffer, "Logs\\LOG[%s][%s][%s].txt", ServerName.c_str(), GetLocalDate(0), GetLocalDate(1));

	static ofstream output(buffer);
	if (color != -1)
	{
		output << "[" << GetLocalTime() << "]" << "[" << "COLOR:" << color << "] " << text << endl;
	}
	else
	{
		output << "[" << GetLocalTime() << "]" << text << endl;
	}
}

void TServerSystem::DisplayPresentation()
{
	SetConsoleTextAttribute(hConsole, VERSION_COLOR);
	cout << "VERSION: " << ServerVersion << endl;
	SetConsoleTextAttribute(hConsole, PRESENTATION_COLOR);

	SSLogInfo("SERVER: " + ServerName, 0);
}

char* TServerSystem::GetLocalTime()
{
	time_t timeNow = time(0);
	char timestamp[16] = "";
	strftime(timestamp, 16, "%H:%M:%S", localtime(&timeNow));

	return timestamp;
}

char* TServerSystem::GetLocalDate(size_t opt)
{
	time_t timeNow = time(0);
	char timestamp[64] = "";

	if(opt == 0)
		strftime(timestamp, 64, "DA-%d MO-%m YE-%Y", localtime(&timeNow));
	else
		strftime(timestamp, 64, "HO-%H MI-%M SE-%S", localtime(&timeNow));

	return timestamp;
}

void TServerSystem::SSLogInfo(string text, int spaceBefore)
{
	SSLogSpace(spaceBefore);
	SetConsoleTextAttribute(hConsole, LOG_INFO_COLOR);
	cout << "[" << GetLocalTime() << "] > " << text << endl;
}

void TServerSystem::SSLogInfo(char* text, int spaceBefore)
{
	SSLogInfo(string(text), spaceBefore);
}

void TServerSystem::SSLogEvent(const char *fmt, ...)
{
	char textString[1024 * 5] = { '\0' };
	memset(textString, '\0', sizeof(textString));

	va_list args;
	va_start(args, fmt);
	vsnprintf(textString, 1024 * 5, fmt, args);
	va_end(args);

	SetConsoleTextAttribute(hConsole, LOG_EVENT_COLOR);
	cout << "[" << GetLocalTime() << "] (LOG EVENT) > " << textString << endl;
	if(LOGGING)
		SSOutputTextFile(textString);
}

void TServerSystem::SSLogEvent(size_t color, const char *fmt, ...)
{
	char textString[1024 * 5] = { '\0' };
	memset(textString, '\0', sizeof(textString));

	va_list args;
	va_start(args, fmt);
	vsnprintf(textString, 1024 * 5, fmt, args);
	va_end(args);

	SetConsoleTextAttribute(hConsole, color);
	cout << "[" << GetLocalTime() << "] > " << textString << endl;
	if (LOGGING)
		SSOutputTextFile(textString, color);
}

void TServerSystem::SSLogError(string text, int spaceBefore)
{
	SSLogSpace(spaceBefore);
	SetConsoleTextAttribute(hConsole, LOG_ERROR_COLOR);
	cout << "[" << GetLocalTime() << "] > " << text << endl;
}

void TServerSystem::SSLogSpace(int space)
{
	for (int i = 0; i < space; i++)
	{
		cout << endl;
	}
}

void TServerSystem::DisplayIni()
{
	SSLogInfo("Loaded configuration from " + ServerNameOriginal + "Svr.ini:", 0);
	SetConsoleTextAttribute(hConsole, INI_PRESENTATION_COLOR);
	cout << "##########################################" << endl;

	for (int i = 0; i < serverConfig.size(); ++i)
	{
		vector <string> currentConf = serverConfig.at(i);
		SetConsoleTextAttribute(hConsole, INI_PRESENTATION_COLOR);
		cout << "# + " << currentConf.at(0);
		for (int i = 0; i < 13 - currentConf.at(0).length(); i++)
		{
			cout << " ";
		}
		SetConsoleTextAttribute(hConsole, INI_VALUES_COLOR);
		cout << currentConf.at(1) << endl;
	}

	SetConsoleTextAttribute(hConsole, INI_PRESENTATION_COLOR);
	cout << "##########################################" << endl;
}

void TServerSystem::LoadIntFromIni(string confName, int defaultValue, int& ref)
{
	string namespaceIni = ServerNameOriginal + "Config";
	ref = stoi(cSimpleIni.GetValue(namespaceIni.c_str(), confName.c_str(), to_string(defaultValue).c_str()));
	vector <string> data;
	data.push_back(confName);
	data.push_back(to_string(ref));

	serverConfig.push_back(data);
}

void TServerSystem::LoadStringFromIni(string confName, string defaultValue, string& ref)
{
	string namespaceIni = ServerNameOriginal + "Config";
	ref = cSimpleIni.GetValue(namespaceIni.c_str(), confName.c_str(), defaultValue.c_str());
	vector <string> data;
	data.push_back(confName);
	data.push_back(ref);

	serverConfig.push_back(data);
}

DWORD TServerSystem::OnEnter()
{
	return OnEnter();
}

void TServerSystem::OnExit()
{
	SSLogInfo("Exiting application!", 0);
}

void TServerSystem::StartServer()
{
	SSLogInfo("Starting up...", 0);

	DWORD result = OnEnter();
	if (result) {
		StartupError(result);
	}
}

void TServerSystem::ExitServer()
{
	OnExit();
}

void TServerSystem::StartupError(DWORD result)
{
	SSLogError("Starup error: ", 2);
	switch (result) {
	case EC_INITSERVICE_DBOPENFAILED: {
		SSLogError("Can't connect to the database !", 0);
		SSLogError("1) Check ODBC x32 or x64, if all is configured", 0);
		SSLogError("2) Bad credentials in your .ini configuration file", 0);
		SSLogError("3) Check if you SQL Server is running !", 0);

		break;
	}
	case EC_INITSERVICE_CONNECTWORLD: {
		SSLogError("Can't connect to TWorld server !", 0);
		SSLogError("1) Check if TWorldSvr is running", 0);
		SSLogError("2) Check your .ini configuration file", 0);
	}
	default:
		SSLogError("Unknown error !", 0);
	}

	SSLogError("Can't start the server...", 2);
}