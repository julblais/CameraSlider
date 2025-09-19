#ifndef ENUM_WRAPPER_H
#define ENUM_WRAPPER_H

#include "enumUtils.h"

namespace Core
{
    template <typename E, IsTypeEnum<E> = true>
    struct EnumFlagsWrapper
    {
        constexpr EnumFlagsWrapper(E value = E()) noexcept
            : m_Value(value) {}

        constexpr bool Has(EnumFlagsWrapper flag) const noexcept { return HasValue(m_Value, flag.m_Value); }
        constexpr bool Any() const noexcept { return m_Value != Default; }
        constexpr bool None() const noexcept { return m_Value == Default; }

        constexpr bool operator==(EnumFlagsWrapper rhs) const noexcept { return m_Value == rhs.m_Value; }
        constexpr bool operator!=(EnumFlagsWrapper rhs) const noexcept { return m_Value != rhs.m_Value; }

        constexpr EnumFlagsWrapper operator|(EnumFlagsWrapper rhs) const noexcept { return m_Value | rhs.m_Value; }
        constexpr EnumFlagsWrapper operator&(EnumFlagsWrapper rhs) const noexcept { return m_Value & rhs.m_Value; }
        constexpr EnumFlagsWrapper operator^(EnumFlagsWrapper rhs) const noexcept { return m_Value ^ rhs.m_Value; }
        constexpr EnumFlagsWrapper operator~() const noexcept { return ~m_Value; }

        EnumFlagsWrapper& operator|=(EnumFlagsWrapper rhs) noexcept
        {
            m_Value |= rhs.m_Value;
            return *this;
        }

        EnumFlagsWrapper& operator&=(EnumFlagsWrapper rhs) noexcept
        {
            m_Value &= rhs.m_Value;
            return *this;
        }

        EnumFlagsWrapper& operator^=(EnumFlagsWrapper rhs) noexcept
        {
            m_Value ^= rhs.m_Value;
            return *this;
        }

    private:
        static constexpr E Default{};
        E m_Value;
    };
}

#endif
