// This file is part of CAF, the C++ Actor Framework. See the file LICENSE in
// the main distribution directory for license terms and copyright or visit
// https://github.com/actor-framework/actor-framework/blob/main/LICENSE.

#pragma once

#include "caf/detail/type_traits.hpp"
#include "caf/fwd.hpp"

#include <utility>

namespace caf::flow::step {

template <class Predicate>
class take_while {
public:
  using trait = detail::get_callable_trait_t<Predicate>;

  static_assert(std::is_convertible_v<typename trait::result_type, bool>,
                "predicates must return a boolean value");

  static_assert(trait::num_args == 1,
                "predicates must take exactly one argument");

  using input_type = std::decay_t<detail::tl_head_t<typename trait::arg_types>>;

  using output_type = input_type;

  explicit take_while(Predicate fn) : fn_(std::move(fn)) {
    // nop
  }

  take_while(take_while&&) = default;
  take_while(const take_while&) = default;
  take_while& operator=(take_while&&) = default;
  take_while& operator=(const take_while&) = default;

  template <class Next, class... Steps>
  bool on_next(const input_type& item, Next& next, Steps&... steps) {
    if (fn_(item)) {
      return next.on_next(item, steps...);
    } else {
      next.on_complete(steps...);
      return false;
    }
  }

  template <class Next, class... Steps>
  void on_complete(Next& next, Steps&... steps) {
    next.on_complete(steps...);
  }

  template <class Next, class... Steps>
  void on_error(const error& what, Next& next, Steps&... steps) {
    next.on_error(what, steps...);
  }

private:
  Predicate fn_;
};

} // namespace caf::flow::step
