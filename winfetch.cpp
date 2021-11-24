// winfetch.cpp : This file contains the 'main' function. Program execution begins and ends there.s  

#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <stdlib.h>
#include <atlstr.h>
#pragma comment(lib, "version.lib")
#define MEMDIV 1073741824
#define VMEMDIV 1024
#define COLOR_YELLOW "\033[33m"
#define WEIGHT_BOLD "\033[1m"
#define COLOR_BLUE "\033[94m"
#define COLOR_NONE "\033[0m"
#define W_LIGHT "\033[96m"
#define W_DARK "\033[36m"

using namespace std;

// Required for RtlGetVersion()
#pragma comment(lib, "ntdll.lib")

string getEnvVar(const char* name) {
	char* pValue;
	size_t len;
	errno_t err = _dupenv_s(&pValue, &len, name);
	if (err) return "Error Getting variable - " + string(name);
	return string(pValue);
}

extern "C" {
	typedef LONG NTSTATUS, * PNTSTATUS;
#define STATUS_SUCCESS (0x00000000)

	// Windows 2000 and newer
	NTSYSAPI NTSTATUS NTAPI RtlGetVersion(PRTL_OSVERSIONINFOEXW lpVersionInformation);
}

RTL_OSVERSIONINFOEXW getWindowsOSinformation() {
	RTL_OSVERSIONINFOEXW osVers;
	osVers.dwOSVersionInfoSize = sizeof(osVers);

	// fill the structure with version details
	NTSTATUS status = RtlGetVersion(&osVers);

	// this should always succeed
	assert(status == STATUS_SUCCESS);

	return osVers;
}

string getWindowsVersionString() {
	RTL_OSVERSIONINFOEXW osVers = getWindowsOSinformation();

	if (osVers.dwMajorVersion == 6) {
		if (osVers.dwMinorVersion == 0) {
			return "Windows Vista";
		} else if (osVers.dwMinorVersion == 1) {
			return "Windows 7";
		} else if (osVers.dwMinorVersion == 2) {
			return "Windows 8";
		} else if (osVers.dwMinorVersion == 3) {
			return "Windows 8.1";
		} else {
			return "There was a problem evaluating the Windows version";
		}
	} if (osVers.dwMajorVersion == 10) {
		if (osVers.dwBuildNumber >= 22000) {
			return "Windows 11";
		} else if (osVers.dwBuildNumber >= 21327) { // Earliest known Win11 Build
			return "Insider Windows 11";
		} else {
			return "Windows 10";
		}
	}
	else {
		return "Windows XP or earlier";
	}
}

namespace SystemInformation {
	void displayBanner() {
		cout << endl;
		cout << W_LIGHT;
		cout << "                ==========" << endl;
		cout << "        ========#########=" << endl;
		cout << "========####=############=" << endl;
		cout << "=###########=############=" << endl;
		cout << "==========================" << endl;
		cout << W_DARK;
		cout << "=###########=############=" << endl;
		cout << "=###########=############=" << endl;
		cout << "========####=############=" << endl;
		cout << "        ========#########=" << endl;
		cout << "                ==========" << endl;
		cout << COLOR_NONE;
		cout << endl;
	}
	void hardwareInformation() {
		string processorType;
		string processorCode;
		SYSTEM_INFO siSysInfo;

		// Copy the hardware information to the SYSTEM_INFO structure. 

		GetSystemInfo(&siSysInfo);


		MEMORYSTATUSEX statex;

		statex.dwLength = sizeof(statex);

		GlobalMemoryStatusEx(&statex);

		cout << COLOR_BLUE << WEIGHT_BOLD << "Processor and Memory information:" << COLOR_NONE << endl;
		cout << COLOR_YELLOW << "  Memory Load: "  << COLOR_NONE << statex.dwMemoryLoad << "%" << endl;
		cout << COLOR_YELLOW << "  Full Physical Memory: " << COLOR_NONE << (float) statex.ullTotalPhys / MEMDIV << " GB" << endl;
		cout << COLOR_YELLOW << "  Page size (Virtual Memory): " << COLOR_NONE << (float) siSysInfo.dwPageSize / VMEMDIV << " GB" << endl;
		cout << COLOR_YELLOW << "  Number of processors: " << COLOR_NONE << siSysInfo.dwNumberOfProcessors << endl;

		if (siSysInfo.dwProcessorType == PROCESSOR_AMD_X8664) {
			processorType = "64-bit (Intel-like)";
			processorCode = "amd64";
		} else if (siSysInfo.dwProcessorType == PROCESSOR_INTEL_IA64) {
			processorType = "Intel Itanium 64-bit"; 
			processorCode = "IA-64";
		} else if (siSysInfo.dwProcessorType == PROCESSOR_INTEL_PENTIUM) {
			processorType = "32-bit Pentium (Intel)";
			processorCode = "x86-32";
		} else if (siSysInfo.dwProcessorType == PROCESSOR_INTEL_386) {
			processorType = "32-bit (Intel-like 386)";
			processorCode = "i386";
		} else if (siSysInfo.dwProcessorType == PROCESSOR_INTEL_486) {
			processorType = "32-bit (Intel-like 486)";
			processorCode = "i486";
		} else {
			processorType = "Unknown Architecture";
		}

		cout << COLOR_YELLOW << "  Processor Architecture Name: " << COLOR_NONE << processorType << endl;
		cout << COLOR_YELLOW << "  Processor Architecture Code: " << COLOR_NONE << processorCode << endl;

		// Device information

		cout << COLOR_BLUE << WEIGHT_BOLD << "Device information:" << COLOR_NONE << endl;


		cout << COLOR_YELLOW << "  Not currently available." << COLOR_NONE << endl;
	}
	void systemInformation() {

		string data;
		cout << COLOR_BLUE << WEIGHT_BOLD << "System information:" << COLOR_NONE << endl;

		string windowsVersionString = getWindowsVersionString();
		RTL_OSVERSIONINFOEXW osVers = getWindowsOSinformation();
		if (windowsVersionString == "Windows XP or earlier") {
			cout << COLOR_YELLOW << "  Windows Version: " << COLOR_NONE <<
				getWindowsVersionString() <<
				". Build is " <<
				osVers.dwMajorVersion << "." <<
				osVers.dwMinorVersion << "." <<
				osVers.dwBuildNumber <<
				endl;
		}
		else {
			cout << COLOR_YELLOW << "  Windows Version: " << COLOR_NONE <<
				getWindowsVersionString() <<
				endl;
		}

		data = getEnvVar("username");
		cout << COLOR_YELLOW << "  Username: " << COLOR_NONE << data << endl;

		data = getEnvVar("windir");
		cout << COLOR_YELLOW << "  Windows Directory: " << COLOR_NONE << data << endl;

		data = getEnvVar("computername");
		cout << COLOR_YELLOW << "  Computer Name: " << COLOR_NONE << data << endl;
	}
}

int main() {
	SystemInformation::displayBanner();
	SystemInformation::hardwareInformation();
	SystemInformation::systemInformation();
	cout << endl;
	return 0;
}
