//  Copyright (c) 2007-2011 Hartmut Kaiser
//  Copyright (c)      2011 Bryce Lelbach
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_fwd.hpp>
#if HPX_AGAS_VERSION > 0x10
    #include <hpx/runtime.hpp>
#endif
#include <hpx/exception.hpp>
#include <hpx/util/ini.hpp>
#include <hpx/util/stringstream.hpp>
#include <hpx/util/logging.hpp>
#include <hpx/util/filesystem_compatibility.hpp>

#include <hpx/runtime/threads/threadmanager.hpp>
#include <hpx/runtime/naming/resolver_client.hpp>
#include <hpx/runtime/components/server/runtime_support.hpp>
#include <hpx/runtime/components/server/manage_component.hpp>
#include <hpx/runtime/components/server/memory_block.hpp>
#include <hpx/runtime/components/stubs/runtime_support.hpp>
#include <hpx/runtime/actions/continuation_impl.hpp>
#include <hpx/lcos/future_wait.hpp>

#include <hpx/util/portable_binary_iarchive.hpp>
#include <hpx/util/portable_binary_oarchive.hpp>

#include <boost/assert.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>

///////////////////////////////////////////////////////////////////////////////
// Serialization support for the runtime_support actions
HPX_REGISTER_ACTION_EX(
    hpx::components::server::runtime_support::factory_properties_action,
    factory_properties_action);
HPX_REGISTER_ACTION_EX(
    hpx::components::server::runtime_support::create_component_action,
    create_component_action);
HPX_REGISTER_ACTION_EX(
    hpx::components::server::runtime_support::create_one_component_action,
    create_one_component_action);
HPX_REGISTER_ACTION_EX(
    hpx::components::server::runtime_support::create_memory_block_action,
    create_memory_block_action);
#if HPX_AGAS_VERSION > 0x10
    HPX_REGISTER_ACTION_EX(
        hpx::components::server::runtime_support::load_components_action,
        load_components_action);
    HPX_REGISTER_ACTION_EX(
        hpx::components::server::runtime_support::call_startup_functions_action,
        call_startup_functions_action);
    HPX_REGISTER_ACTION_EX(
        hpx::components::server::runtime_support::call_shutdown_functions_action,
        call_shutdown_functions_action);
#endif
HPX_REGISTER_ACTION_EX(
    hpx::components::server::runtime_support::free_component_action,
    free_component_action);
HPX_REGISTER_ACTION_EX(
    hpx::components::server::runtime_support::shutdown_action,
    shutdown_action);
HPX_REGISTER_ACTION_EX(
    hpx::components::server::runtime_support::shutdown_all_action,
    shutdown_all_action);
HPX_REGISTER_ACTION_EX(
    hpx::components::server::runtime_support::get_config_action,
    get_config_action);

///////////////////////////////////////////////////////////////////////////////
HPX_DEFINE_GET_COMPONENT_TYPE_STATIC(
    hpx::components::server::runtime_support,
    hpx::components::component_runtime_support);

///////////////////////////////////////////////////////////////////////////////
namespace hpx { namespace components { namespace server
{
    ///////////////////////////////////////////////////////////////////////////
    runtime_support::runtime_support(util::section& ini, 
            naming::gid_type const& prefix, naming::resolver_client& agas_client, 
            applier::applier& applier)
      : stopped_(false), terminated_(false), ini_(ini)
    {
#if HPX_AGAS_VERSION <= 0x10
        load_components(ini, prefix, agas_client);
#endif
    }

    ///////////////////////////////////////////////////////////////////////////
    // return, whether more than one instance of the given component can be 
    // created at the same time
    int runtime_support::factory_properties(components::component_type type)
    {
    // locate the factory for the requested component type
        component_map_type::const_iterator it = components_.find(type);
        if (it == components_.end() || !(*it).second.first) {
            // we don't know anything about this component
            hpx::util::osstream strm;
            strm << "attempt to query factory properties for components "
                    "invalid/unknown type: "
                 << components::get_component_type_name(type);
            HPX_THROW_EXCEPTION(hpx::bad_component_type, 
                "runtime_support::factory_properties",
                hpx::util::osstream_get_string(strm));
            return factory_invalid;
        }

    // ask for the factory's capabilities
        return (*it).second.first->get_factory_properties();
    }

