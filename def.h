//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include <iostream>
#include <string>
#include <map>

using String = std::wstring;
template<typename K, typename V, typename Cmp = std::less<K>, typename Alloc = std::allocator<std::pair<const K, V>>> using Map = std::map<K, V, Cmp, Alloc>;

#if false
#ifdef _MSC_VER
#define __FUNCSIGW__ L"" __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
#define __FUNCSIGW__ L"" __PRETTY_FUNCTION__
#else
#define __FUNCSIGW__ L"" __func__
#endif
#endif

#if false
#define _WINSOCKAPI_ /* 防止winsock.h被引入。winsock.h和winsock2.h冲突。 */
#include <WinSock2.h>
#endif

#define NOMINMAX
#include <Windows.h>
