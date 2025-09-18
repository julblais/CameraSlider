#ifndef ENUM_WRAPPER_H
#define ENUM_WRAPPER_H

#include "enumUtils.h"

namespace Core
{
    template <typename E, IsTypeEnum<E> = true>
    struct EnumWrapper
    {
        constexpr EnumWrapper() noexcept
            : m_Value(0) {}

        constexpr EnumWrapper(E value) noexcept
            : m_Value(value) {}

        constexpr bool Has(EnumWrapper flag) const noexcept { return HasValue(m_Value, flag.m_Value); }
        constexpr bool Any() const noexcept { return m_Value != Default; }
        constexpr bool None() const noexcept { return m_Value == Default; }

        constexpr bool operator==(EnumWrapper rhs) const noexcept { return m_Value == rhs.m_Value; }
        constexpr bool operator!=(EnumWrapper rhs) const noexcept { return m_Value != rhs.m_Value; }

        constexpr EnumWrapper operator|(EnumWrapper rhs) const noexcept { return m_Value | rhs.m_Value; }
        constexpr EnumWrapper operator&(EnumWrapper rhs) const noexcept { return m_Value & rhs.m_Value; }
        constexpr EnumWrapper operator^(EnumWrapper rhs) const noexcept { return m_Value ^ rhs.m_Value; }
        constexpr EnumWrapper operator~() const noexcept { return ~m_Value; }

        EnumWrapper& operator|=(EnumWrapper rhs) noexcept
        {
            m_Value = m_Value | rhs.m_Value;
            return *this;
        }

        EnumWrapper& operator&=(EnumWrapper rhs) noexcept
        {
            m_Value = m_Value & rhs.m_Value;
            return *this;
        }

        EnumWrapper& operator^=(EnumWrapper rhs) noexcept
        {
            m_Value = m_Value ^ rhs.m_Value;
            return *this;
        }

    private:
        static constexpr E Default{};
        E m_Value;
    };
}

#endif
