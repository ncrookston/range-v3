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

#ifndef RANGES_V3_ACTION_SLICE_HPP
#define RANGES_V3_ACTION_SLICE_HPP

#include <functional>
#include <meta/meta.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/action/action.hpp>
#include <range/v3/action/erase.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-actions
        /// @{
        namespace action
        {
            struct slice_fn
            {
            private:
                friend action_access;
                template<typename D, CONCEPT_REQUIRES_(Integral<D>())>
                static auto bind(slice_fn slice, D from, D to)
                RANGES_DECLTYPE_AUTO_RETURN
                (
                    std::bind(slice, std::placeholders::_1, from, to)
                )
            public:
                struct ConceptImpl
                {
                    template<typename Rng, typename T, typename U,
                        typename I = range_iterator_t<Rng>,
                        typename D = range_difference_t<Rng>>
                    auto requires_(Rng&&, T&&, U&&) -> decltype(
                        concepts::valid_expr(
                            concepts::model_of<concepts::ForwardIterable, Rng>(),
                            concepts::model_of<concepts::EraseableIterable, Rng, I, I>(),
                            concepts::model_of<concepts::Convertible, T, D>(),
                            concepts::model_of<concepts::Convertible, U, D>()
                        ));
                };

                template<typename Rng, typename T, typename U>
                using Concept = concepts::models<ConceptImpl, Rng, T, U>;

                // TODO support slice from end.
                template<typename Rng,
                    typename I = range_iterator_t<Rng>,
                    typename D = range_difference_t<Rng>,
                    CONCEPT_REQUIRES_(Concept<Rng, D, D>())>
                Rng operator()(Rng && rng, range_difference_t<Rng> from,
                    range_difference_t<Rng> to) const
                {
                    RANGES_ASSERT(from <= to);
                    ranges::action::erase(rng, next(begin(rng), to), end(rng));
                    ranges::action::erase(rng, begin(rng), next(begin(rng), from));
                    return std::forward<Rng>(rng);
                }

            #ifndef RANGES_DOXYGEN_INVOKED
                template<typename Rng, typename T, typename U,
                    CONCEPT_REQUIRES_(!Concept<Rng, T, U>())>
                void operator()(Rng &&, T &&, U &&) const
                {
                    CONCEPT_ASSERT_MSG(ForwardIterable<Rng>(),
                        "The object on which action::slice operates must be a model of the "
                        "ForwardIterable concept.");
                    using I = range_iterator_t<Rng>;
                    CONCEPT_ASSERT_MSG(EraseableIterable<Rng, I, I>(),
                        "The object on which action::slice operates must allow element "
                        "removal.");
                    CONCEPT_ASSERT_MSG(meta::and_<Convertible<T, range_difference_t<Rng>>,
                            Convertible<U, range_difference_t<Rng>>>(),
                        "The bounds passed to action::slice must be convertible to the range's "
                        "difference type. TODO slicing from the end with 'end-2' syntax is not "
                        "supported yet, sorry!");
                }
            #endif
            };

            /// \ingroup group-actions
            /// \relates slice_fn
            /// \sa action
            namespace
            {
                constexpr auto&& slice = static_const<action<slice_fn>>::value;
            }
        }
        /// @}
    }
}

#endif
