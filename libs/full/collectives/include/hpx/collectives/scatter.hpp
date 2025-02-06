//  Copyright (c) 2014-2024 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file scatter.hpp

#pragma once

#if defined(DOXYGEN)
// clang-format off
namespace hpx { namespace collectives {

    /// Scatter (receive) a set of values to different call sites
    ///
    /// This function receives an element of a set of values operating on
    /// the given base name.
    ///
    /// \param  basename    The base name identifying the scatter operation
    /// \param this_site    The sequence number of this invocation (usually
    ///                     the locality id). This value is optional and
    ///                     defaults to whatever hpx::get_locality_id() returns.
    /// \param  generation  The generational counter identifying the sequence
    ///                     number of the all_gather operation performed on the
    ///                     given base name. This is optional and needs to be
    ///                     supplied only if the all_gather operation on the
    ///                     given base name has to be performed more than once.
    ///                     The generation number (if given) must be a positive
    ///                     number greater than zero.
    /// \param root_site    The sequence number of the central scatter point
    ///                     (usually the locality id). This value is optional
    ///                     and defaults to 0.
    ///
    /// \returns    This function returns a future holding a the
    ///             scattered value. It will become ready once the scatter
    ///             operation has been completed.
    ///
    template <typename T>
    hpx::future<T> scatter_from(char const* basename,
        this_site_arg this_site = this_site_arg(),
        generation_arg generation = generation_arg(),
        root_site_arg root_site = root_site_arg());

    /// Scatter (receive) a set of values to different call sites
    ///
    /// This function receives an element of a set of values operating on
    /// the given base name.
    ///
    /// \param  comm        A communicator object returned from \a create_communicator
    /// \param this_site    The sequence number of this invocation (usually
    ///                     the locality id). This value is optional and
    ///                     defaults to whatever hpx::get_locality_id() returns.
    /// \param  generation  The generational counter identifying the sequence
    ///                     number of the all_gather operation performed on the
    ///                     given base name. This is optional and needs to be
    ///                     supplied only if the all_gather operation on the
    ///                     given base name has to be performed more than once.
    ///                     The generation number (if given) must be a positive
    ///                     number greater than zero.
    ///
    /// \note       The generation values from corresponding \a scatter_to and
    ///             \a scatter_from have to match.
    ///
    /// \returns    This function returns a future holding a the
    ///             scattered value. It will become ready once the scatter
    ///             operation has been completed.
    ///
    template <typename T>
    hpx::future<T> scatter_from(
        communicator comm,
        this_site_arg this_site = this_site_arg(),
        generation_arg generation = generation_arg());

    /// Scatter (receive) a set of values to different call sites
    ///
    /// This function receives an element of a set of values operating on
    /// the given base name.
    ///
    /// \param  comm        A communicator object returned from \a creat>e_communicator
    /// \param  generation  The generational counter identifying the sequence
    ///                     number of the all_gather operation performed on the
    ///                     given base name. This is optional and needs to be
    ///                     supplied only if the all_gather operation on the
    ///                     given base name has to be performed more than once.
    ///                     The generation number (if given) must be a positive
    ///                     number greater than zero.
    /// \param this_site    The sequence number of this invocation (usually
    ///                     the locality id). This value is optional and
    ///                     defaults to whatever hpx::get_locality_id() returns.
    ///
    /// \note       The generation values from corresponding \a scatter_to and
    ///             \a scatter_from have to match.
    ///>
    /// \returns    This function returns a future holding a the
    ///             scattered value. It will become ready once the scatter
    ///             operation has been completed.
    ///
    template <typename T>
    hpx::future<T> scatter_from(
        communicator comm,
        generation_arg generation,
        this_site_arg this_site = this_site_arg());

