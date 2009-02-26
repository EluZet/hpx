//  Copyright (c) 2007-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <hpx/hpx.hpp>
#include <hpx/runtime/actions/plain_action.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>

using namespace hpx;

///////////////////////////////////////////////////////////////////////////////
// plain thread function throwing an exception
int throw_error_locally()
{
    HPX_THROW_EXCEPTION(hpx::no_success, "throw_error_locally", "Some error occurred");
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// remote thread function invoked using a future, throwing an exception
int remote_error()
{
    HPX_THROW_EXCEPTION(hpx::no_success, "remote_error", "Some error occurred");
    return 0;
}

typedef actions::plain_result_action0<int, remote_error> remote_error_action;
HPX_REGISTER_ACTION(remote_error_action);

int throw_error_remotely()
{
    typedef lcos::eager_future<remote_error_action> remote_error_future;

    naming::id_type prefix = applier::get_applier().get_runtime_support_gid();
    remote_error_future f(prefix);
    return f.get();                 // should (re-)throw!
}

///////////////////////////////////////////////////////////////////////////////
bool error_sink_called = false;

void errorsink(boost::uint32_t src, std::string const& msg)
{
    error_sink_called = true;
    std::cerr << "locality (" << std::hex << std::setw(4) 
              << std::setfill('0') << src << "):" << std::endl
              << msg << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    try {
        // initialize the AGAS service
        hpx::util::io_service_pool agas_pool; 
        hpx::naming::resolver_server agas(agas_pool);

        // start the HPX runtime
        hpx::runtime rt("localhost", HPX_PORT, "localhost", 0, hpx::runtime::console);

        boost::signals::scoped_connection errorsink_connection;
        rt.register_error_sink(errorsink, errorsink_connection);

        rt.run(throw_error_locally);
        BOOST_TEST(error_sink_called);

        error_sink_called = false;
        rt.run(throw_error_remotely);
        BOOST_TEST(error_sink_called);
    }
    catch (std::exception& e) {
        BOOST_TEST(false);
        std::cerr << "std::exception caught: " << e.what() << "\n";
    }
    catch (...) {
        BOOST_TEST(false);
        std::cerr << "unexpected exception caught\n";
    }
    return boost::report_errors();
}
