#ifndef __BASIC_TCP_CONNECTION_H__
#define __BASIC_TCP_CONNECTION_H__

#include <string>
#include <iostream>
#include <vector>

using namespace std;

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/date_time.hpp>
#include <boost/shared_ptr.hpp>
using namespace boost::asio::ip;


class basic_tcp_connection{
	friend class async_timer;
public:
	basic_tcp_connection()
		:_socket( _io_service )
	{}

	static void wait_all()
	{
		_io_service_work_thread.join();
	}

protected:

	static boost::asio::io_service _io_service;
	static boost::asio::io_service::work _io_service_work;

	static boost::thread _io_service_work_thread;

	tcp::socket _socket;
};



class async_timer{
public:
	async_timer()
		:_timer( basic_tcp_connection::_io_service )
	{}

	void sleep( const boost::posix_time::time_duration& time_duration, const boost::function<void()>& wake_up )
	{
		_wake_up = wake_up;

		_timer.expires_from_now( time_duration );

		_timer.async_wait( boost::bind( &async_timer::check_sleep, this ) );
	}

	void check_sleep()
	{
		if( _timer.expires_at() <= boost::asio::deadline_timer::traits_type::now() )
			_wake_up();
	}

	boost::function<void()> _wake_up;
	
	boost::asio::deadline_timer _timer;
};


/*

class tcp_connection : public basic_tcp_connection{
public:
	tcp_connection()
		:_timer( _io_service )
	{}

	void endpoint( const string& ip, unsigned short port )
	{
		_endpoint.address( address::from_string( ip ) );
		_endpoint.port( port );
	}

	void start_connect()
	{
		_socket.async_connect( _endpoint, 
			boost::bind( &tcp_connection::handle_connect, this, boost::asio::placeholders::error ) );
	}

	void start_connect( tcp::endpoint endpoint_ )
	{
		_endpoint = endpoint_;
		_socket.async_connect( endpoint_, 
			boost::bind( &tcp_connection::handle_connect, this, boost::asio::placeholders::error ) );
	}

	void handle_connect( const boost::system::error_code& error_code )
	{
		if( !error_code )
		{
			cout << "connected" << endl;
			_timer.sleep( boost::posix_time::seconds( 30 ), 
				boost::bind( &tcp_connection::restart_connect, this, boost::posix_time::seconds( 30 ) ) );
		}
		else
		{
			cout << boost::system::system_error( error_code ).what() << endl;
			_timer.sleep( boost::posix_time::seconds( 30 ), 
				boost::bind( &tcp_connection::restart_connect, this, boost::posix_time::seconds( 30 ) ) );
		}
	}

	void restart_connect( const boost::posix_time::time_duration& time_duration = boost::posix_time::seconds( 0 ) )
	{
		cout << "close_connect" << endl;
		_socket.close();
		_timer.sleep( time_duration,
			boost::bind( &tcp_connection::start_connect, this, _endpoint ) );
	}

	void close_connect()
	{
		_socket.close();
	}

	timer _timer;
	tcp::endpoint _endpoint;
};

*/


template <typename _Connection>
class ConnectionManager{
public:
	typedef boost::shared_ptr<_Connection> ConnectionPtr;
	typedef vector<ConnectionPtr> ConnectionContainer;

	void endpoint( const string& ip, unsigned short port )
	{
		_endpoint.address( address::from_string( ip ) );
		_endpoint.port( port );
	}

	void init_connection( size_t connection_number )
	{
		for( size_t i = 0; i < connection_number; i++ )
		{
			ConnectionPtr connection_ptr( new _Connection );
			_connection_container.push_back( connection_ptr );
		}
	}

	void start_connect()
	{
		for( ConnectionContainer::iterator it = _connection_container.begin();
			it != _connection_container.end(); it++ )
			(*it)->start_connect( _endpoint );
	}

	void close_connect()
	{
		for( ConnectionContainer::iterator it = _connection_container.begin();
			it != _connection_container.end(); it++ )
			(*it)->close_connect();
	}

	tcp::endpoint _endpoint;

	ConnectionContainer _connection_container;
};





#endif