    ///////////////////////////////////////////////////////////////////////////
    // create a new instance of a component
    naming::gid_type runtime_support::create_component(
        components::component_type type, std::size_t count)
    {
    // locate the factory for the requested component type
        component_map_type::const_iterator it = components_.find(type);
        if (it == components_.end() || !(*it).second.first) {
            // we don't know anything about this component
            hpx::util::osstream strm;
            strm << "attempt to create component instance of invalid/unknown type: "
                 << components::get_component_type_name(type);
            HPX_THROW_EXCEPTION(hpx::bad_component_type, 
                "runtime_support::create_component",
                hpx::util::osstream_get_string(strm));
            return naming::invalid_gid;
        }

    // create new component instance
        naming::gid_type id = (*it).second.first->create(count);

    // set result if requested
        if (LHPX_ENABLED(info)) {
            if ((*it).second.first->get_factory_properties() & factory_instance_count_is_size) 
            {
                LRT_(info) << "successfully created component " << id 
                           << " of type: " 
                           << components::get_component_type_name(type) 
                           << " (size: " << count << ")";
            }
            else {
                LRT_(info) << "successfully created " << count 
                           << " component(s) " << id << " of type: " 
                           << components::get_component_type_name(type);
            }
        }
        return id;
    }

    ///////////////////////////////////////////////////////////////////////////
    // create a new instance of a component
    naming::gid_type runtime_support::create_one_component(
        components::component_type type, constructor_argument const& arg0)
    {
    // locate the factory for the requested component type
        component_map_type::const_iterator it = components_.find(type);
        if (it == components_.end()) {
            // we don't know anything about this component
            hpx::util::osstream strm;
            strm << "attempt to create component instance of invalid/unknown type: "
                 << components::get_component_type_name(type) 
                 << " (component not found in map)";
            HPX_THROW_EXCEPTION(hpx::bad_component_type, 
                "runtime_support::create_component",
                hpx::util::osstream_get_string(strm));
            return naming::invalid_gid;
        }
        else if (!(*it).second.first) {
            // we don't know anything about this component
            hpx::util::osstream strm;
            strm << "attempt to create component instance of invalid/unknown type: "
                 << components::get_component_type_name(type)
                 << " (map entry is NULL)";
            HPX_THROW_EXCEPTION(hpx::bad_component_type, 
                "runtime_support::create_component",
                hpx::util::osstream_get_string(strm));
            return naming::invalid_gid;
        }

    // create new component instance
        naming::gid_type id = (*it).second.first->create_one(arg0);

    // set result if requested
        LRT_(info) << "successfully created component " << id 
                   << " of type: " 
                   << components::get_component_type_name(type);

        return id;
    }

    ///////////////////////////////////////////////////////////////////////////
    // create a new instance of a memory block
    naming::gid_type runtime_support::create_memory_block(
        std::size_t count, hpx::actions::manage_object_action_base const& act)
    {
        server::memory_block* c = server::memory_block::create(count, act);
        naming::gid_type gid = c->get_base_gid();
        if (gid) {
            LRT_(info) << "successfully created memory block of size " << count 
                       << ": " << gid;
            return gid;
        }

        delete c;
        HPX_THROW_EXCEPTION(hpx::duplicate_component_address,
            "runtime_support::create_memory_block", 
            "global id is already bound to a different "
            "component instance");
        return naming::invalid_gid;
    }

    ///////////////////////////////////////////////////////////////////////////
    // delete an existing instance of a component
    void runtime_support::free_component(
        components::component_type type, naming::gid_type const& gid)
    {
    // locate the factory for the requested component type
        if (type == components::component_memory_block) {
            // retrieve the local address bound to the given global id
            applier::applier& appl = hpx::applier::get_applier();
            naming::address addr;
            if (!appl.get_agas_client().resolve(gid, addr)) 
            {
                hpx::util::osstream strm;
                strm << "global id " << gid << " is not bound to any "
                        "component instance";
                HPX_THROW_EXCEPTION(hpx::unknown_component_address,
                    "runtime_support::free_component", 
                    hpx::util::osstream_get_string(strm));
                return;
            }

            // make sure this component is located here
            if (appl.here() != addr.locality_) 
            {
                // FIXME: should the component be re-bound ?
                hpx::util::osstream strm;
                strm << "global id " << gid << " is not bound to any local "
                        "component instance";
                HPX_THROW_EXCEPTION(hpx::unknown_component_address,
                    "runtime_support::free_component", 
                    hpx::util::osstream_get_string(strm));
                return;
            }

            // free the memory block
            components::server::memory_block::destroy(
                reinterpret_cast<components::server::memory_block*>(addr.address_));

            LRT_(info) << "successfully destroyed memory block " << gid;
            return;
        }

        component_map_type::const_iterator it = components_.find(type);
        if (it == components_.end()) {
            // we don't know anything about this component
            hpx::util::osstream strm;

            strm << "attempt to destroy component " << gid 
                 << " of invalid/unknown type: " 
                 << components::get_component_type_name(type)
                 << std::endl;

            strm << "list of registered components: \n";
            component_map_type::iterator end = components_.end();
            for (component_map_type::iterator it = components_.begin(); it!= end; ++it)
            {
                strm << "  " << components::get_component_type_name((*it).first) 
                     << std::endl;
            }

            HPX_THROW_EXCEPTION(hpx::bad_component_type, 
                "runtime_support::free_component",
                hpx::util::osstream_get_string(strm));
            return;
        }

    // destroy the component instance
        (*it).second.first->destroy(gid);

        LRT_(info) << "successfully destroyed component " << gid 
            << " of type: " << components::get_component_type_name(type);
    }

