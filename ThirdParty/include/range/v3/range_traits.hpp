/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef RANGES_V3_RANGE_TRAITS_HPP
#define RANGES_V3_RANGE_TRAITS_HPP

#include <utility>
#include <iterator>
#include <type_traits>
#include <meta/meta.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-core
        /// @{

        // Aliases (SFINAE-able)
        template<typename Rng>
        using range_iterator_t = concepts::Range::iterator_t<Rng>;

        template<typename Rng>
        using range_sentinel_t = concepts::Range::sentinel_t<Rng>;

        template<typename Rng>
        using range_difference_t = concepts::Range::difference_t<Rng>;

        template<typename Rng>
        using range_size_t = meta::_t<std::make_unsigned<range_difference_t<Rng>>>;

        template<typename Rng>
        using range_value_t = concepts::InputRange::value_t<Rng>;

        template<typename Rng>
        using range_reference_t = concepts::InputRange::reference_t<Rng>;

        template<typename Rng>
        using range_rvalue_reference_t = concepts::InputRange::rvalue_reference_t<Rng>;

        template<typename Rng>
        using range_common_reference_t = concepts::InputRange::common_reference_t<Rng>;

        template<typename Rng>
        using range_category_t = concepts::InputRange::category_t<Rng>;

        template<typename Rng>
        using range_common_iterator_t = common_iterator<range_iterator_t<Rng>, range_sentinel_t<Rng>>;

#ifdef RANGES_WORKAROUND_MSVC_SFINAE_ALIAS_DECLTYPE
        template <typename T>
        using range_safe_iterator_t_helper_void_t = void;
        template <class T, class V = void> struct range_safe_iterator_t_helper {};
        template <class T> struct range_safe_iterator_t_helper<T, range_safe_iterator_t_helper_void_t<decltype(ranges::safe_begin(std::declval<T>()))>> {
            typedef decltype(ranges::safe_begin(std::declval<T>())) type;
        };
        template<typename T>
        using range_safe_iterator_t = typename range_safe_iterator_t_helper<T>::type;

        template <typename T>
        using range_safe_sentinel_t_helper_void_t = void;
        template <class T, class V = void> struct range_safe_sentinel_t_helper {};
        template <class T> struct range_safe_sentinel_t_helper<T, range_safe_sentinel_t_helper_void_t<decltype(ranges::safe_end(std::declval<T>()))>> {
            typedef decltype(ranges::safe_end(std::declval<T>())) type;
        };
        template<typename T>
        using range_safe_sentinel_t = typename range_safe_sentinel_t_helper<T>::type;
#else
        template<typename Rng>
        using range_safe_iterator_t = decltype(ranges::safe_begin(std::declval<Rng>()));

        template<typename Rng>
        using range_safe_sentinel_t = decltype(ranges::safe_end(std::declval<Rng>()));
#endif

        // Metafunctions
        template<typename Rng>
        using range_iterator = meta::defer<range_iterator_t, Rng>;

        template<typename Rng>
        using range_sentinel = meta::defer<range_sentinel_t, Rng>;

        template<typename Rng>
        using range_category = meta::defer<range_category_t, Rng>;

        template<typename Rng>
        using range_value = meta::defer<range_value_t, Rng>;

        template<typename Rng>
        using range_difference = meta::defer<range_difference_t, Rng>;

        template<typename Rng>
        using range_reference = meta::defer<range_reference_t, Rng>;

        template<typename Rng>
        using range_rvalue_reference = meta::defer<range_rvalue_reference_t, Rng>;

        template<typename Rng>
        using range_common_reference = meta::defer<range_common_reference_t, Rng>;

        template<typename Rng>
        using range_size = meta::defer<range_size_t, Rng>;

        /// \cond
        namespace detail
        {
            std::integral_constant<cardinality, finite> test_cardinality(void *);
            template<cardinality Card>
            std::integral_constant<cardinality, Card> test_cardinality(basic_view<Card> *);
        }
        /// \endcond

        // User customization point for specifying the cardinality of ranges:
        template<typename Rng, typename Void /*= void*/>
        struct range_cardinality
          : decltype(detail::test_cardinality(static_cast<Rng *>(nullptr)))
        {};

        template<typename Rng>
        struct range_cardinality<Rng &>
          : range_cardinality<Rng>
        {};

        template<typename Rng>
        struct range_cardinality<Rng const>
          : range_cardinality<Rng>
        {};

        /// @}
    }
}

#endif
