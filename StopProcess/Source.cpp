#include <windows.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <set>
#include <string>
#include <vector>
#include <winternl.h>
using namespace std;

typedef LONG(NTAPI* NtSuspendProcess)(HANDLE);
typedef LONG(NTAPI* NTQueryInformationProcess)(HANDLE, LONG, PVOID, LONG, LONG);

vector<pair<DWORD, wstring>> Search()
{
	PROCESSENTRY32 pent = { 0 };
	vector<pair<DWORD, wstring>> items;

	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pent.dwSize = sizeof(pent);
	Process32First(h, &pent);
	do {
		if (!wcscmp(pent.szExeFile, L"cl.exe") || 
			!wcscmp(pent.szExeFile, L"link.exe") ||
			!wcscmp(pent.szExeFile, L"gcc.exe") || 
			!wcscmp(pent.szExeFile, L"g++.exe") || 
			!wcscmp(pent.szExeFile, L"as.exe") || 
			!wcscmp(pent.szExeFile, L"ld.exe") ||
			!wcscmp(pent.szExeFile, L"cc1.exe") ||
			!wcscmp(pent.szExeFile, L"collect2.exe") ||
			!wcscmp(pent.szExeFile, L"tcc.exe") ||
			!wcscmp(pent.szExeFile, L"go.exe") ||
			!wcscmp(pent.szExeFile, L"cgo.exe") ||
			!wcscmp(pent.szExeFile, L"compile.exe") ||
			!wcscmp(pent.szExeFile, L"csc.exe"))
		{
			items.push_back({pent.th32ProcessID, pent.szExeFile});
			//name = pent.szExeFile;
			//CloseHandle(h);
			//return pent.th32ProcessID;
		}
	} while (Process32Next(h, &pent));
	CloseHandle(h);
	//return 0;
	return items;
}

wstring GetCMDLine(HANDLE hProcess)
{
	void* pinfo[6];

	LONG status = ((NTQueryInformationProcess)GetProcAddress(GetModuleHandle(L"ntdll"), "NtQueryInformationProcess"))(hProcess, (PROCESSINFOCLASS)0, pinfo, sizeof(PVOID) * 6, NULL);

	PPEB ppeb = (PPEB)((PVOID*)pinfo)[1];
	PPEB ppebCopy = (PPEB)malloc(sizeof(PEB));
	BOOL result = ReadProcessMemory(hProcess, ppeb, ppebCopy, sizeof(PEB), NULL);
	PRTL_USER_PROCESS_PARAMETERS pRtlProcParam = ppebCopy->ProcessParameters;
	PRTL_USER_PROCESS_PARAMETERS pRtlProcParamCopy = (PRTL_USER_PROCESS_PARAMETERS)malloc(sizeof(RTL_USER_PROCESS_PARAMETERS));
	result = ReadProcessMemory(hProcess, pRtlProcParam, pRtlProcParamCopy, sizeof(RTL_USER_PROCESS_PARAMETERS), NULL);
	PWSTR wBuffer = pRtlProcParamCopy->CommandLine.Buffer;
	USHORT len = pRtlProcParamCopy->CommandLine.Length;
	PWSTR wBufferCopy = (PWSTR)malloc(len);
	result = ReadProcessMemory(hProcess, wBuffer, wBufferCopy, len, NULL);
	return wBufferCopy;
}

void suspend(DWORD processId)
{
	HANDLE processHandle = OpenProcess(PROCESS_SUSPEND_RESUME, FALSE, processId);
	if (!processHandle)
	{
		printf("Error: %d\n", GetLastError());
		return;
	}
	NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(GetModuleHandle(L"ntdll"), "NtSuspendProcess");
	pfnNtSuspendProcess(processHandle);
	//wstring cmd = GetCMDLine(processHandle);
	CloseHandle(processHandle);
	//return cmd;
}

int main(int argc, char* argv[])
{
	vector<pair<DWORD, wstring>> pids;
	set<DWORD> exceptions;

	while (true)
	{
		pids = Search();
		for (auto it : pids)
		{
			if (exceptions.find(it.first) == exceptions.end())
			{				
				if (argc == 2 && !strcmp(argv[1], "suspend"))
					suspend(it.first);
				printf("%S -> %d\n", it.second.c_str(), it.first);
				exceptions.insert(it.first);
			}
		}
	}
	return 0;
}