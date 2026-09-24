#pragma once
#include <cstddef>
#include <LiquidHookEx/Include.h>
namespace CrimsonDesert {
	class WinAppLauncher {
	public:

		typedef void(*PearlAbyssEngine_ShowDebugConsole)(WinAppLauncher*);
		typedef void(*PearlAbyssEngine_HideDebugConsole)(WinAppLauncher*);
		typedef void(*PearlAbyssEngine_ToggleDebugMode)(WinAppLauncher*);
		
		typedef FILE* (__cdecl* FreopenDef)(
			const char*,
			const char*,
			FILE*
			);

		using AllocConsoleDef = BOOL(WINAPI*)();

		typedef FILE* (__cdecl* ACRTIOBFuncDef)(unsigned int);

		struct ConsoleStateCtx {
			PearlAbyssEngine_ShowDebugConsole ShowConsoleFn;
			PearlAbyssEngine_HideDebugConsole HideConsoleFn;
			WinAppLauncher* pThis;
			bool bShow;
		};

		struct AllocConsoleCtx {
			AllocConsoleDef AllocConsole;
			ACRTIOBFuncDef GetStdOut;
			FreopenDef Freopen;
			const char* szConOut;
			const char* szW;
			WinAppLauncher* pThis;
			int m_StdOutIdx;
			bool bDebugEnabled;
		};



		struct DisablerHookData : public LiquidHookEx::Detour::BaseHookData {

		};

	private:
		inline static ConsoleStateCtx* pRemoteConsoleCtx = nullptr;
		inline static void* pHandleConsoleVisibilityShellcode = nullptr;

		inline static AllocConsoleCtx* pAllocConsoleCtx = nullptr;
		inline static void* pRemoteConOutStr = nullptr;
		inline static void* pRemoteWStr = nullptr;
		inline static void* pAllocConsoleShellcode= nullptr;

		void SetConsoleVisibility(bool bIsVisible);
		inline static LiquidHookEx::Detour m_Hook = LiquidHookEx::Detour("DisablerHook");

	public:

		static __int64 __fastcall hkDisabler(__int64 a1, __int64 a2, __int64 a3, __int64 a4);
		static void hkDisablerEnd();

		static void EnableDevMode();
		static bool ApplyHooks();
		static WinAppLauncher* GetInstance();
		static void AllocConsole();
		void ShowConsole();
		void HideConsole();
		// this = [CrimsonDesert.exe+6B52D70]
	};
}