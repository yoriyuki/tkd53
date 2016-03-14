#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include "engine/engine.hpp"
#include "converter/bigram_converter.hpp"
#include "dictionary/map_dictionary.hpp"

#include "request.hpp"

using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;
using namespace lime::converter;
using namespace lime::dictionary;
using namespace lime::engine;
using namespace lime::server;

const string kNgramFreqFile = "../var/WordMarkov.freq";

const string kWordKkciMappingFile = "../var/WordKkci.text";

const string kUnixDomainPath = "/tmp/lime_sock";


namespace {


class SocketHolder {
public:
  SocketHolder(const int sock)
    : sock_(sock) {
  }

  ~SocketHolder() {
    if (sock_ < 0) {
      return;
    }
    close(sock_);
  }

  int get() const {
    return sock_;
  }

private:
  const int sock_;
};


void PerrorAndExit(string msg) {
  cerr << msg << endl;
  exit(1);
}


void HandleClient(const SocketHolder &client_socket, Engine *engine) {
  char read_buffer[1024];

  while (1) {
    int read_index = 0;

    while (read_index == 0 || read_buffer[read_index-1] != '\n') {
      // todo: buffer overlflow
      int recieved_size = 0;
      if ((recieved_size = recv(client_socket.get(),
                                read_buffer + read_index,
                                sizeof(read_buffer) - read_index,
                                0)) < 0) {
        PerrorAndExit("recv() failed.");
      }

      read_index += recieved_size;
    }

    read_buffer[read_index - 1] = '\0';

    cout << "[" << read_buffer << "]" << endl;

    string response_string;

    if (!HandleRequest(string(read_buffer), engine, &response_string)) {
      continue;
    }

    cout << "done" << endl;

    if (send(client_socket.get(),
             response_string.c_str(),
             response_string.size(), 0) != int(response_string.size())) {
      PerrorAndExit("send() failed.");
    }
  }
}


} // namespace

int main(void) {
  cout << "Starting Kkc server"  << endl;

  shared_ptr<Bigram> bigram(new Bigram());
  bigram->Init(ifstream(kNgramFreqFile, ios::in));

  shared_ptr<MapDictionary> dict(new MapDictionary());
  dict->Init(ifstream(kWordKkciMappingFile, ios::in));

  Engine engine(shared_ptr<ConverterInterface>(
      new BigramConverter(bigram, dict)));

  SocketHolder server_socket(socket(AF_UNIX, SOCK_STREAM, 0));

  if ((server_socket.get()) < 0) {
    PerrorAndExit("faild to open socket\n");
  }

  int reuse_addr = 1;
  if ((setsockopt(server_socket.get(),
                  SOL_SOCKET,
                  SO_REUSEADDR,
                  (const void *)&reuse_addr,
                  sizeof(reuse_addr))) < 0) {
    PerrorAndExit("faild to set socket opt\n");
  }

  remove(kUnixDomainPath.c_str());

  struct sockaddr_un server_addr;
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "/tmp/lime_sock");

  if (bind(server_socket.get(),
           (struct sockaddr *) &server_addr,
           sizeof(server_addr)) < 0) {
    PerrorAndExit("bind() failed.");
  }

  if (listen(server_socket.get(), 5) < 0) {
    PerrorAndExit("listen() failed.");
  }

  while (1) {
    struct sockaddr_in client_addr;
    socklen_t size = sizeof(client_addr);
    SocketHolder client_socket(
        accept(server_socket.get(), (struct sockaddr *) &client_addr, &size));

    if (client_socket.get() < 0) {
      PerrorAndExit("accept() failed.");
    }

    cout << "Processing new connection" << endl;

    HandleClient(client_socket, &engine);

    cout << "close client" << endl;
  }

  return 0;
}

