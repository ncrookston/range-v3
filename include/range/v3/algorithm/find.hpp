/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef RANGES_V3_ALGORITHM_FIND_HPP
#define RANGES_V3_ALGORITHM_FIND_HPP

#include <utility>
#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-algorithms
        /// @{
        struct find_fn
        {
            /// \brief template function \c find_fn::operator()
            ///
            /// range-based version of the \c find std algorithm
            ///
            /// \pre `Rng` is a model of the `Iterable` concept
            /// \pre `I` is a model of the `InputIterator` concept
            /// \pre `S` is a model of the `Sentinel<I>` concept
            /// \pre `P` is a model of the `Callable<iterator_common_reference_t<I>>` concept
            /// \pre The ResultType of `P` is EqualityComparable with V
            template<typename I, typename S, typename V, typename P = ident,
                CONCEPT_REQUIRES_(InputIterator<I>() && IteratorRange<I, S>() &&
                    IndirectCallableRelation<equal_to, Project<I, P>, V const *>())>
            I operator()(I begin, S end, V const &val, P proj_ = P{}) const
            {
                auto &&proj = as_function(proj_);
                for(; begin != end; ++begin)
                    if(proj(*begin) == val)
                        break;
                return begin;
            }

            /// \overload
            template<typename Rng, typename V, typename P = ident,
                typename I = range_iterator_t<Rng>,
                CONCEPT_REQUIRES_(InputIterable<Rng>() &&
                    IndirectCallableRelation<equal_to, Project<I, P>, V const *>())>
            range_safe_iterator_t<Rng> operator()(Rng &&rng, V const &val, P proj = P{}) const
            {
                return (*this)(begin(rng), end(rng), val, std::move(proj));
            }
        };

        /// \sa `find_fn`
        /// \ingroup group-algorithms
        namespace
        {
            constexpr auto&& find = static_const<with_braced_init_args<find_fn>>::value;
        }

        /// @}
    } // namespace v3
} // namespace ranges

#endif // include guard
