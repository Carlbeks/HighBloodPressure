//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <map>

using wchar = wchar_t;
using QWORD = unsigned long long int;
using String = std::wstring;
template<typename K, typename V, typename Cmp = std::less<K>, typename Alloc = std::allocator<std::pair<const K, V>>> using Map = std::map<K, V, Cmp, Alloc>;
template<typename T, typename Alloc = std::allocator<T>> using List = std::list<T, Alloc>;
template<typename F> using Function = std::function<F>;

#define Success() { return 0; }
#define Failed() { return 1; }
#define Error() { return -1; }

#if false
#ifdef _MSC_VER
#define __FUNCSIGW__ L"" __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
#define __FUNCSIGW__ L"" __PRETTY_FUNCTION__
#else
#define __FUNCSIGW__ L"" __func__
#endif
#endif

#define _WINSOCKAPI_ /* 防止winsock.h被引入。winsock.h和winsock2.h冲突。 */
#if false
#include <WinSock2.h>
#endif

#define NOMINMAX
#include <Windows.h>
#include <Windowsx.h>
#include <minwindef.h>
#include <windef.h>
#include <wingdi.h>
#include <WinUser.h>
#include <Uxtheme.h>
#include <iostream>
#include <dwmapi.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "Uxtheme.lib")
#pragma comment(lib, "winmm.lib")
