#pragma once
#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <chrono>
#include <cstdint>
#include <concepts>
#include <variant>

#include <libhat/fixed_string.hpp>
#include <core/Memory.hpp>

#define MCAPI __declspec(dllimport)
#define MCVIRTUAL __declspec(dllimport)