    // function to be called during shutdown
    // Action: shut down this runtime system instance
    void runtime_support::shutdown(double timeout, naming::id_type respond_to)
    {
        // initiate system shutdown
        stop(timeout, respond_to);
    }

    ///////////////////////////////////////////////////////////////////////////
    // initiate system shutdown for all localities
    void runtime_support::shutdown_all(double timeout)
    {
        std::vector<naming::gid_type> prefixes;
        applier::applier& appl = hpx::applier::get_applier();
        appl.get_agas_client().get_prefixes(prefixes);

#if HPX_AGAS_VERSION > 0x10
        // execute registered shutdown functions on all localities
        {
            std::vector<lcos::future_value<void> > lazy_actions;

            BOOST_FOREACH(naming::gid_type gid, prefixes)
            {
                using components::stubs::runtime_support;
                naming::id_type id(gid, naming::id_type::unmanaged);
                lazy_actions.push_back(runtime_support::call_shutdown_functions_async(id));
            }

            // wait for all localities to finish executing their registered 
            // shutdown functions
            components::wait(lazy_actions);
        }
#endif

        // shut down all localities except the the local one
        {
            boost::uint32_t prefix = applier::get_prefix_id();
            std::vector<lcos::future_value<void> > lazy_actions;

            BOOST_FOREACH(naming::gid_type gid, prefixes)
            {
                if (prefix != naming::get_prefix_from_gid(gid))
                {
                    using components::stubs::runtime_support;
                    naming::id_type id(gid, naming::id_type::unmanaged);
                    lazy_actions.push_back(runtime_support::shutdown_async(id, timeout));
                }
            }

            // wait for all localities to be stopped
            components::wait(lazy_actions);
        }

        // now make sure this local locality gets shut down as well.
        stop(timeout, naming::invalid_id);    // no need to respond
    }

    // Retrieve configuration information
    util::section runtime_support::get_config()
    {
        #if HPX_AGAS_VERSION > 0x10
            return *(get_runtime().get_config().get_section("application"));
        #else
            return *ini_.get_section("application");
        #endif
    }

