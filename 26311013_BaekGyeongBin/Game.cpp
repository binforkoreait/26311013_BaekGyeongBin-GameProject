// link the 2d game library
#if defined(_DEBUG)

#if defined(_M_X64)
#pragma comment(lib, "glc2d_x64_debug.lib")
#elif defined(_M_IX86)
#pragma comment(lib, "glc2d_win32_debug.lib")
#endif

#else

#if defined(_M_X64)
#pragma comment(lib, "glc2d_x64_release.lib")
#elif defined(_M_IX86)
#pragma comment(lib, "glc2d_win32_release.lib")
#endif

#endif

#include "glc2d.h"
#include "CApplication.h"

CApplication g_app;

int main()
{
    // 실행 위치와 관계없이 리소스를 찾도록 실행 파일 폴더로 이동한다.
    wchar_t executable[MAX_PATH]{};
    GetModuleFileNameW(nullptr, executable, MAX_PATH);
    wchar_t *slash = wcsrchr(executable, L'\\');
    if (slash)
    {
        *slash = L'\0';
        SetCurrentDirectoryW(executable);
    }
    if (g_app.Init() != 0)
    {
        g_app.Destroy();
        return 1;
    }

    g2_Run();

    g_app.Destroy();

    return 0;
}
