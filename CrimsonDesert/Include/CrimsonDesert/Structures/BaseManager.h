#pragma once
#include <GlobalData/Include.h>
#include <Math/Vector.h>
#include <string_view>
#include <CrimsonDesert/Structures/ManagerLoopDumper.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {

	class MetaData {
	public:
		inline std::string GetName() {
			return pProc->ReadString(pProc->ReadDirect<uintptr_t>(pProc->ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this))));
		}
		inline std::string GetLocation() {
			return pProc->ReadString(pProc->ReadDirect<uintptr_t>(pProc->ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this))) + 0x19);
		}
	};

	class ManagerListEntry {
	private:
		char pad[0x18];
	public:
		PROPERTY(m_iHash, int, 0x0);
		PROPERTY(m_iUnknown, int, 0x4);
		PROPERTY(m_pMetaData, MetaData*, 0x8);
		PROPERTY(m_iUnknown2, int, 0x10);
		PROPERTY(m_iUnknown3, int, 0x14);
	};

	class BaseManager {
	public:
		PROPERTY(m_iCount, int, 0x28);
		ManagerListEntry* GetEntry(int i);
	protected:

		template <typename T>
		inline static T *GetInstanceFor() {
			std::string name = typeid(T).name();

			auto pos = name.rfind("::");
			if (pos != std::string::npos)
				name = name.substr(pos + 2);

			uintptr_t pOffset = ManagerLoopDumper::GetOffsetFor(name);
			if (!pOffset) {
				printf("Error Collecting offset for \"%s\"!", name.c_str());
				return nullptr;
			}

			return pProc->ReadDirect<T*>(pExeMod->GetAddr() + pOffset);
		}
	};
}