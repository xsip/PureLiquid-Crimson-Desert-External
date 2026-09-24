#include <CrimsonDesert/Structures/ServerTransformSyncActorComponent.h>
#include <GlobalData/Include.h>
using namespace Globals;


/*
pa::ServerTransformSyncActorComponent => + 30 = WilLReceiveFallDamage

CrimsonDesert.exe+21A1DB5:
1421A1DAF - 48 8B FA  - mov rdi,rdx
1421A1DB2 - 48 8B F1  - mov rsi,rcx
1421A1DB5 - 48 8B 01  - mov rax,[rcx] << pa::ServerTransformSyncActorComponent
1421A1DB8 - 48 8D 54 24 40  - lea rdx,[rsp+40]
1421A1DBD - FF 90 40010000  - call qword ptr [rax+00000140]



CrimsonDesert.exe+21A01A5 - C6 00 01              - mov byte ptr [rax],01 { 1 } // moves into FallDamageEnabled
*/


namespace CrimsonDesert {

	static void* g_pOriginalUpdate = nullptr;
	static ServerTransformSyncActorComponent::UpdateHookData* g_pUpdateHookData = nullptr;

	LH_START(".hkUpdate")

		__int64 __fastcall ServerTransformSyncActorComponent::hkUpdate(ServerTransformSyncActorComponent::ServerTransformSyncActorComponentInst* a1, __int64 *a2)
	{

		volatile ServerTransformSyncActorComponent::UpdateHookData* data = g_pUpdateHookData;

		typedef __int64(__fastcall* UpdateFn)(ServerTransformSyncActorComponent::ServerTransformSyncActorComponentInst* a1, __int64* a2);
		
		volatile UpdateFn original = (UpdateFn)g_pOriginalUpdate;
		volatile __int64 res = original(a1, a2);
		
		if (!data->bFallDamageEnabled && a1->pIsFallingFlagContainer->bIsFalling)
			a1->pIsFallingFlagContainer->bIsFalling = data->bFallDamageEnabled;
		
		data->pInst = reinterpret_cast<CrimsonDesert::ServerTransformSyncActorComponent*>(a1);

		return res;

	}

	void ServerTransformSyncActorComponent::hkUpdateEnd() {}

	LH_END()

	ServerTransformSyncActorComponent* ServerTransformSyncActorComponent::GetInstance() {
		
		if (!m_Hook.IsHooked()) {
			printf("Cant retrieve ServerTransformActorComponent Pointer! ( ::Update not hooked. )\n");
			return nullptr;
		}

		if (!pInstAddr) {
			pInstAddr = m_Hook.ReadData<UpdateHookData>().pInst;
		}

		return pInstAddr;

	}


	bool ServerTransformSyncActorComponent::Unhook() {
		return m_Hook.Unhook();
	}

	bool ServerTransformSyncActorComponent::HookUpdate() {
#define UPDATE_ACTOR_FN_PATTERN "48 8B C4 48 89 58 ?? 55 56 57 41 54 41 55 41 56 41 57 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? C5 F8 29 70 ?? C5 F8 29 78 ?? C5 78 29 40 ?? 4C 8B EA 4C 8B F9 4C 8B 49"

		UpdateHookData initData{};
		initData.pInst = nullptr;
		initData.bFallDamageEnabled = true;

		return m_Hook.Hook<UpdateHookData>(
			UPDATE_ACTOR_FN_PATTERN,
			"CrimsonDesert.exe",
			initData,
			reinterpret_cast<void*>(hkUpdate),
			reinterpret_cast<void*>(hkUpdateEnd),
			{
				LiquidHookEx::Detour::RipSlot::Data(&g_pUpdateHookData),
				LiquidHookEx::Detour::RipSlot::Orig(&g_pOriginalUpdate),
			},
			17
			);
	}

// #define FALL_DAMAGE_ENABLED_INSTRUCTION_PATTERN "C6 00 ?? 4C 8B 65"

	void ServerTransformSyncActorComponent::SetFallDamageEnabled(bool bEnabled) {
		if (!m_Hook.IsHooked()) {
			return;
		}

		m_Hook.WriteField<bool>(offsetof(ServerTransformSyncActorComponent::UpdateHookData, bFallDamageEnabled), bEnabled);
	}

	void ServerTransformSyncActorComponent::DisableFallDamage() {
		SetFallDamageEnabled(false);
	}
	void ServerTransformSyncActorComponent::EnableFallDamage() {
		SetFallDamageEnabled(true);
	}
	bool ServerTransformSyncActorComponent::IsFalldamageEnabled() {
		if (!m_Hook.IsHooked()) {
			return true;
		}
		return m_Hook.ReadData<UpdateHookData>().bFallDamageEnabled;
	}
}