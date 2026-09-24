#include <CrimsonDesert/Structures/WinAppLauncher.h>
#include <GlobalData/Include.h>
#include <LiquidHookEx/Include.h>

using namespace Globals;

namespace CrimsonDesert {
	LH_START(".togC")

		DWORD WINAPI SetConsoleStateThread(LPVOID lpParam) {
		WinAppLauncher::ConsoleStateCtx* ctx = reinterpret_cast<WinAppLauncher::ConsoleStateCtx*>(lpParam);


		if (ctx->bShow) {
			using ShowConsoleFn = void(*)(WinAppLauncher*);
			ShowConsoleFn Exec = reinterpret_cast<ShowConsoleFn>(ctx->ShowConsoleFn);
			Exec(ctx->pThis);
		}
		else {
			using HideConsoleFn = void(*)(WinAppLauncher*);
			HideConsoleFn  Exec = reinterpret_cast<HideConsoleFn>(ctx->HideConsoleFn);
			Exec(ctx->pThis);
		}
		

		return 1;
	}

	DWORD WINAPI SetConsoleStateThreadEnd() { return 0; }
	LH_END()
	LH_START(".all")

		DWORD WINAPI AllocConsoleThread(LPVOID lpParam) {
		WinAppLauncher::AllocConsoleCtx* ctx = reinterpret_cast<WinAppLauncher::AllocConsoleCtx*>(lpParam);

		if (!ctx->bDebugEnabled) {
			using AllocConsoleDef = BOOL(WINAPI*)();
			AllocConsoleDef _AllocConsole = reinterpret_cast<AllocConsoleDef>(ctx->AllocConsole);
			if (_AllocConsole()) {
				using FreopenDef = FILE * (__cdecl*)(const char*, const char*, FILE*);
				FreopenDef Freopen = reinterpret_cast<FreopenDef>(ctx->Freopen);
				// freopen("CONOUT$", "w", stdout);
				using ACRTIOBFuncDef = FILE * (__cdecl*)(unsigned int);
				ACRTIOBFuncDef GetStdOut = reinterpret_cast<ACRTIOBFuncDef>(ctx->GetStdOut);
				Freopen(ctx->szConOut, ctx->szW, GetStdOut(ctx->m_StdOutIdx));
				ctx->bDebugEnabled = true;
			}
		}

		return 1;
	}

	DWORD WINAPI AllocConsoleThreadEnd() { return 0; }
	LH_END()



	static void* g_pOriginalDisabler = nullptr;
	static WinAppLauncher::DisablerHookData* g_pDisablerHookData = nullptr;

	LH_START(".hkDisabler")

		__int64 __fastcall WinAppLauncher::hkDisabler(__int64 a1, __int64 a2, __int64 a3, __int64 a4)
	{

		volatile WinAppLauncher::DisablerHookData* data = g_pDisablerHookData;

		typedef __int64(__fastcall* DisablerFn)(__int64 a1, __int64 a2, __int64 a3, __int64 a4);
		volatile DisablerFn original = (DisablerFn)g_pOriginalDisabler;
		return 0x0;

	}

	void WinAppLauncher::hkDisablerEnd() {}

	LH_END()

#define DISABLE_PATTERN "48 89 5C 24 ? 48 89 54 24 ? 55 56 57 41 56 41 57 48 83 EC ? 49 8B E9 49 8B F0 4C 8B F1 48 8B 01 4C 8B 78 ? BB ? ? ? ? 65 48 8B 04 25 ? ? ? ? 48 03 18 BA ? ? ? ? 8B CA 80 3B ? 74 ? E8 ? ? ? ? EB ? E8 ? ? ? ? 48 8B F8 48 85 C0 75 ? E8 ? ? ? ? 45 33 C9 45 33 C0 BA ? ? ? ? B9 ? ? ? ? FF 15 ? ? ? ? 48 89 7C 24 ? 48 85 FF 74 ? BA ? ? ? ? B9 ? ? ? ? 80 3B ? 74 ? E8 ? ? ? ? EB ? E8 ? ? ? ? 48 8B D8 48 85 C0 75 ? E8 ? ? ? ? 45 33 C9 45 33 C0 BA ? ? ? ? B9 ? ? ? ? FF 15 ? ? ? ? 48 89 5C 24 ? 48 85 DB 74 ? 48 8D 05 ? ? ? ? 48 89 03 48 8D 05 ? ? ? ? 48 89 03 48 89 73 ? C5 F8 10 45 ? C5 F8 11 43 ? 48 C7 07 ? ? ? ? 48 85 DB 74 ? 48 89 1F C6 47 ? ? C6 84 24 ? ? ? ? ? 48 89 BC 24 ? ? ? ? 45 33 C9 4C 8B C6 48 8D 15 ? ? ? ? 49 8B CE 49 8B C7 48 8B 5C 24 ? 48 83 C4 ? 41 5F 41 5E 5F 5E 5D 48 FF E0 CC CC 40 55"