    /// Scatter (send) a part of the value set at the given call site
    ///
    /// This function transmits the value given by \a result to a central scatter
    /// site (where the corresponding \a scatter_from is executed)
    ///
    /// \param  basename    The base name identifying the scatter operation
    /// \param  result      The value to transmit to the central scatter point
    ///                     from this call site.
    /// \param  num_sites   The number of participating sites (default: all
    ///                     localities).
    /// \param  generation  The generational counter identifying the sequence
    ///                     number of the all_gather operation performed on the
    ///                     given base name. This is optional and needs to be
    ///                     supplied only if the all_gather operation on the
    ///                     given base name has to be performed more than once.
    ///                     The generation number (if given) must be a positive
    ///                     number greater than zero.
    /// \param this_site    The sequence number of this invocation (usually
    ///                     the locality id). This value is optional and
    ///                     defaults to whatever hpx::get_locality_id() returns.
    ///
    /// \returns    This function returns a future holding a the
    ///             scattered value. It will become ready once the scatter
    ///             operation has been completed.
    ///
    template <typename T>
    hpx::future<T> scatter_to(char const* basename,
        std::vector<T>&& result,
        num_sites_arg num_sites = num_sites_arg(),
        this_site_arg this_site = this_site_arg(),
        generation_arg generation = generation_arg());

    /// Scatter (send) a part of the value set at the given call site
    ///
    /// This function transmits the value given by \a result to a central scatter
    /// site (where the corresponding \a scatter_from is executed)
    ///
    /// \param  comm        A communicator object returned from \a create_communicator
    /// \param  num_sites   The number of participating sites (default: all
    ///                     localities).
    /// \param this_site    The sequence number of this invocation (usually
    ///                     the locality id). This value is optional and
    ///                     defaults to whatever hpx::get_locality_id() returns.
    /// \param  generation  The generational counter identifying the sequence
    ///                     number of the all_gather operation performed on the
    ///                     given base name. This is optional and needs to be
    ///                     supplied only if the all_gather operation on the
    ///                     given base name has to be performed more than once.
    ///                     The generation number (if given) must be a positive
    ///                     number greater than zero.
    ///
    /// \note       The generation values from corresponding \a scatter_to and
    ///             \a scatter_from have to match.
    ///
    /// \returns    This function returns a future holding a the
    ///             scattered value. It will become ready once the scatter
    ///             operation has been completed.
    ///
    template <typename T>
    hpx::future<T> scatter_to(
        communicator comm, std::vector<T>&& result,
        this_site_arg this_site = this_site_arg(),
        generation_arg generation = generation_arg());

    /// Scatter (send) a part of the value set at the given call site
    ///
    /// This function transmits the value given by \a result to a central scatter
    /// site (where the corresponding \a scatter_from is executed)
    ///
    /// \param  comm        A communicator object returned from \a create_communicator
    /// \param  num_sites   The number of participating sites (default: all
    ///                     localities).
    /// \param  generation  The generational counter identifying the sequence
    ///                     number of the all_gather operation performed on the
    ///                     given base name. This is optional and needs to be
    ///                     supplied only if the all_gather operation on the
    ///                     given base name has to be performed more than once.
    ///                     The generation number (if given) must be a positive
    ///                     number greater than zero.
    /// \param this_site    The sequence number of this invocation (usually
    ///                     the locality id). This value is optional and
    ///                     defaults to whatever hpx::get_locality_id() returns.
    ///
    /// \note       The generation values from corresponding \a scatter_to and
    ///             \a scatter_from have to match.
    ///
    /// \returns    This function returns a future holding a the
    ///             scattered value. It will become ready once the scatter
    ///             operation has been completed.
    ///
    template <typename T>
    hpx::future<T> scatter_to(
        communicator comm, std::vector<T>&& result,
        generation_arg generation,
        this_site_arg this_site = this_site_arg());
}}    // namespace hpx::collectives

// clang-format on
#else

#include <hpx/config.hpp>

#if !defined(HPX_COMPUTE_DEVICE_CODE)

