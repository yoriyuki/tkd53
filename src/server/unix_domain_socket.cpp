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


class SocketCloser {
public:
  SocketCloser(const int sock)
    : sock_(sock) {
  }

  ~SocketCloser() {
    if (sock_ < 0) {
      return;
    }
    close(sock_);
  }
private:
  const int sock_;
};


void PerrorAndExit(string msg) {
  cerr << msg << endl;
  exit(1);
}


bool ReadLine(const int socket, char* const read_buffer, const size_t size) {
  size_t read_index = 0;
  while ((read_index == 0) ||
         (read_index < size && read_buffer[read_index-1] != '\n')) {
    const int recieved_size =
      recv(socket, &read_buffer[read_index], size - read_index, 0);
    if (recieved_size < 0) {
      return false;
    }
    read_index += static_cast<size_t>(recieved_size);
  }
  read_buffer[read_index - 1] = '\0';
  return true;
}


bool WriteLine(const int socket,
               const char* const write_buffer,
               const size_t size) {
  size_t bytes_send = 0;
  while (bytes_send != size) {
    const int bytes = send(socket, write_buffer + bytes_send, size, 0);
    if (bytes < 0) {
      return false;
    }
    bytes_send += static_cast<size_t>(bytes);
  }
  return true;
}


bool HandleClient(const int client_socket, Engine *engine) {
  char read_buffer[1024];

  while (1) {
    if (!ReadLine(client_socket, read_buffer, sizeof(read_buffer))) {
      PerrorAndExit("ReadLine() failed.");
    }

    cerr << "Request: " << read_buffer <<  endl;

    string response_string = read_buffer;
    if (!HandleRequest(string(read_buffer), engine, &response_string)) {
      break;
    }

    cerr << "Response: " << response_string;

    if (!WriteLine(client_socket,
                   response_string.c_str(),
                   response_string.size())) {
      PerrorAndExit("WriteLine() failed.");
    }
  }
  return true;
}


} // namespace

int main(void) {
  cerr << "Starting Kkc server"  << endl;

  shared_ptr<Bigram> bigram(new Bigram());
  bigram->Init(ifstream(kNgramFreqFile, ios::in));

  shared_ptr<MapDictionary> dict(new MapDictionary());
  dict->Init(ifstream(kWordKkciMappingFile, ios::in));

  Engine engine(shared_ptr<ConverterInterface>(
      new BigramConverter(bigram, dict)));

  int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if ((server_socket) < 0) {
    PerrorAndExit("faild to open socket\n");
  }

  SocketCloser server_socket_closer(server_socket);

  int reuse_addr = 1;
  if ((setsockopt(server_socket,
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

  if (bind(server_socket,
           (struct sockaddr *) &server_addr,
           sizeof(server_addr)) < 0) {
    PerrorAndExit("bind() failed.");
  }

  if (listen(server_socket, 5) < 0) {
    PerrorAndExit("listen() failed.");
  }

  while (1) {
    struct sockaddr_in client_addr;
    socklen_t size = sizeof(client_addr);
    int client_socket = accept(server_socket,
                               (struct sockaddr *) &client_addr,
                               &size);
    if (client_socket < 0) {
      PerrorAndExit("accept() failed.");
    }

    SocketCloser client_socket_closer(client_socket);

    cout << "Processing new connection" << endl;

    HandleClient(client_socket, &engine);

    cout << "close client" << endl;
  }

  return 0;
}

