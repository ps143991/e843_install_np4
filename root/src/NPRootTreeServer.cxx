#include "NPRootTreeServer.h"
#include "NPFunction.h"
#include <iostream>
using namespace nptool;
using namespace root;
////////////////////////////////////////////////////////////////////////////////
RootTreeServer::RootTreeServer(unsigned long long size, unsigned int port) {
  m_Tree = new TTree("CircularTree", "CircularTree");
  m_Tree->SetCircular(size);
  m_Server = new TServerSocket(port, kTRUE);
  m_Monitor = new TMonitor;
  m_Monitor->Add(m_Server);
  m_entries = size;
};
////////////////////////////////////////////////////////////////////////////////
RootTreeServer::~RootTreeServer() {
  delete m_Tree;
  delete m_Server;
  delete m_Monitor;
};

////////////////////////////////////////////////////////////////////////////////
void RootTreeServer::Fill() {
  thread_local uint64_t entries = 0;
  m_Tree->Fill();
  entries++;
  if (entries == m_entries) {
    entries = 0;
    // Check if there is a message waiting on one of the sockets.
    // Wait not longer than 20ms (returns -1 in case of time-out).
    auto read = new TList;
    auto write = new TList;

    TSocket* s;
    int num = m_Monitor->Select(read, write, 0);
    TIter rd(read);
    while ((s = (TSocket*)rd())) {
      HandleSocket(s);
    }
    delete read;
    delete write;
  }
}
////////////////////////////////////////////////////////////////////////////////
void RootTreeServer::HandleSocket(TSocket* s) {
  static std::string name;
  if (s->IsA() == TServerSocket::Class()) {
    // accept new connection from spy
    TSocket* sock = ((TServerSocket*)s)->Accept();
    m_Monitor->Add(sock);
    m_Sockets.Add(sock);
    name = sock->GetInetAddress().GetHostName();
    std::cout << std::endl;
    message("green", "root", "nptool::RootTreeServer", "Accepted connection from " + name);
  }
  else {
    // we only get string based requests from the spy
    char request[64];
    if (s->Recv(request, sizeof(request)) <= 0) {
      m_Monitor->Remove(s);
      m_Sockets.Remove(s);
      name = s->GetInetAddress().GetHostName();
      std::cout << std::endl;
      message("yellow", "root", "nptool::RootTreeServer", "Closed connection with " + name);
      delete s;
      return;
    }

    // send requested object back
    TMessage answer(kMESS_OBJECT);
    if (!strcmp(request, "tree")) {
      answer.WriteObject(m_Tree);
    }
    else {
      name = s->GetInetAddress().GetHostName();
      std::cout << std::endl;
      message("red", "root", "nptool::RootTreeServer", "Unexpected request from " + name);
    }

    s->Send(answer);
  }
}