#include <hpx/assert.hpp>
#include <hpx/async_base/launch_policy.hpp>
#include <hpx/async_distributed/async.hpp>
#include <hpx/collectives/argument_types.hpp>
#include <hpx/collectives/create_communicator.hpp>
#include <hpx/components_base/agas_interface.hpp>
#include <hpx/futures/future.hpp>
#include <hpx/type_support/unused.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>
#include <vector>

namespace hpx::traits {

    ///////////////////////////////////////////////////////////////////////////
    // support for scatter
    namespace communication {

        struct scatter_tag;

        template <>
        struct communicator_data<scatter_tag>
        {
            HPX_EXPORT static char const* name() noexcept;
        };
    }    // namespace communication

    template <typename Communicator>
    struct communication_operation<Communicator, communication::scatter_tag>
    {
        template <typename Result>
        static Result get(Communicator& communicator, std::size_t which,
            std::size_t generation)
        {
            using data_type = typename Result::result_type;

            return communicator.template handle_data<data_type>(
                communication::communicator_data<
                    communication::scatter_tag>::name(),
                which, generation,
                // step function (invoked once for get)
                nullptr,
                // finalizer (invoked after all sites have checked in)
                [](auto& data, bool&, std::size_t which) {
                    return Communicator::template handle_bool<data_type>(
                        HPX_MOVE(data[which]));
                });
        }

        template <typename Result, typename T>
        static Result set(Communicator& communicator, std::size_t which,
            std::size_t generation, std::vector<T>&& t)
        {
            return communicator.template handle_data<T>(
                communication::communicator_data<
                    communication::scatter_tag>::name(),
                which, generation,
                // step function (invoked once for set)
                [&t](auto& data, std::size_t) { data = HPX_MOVE(t); },
                // finalizer (invoked after all sites have checked in)
                [](auto& data, bool&, std::size_t which) {
                    return Communicator::template handle_bool<T>(
                        HPX_MOVE(data[which]));
                });
        }
    };
}    // namespace hpx::traits

namespace hpx::collectives {

    ///////////////////////////////////////////////////////////////////////////
    // destination site needs to be handled differently
    template <typename T>
    hpx::future<T> scatter_from(communicator fid,
        this_site_arg this_site = this_site_arg(),
        generation_arg generation = generation_arg())
    {
        if (this_site == static_cast<std::size_t>(-1))
        {
            this_site = agas::get_locality_id();
        }
        if (generation == 0)
        {
            return hpx::make_exceptional_future<T>(HPX_GET_EXCEPTION(
                hpx::error::bad_parameter, "hpx::collectives::scatter_from",
                "the generation number shouldn't be zero"));
        }

        auto scatter_from_data = [this_site, generation](
                                     communicator&& c) -> hpx::future<T> {
            using action_type =
                detail::communicator_server::communication_get_direct_action<
                    traits::communication::scatter_tag, hpx::future<T>>;

            // explicitly unwrap returned future
            hpx::future<T> result =
                hpx::async(action_type(), c, this_site, generation);

            if (!result.is_ready())
            {
                // make sure id is kept alive as long as the returned future
                traits::detail::get_shared_state(result)->set_on_completed(
                    [client = HPX_MOVE(c)] { HPX_UNUSED(client); });
            }

            return result;
        };

        return fid.then(hpx::launch::sync, HPX_MOVE(scatter_from_data));
    }

    template <typename T>
    hpx::future<T> scatter_from(communicator fid, generation_arg generation,
        this_site_arg this_site = this_site_arg())
    {
        return scatter_from<T>(HPX_MOVE(fid), this_site, generation);
    }

    template <typename T>
    hpx::future<T> scatter_from(char const* basename,
        this_site_arg this_site = this_site_arg(),
        generation_arg generation = generation_arg(),
        root_site_arg root_site = root_site_arg())
    {
        HPX_ASSERT(this_site != root_site);
        return scatter_from<T>(create_communicator(basename, num_sites_arg(),
                                   this_site, generation, root_site),
            this_site);
    }

    

