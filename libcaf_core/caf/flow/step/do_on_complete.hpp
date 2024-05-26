// This file is part of CAF, the C++ Actor Framework. See the file LICENSE in
// the main distribution directory for license terms and copyright or visit
// https://github.com/actor-framework/actor-framework/blob/main/LICENSE.

#pragma once

#include "caf/detail/type_traits.hpp"
#include "caf/fwd.hpp"

#include <utility>

namespace caf::flow::step {

template <class T, class F>
class do_on_complete {
public:
  using input_type = T;

  using output_type = T;

  explicit do_on_complete(F fn) : fn_(std::move(fn)) {
    // nop
  }

  do_on_complete(do_on_complete&&) = default;
  do_on_complete(const do_on_complete&) = default;
  do_on_complete& operator=(do_on_complete&&) = default;
  do_on_complete& operator=(const do_on_complete&) = default;

  template <class Next, class... Steps>
  bool on_next(const T& item, Next& next, Steps&... steps) {
    return next.on_next(item, steps...);
  }

  template <class Next, class... Steps>
  void on_complete(Next& next, Steps&... steps) {
    fn_();
    next.on_complete(steps...);
  }

  template <class Next, class... Steps>
  void on_error(const error& what, Next& next, Steps&... steps) {
    next.on_error(what, steps...);
  }

private:
  F fn_;
};

} // namespace caf::flow::step