    void runtime_support::tidy()
    {
        mutex_type::scoped_lock l(mtx_);

        // Only after releasing the components we are allowed to release 
        // the modules. This is done in reverse order of loading.
        component_map_type::iterator end = components_.end();
        for (component_map_type::iterator it = components_.begin(); it != end; /**/)
        {
            component_map_type::iterator curr = it;
            ++it;
            if ((*curr).second.first)
            {
                // this is a workaround for sloppy memory management...
                // keep module in memory until application terminated
                if (!(*curr).second.first->may_unload())
                    (*curr).second.second.keep_alive();

                // delete factory in any case
                (*curr).second.first.reset();
            }

            // now delete the entry
            components_.erase(curr);
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    void runtime_support::run()
    {
        mutex_type::scoped_lock l(mtx_);
        stopped_ = false;
        terminated_ = false;
    }

    void runtime_support::wait()
    {
        mutex_type::scoped_lock l(mtx_);
        if (!stopped_) {
            LRT_(info) << "runtime_support: about to enter wait state";
            wait_condition_.wait(l);
            LRT_(info) << "runtime_support: exiting wait state";
        }
    }

    // let thread manager clean up HPX threads 
    template <typename Lock>
    inline void 
    cleanup_threads(threads::threadmanager_base& tm, Lock& l)
    {
        // re-acquire pointer to self as it might have changed
        threads::thread_self* self = threads::get_self_ptr();
        BOOST_ASSERT(0 != self);    // needs to be executed by a PX thread 

        // give the scheduler some time to work on remaining tasks
        {
            util::unlock_the_lock<Lock> ul(l);
            self->yield(threads::pending);
        }

        // get rid of all terminated threads
        tm.cleanup_terminated();
    }

    void runtime_support::stop(double timeout, naming::id_type respond_to)
    {
        mutex_type::scoped_lock l(mtx_);
        if (!stopped_) {
            BOOST_ASSERT(!terminated_);

            stopped_ = true;

            applier::applier& appl = hpx::applier::get_applier();
            threads::threadmanager_base& tm = appl.get_thread_manager();

            util::high_resolution_timer t;
            double start_time = t.elapsed();
            bool timed_out = false;

            while (tm.get_thread_count() > 1) {
                // let thread-manager clean up threads
                cleanup_threads(tm, l);

                // obey timeout
                if (timeout != -1 && timeout < (t.elapsed() - start_time)) {
                    // we waited long enough
                    timed_out = true;
                    break;
                }
            }

            // If it took longer than expected, kill all suspended threads as 
            // well.
            if (timed_out) {
                // now we have to wait for all threads to be aborted
                while (tm.get_thread_count() > 1) {
                    // abort all suspended threads
                    tm.abort_all_suspended_threads();

                    // let thread-manager clean up threads
                    cleanup_threads(tm, l);
                }
            }

            if (respond_to) {
                // respond synchronously
                typedef lcos::base_lco_with_value<void> void_lco_type;
                typedef void_lco_type::set_event_action action_type;

                naming::address addr;
                if (appl.address_is_local(respond_to.get_gid(), addr)) {
                    // execute locally, action is executed immediately as it is
                    // a direct_action
                    hpx::applier::apply_l<action_type>(addr);
                }
                else {
                    // apply remotely, parcel is sent synchronously
                    hpx::applier::apply_r_sync<action_type>(addr, respond_to);
                }
            }

            wait_condition_.notify_all();
            stop_condition_.wait(l);        // wait for termination
        }
    }

    // this will be called after the thread manager has exited
    void runtime_support::stopped()
    {
        mutex_type::scoped_lock l(mtx_);
        if (!terminated_) {
            stop_condition_.notify_all();   // finished cleanup/termination
            terminated_ = true;
        }
    }


#if HPX_AGAS_VERSION > 0x10
    void runtime_support::load_components()
    {
        // load components now that AGAS is up
        get_runtime().get_config().load_components();
        load_components(get_runtime().get_config()
                      , get_runtime().get_agas_client().local_prefix()
                      , get_runtime().get_agas_client());
    }

    void runtime_support::call_startup_functions()
    {
        BOOST_FOREACH(boost::function<void()> const& f, startup_functions_)
        { f(); } 
    }

    void runtime_support::call_shutdown_functions()
    {
        BOOST_FOREACH(boost::function<void()> const& f, shutdown_functions_)
        { f(); } 
    }
#endif

    ///////////////////////////////////////////////////////////////////////////
    // Load all components from the ini files found in the configuration
    void runtime_support::load_components(util::section& ini, 
        naming::gid_type const& prefix, naming::resolver_client& agas_client)
    {
        // load all components as described in the configuration information
        if (!ini.has_section("hpx.components")) {
            LRT_(info) << "No components found/loaded, HPX will be mostly "
                          "non-functional (no section [hpx.components] found).";
            return;     // no components to load
        }

        // each shared library containing components may have an ini section
        //
        // # mandatory section describing the component module
        // [hpx.components.instance_name]
        //  name = ...           # the name of this component module
        //  path = ...           # the path where to find this component module
        //  enabled = false      # optional (default is assumed to be true)
        //
        // # optional section defining additional properties for this module
        // [hpx.components.instance_name.settings]
        //  key = value
        //
        util::section* sec = ini.get_section("hpx.components");
        if (NULL == sec)
            return;     // something bad happened

        util::section::section_map const& s = (*sec).get_sections();

        typedef util::section::section_map::const_iterator iterator;
        iterator end = s.end();
        for (iterator i = s.begin (); i != end; ++i) 
        {
            namespace fs = boost::filesystem;

            // the section name is the instance name of the component
            std::string instance (i->second.get_name());
            std::string component;

            if (i->second.has_entry("name"))
                component = HPX_MANGLE_COMPONENT_NAME_STR(i->second.get_entry("name"));
            else
                component = HPX_MANGLE_COMPONENT_NAME_STR(instance);

            if (i->second.has_entry("enabled")) {
                std::string tmp = i->second.get_entry("enabled");
                boost::algorithm::to_lower (tmp);
                if (tmp == "no" || tmp == "false" || tmp == "0") {
                    LRT_(info) << "dynamic loading disabled: " << instance;
                    continue;     // this component has been disabled
                }
            }

            // test whether this component section was generated 
            bool isdefault = false;
            if (i->second.has_entry("isdefault")) {
                std::string tmp = i->second.get_entry("isdefault");
                boost::algorithm::to_lower (tmp);
                if (tmp == "true") 
                    isdefault = true;
            }

            fs::path lib;
            try {
                if (i->second.has_entry("path"))
                    lib = hpx::util::create_path(i->second.get_entry("path"));
                else
                    lib = hpx::util::create_path(HPX_DEFAULT_COMPONENT_PATH);

                if (!load_component(ini, instance, component, lib, prefix, agas_client, isdefault)) {
                    // build path to component to load
                    std::string libname(component + HPX_SHARED_LIB_EXTENSION);
                    lib /= hpx::util::create_path(libname);
                    if (!load_component (ini, instance, component, lib, prefix, agas_client, isdefault))
                        continue;   // next please :-P
                }
            } 
            catch (hpx::exception const& /*e*/) {
                ; // FIXME: use default component location
            }
        } // for
    }

    bool runtime_support::load_component(util::section& ini, 
        std::string const& instance, std::string const& component, 
        boost::filesystem::path lib, naming::gid_type const& prefix, 
        naming::resolver_client& agas_client, bool isdefault)
    {
        namespace fs = boost::filesystem;
        if (fs::extension(lib) != HPX_SHARED_LIB_EXTENSION)
        {
            LRT_(info) << lib.string() << " is not a shared object: " << instance;
            return false;
        }

        try {
            // get the handle of the library 
            boost::plugin::dll d (lib.string(), component);

            // get the factory
            boost::plugin::plugin_factory<component_factory_base> pf (d, 
                BOOST_PP_STRINGIZE(HPX_MANGLE_COMPONENT_NAME(factory)));

            // initialize the factory instance using the preferences from the 
            // ini files
            util::section const* glob_ini = NULL;
            if (ini.has_section("settings"))
                glob_ini = ini.get_section("settings");

            util::section const* component_ini = NULL;
            std::string component_section("hpx.components." + instance);
            if (ini.has_section(component_section))
                component_ini = ini.get_section(component_section);

            // create the component factory object
            boost::shared_ptr<component_factory_base> factory (
                pf.create(instance, glob_ini, component_ini)); 

            component_type t = factory->get_component_type(prefix, agas_client);
            if (0 == t) {
                LRT_(info) << "component refused to load: "  << instance;
                return false;   // module refused to load
            }

            // store component factory and module for later use
            component_factory_type data(factory, d);
            std::pair<component_map_type::iterator, bool> p = 
                components_.insert(component_map_type::value_type(t, data));

            if (components::get_derived_type(t) != 0) {
            // insert three component types, the base type, the derived 
            // type and the combined one.
                if (p.second) {
                    p = components_.insert(component_map_type::value_type(
                            components::get_derived_type(t), data));
                }
                if (p.second) {
                    p = components_.insert(component_map_type::value_type(
                            components::get_base_type(t), data));
                }
            }

            if (!p.second) {
                LRT_(error) << "duplicate component id: " << instance
                           << ": " << components::get_component_type_name(t);
                return false;   // duplicate component id?
            }

            LRT_(info) << "dynamic loading succeeded: " << lib.string() 
                       << ": " << instance << ": " 
                       << components::get_component_type_name(t);
        }
        catch (std::logic_error const& e) {
            //if (!isdefault) {
                LRT_(warning) << "dynamic loading failed: " << lib.string() 
                              << ": " << instance << ": " << e.what();
            //}
            return false;
        }
        catch (std::exception const& e) {
            //if (!isdefault) {
                LRT_(warning) << "dynamic loading failed: " << lib.string() 
                              << ": " << instance << ": " << e.what();
            //}
            return false;
        }
        return true;    // component got loaded
    }
}}}