	bool WinAppLauncher::ApplyHooks() {
		DisablerHookData initData{};

		return m_Hook.Hook<DisablerHookData>(
			DISABLE_PATTERN,
			"CrimsonDesert.exe",
			initData,
			reinterpret_cast<void*>(hkDisabler),
			reinterpret_cast<void*>(hkDisablerEnd),
			{
				LiquidHookEx::Detour::RipSlot::Data(&g_pDisablerHookData),
				LiquidHookEx::Detour::RipSlot::Orig(&g_pOriginalDisabler),
			},
			17
			);
	}

	

	WinAppLauncher* WinAppLauncher::GetInstance() {
		return pProc->ReadDirect<WinAppLauncher*>(pExeMod->GetAddr() + 0x6B52D70);
	}

	void WinAppLauncher::AllocConsole() {
		if (!pAllocConsoleCtx) {
			auto pBaseAddr = pExeMod->GetAddr();
			AllocConsoleCtx ctx;

			auto kernel32 = GetModuleHandle("KERNEL32.dll");
			if (!kernel32) {
				printf("Error retrieving KERNEL32.dll!!\n");
				return;
			}
			ctx.AllocConsole = reinterpret_cast<AllocConsoleDef>(GetProcAddress(kernel32, "AllocConsole"));

			auto ucrtbase = GetModuleHandle("ucrtbase.dll");
			if (!ucrtbase) {
				printf("Error retrieving ucrtbase.dll!!\n");
				return;
			}
			ctx.Freopen = reinterpret_cast<FreopenDef>(GetProcAddress(ucrtbase, "freopen"));
			ctx.GetStdOut = reinterpret_cast<ACRTIOBFuncDef>(__acrt_iob_func);
			ctx.m_StdOutIdx = 1;

			pRemoteConOutStr = pProc->AllocateAndWriteString("CONOUT$");
			if (!pRemoteConOutStr) {
				printf("Error allocating \"CONOUT$\" string!!\n");
				return;
			}

			ctx.szConOut = reinterpret_cast<const char*>(pRemoteConOutStr);


			pRemoteWStr = pProc->AllocateAndWriteString("w");
			if (!pRemoteWStr) {
				printf("Error allocating \"w\" string!!\n");
				pProc->FreeRemote(pRemoteConOutStr);
				pRemoteConOutStr = nullptr;
				return;
			}

			ctx.szW = reinterpret_cast<const char*>(pRemoteWStr);


			ctx.bDebugEnabled = false;


			printf("Freopen: 0x%p\n", ctx.AllocConsole);
			printf("GetStdOut: 0x%p\n", ctx.GetStdOut);
			pAllocConsoleCtx = reinterpret_cast<AllocConsoleCtx*>(pProc->Alloc(sizeof(ctx)));

			if (!pAllocConsoleCtx) {
				printf("Error Allocating RemoteConsoleCtx!!\n");
				return;
			}

			if (!pProc->Write<AllocConsoleCtx>(reinterpret_cast<uintptr_t>(pAllocConsoleCtx), ctx)) {
				printf("Failed to write pAllocConsoleCtx!!\n");
				pProc->FreeRemote(pAllocConsoleCtx);
				pAllocConsoleCtx = nullptr;
				return;
			}
		}

		printf("CTX: 0x%p\n", pAllocConsoleCtx);

		if (!pAllocConsoleShellcode) {
			pAllocConsoleShellcode = pProc->AllocAndWriteShellcode(AllocConsoleThread, AllocConsoleThreadEnd);
			if (!pAllocConsoleShellcode) {
				printf("Error Allocating SetConsoleStateThread!!\n");
				return;
			}
			printf("ShellCode: 0x%p\n", pAllocConsoleShellcode);
		}
		HANDLE hThread = pProc->CreateRemoteThreadEx(
			reinterpret_cast<LPTHREAD_START_ROUTINE>(pAllocConsoleShellcode),
			pAllocConsoleCtx
		);

		if (!hThread) {
			printf("Failed to create SetConsoleStateThread!!\n");
			pProc->FreeRemote(pAllocConsoleShellcode);
			pAllocConsoleShellcode = nullptr;
			pProc->FreeRemote(pAllocConsoleCtx);
			pAllocConsoleCtx = nullptr;
			return;
		}

		DWORD waitResult = WaitForSingleObject(hThread, 10000);

		if (waitResult != WAIT_OBJECT_0) {
			printf("Thread wait failed or timed out! Result: %d\n", waitResult);
			CloseHandle(hThread);
			pProc->FreeRemote(pAllocConsoleShellcode);
			pAllocConsoleShellcode = nullptr;
			pProc->FreeRemote(pAllocConsoleCtx);
			pAllocConsoleCtx = nullptr;

			return;
		}

		DWORD exitCode = 0;
		GetExitCodeThread(hThread, &exitCode);
		CloseHandle(hThread);
	}


