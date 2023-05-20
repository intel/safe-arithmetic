#pragma once


#include <safe/int.hpp>

// FIXME: make this work with big integer
namespace safe {
    template<typename BeginT, typename EndT>
    struct irange {
    private:
        BeginT begin_;
        EndT end_;

        template<typename T>
        struct iterator {
        private:
            irange const * parent_;
            T value_;
            bool end_;

        public:
            using ret_t = decltype(clamp(0, parent_->begin_, parent_->end_ - s32_<1>));

            constexpr iterator(irange const * parent, T value, bool end)
                : parent_{parent}
                , value_{value}
                , end_{end}
            {}

            constexpr ret_t operator*() const {
                return unsafe_cast<ret_t>(value_);
            }

            constexpr auto operator++() {
                auto const new_unsafe_value = value_++;

                // FIXME: consolidate range checks between == and ++
                if (new_unsafe_value < parent_->end_.unsafe_value()) {
                    value_ = new_unsafe_value;

                } else {
                    end_ = true;
                }

                return *this;
            }

            constexpr bool operator==(iterator rhs) {
                if (end_) {
                    return rhs.end_;

                } else {
                    return
                        parent_ == rhs.parent_ &&
                        value_ == rhs.value_ &&
                        !rhs.end_;
                }
            }
        };

    public:
        constexpr irange(
            BeginT begin,
            EndT end
        )
            : begin_{begin}
            , end_{end}
        {}

        constexpr auto begin() const {
            return iterator<decltype(begin_.unsafe_value())>{this, begin_.unsafe_value(), begin_ == end_};
        }

        constexpr auto end() const {
            // FIXME: need to find the right value for the end
            return iterator<decltype(begin_.unsafe_value())>{this, end_.unsafe_value() - 1, true};
        }
    };
}