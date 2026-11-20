#include <CrimsonDesert/Structures/ManagerLoopDumper.h>
#include <Windows.h>
#include <DbgHelp.h>
#include <cstdint>
#include <string>
#include <string_view>
#pragma comment(lib, "Dbghelp.lib")
#include <GlobalData/Include.h>
using namespace Globals;

#include <fstream>
#include <iomanip>
#include <string>
#include <unordered_set>
namespace CrimsonDesert {
    struct MSVC_RTTICompleteObjectLocator
    {
        uint32_t signature;         // 0x00
        uint32_t offset;            // 0x04
        uint32_t cdOffset;          // 0x08
        uint32_t pTypeDescriptor;   // 0x0C
        uint32_t pClassDescriptor;  // 0x10
        uint32_t pSelf;             // 0x14
    };

    std::string CustomDemangle(std::string_view name)
    {
        constexpr std::string_view prefix = ".?AV";
        constexpr std::string_view suffix = "@pa@@";

        if (!name.starts_with(prefix) || !name.ends_with(suffix))
            return std::string(name);

        auto className = name.substr(
            prefix.size(),
            name.size() - prefix.size() - suffix.size()
        );

        return std::string(className);
    }

    void ManagerLoopDumper::CollectOffsets()
    {
        if (bOffsetsCollected)
            return;

        ForEachManager([](
            const std::string& className,
            uintptr_t pOffset,
            uintptr_t functionLocation)
            {
                ManagerLoopDumper::managerOffsetMap.insert({ className, pOffset });
            });

        bOffsetsCollected = true;
    }

    void ManagerLoopDumper::ForEachManager(
        const std::function<void(
            const std::string&,
            uintptr_t,
            uintptr_t
            )>& callback
    )
    {
        auto mgrFns = pExeMod->ScanMemoryMulti(
            "48 83 EC ? 44 0F B7 01 48 8B 0D ? ? ? ? 44 3B 41 ? 72 ? 48 8B 05 ? ? ? ? 48 83 C4 ? C3"
        );

        uintptr_t pBaseAddr = pExeMod->GetAddr();

        for (const auto& mgrFn : mgrFns)
        {
            auto pOffset = pExeMod->ResolveRIP(
                mgrFn + 0x8,
                3,
                7
            ) - pBaseAddr;

            auto pInstAddr = pProc->ReadDirect<uintptr_t>(
                pOffset + pBaseAddr
            );

            if (!pInstAddr)
                continue;

            uintptr_t vtable = pProc->ReadDirect<uintptr_t>(pInstAddr);

            if (!vtable)
                continue;

            uintptr_t colAddress = pProc->ReadDirect<uintptr_t>(vtable - 0x8);

            if (!colAddress)
                continue;

            auto col = pProc->ReadDirect<MSVC_RTTICompleteObjectLocator>(
                colAddress
            );

            if (col.signature != 1 || !col.pTypeDescriptor || !col.pSelf)
                continue;

            uintptr_t imageBase = colAddress - col.pSelf;
            uintptr_t typeDescriptor = imageBase + col.pTypeDescriptor;

            char name[256]{};

            for (size_t i = 0; i < sizeof(name) - 1; ++i)
            {
                name[i] = pProc->ReadDirect<char>(
                    typeDescriptor + 0x10 + i
                );

                if (!name[i])
                    break;
            }

            if (!name[0])
                continue;

            callback(
                CustomDemangle(name),
                pOffset,
                reinterpret_cast<uintptr_t>(mgrFn) - pBaseAddr
            );
        }
    }
    void ManagerLoopDumper::Dump() {
        ForEachManager([](const std::string& className, uintptr_t pOffset, uintptr_t functionLocation)
            {
                printf(
                    "Function: CrimsonDesert.exe + 0x%llX = %s loop (ctx: CrimsonDesert.exe + 0x%llX)\n",
                    functionLocation,
                    className.c_str(),
                    pOffset
                );
            });
    }

