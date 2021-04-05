#include "Exception.h"
#include "Memory.h"
#include "Log.h"
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <string>

#ifdef WIN32
#include "Windows.h"
#include "DbgHelp.h"
#include "Psapi.h"
#endif

namespace CException
{
    void Check(bool Exp)
    {
#ifdef WIN32
        if(!Exp)
        {
            PrintCallstack();
            DebugBreak();
        }
#endif
        assert(Exp);
    }

    void CheckFormatted(bool Exp, const char* Format, ...)
    {
        if (!Exp)
        {
            va_list Args;
            va_start(Args, Format);
            vprintf(Format, Args);
            va_end(Args);
#ifdef WIN32
            PrintCallstack();
            DebugBreak();
#endif
            assert(0);
        }
    }

    // https://www.rioki.org/2017/01/09/windows_stacktrace.html
    void PrintCallstack()
    {
#ifdef WIN32
        CLog::DebugLog("Callstack: \n");
        DWORD machine = IMAGE_FILE_MACHINE_AMD64;
        HANDLE process = GetCurrentProcess();
        HANDLE thread = GetCurrentThread();
        CONTEXT context = {};
        context.ContextFlags = CONTEXT_FULL;
        RtlCaptureContext(&context);
        SymInitialize(process, NULL, TRUE);
        SymSetOptions(SYMOPT_LOAD_LINES);
        STACKFRAME frame = {};
        frame.AddrPC.Offset = context.Rip;
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Offset = context.Rbp;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Offset = context.Rsp;
        frame.AddrStack.Mode = AddrModeFlat;
        while (StackWalk64(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
        {
            DWORD64 functionAddress;
            std::string moduleName;
            std::string functionName;
            std::string file;
            unsigned int linenum = 0;

            functionAddress = frame.AddrPC.Offset;

            auto moduleBase = SymGetModuleBase(process, frame.AddrPC.Offset);
            char moduleBuff[MAX_PATH];
            if (moduleBase && GetModuleFileName(reinterpret_cast<HMODULE>(moduleBase), moduleBuff, MAX_PATH))
            {
                moduleName = moduleBuff;
            }

            char symbolBuffer[sizeof(PIMAGEHLP_SYMBOL64) + 255];
            PIMAGEHLP_SYMBOL64 symbol = (PIMAGEHLP_SYMBOL64)symbolBuffer;
            symbol->SizeOfStruct = (sizeof PIMAGEHLP_SYMBOL64) + 255;
            symbol->MaxNameLength = 254;

            if (SymGetSymFromAddr(process, frame.AddrPC.Offset, NULL, symbol))
            {
                functionName = symbol->Name;
            }

            DWORD offset = 0;
            IMAGEHLP_LINE64 line;
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

            if (SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset, &line))
            {
                file = line.FileName;
                linenum = line.LineNumber;
            }
            
            CLog::DebugLog("\t%s,%u : %s()\n", file.c_str(), 
                linenum, functionName.c_str());
        }
        SymCleanup(process);
#endif
    }
} // namespace Exception