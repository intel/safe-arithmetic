#include <safe.hpp>

using namespace safe;
using namespace safe::literals;

static_assert((checked{5} + checked{10} == checked{15}).value());

static_assert((checked{1500000000} + checked{1500000000}).is_overflow());
static_assert((checked{1500000000} + checked{1500000000u} == checked{3000000000u}).value());
static_assert((checked{-1} + checked{0u} == checked{-1u}).value());
static_assert((checked{1} + checked{0u} == checked{1u}).value());

int main() {
    return 0;
}
