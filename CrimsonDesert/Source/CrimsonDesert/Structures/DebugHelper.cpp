#include <CrimsonDesert/Structures/DebugHelper.h>
#include <GlobalData/Include.h>
#include <LiquidHookEx/Include.h>

using namespace Globals;
namespace CrimsonDesert {
	static void* g_pOriginalRegisterCmds = nullptr;
	static DebugHelper::RegisterConsoleCommandsHookData* g_pDisablerHookData = nullptr;

	LH_START(".hkReg")

		__int64 __fastcall DebugHelper::hkRegisterCommands(__int64 a1, __int64 a2)
	{

		volatile DebugHelper::RegisterConsoleCommandsHookData* data = g_pDisablerHookData;

		typedef __int64(__fastcall* RegisterCommandsFn)(__int64 a1, __int64 a2);
		volatile RegisterCommandsFn original = (RegisterCommandsFn)g_pOriginalRegisterCmds;
		data->a1Cpy = a1;
		data->a2Cpy = a2;
		return original(a1, a2);
	}

	void DebugHelper::hkRegisterCommandsEnd() {}

	LH_END()
#define REGISTERCMDS_PATTERN "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B DA 48 8B F1 45 33 F6 44 89 75 ? 65 48 8B 04 25"
	bool DebugHelper::ApplyHooks() {
		// 

		RegisterConsoleCommandsHookData initData{};
		initData.a1Cpy = 0x0;
		initData.a2Cpy = 0x0;
		if (m_Hook.Hook<RegisterConsoleCommandsHookData>(
			REGISTERCMDS_PATTERN,
			"CrimsonDesert.exe",
			initData,
			reinterpret_cast<void*>(hkRegisterCommands),
			reinterpret_cast<void*>(hkRegisterCommandsEnd),
			{
				LiquidHookEx::Detour::RipSlot::Data(&g_pDisablerHookData),
				LiquidHookEx::Detour::RipSlot::Orig(&g_pOriginalRegisterCmds),
			},
			17
			)) {
			auto res = m_Hook.ReadData<RegisterConsoleCommandsHookData>();
			while (res.a1Cpy || !res.a2Cpy) {
				Sleep(100);
				res = m_Hook.ReadData<RegisterConsoleCommandsHookData>();
			}
			printf("\nRegisterConsoleCommandsHookData:\n\tA1: 0x%p\nA2: 0x%p\n", res.a1Cpy, res.a2Cpy);
			system("pause");

			return true;
		}

		return false;
	}


	LH_START(".regCmd")

		DWORD WINAPI RegisterCmdThread(LPVOID lpParam) {
		DebugHelper::RegisterCustomCommandCtx* ctx = reinterpret_cast<DebugHelper::RegisterCustomCommandCtx*>(lpParam);

		typedef int(__cdecl* RegisterCmdDef)(void* registry,
			const char* command,
			const char* handlerName,
			DebugHelper::RegisterCustomCommandHandlerDescriptorCtx* descriptor);
		RegisterCmdDef RegCmd = reinterpret_cast<RegisterCmdDef>(ctx->fnToExecute);


		RegCmd(ctx->pRegistry, ctx->szCmd, ctx->szConsoleCommandHandler, ctx->pDescHandler);

		return 1;
	}

	DWORD WINAPI RegisterCmdThreadEnd() { return 0; }
	LH_END()


