#ifndef WISH_COMMON_H
#define WISH_COMMON_H

#if defined (__LINUX__) 
#define _LINUX
#endif

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <cstdarg>
#include "time.h"
#include <string.h>
#include <atomic>
#include <sstream>
#include <mutex>
#include <chrono>
#include <csignal>
#include <signal.h>
#include <tuple>
#include <array>
#include <type_traits>
#include <optional>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <assert.h>
#include <functional>
#define DECLARE_PTR(X) using X_ptr = std::shared_ptr<X>

#endif