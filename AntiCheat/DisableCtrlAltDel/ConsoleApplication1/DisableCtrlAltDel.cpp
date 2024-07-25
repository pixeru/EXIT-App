#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <iostream>
#include <thread>

#define SE_DEBUG_PRIVILEGE 20

// Fill these
std::string password = "Josh";

typedef LONG(__stdcall* RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
typedef LONG(__stdcall* SuspendOrResumeProcess)(HANDLE hProcess);

// Function to get the process ID of winlogon.exe
DWORD GetWinlogonPid() {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // Take a snapshot of all processes
    PROCESSENTRY32W pEntry;
    pEntry.dwSize = sizeof(pEntry);
    BOOL rtn = Process32FirstW(snap, &pEntry);
    while (rtn) {
        if (!wcscmp(pEntry.szExeFile, L"winlogon.exe")) { 
            CloseHandle(snap);
            return pEntry.th32ProcessID; // Return the process ID of winlogon.exe if found
        }
        memset(pEntry.szExeFile, 0, sizeof(pEntry.szExeFile)); // Clear the szExeFile buffer
        rtn = Process32NextW(snap, &pEntry); // Use Process32NextW for wide character support
    }

    CloseHandle(snap);
    return 0;
}


// Function to disable the close button of the console window
void disableCloseButton(HWND hwnd) {
    HMENU hMenu = GetSystemMenu(hwnd, FALSE);
    if (hMenu != NULL) {
        EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    }
}

void disableExplorer() {
    system("taskkill /f /im explorer.exe");
}

void enableExporer() {
    system("explorer.exe");
}

void disablePowerButton() {
    system("powercfg /setacvalueindex SCHEME_CURRENT SUB_BUTTONS PBUTTONACTION 0");
}

void enablePowerButton() {
    system("powercfg /setacvalueindex SCHEME_CURRENT SUB_BUTTONS PBUTTONACTION 1");
}

////////////////////////////////// Disable Alt + F4 //////////////////////////////////////

// Callback function to intercept keystrokes
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            if (pkbhs->vkCode == VK_F4 && (GetAsyncKeyState(VK_MENU) & 0x8000)) {
                // Do nothing when Alt + F4 is pressed
                return 1;
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// Sets a low-level keyboard hook and runs a message loop
void HookThread() {
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(hhkLowLevelKybd);
}

void disableAltF4() {
	// Start the hook in a separate thread
	std::thread hookThread(HookThread);
	hookThread.detach();  // Allow the thread to run independently
}

////////////////////////////////// Disable Alt + F4 //////////////////////////////////////

int main() {
    HWND hwndConsole = GetConsoleWindow();
    disableCloseButton(hwndConsole);

    // Set window title
    SetConsoleTitle(L"NOT IN PLAY! EXIT AntiCheat");

    HMODULE hMod = LoadLibrary(L"ntdll");
    if (hMod == 0) {
        MessageBoxW(NULL, L"Failed to load ntdll!", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return 1;
    }

    RtlAdjustPrivilege lpfnRtlAdjustPrivilege = (RtlAdjustPrivilege)GetProcAddress(hMod, "RtlAdjustPrivilege");
    if (lpfnRtlAdjustPrivilege == 0) {
        MessageBoxW(NULL, L"Failed to get address of RtlAdjustPrivilege!", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return 1;
    }

    SuspendOrResumeProcess lpfnNtSuspendProcess = (SuspendOrResumeProcess)GetProcAddress(hMod, "NtSuspendProcess");
    if (lpfnNtSuspendProcess == 0) {
        MessageBoxW(NULL, L"Failed to get address of NtSuspendProcess!", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return 1;
    }

    SuspendOrResumeProcess lpfnNtResumeProcess = (SuspendOrResumeProcess)GetProcAddress(hMod, "NtResumeProcess");
    if (lpfnNtResumeProcess == 0) {
        MessageBoxW(NULL, L"Failed to get address of NtResumeProcess!", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return 1;
    }

    // Get the process ID of winlogon.exe
    DWORD pid = GetWinlogonPid(); 
    if (pid == 0) {
        MessageBoxW(NULL, L"Cannot find winlogon.exe!", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return 1;
    }

    BOOLEAN dummy = 0;
    lpfnRtlAdjustPrivilege(SE_DEBUG_PRIVILEGE, true, false, &dummy);

    // Open a handle to the winlogon.exe process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid); 
    if (hProcess == 0) {
        MessageBoxW(NULL, L"Failed to open winlogon.exe! Do you have administrative privilege?", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return 1;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////// Disable things /////////////////////////////////
    // Suspend the winlogon.exe process (DISABLE CTRL+ALT+DEL)
    lpfnNtSuspendProcess(hProcess);

    // Disable explorer.exe
    disableExplorer();

    // Disable Power button
    disablePowerButton();

    disableAltF4();

    //MessageBoxW(NULL, L"Now Ctrl+Alt+Del is disabled! Press OK to restore.", L"Ha!", MB_OK | MB_ICONINFORMATION);
    
    // Password Check
    std::string passwordInput;
    do {
		std::cout << "NOT IN PLAY! Enter the password to exit AntiCheat: ";
		std::cin >> passwordInput;
	} while (passwordInput != password);

    //////////////////////////// Enable things /////////////////////////////////
    // Resume the winlogon.exe process (ENABLE CTRL+ALT+DEL)
    lpfnNtResumeProcess(hProcess); 

    // Restore explorer.exe
    enableExporer();

    // Enable Power button
    enablePowerButton();


    // Close the handle to the winlogon.exe process
    CloseHandle(hProcess);

    // Close window
    CloseWindow(hwndConsole);

    return 0;
}