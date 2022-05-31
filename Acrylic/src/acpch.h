#pragma once

#include "Acrylic/Core/PlatformDetection.h"

#ifdef AC_PLATFORM_WINDOWS
#ifndef NOINMAX
//github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOINMAX
#endif
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include "Acrylic/Core/Log.h"
#include "Acrylic/Debug/Instrumentor.h"

#ifdef AC_PLATFORM_WINDOWS
#include <Windows.h>
#endif