    void ManagerLoopDumper::GenerateFunctionRenameList()
    {
        std::string outputDir = SOLUTION_DIR;

        std::ofstream mgrFnNameListFile(
            outputDir + "manager_function_names.txt"
        );

        if (!mgrFnNameListFile.is_open())
        {
            printf("Failed to create generated headers!\n");
            return;
        }

        ForEachManager([&mgrFnNameListFile](
            const std::string& className,
            uintptr_t pOffset,
            uintptr_t functionLocation)
            {
                mgrFnNameListFile
                    << "0x" << std::hex << std::uppercase << functionLocation << " "
                    << className << "_GetEntry_"
                    << std::dec << "\n";
            });

        mgrFnNameListFile.close();
    }



    void ManagerLoopDumper::Generate(bool bStatic)
    {


        std::string outputDir = GENERATED_MANAGERS_DIR;

        // Generate shared offsets header
        std::ofstream offsetsFile(outputDir + "Offsets.h");

        // Generate shared manager includes header
        std::ofstream includesFile(outputDir + "Include.h");

        if (!offsetsFile.is_open() || !includesFile.is_open())
        {
            printf("Failed to create generated headers!\n");
            return;
        }

        offsetsFile << "#pragma once\n\n";

        includesFile << "#pragma once\n\n";

        std::unordered_set<std::string> generatedManagers;



        ForEachManager([&offsetsFile,&outputDir,&generatedManagers, &includesFile, &bStatic](
            const std::string& className,
            uintptr_t pOffset,
            uintptr_t functionLocation)
            {

                if (!generatedManagers.insert(className).second)
                    return;

                // Generate manager header
                std::string headerPath = outputDir + className + ".h";

                std::ofstream headerFile(headerPath);

                if (!headerFile.is_open())
                {
                    printf("Failed to create %s!\n", headerPath.c_str());
                    generatedManagers.erase(className);
                    return;
                }

                headerFile
                    << "#pragma once\n"
                    << "#include <GlobalData/Include.h>\n"
                    << "#include <CrimsonDesert/Structures/BaseManager.h>\n";
                if (bStatic)
                    headerFile << "#include <CrimsonDesert/Managers/Offsets.h>\n";
                headerFile
                    << "#include <Math/Vector.h>\n"
                    << "\n"
                    << "#include <cstdint>\n"
                    << "#include <cstddef>\n"
                    << "\n"
                    << "using namespace Globals;\n"
                    << "\n"
                    << "namespace CrimsonDesert {\n"
                    << "\tclass " << className << " : public BaseManager {\n"
                    << "\tpublic:\n"
                    << "\t\tinline static " << className << "* GetInstance() {\n";
                if (bStatic) {
                    headerFile
                        << "\t\t\treturn pProc->ReadDirect<" << className
                        << "*>(pExeMod->GetAddr() + " << className << "_offset);\n";
                }
                else {
                    headerFile
                        << "\t\t\treturn BaseManager::GetInstanceFor<"<<className << ">();\n";

                }
                headerFile
                << "\t\t}\n"
                    << "\t};\n"
                    << "}\n";
                headerFile.close();

                // Generate offset definition
                offsetsFile
                    << "#define " << className << "_offset 0x"
                    << std::hex << std::uppercase << pOffset
                    << std::dec << "\n";

                // Generate include entry
                includesFile
                    << "#include <CrimsonDesert/Managers/"
                    << className << ".h>\n";

                printf(
                    "Generated: %s (offset: 0x%llX)\n",
                    className.c_str(),
                    static_cast<unsigned long long>(pOffset)
                );
            });


        offsetsFile.close();
        includesFile.close();

        printf(
            "Generated %zu manager headers.\n",
            generatedManagers.size()
        );
    }

    uintptr_t ManagerLoopDumper::GetOffsetFor(std::string managerName)
    {

        if (!bOffsetsCollected)
            CollectOffsets();

        return managerOffsetMap.contains(managerName) ? managerOffsetMap.at(managerName) : uintptr_t();
    }



}