    ///////////////////////////////////////////////////////////////////////////
    // scatter plain values
    template <typename T>
    hpx::future<T> scatter_to(communicator fid, std::vector<T>&& local_result,
        this_site_arg this_site = this_site_arg(),
        generation_arg generation = generation_arg())
    {
        if (this_site == static_cast<std::size_t>(-1))
        {
            this_site = agas::get_locality_id();
        }
        if (generation == 0)
        {
            return hpx::make_exceptional_future<T>(HPX_GET_EXCEPTION(
                hpx::error::bad_parameter, "hpx::collectives::scatter_to",
                "the generation number shouldn't be zero"));
        }

        auto scatter_to_data = [local_result = HPX_MOVE(local_result),
                                   this_site, generation](
                                   communicator&& c) mutable -> hpx::future<T> {
            using action_type =
                detail::communicator_server::communication_set_direct_action<
                    traits::communication::scatter_tag, hpx::future<T>,
                    std::vector<T>>;

            // explicitly unwrap returned future
            hpx::future<T> result = hpx::async(action_type(), c, this_site,
                generation, HPX_MOVE(local_result));

            if (!result.is_ready())
            {
                // make sure id is kept alive as long as the returned future
                traits::detail::get_shared_state(result)->set_on_completed(
                    [client = HPX_MOVE(c)]() { HPX_UNUSED(client); });
            }

            return result;
        };

        return fid.then(hpx::launch::sync, HPX_MOVE(scatter_to_data));
    }

    template <typename T>
    hpx::future<T> scatter_to(communicator fid, std::vector<T>&& local_result,
        generation_arg generation, this_site_arg this_site = this_site_arg())
    {
        return scatter_to(
            HPX_MOVE(fid), HPX_MOVE(local_result), this_site, generation);
    }

    template <typename T>
    hpx::future<T> scatter_to(char const* basename,
        std::vector<T>&& local_result,
        num_sites_arg num_sites = num_sites_arg(),
        this_site_arg this_site = this_site_arg(),
        generation_arg generation = generation_arg())
    {
        return scatter_to(create_communicator(basename, num_sites, this_site,
                              generation, root_site_arg(this_site.argument_)),
            HPX_MOVE(local_result), this_site);
    }


