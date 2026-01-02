#pragma once
#include <array>
#include <concepts>
#include <sdk/mc/platform/brstd/detail/bitset_base.hpp>

namespace brstd {
    template<std::size_t Bits, std::integral UnderlyingType>
    constexpr std::size_t get_num_values() {
        static_assert(Bits > 0, "Bits must be greater than 0");
        constexpr std::size_t bits_per_type = sizeof(UnderlyingType) * 8;
        return (Bits + bits_per_type - 1) / bits_per_type;
    }

    template<std::size_t Bits, typename UnderlyingType>
    class bitset : public detail::bitset_base<bitset<Bits, UnderlyingType>, std::array<UnderlyingType, get_num_values<Bits, UnderlyingType>()>> {
        static constexpr size_t num_values = get_num_values<Bits, UnderlyingType>();
        using underlying_type = typename detail::bitset_base<bitset, std::array<UnderlyingType, num_values>>::underlying_type;
    };
}