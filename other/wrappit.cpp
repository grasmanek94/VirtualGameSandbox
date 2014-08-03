// wrappit.cpp : Defines the entry point for the console application.
// 1.1


#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <vector>
using namespace std;

struct EXPORT_ITEM
	{
	bool IsOnlyOrdinal;
	char in[300];
	char en[300];
	int o;
	int h;
	};

vector<EXPORT_ITEM> v;


#pragma warning(disable : 4996)

int main(int argc, char* argv[])
{
	printf("Wrappit. Copyright (C) Chourdakis Michael\r\n");
	printf("Usage: WRAPPIT <dll> <txt> <convention> <new dll name> <cpp> <def>\r\n");
	printf("==================================================================\r\n");

	if (argc < 7)
		return 0;

	// STEP 1: Read exports
	_tprintf(_T("Step 1: Parsing %s...\r\n"),argv[2]);
	FILE* fp = _tfopen(argv[2],_T("rb"));
	if (!fp)
		{
		printf("Error 1: Txt file not found.\r\n");
		return 0;
		}
	int MSStart = 0;
	for(int i = 0 ;; )
		{
		char x[1000] = {0};
		EXPORT_ITEM e = {0};
		if (!fgets(x,1000,fp))
			break;

		if (strnicmp(x,"EXPORT ord",10) == 0) // tdump
			{
			//EXPORT ord:1141='AcceptEx' => 'MSWSOCK.AcceptEx'
			e.o = atoi(x + 11);
			sprintf(e.in,"__E__%u__",i);
			char* y = strchr(x,'\'');
			if (y)
				{
				y++;
				char* y2 = strchr(y,'\'');
				if (y2)
					*y2 = 0;
				strcpy(e.en,y);
				e.IsOnlyOrdinal = false;
				}
			else
				{
				e.IsOnlyOrdinal = true;
				sprintf(e.en,"___XXX___%u",i);
				}
			v.insert(v.end(),e);
			}
		else
			{
			if (strstr(x,"ordinal") != 0 && strstr(x,"hint") != 0 && strstr(x,"RVA") != 0)
				{
				MSStart = 1;
				continue;
				}
			if (!MSStart)
				continue;
			char* a1 = x;
			while(*a1 == ' ')
				a1++;
			if (*a1 == '\r' || *a1 == '\n')
				{
				if (MSStart == 1)
					{
					MSStart = 2;
					continue;
					}
				break;
				}
			e.o = atoi(a1);
			while(*a1 != ' ')
				a1++;
			while(*a1 == ' ')
				a1++;
			if (*a1 == '\r' || *a1 == '\n')
				{
				if (MSStart == 1)
					{
					MSStart = 2;
					continue;
					}
				break;
				}
			e.h = atoi(a1);
			while(*a1 != ' ')
				a1++;
			while(*a1 == ' ')
				a1++;
			if (*a1 == '\r' || *a1 == '\n')
				{
				if (MSStart == 1)
					{
					MSStart = 2;
					continue;
					}
				break;
				}
			if (*a1 >= 0x30 && *a1 <= 0x39) // RVA exists
				{
				while(*a1 != ' ')
					a1++;
				while(*a1 == ' ')
					a1++;
				if (*a1 == '\r' || *a1 == '\n')
					break;
				}

			sprintf(e.in,"__E__%u__",i++);
			e.IsOnlyOrdinal = false;
			if (strnicmp(a1,"[NONAME]",8) == 0)
				{
				e.IsOnlyOrdinal = true;
				sprintf(e.en,"___XXX___%u",i);
				}
			else
				{
				for(int y = 0 ; ; y++)
					{
					if (*a1 == ' ' || *a1 == '\r' || *a1 == '\n' || *a1 == '\t')
						break;
					e.en[y] = *a1++;
					}
				}
			v.insert(v.end(),e);
			}
		}
	fclose(fp);
	_tprintf(_T("Step 1: %u exported functions parsed.\r\n"),v.size());
	printf("------------------------------------------\r\n");

	_tprintf(_T("Step 2: Generating .DEF file %s...\r\n"),argv[6]);
	FILE* fdef = _tfopen(argv[6],_T("wb"));
	if (!fdef)
		{
		printf("Error 2: DEF file cannot be created.\r\n");
		return 0;
		}
	fprintf(fdef,"EXPORTS\r\n");
	for(unsigned int i = 0 ; i < v.size () ; i++)
		{
		if (v[i].IsOnlyOrdinal == false)
			fprintf(fdef,"%s=%s @%u\r\n",v[i].en,v[i].in,v[i].o);
		else
			fprintf(fdef,"%s=%s @%u NONAME\r\n",v[i].en,v[i].in,v[i].o);
		}
	fclose(fdef);
	_tprintf(_T("Step 2: %u exported functions written to DEF.\r\n"),v.size());
	printf("------------------------------------------\r\n");

	_tprintf(_T("Step 3: Generating .CPP file %s...\r\n"),argv[5]);
	FILE* fcpp = _tfopen(argv[5],_T("wb"));
	if (!fcpp)
		{
		printf("Error 3: CPP file cannot be created.\r\n");
		return 0;
		}

	// Write headers
	fprintf(fcpp,"#include <windows.h>\r\n");
	fprintf(fcpp,"#pragma pack(1)\r\n\r\n\r\n");

	// Write variables
	fprintf(fcpp,"HINSTANCE hLThis = 0;\r\n");
	fprintf(fcpp,"HINSTANCE hL = 0;\r\n");
	fprintf(fcpp,"FARPROC p[%u] = {0};\r\n\r\n",v.size());

	// Write DllMain
	fprintf(fcpp,"BOOL WINAPI DllMain(HINSTANCE hInst,DWORD reason,LPVOID)\r\n");
	fprintf(fcpp,"\t{\r\n");
	fprintf(fcpp,"\tif (reason == DLL_PROCESS_ATTACH)\r\n");
	fprintf(fcpp,"\t\t{\r\n");

	fprintf(fcpp,_T("\t\thLThis = hInst;\r\n"),argv[4]);
	fprintf(fcpp,_T("\t\thL = LoadLibrary(\"%s\");\r\n"),argv[4]);
	fprintf(fcpp,"\t\tif (!hL) return false;\r\n");

	fprintf(fcpp,"\r\n\r\n");
	for(unsigned int i = 0 ; i < v.size () ; i++)
		{
		if (v[i].IsOnlyOrdinal == true)
			fprintf(fcpp,"\t\tp[%u] = GetProcAddress(hL,(LPCSTR)\"%u\");\r\n",i,v[i].o);
		else
			fprintf(fcpp,"\t\tp[%u] = GetProcAddress(hL,\"%s\");\r\n",i,v[i].en);
		}

	fprintf(fcpp,"\r\n\r\n");
	fprintf(fcpp,"\t\t}\r\n");

	fprintf(fcpp,"\tif (reason == DLL_PROCESS_DETACH)\r\n");
	fprintf(fcpp,"\t\t{\r\n");
	fprintf(fcpp,_T("\t\tFreeLibrary(hL);\r\n"));
	fprintf(fcpp,"\t\t}\r\n\r\n");


	fprintf(fcpp,"\treturn 1;\r\n");
	fprintf(fcpp,"\t}\r\n\r\n");


	// Write function to be exported
	for(unsigned int i = 0 ; i < v.size() ; i++)
		{
		fprintf(fcpp,"// %s\r\nextern \"C\" __declspec(naked) void %s %s()\r\n",v[i].en,argv[3],v[i].in);
		fprintf(fcpp,"\t{\r\n");
		fprintf(fcpp,"\t__asm\r\n");
		fprintf(fcpp,"\t\t{\r\n");

		fprintf(fcpp,"\t\tjmp p[%u*%u];\r\n",i,sizeof(void*));

		fprintf(fcpp,"\t\t}\r\n");
		fprintf(fcpp,"\t}\r\n\r\n");
		}




	fclose(fcpp);


	TCHAR ay[1000] = {0};
//	_stprintf(ay,_T("BCC32 -o%s.obj -c %s\r\n"),argv[5],argv[5]);
//	_stprintf(ay,_T("CL.EXE /O2 /GL /I \".\" /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /D \"_WINDLL\" /FD /EHsc /MT /Fo\".\\%s.obj\" /Fd\".\\vc80.pdb\" /W3 /nologo /c /Wp64 /TP /errorReport:prompt %s\r\n"),argv[5],argv[5]);
//	system(ay);
//	_stprintf(ay,_T("ILINK32 -c -Tpd %s.obj,%s,,,%s\r\n"),argv[5],argv[1],argv[6]);
//	_stprintf(ay,_T("LINK.EXE /OUT:\"%s\" /INCREMENTAL:NO /NOLOGO /DLL /MANIFEST /DEF:\"%s\" /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /MACHINE:X86 /ERRORREPORT:PROMPT %s.obj kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib\r\n"),argv[1],argv[6],argv[5]);
//	system(ay);
//
//	system("pause");
	return 0;
}

