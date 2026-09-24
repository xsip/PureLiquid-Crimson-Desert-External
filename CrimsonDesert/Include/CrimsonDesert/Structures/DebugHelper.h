#pragma once
#include <cstddef>
#include <LiquidHookEx/Include.h>
namespace CrimsonDesert {
	class DebugHelper {
	public:
		typedef int(__cdecl* PrintfDef)(const char*, ...);
		struct RegisterConsoleCommandsHookData : public LiquidHookEx::Detour::BaseHookData {
			__int64 a1Cpy;
			__int64 a2Cpy;
		};
		inline static LiquidHookEx::Detour m_Hook = LiquidHookEx::Detour("RegisterConsoleCommandsHook");

		static __int64 __fastcall hkRegisterCommands(__int64 a1, __int64 a2);
		static void hkRegisterCommandsEnd();

		struct RegisterCustomCommandHandlerDescriptorCtx {
			void* field0;          // 0x00
			void (*callback)();    // 0x08
			uint64_t field10;      // 0x10, unknown
			uint8_t field18;       // 0x18, written as 0
			char pad[0x6];
			// padding              0x19–0x1F
		};
		static_assert(sizeof(RegisterCustomCommandHandlerDescriptorCtx) == 0x20);
		typedef int(__cdecl* RegisterCmdDef)(void* registry,
			const char* command,
			const char* handlerName,
			RegisterCustomCommandHandlerDescriptorCtx* descriptor);

		struct RegisterCustomCommandCtx {
			RegisterCmdDef fnToExecute;
			void* pRegistry; // [[[CrimsonDesert.exe+6B53230]] + 0x28]
			const char* szCmd;
			const char* szConsoleCommandHandler;
			RegisterCustomCommandHandlerDescriptorCtx* pDescHandler;
		};


		static void RegisterTestCommand();

		static bool ApplyHooks();
	};
}