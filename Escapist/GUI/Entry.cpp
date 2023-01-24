#include"Application.h"

#ifdef ESCAPIST_OS_WINDOWS
#include<tchar.h>
#include<crtdbg.h>

extern int Main(int argc, Char** argv);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, char* cmdLine, int cmdShow) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    int argc = 0;
    wchar_t** origArgv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
#ifdef ESCAPIST_32BITS
    char** argv = new char* [argc];
    for (int i = 0; i < argc; ++i) {
        int length = ::WideCharToMultiByte(CP_ACP, 0, origArgv[i], -1, nullptr, 0, nullptr, nullptr);
        argv[i] = new char[length + 1];
        ::WideCharToMultiByte(CP_ACP, 0, origArgv[i], -1, argv[i], length, nullptr, nullptr);
        argv[i][length] = 0;
    }
    int returnCode = Main(argc, argv);
#else
    int returnCode = Main(argc, origArgv);
#endif
#ifdef ESCAPIST_32BITS
    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
#endif
    ::LocalFree(origArgv);
    return returnCode;
}
#endif