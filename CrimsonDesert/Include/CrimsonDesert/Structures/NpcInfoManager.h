/*
#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class NpcInfoManager: public BaseManager {
	public:
		inline static NpcInfoManager* GetInstance() {
			return pProc->ReadDirect<NpcInfoManager*>(pExeMod->GetAddr() + 0x0000000007254300);
		}
	};
}*/