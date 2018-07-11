This project is a minimal demonstration of a memory leak I found when using GSOAP to make a SOAP client. No server need be running (although leaks are seen in that too if it is running).

Notes:

* The leak appears to be on a per-request basis - the faster the requests, the faster the leak
* Valgrind does not catch this memory leak - I found it by looking at the process with ps u -p [pid]
* Building without -DDEBUG suppresses the memory leak

To replicate memory leak:

1. Edit Makefile, set GSOAP_SRC to the path where you built gsoap source
2. Set GSOAP_DIR to the path where you installed gsoap library
3. Run "make gen" to generate source from WSDL
4. Run "make" to build source into binary "client_calc"
5. Run client_calc, and optionally server_calc
6. You can monitor memory usage with the monitor.sh script

