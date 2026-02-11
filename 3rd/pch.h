#pragma once

// Include standard library headers
#include <memory>               // IWYU pragma: keep
#include <limits>               // IWYU pragma: keep
#include <cmath>                // IWYU pragma: keep
#include <iostream>             // IWYU pragma: keep

using std::make_shared;
using std::shared_ptr;

// Include all third-party library headers here
#include "spdlog/spdlog.h"              // IWYU pragma: keep
#include "spdlog/common.h"              // IWYU pragma: keep
#include "CLI11.hpp"                    // IWYU pragma: keep
#include "indicators/progress_bar.hpp"  // IWYU pragma: keep