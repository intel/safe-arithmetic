#pragma once


#include <safe/int.hpp>


namespace safe {
    template<typename BeginT, typename EndT>
    struct irange {
    private:
        BeginT begin_;
        EndT end_;

        template<typename T>
        struct iterator {
            irange const * parent_;
            T value_;
            bool end_;

            using ret_t = decltype(clamp(0, parent_->begin_, parent_->end_ - s32_<1>));

            constexpr iterator(irange const * parent, T value, bool end)
                : parent_{parent}
                , value_{value}
                , end_{end}
            {}

            constexpr ret_t operator*() const {
                return clamp(value_, parent_->begin_, parent_->end_ - s32_<1>);
            }

            constexpr auto operator++() {
                auto const old = *this;
                auto const new_unsafe_value = value_ + 1;

                if (new_unsafe_value >= parent_->end_.unsafe_value()) {
                    end_ = true;
                } else {
                    auto new_value = clamp(new_unsafe_value, parent_->begin_, parent_->end_ - s32_<1>);
                    value_ = new_value.unsafe_value();
                }

                return old;
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
            return iterator<decltype(begin_.unsafe_value())>{this, begin_.unsafe_value(), false};
        }

        constexpr auto end() const {
            return iterator<decltype(begin_.unsafe_value())>{this, end_.unsafe_value(), true};
        }
    };
}