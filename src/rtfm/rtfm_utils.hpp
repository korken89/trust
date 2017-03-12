

#pragma once

#include "kvasir/mpl/mpl.hpp"

namespace rtfm
{
namespace util
{
namespace details
{

/**
 * @brief Get the type of an mpl::integral_constant, base case
 *        that always will fail.
 *
 * @tparam Ts   Parameter pack.
 */
template <typename... Ts>
struct get_integral_type_impl
{
  static_assert(kvasir::mpl::always_false<Ts...>{},
                "The input is not an integral_constant.");
};

/**
 * @brief Get the type of an mpl::integral_constant, general case.
 *
 * @tparam T    Type of the integral_constant.
 * @tparam val  Value of the integral_constant.
 */
template <typename T, T val>
struct get_integral_type_impl<kvasir::mpl::integral_constant<T, val>>
{
  using f = T;
};

/**
 * @brief Get the type of an mpl::integral_constant, special case for nullptr.
 */
template <>
struct get_integral_type_impl<
    kvasir::mpl::integral_constant<decltype(nullptr), nullptr>>
{
  using f = decltype(nullptr);
};


/**
 * @brief Checks if an integral_constant contains a nullptr, base case that
 *        will always fail.
 *
 * @tparam Ts   Parameter pack.
 */
template <typename... Ts>
struct is_nullptr_impl
{
  static_assert(kvasir::mpl::always_false<Ts...>{},
                "The input is not an integral_constant.");
};

/**
 * @brief Checks if an integral_constant contains a nullptr, general case.
 *
 * @tparam T    Type of the integral_constant.
 * @tparam val  Value of the integral_constant.
 */
template <typename T, T val>
struct is_nullptr_impl<kvasir::mpl::integral_constant<T, val>>
{
  using f = kvasir::mpl::bool_<false>;
};

/**
 * @brief Checks if an integral_constant contains a nullptr, first special case.
 *
 * @tparam T    Type of the integral_constant.
 */
template <typename T>
struct is_nullptr_impl<kvasir::mpl::integral_constant<T, nullptr>>
{
  using f = kvasir::mpl::bool_<true>;
};

/**
 * @brief Checks if an integral_constant contains a nullptr, second special
 *        case.
 *
 * @tparam T    Type of the integral_constant.
 */
template <>
struct is_nullptr_impl<
    kvasir::mpl::integral_constant<decltype(nullptr), nullptr>>
{
  using f = kvasir::mpl::bool_<true>;
};
} /* END namespace details */

/**
 * @brief Get the type of an mpl::integral_constant.
 *
 * @tparam T  Integral_constant to get type from.
 */
template <typename T>
using get_integral_type = typename details::get_integral_type_impl<T>::f;

/**
 * @brief Checks if an integral_constant contains a nullptr.
 *
 * @tparam T    Integral_constant to check.
 */
template <typename T>
using is_nullptr = typename details::is_nullptr_impl<T>::f;

/**
 * @brief   Takes a user priority (0 = lowest, increasing numbers means higher
 *          priority) and transforms to NVIC priority (0 = highest, increasing
 *          numbers means lower priority).
 *
 * @return  The transformed priority.
 */
constexpr auto priority_to_NVIC_priority(unsigned priority)
{
  /* Max priority */
  auto N = (1U << __NVIC_PRIO_BITS) - 1U;
  return ((N - priority) << (8U - __NVIC_PRIO_BITS)) & 0xFFU;
}

/**
 * @brief  A convenience function for transforming strings to a hashed value.
 *
 * @param[in] txt   A string with '\0' termination.
 */
constexpr uint32_t hashit(const char *txt)
{
  uint32_t h = 0;
  while (*txt != '\0')
    h = h * 1572869 + *txt++;

  return h;
}

} /* END namespace util */
} /* END namespace rtfm */
