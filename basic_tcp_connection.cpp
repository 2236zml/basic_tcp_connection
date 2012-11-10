#include "basic_tcp_connection.h"


boost::asio::io_service basic_tcp_connection::_io_service;
boost::asio::io_service::work basic_tcp_connection::_io_service_work( basic_tcp_connection::_io_service );

boost::thread basic_tcp_connection::_io_service_work_thread( boost::bind( &boost::asio::io_service::run, &basic_tcp_connection::_io_service ) );
