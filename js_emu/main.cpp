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
#include "clua.h"
#include "cmd.h"

#define JS_DEV "/dev/hidg0"

#define SOPS "s:t:c:"
option long_options[] = {
	{"server", 1, 0, 's'},
	{"test", 1, 0, 't'},
	{0, 0, 0, 0},
};

CJoyStick *g_js = nullptr;

int g_nPort = 0;
int g_sock = -1;
std::thread *g_serverThread = nullptr;

static void cmdRun(CCommand &cmd) {

}

static void cmdStop(CCommand &cmd) {

}

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
		sockaddr_in client = {0};
		while (-1 != g_sock) {
			unsigned int leng = sizeof(client);
			int client_fd = ::accept(g_sock, reinterpret_cast<sockaddr*>(&client), &leng);
			int nDisConn = 0;

			while (0 == nDisConn) {
				char szBuf[1024] = {0};
				ssize_t ret = ::recv(client_fd, szBuf, sizeof(szBuf) -1, 0);
				if (0 >= ret) {
					break;
				}

				CCommand cmd;
				cmd.command(szBuf);
				switch (cmd.getParam(PARAM_CMD)) {
					default: break;
					case VALUE_RUN: cmdRun(cmd); break;
					case VALUE_STOP: cmdStop(cmd); break;
					case VALUE_DISCONNECT: nDisConn = 1; break;
					case VALUE_QUIT:
						::close(client_fd);
						::close(g_sock);
						g_sock = -1;
						return;
				}
			}
			::close(client_fd);
		}
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
			case 't': {
				CLua lua;
				lua.setJoyStick(g_js);
				lua.runString(reinterpret_cast<char*>(optarg));
			}	break;
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
