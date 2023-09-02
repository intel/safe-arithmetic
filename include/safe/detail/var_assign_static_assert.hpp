#pragma

namespace safe {
template <typename U> struct lhs_req {
    constexpr static U value{};
};

template <typename U> struct rhs_req {
    constexpr static U value{};
};

template <typename LhsT, typename RhsT> struct rhs_must_be_subset_of_lhs {
    constexpr static bool value = LhsT::value >= RhsT::value;
};

constexpr inline void static_assert_assign_requirements(auto lhs, auto rhs) {
    static_assert(
        rhs_must_be_subset_of_lhs<lhs_req<decltype(lhs.requirement)>,
                                  rhs_req<decltype(rhs.requirement)>>::value);
}
} // namespace safe