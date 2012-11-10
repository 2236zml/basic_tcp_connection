#include "basic_tcp_connection.h"

int main()
try
{

	ConnectionManager<tcp_connection> connection_manager;
	connection_manager.init_connection( 10 );
	connection_manager.endpoint( "173.224.215.80", 2236 );
	connection_manager.start_connect();	

	basic_tcp_connection::join();
	
	return 0;
}
catch( exception& e )
{
	cout << e.what() << endl;
}