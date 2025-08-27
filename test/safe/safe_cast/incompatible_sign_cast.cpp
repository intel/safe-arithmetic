#include <cstdint>

#include <safe.hpp>

using namespace safe::interval_types;
using namespace safe::int_types;
using namespace safe::literals;

auto main() -> int { auto v = safe_cast<std::uint32_t>(-99_s32); }
