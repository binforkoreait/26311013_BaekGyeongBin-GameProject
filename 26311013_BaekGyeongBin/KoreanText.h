#pragma once

#include <string>
#include <windows.h>

inline std::string ToLocalText(const std::string &utf8)
{
    if (utf8.empty())
    {
        return {};
    }
    int wideLength = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(),
                                         static_cast<int>(utf8.size()), nullptr, 0);
    if (wideLength == 0)
    {
        return utf8;
    }
    std::wstring wide(wideLength, L'\0');
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), static_cast<int>(utf8.size()),
                        &wide[0], wideLength);

    int localLength =
        WideCharToMultiByte(CP_ACP, 0, wide.data(), wideLength, nullptr, 0, nullptr, nullptr);
    if (localLength == 0)
    {
        return utf8;
    }
    std::string local(localLength, '\0');
    WideCharToMultiByte(CP_ACP, 0, wide.data(), wideLength, &local[0], localLength, nullptr,
                        nullptr);
    return local;
}
