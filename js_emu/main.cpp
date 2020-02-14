#include <vector>
#include <map>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

#include <getopt.h>

#include <sys/socket.h>
#include <sys/syslog.h>
#include <netinet/in.h>

#include <unistd.h>

#include "js.h"

#define JS_DEV "/dev/hidg0"

#define SOPS "s:t:c:"
option long_options[] = {
	{"server", 1, 0, 's'},
	{"test", 1, 0, 't'},
	{"cmd", 1, 0, 'c' },
	{0, 0, 0, 0},
};

CJoyStick *g_js = nullptr;

int g_nPort = 0;
int g_sock = -1;
std::thread *g_serverThread = nullptr;

static void start_server() {
	g_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == g_sock) return;

	sockaddr_in in = {0};
	in.sin_family = PF_INET;
	in.sin_addr.s_addr = INADDR_ANY;
	in.sin_port = htons(g_nPort);
	::bind(g_sock, reinterpret_cast<sockaddr*>(&in), sizeof(in));
	::listen(g_sock, 5);
	g_serverThread = new std::thread([]()->void{
		// fetch command from network
	});
}

int main(int argc, char* argv[]) {
	g_js = new CJoyStick(JS_DEV);

	if (nullptr == g_js) {
		std::cout << "allocate joystick control memory failed" << std::endl;
		return -1;
	}
	g_js->init();
	g_js->resetJoyStick();
	g_js->resetData();
	int c, index;
	while (1) {
		c = getopt_long(argc, argv, SOPS, long_options, &index);
		if (-1 == c) break;

		switch(c) {
			default: break;
			case 's':
				g_nPort = std::stoi(reinterpret_cast<char*>(optarg));
				start_server();
				break;
			case 't':
				break;
			case 'c':
				break;
		}
	}

	if (nullptr != g_serverThread) {
		g_serverThread->join();
	}

	if (-1 != g_sock) {
		::close(g_sock);
		g_sock = -1;
	}

	if (nullptr != g_js) {
		g_js->close();
		delete g_js;
		g_js = nullptr;
	}
	return 0;
}