	void WinAppLauncher::SetConsoleVisibility(bool bIsVisible) {
		
		if (!pRemoteConsoleCtx) {
			auto pBaseAddr = pExeMod->GetAddr();
			ConsoleStateCtx ctx;
			ctx.ShowConsoleFn = reinterpret_cast<PearlAbyssEngine_ShowDebugConsole>(pBaseAddr + 0x5A67C0);
			ctx.HideConsoleFn = reinterpret_cast<PearlAbyssEngine_HideDebugConsole>(pBaseAddr + 0x5A68B0);


			ctx.pThis = this;
			ctx.bShow = bIsVisible;

			pRemoteConsoleCtx = reinterpret_cast<ConsoleStateCtx*>(pProc->Alloc(sizeof(ctx)));

			if (!pRemoteConsoleCtx) {
				printf("Error Allocating RemoteConsoleCtx!!\n");
				return;
			}

			if (!pProc->Write<ConsoleStateCtx>(reinterpret_cast<uintptr_t>(pRemoteConsoleCtx), ctx)) {
				printf("Failed to write pGetClassCountCtx!!\n");
				pProc->FreeRemote(pRemoteConsoleCtx);
				pRemoteConsoleCtx = nullptr;
				return;
			}
		}

		pProc->Write<size_t>(reinterpret_cast<uintptr_t>(pRemoteConsoleCtx) + offsetof(ConsoleStateCtx, bShow), bIsVisible);
		printf("CTX: 0x%p\n", pRemoteConsoleCtx);

		if (!pHandleConsoleVisibilityShellcode) {
			pHandleConsoleVisibilityShellcode = pProc->AllocAndWriteShellcode(SetConsoleStateThread, SetConsoleStateThreadEnd);
			if (!pHandleConsoleVisibilityShellcode) {
				printf("Error Allocating SetConsoleStateThread!!\n");
				return;
			}
			printf("ShellCode: 0x%p\n", pHandleConsoleVisibilityShellcode);
		}
		HANDLE hThread = pProc->CreateRemoteThreadEx(
			reinterpret_cast<LPTHREAD_START_ROUTINE>(pHandleConsoleVisibilityShellcode),
			pRemoteConsoleCtx
		);

		if (!hThread) {
			printf("Failed to create SetConsoleStateThread!!\n");
			pProc->FreeRemote(pHandleConsoleVisibilityShellcode);
			pHandleConsoleVisibilityShellcode = nullptr;
			pProc->FreeRemote(pRemoteConsoleCtx);
			pRemoteConsoleCtx = nullptr;
			return;
		}

		DWORD waitResult = WaitForSingleObject(hThread, 10000);

		if (waitResult != WAIT_OBJECT_0) {
			printf("Thread wait failed or timed out! Result: %d\n", waitResult);
			CloseHandle(hThread);
			pProc->FreeRemote(pHandleConsoleVisibilityShellcode);
			pHandleConsoleVisibilityShellcode = nullptr;
			pProc->FreeRemote(pRemoteConsoleCtx);
			pRemoteConsoleCtx = nullptr;

			return;
		}

		DWORD exitCode = 0;
		GetExitCodeThread(hThread, &exitCode);
		CloseHandle(hThread);
	}



	void WinAppLauncher::ShowConsole() {
		SetConsoleVisibility(true);
	}
	void WinAppLauncher::HideConsole() {
		SetConsoleVisibility(false);
	}


	void WinAppLauncher::EnableDevMode() {

	}
}