	void DebugHelper::RegisterTestCommand() {
		// // [[[CrimsonDesert.exe+6B53230]] + 0x28]
		void* pRegistry = pProc->ReadDirect<void*>(pProc->ReadDirect<uintptr_t>(pProc->ReadDirect<uintptr_t>(pExeMod->GetAddr() + 0x6B53230)) + 0x28);
		if (!pRegistry) {
			printf("Error Retrieving Registry!!\n");
			return;
		}
		printf("CommandRegistry: 0x%p\n", pRegistry);

		void* pCmdName = pProc->AllocateAndWriteString("/test");
		if (!pCmdName) {
			printf("Error writing \"/test\"!!\n");
			return;
		}
		printf("/test Str: 0x%p\n", pCmdName);

		void* pEngineConsoleCommandHandler = pProc->AllocateAndWriteString("EngineConsoleCommandHandler");
		if (!pEngineConsoleCommandHandler) {
			printf("Error writing \"EngineConsoleCommandHandler\"!!\n");
			return;
		}
		printf("EngineConsoleCommandHandler Str: 0x%p\n", pEngineConsoleCommandHandler);


		RegisterCustomCommandCtx customCmdCtx{};
		customCmdCtx.pRegistry = pRegistry;
		customCmdCtx.szCmd = reinterpret_cast<const char*>(pCmdName);
		customCmdCtx.szConsoleCommandHandler = reinterpret_cast<const char*>(pEngineConsoleCommandHandler);
		customCmdCtx.fnToExecute = reinterpret_cast<RegisterCmdDef>(pExeMod->GetAddr() + 0x386CDE0);

		auto pDescCtxRemote = pProc->Alloc(sizeof(RegisterCustomCommandHandlerDescriptorCtx));
		RegisterCustomCommandHandlerDescriptorCtx descriptorCtx{};
		descriptorCtx.field0 = 0x0;
		descriptorCtx.callback = reinterpret_cast<void(*)()>(pExeMod->GetAddr() + 0x389EF80);
		descriptorCtx.field10 = 0;
		descriptorCtx.field18 = 0;

		if (!pDescCtxRemote) {
			printf("Error Allocating pCustomCmdCtx!!\n");
			return;
		}

		if (!pProc->Write<RegisterCustomCommandHandlerDescriptorCtx>(reinterpret_cast<uintptr_t>(pDescCtxRemote), descriptorCtx)) {
			printf("Failed to write pDescCtxRemote!!\n");
			pProc->FreeRemote(pDescCtxRemote);
			pDescCtxRemote = nullptr;
			return;
		}

		

		customCmdCtx.pDescHandler = reinterpret_cast<RegisterCustomCommandHandlerDescriptorCtx*>(pDescCtxRemote);


		auto pCustomCmdCtx = reinterpret_cast<RegisterCustomCommandCtx*>(pProc->Alloc(sizeof(customCmdCtx)));

		if (!pCustomCmdCtx) {
			printf("Error Allocating pCustomCmdCtx!!\n");
			return;
		}

		if (!pProc->Write<RegisterCustomCommandCtx>(reinterpret_cast<uintptr_t>(pCustomCmdCtx), customCmdCtx)) {
			printf("Failed to write pCustomCmdCtx!!\n");
			pProc->FreeRemote(pCustomCmdCtx);
			pCustomCmdCtx = nullptr;
			pProc->FreeRemote(pDescCtxRemote);
			pDescCtxRemote = nullptr;
			return;
		}

		
		auto pCustomCmdRegShellcode = pProc->AllocAndWriteShellcode(RegisterCmdThread, RegisterCmdThreadEnd);
		if (!pCustomCmdRegShellcode) {
			printf("Error Allocating pCustomCmdRegShellcode!!\n");
			return;
		}
		printf("pCustomCmdRegShellcode: 0x%p\n", pCustomCmdRegShellcode);
	
		HANDLE hThread = pProc->CreateRemoteThreadEx(
			reinterpret_cast<LPTHREAD_START_ROUTINE>(pCustomCmdRegShellcode),
			pCustomCmdCtx
		);

		if (!hThread) {
			printf("Failed to create SetConsoleStateThread!!\n");
			pProc->FreeRemote(pCustomCmdRegShellcode);
			pCustomCmdRegShellcode = nullptr;
			pProc->FreeRemote(pCustomCmdCtx);
			pCustomCmdCtx = nullptr;
			pProc->FreeRemote(pDescCtxRemote);
			pDescCtxRemote = nullptr;
			return;
		}

		DWORD waitResult = WaitForSingleObject(hThread, 10000);

		if (waitResult != WAIT_OBJECT_0) {
			printf("Thread wait failed or timed out! Result: %d\n", waitResult);
			CloseHandle(hThread);
			pProc->FreeRemote(pCustomCmdRegShellcode);
			pCustomCmdRegShellcode = nullptr;
			pProc->FreeRemote(pCustomCmdCtx);
			pCustomCmdCtx = nullptr;
			pProc->FreeRemote(pDescCtxRemote);
			pDescCtxRemote = nullptr;

			return;
		}

		DWORD exitCode = 0;
		GetExitCodeThread(hThread, &exitCode);
		CloseHandle(hThread);
	}
}