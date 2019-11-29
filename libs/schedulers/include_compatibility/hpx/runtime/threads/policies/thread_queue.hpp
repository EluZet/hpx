//  Copyright (c) 2019 Ste||ar Group
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/config.hpp>
#include <hpx/schedulers/config/defines.hpp>
#include <hpx/schedulers/thread_queue.hpp>

#if defined(HPX_SCHEDULERS_HAVE_DEPRECATION_WARNINGS)
#if defined(HPX_MSVC)
#pragma message(                                                               \
    "The header hpx/runtime/threads/policies/thread_queue.hpp is deprecated, \
    please include hpx/schedulers/thread_queue.hpp instead")
#else
#warning                                                                       \
    "The header hpx/runtime/threads/policies/thread_queue.hpp is deprecated, \
    please include hpx/thread_queue.hpp instead"
#endif
#endif
