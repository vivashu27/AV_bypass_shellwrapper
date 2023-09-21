
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <stdio.h>

int Error(const char* text) {
	printf("%s (%d)\n", text, GetLastError());
	return 1;
}
HHOOK (WINAPI* hk)(
	int       idHook,
	HOOKPROC  lpfn,
	HINSTANCE hmod,
	DWORD     dwThreadId
);

BOOL (WINAPI * unk)(
	HHOOK hhk
);
DWORD FindMainNotepadThread() {
	auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	DWORD tid = 0;
	THREADENTRY32 th32;
	th32.dwSize = sizeof(th32);

	Thread32First(hSnapshot, &th32);
	do {
		auto hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, th32.th32OwnerProcessID);
		if (hProcess) {
			WCHAR name[MAX_PATH];
			if (GetProcessImageFileName(hProcess, name, MAX_PATH) > 0) {
				auto bs = wcsrchr(name, L'\\');
				if (bs && _wcsicmp(bs, L"\\explorer.exe") == 0) {
					tid = th32.th32ThreadID;
					break;
				}
			}
			CloseHandle(hProcess);
		}
	} while (Thread32Next(hSnapshot, &th32));
	CloseHandle(hSnapshot);
	return tid;
}

int main() {
	DWORD tid = FindMainNotepadThread();
	if (tid == 0)
		return Error("Failed to locate Notepad");

	auto hDll = LoadLibrary(L"hookme.dll");
	if (!hDll)
		return Error("Failed to locate Dll\n");


	auto setNotify = (void (WINAPI*)(DWORD, HHOOK))GetProcAddress(hDll, "SetNotificationThread");
	
	hk = (HHOOK (WINAPI *)(int idHook, HOOKPROC  lpfn, HINSTANCE hmod, DWORD dwThreadId))GetProcAddress(GetModuleHandle((LPCWSTR)"User32.dll"),"SetWindowsHookEx");
	auto hHook = hk(WH_GETMESSAGE, (HOOKPROC)GetProcAddress(hDll, "DllMain"), hDll, tid);
	if (!hHook)
		return Error("Failed to install hook");

	setNotify(GetCurrentThreadId(), hHook);
	PostThreadMessage(tid, WM_NULL, 0, 0);
	unk = (BOOL (WINAPI*)(HHOOK hhk))GetProcAddress(GetModuleHandle((LPCWSTR)"User32.dll"), "UnhookWindowsHookEx");;
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (msg.message == WM_APP) {
			printf("%c", (int)msg.wParam);
			if (msg.wParam == 13)
				printf("\n");
		}
	}
	unk(hHook);
	FreeLibrary(hDll);

	return 0;
}