    template <typename T>
    hpx::future<T> scatter_hierarchically(
        std::vector<std::tuple<communicator,int>> communicators, 
        std::vector<T>&& local_result,
        this_site_arg this_site = this_site_arg(),
        generation_arg generation = generation_arg(),
        root_site_arg root_site = root_site_arg(),
        int arity = 2)
{
        if (this_site == static_cast<std::size_t>(-1))
        {
            this_site = agas::get_locality_id();
        }
        if (generation == 0)
        {
            return hpx::make_exceptional_future<T>(HPX_GET_EXCEPTION(
                hpx::error::bad_parameter, "hpx::collectives::scatter_to",
                "the generation number shouldn't be zero"));
        }
        //std::cout << "this_site: " << this_site << " number of Communicators " << communicators.size() <<"\n";
        int min_sites  = agas::get_num_localities(hpx::launch::sync);
        //std::cout << "min_sites before: " << min_sites << "\n";
        while (min_sites % arity == 0 && min_sites != 0)
        {
            min_sites /= arity;
        }
        if (min_sites == 1){min_sites = arity;}
        //std::cout << "min_sites after: " << min_sites << "\n";
        
        communicator current_communicator = std::get<0>(communicators[0]);
        if (this_site == root_site)
        {
            ///////////////////////////////////////////////////////////////////////////
            // Root Scatter To
            
            std::vector<std::vector<T>> grouped(arity);
            int step_size = local_result.size()/arity;
            for (int i = 0; i< arity; i++)
            {
                grouped[i] = std::vector<T>(local_result.begin() + i*step_size, local_result.begin() + (i+1)*step_size);
                //grouped[i] = std::vector<T>(arity, local_result[0]);
            }
            
            hpx::future<std::vector<T>> intermediary_node = scatter_to(current_communicator, std::move(grouped), this_site_arg(0), generation);
            for (int i = 1; i < communicators.size();i++)
            {
                current_communicator = std::get<0>(communicators[i]);
                if (i == communicators.size()-1)
                {
                    return scatter_to(current_communicator, intermediary_node.get(), this_site_arg(0), generation);
                }
                else 
                {
                    std::vector<T> intermediate_result= std::move(intermediary_node.get());
                    std::vector<std::vector<T>> grouped_intermediate(arity);
                    int step_size = intermediate_result.size()/arity;
                    for (int j = 0; j< arity; j++)
                    {
                        grouped_intermediate[j] = std::vector<T>(intermediate_result.begin() + j*step_size, intermediate_result.begin() + (j+1)*step_size);
                        //grouped_intermediate[j] = std::vector<T>(arity, intermediate_result[0]);
                    }
                    intermediary_node = scatter_to(current_communicator,std::move(grouped_intermediate), this_site_arg(this_site % arity), generation);
                }
            }
        } 
        else if (this_site % min_sites != 0){

            return scatter_from<T>(current_communicator, this_site_arg(this_site % min_sites), generation_arg(generation));
        }
        else
        {
            int adjusted_site = this_site;
            while (adjusted_site % arity == 0 && adjusted_site != 0)
            {
                adjusted_site /= arity;
            }
            hpx::future<std::vector<T>> intermediary_node = scatter_from<std::vector<T>>(current_communicator, this_site_arg(adjusted_site % arity), generation);
            for (int i = 1; i < communicators.size();i++)
            {
                current_communicator = std::get<0>(communicators[i]);
                if (i == communicators.size()-1)
                {
                    return scatter_to(current_communicator, intermediary_node.get(), this_site_arg(0), generation);
                }
                else 
                {
                    std::vector<T> intermediate_result= std::move(intermediary_node.get());
                    std::vector<std::vector<T>> grouped_intermediate(arity);
                    //std::cout << "this_site: " << this_site << " number of Intermediary Results " << intermediate_result.size() <<"\n";
                    int step_size = intermediate_result.size()/arity;
                    for (int j = 0; j< arity; j++)
                    {
                        //grouped_intermediate[j] = std::vector<T>(intermediate_result.begin() + j*step_size, intermediate_result.begin() + (j+1)*step_size);
                        grouped_intermediate[j] = std::vector<T>(arity, intermediate_result[0]);
                    }
                    intermediary_node = scatter_to(current_communicator,std::move(grouped_intermediate), this_site_arg(0), generation);
                }
            }

        }

        /* for (int i = 0; i < communicators.size(); i++){

            if (std::get<1>(communicators[i])>=0 && i != communicators.size()-1)
            {
            ///////////////////////////////////////////////////////////////////////////
            // Intermediary Scatter From.
                this_site = this_site_arg(std::get<1>(communicators[i]));
                auto scatter_from_data = [this_site, generation](
                                     communicator&& c) -> hpx::future<std::vector<T>> {
            using action_type =
                detail::communicator_server::communication_get_direct_action<
                    traits::communication::scatter_tag, hpx::future<std::vector<T>>>;

            // explicitly unwrap returned future
            hpx::future<std::vector<T>> result =
                hpx::async(action_type(), c, this_site, generation);

            if (!result.is_ready())
            {
                // make sure id is kept alive as long as the returned future
                traits::detail::get_shared_state(result)->set_on_completed(
                    [client = HPX_MOVE(c)] { HPX_UNUSED(client); });
            }

            return result;
            };
            hpx::future<std::vector<T>> intermediate_node = std::get<0>(communicators[i]).then(hpx::launch::sync, HPX_MOVE(scatter_from_data));

            std::vector<T> intermediate_result= intermediate_node.get();
            ///////////////////////////////////////////////////////////////////////////
            // Leaf scatter To.
            if (i == communicators.size()-1)
            {
                this_site = this_site_arg(0);
                auto scatter_to_data = [local_result = intermediate_result,
                                   this_site, generation](
                                   communicator&& c) mutable -> hpx::future<T> {
            using action_type =
                detail::communicator_server::communication_set_direct_action<
                    traits::communication::scatter_tag, hpx::future<T>,
                    std::vector<T>>;

            // explicitly unwrap returned future
            hpx::future<T> result = hpx::async(action_type(), c, this_site,
                generation, HPX_MOVE(local_result));

            if (!result.is_ready())
            {
                // make sure id is kept alive as long as the returned future
                traits::detail::get_shared_state(result)->set_on_completed(
                    [client = HPX_MOVE(c)]() { HPX_UNUSED(client); });
            }

            return result;
        };
            return std::get<0>(communicators[i+1]).then(hpx::launch::sync, HPX_MOVE(scatter_to_data));

            }
            ///////////////////////////////////////////////////////////////////////////
            // Intermediary Scatter to
            else 
            {
                this_site = this_site_arg(0);
                std::vector<std::vector<T>> grouped_intermediate(arity);
                for (int j = 0; j< arity; j++)
                {
                    grouped_intermediate[j] = std::vector<T>(intermediate_result.begin() + j*arity, intermediate_result.begin() + (j+1)*arity);
                }
                auto scatter_to_data = [local_result = HPX_MOVE(grouped_intermediate),
                                   this_site, generation](
                                   communicator&& c) mutable -> hpx::future<std::vector<T>> {
            using action_type =
                detail::communicator_server::communication_set_direct_action<
                    traits::communication::scatter_tag, hpx::future<std::vector<T>>,
                    std::vector<std::vector<T>>>;

            // explicitly unwrap returned future
            hpx::future<std::vector<T>> result = hpx::async(action_type(), c, this_site,
                generation, HPX_MOVE(local_result));

            if (!result.is_ready())
            {
                // make sure id is kept alive as long as the returned future
                traits::detail::get_shared_state(result)->set_on_completed(
                    [client = HPX_MOVE(c)]() { HPX_UNUSED(client); });
            }

            return result;
            };

            std::get<0>(communicators[0]).then(hpx::launch::sync, HPX_MOVE(scatter_to_data));
            }
            }
            
            
            
                
                
            
        }
        
        ///////////////////////////////////////////////////////////////////////////
        // Leaf scatter from.
        this_site = this_site_arg(std::get<1>(communicators.end()[-1]));
        auto scatter_from_data = [this_site, generation](
                                     communicator&& c) -> hpx::future<T> {
            using action_type =
                detail::communicator_server::communication_get_direct_action<
                    traits::communication::scatter_tag, hpx::future<T>>;

            // explicitly unwrap returned future
            hpx::future<T> result =
                hpx::async(action_type(), c, this_site, generation);

            if (!result.is_ready())
            {
                // make sure id is kept alive as long as the returned future
                traits::detail::get_shared_state(result)->set_on_completed(
                    [client = HPX_MOVE(c)] { HPX_UNUSED(client); });
            }

            return result;
        };
        return std::get<0>(communicators.end()[-1]).then(hpx::launch::sync, HPX_MOVE(scatter_from_data));
    */
    }
 
}    // namespace hpx::collectives



///////////////////////////////////////////////////////////////////////////////
#define HPX_REGISTER_SCATTER_DECLARATION(...) /**/

///////////////////////////////////////////////////////////////////////////////
#define HPX_REGISTER_SCATTER(...)             /**/

#endif    // !HPX_COMPUTE_DEVICE_CODE
#endif    // DOXYGEN
