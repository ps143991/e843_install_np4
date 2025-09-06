#include "NPRootTreeClient.h"
#include "TMessageHandler.h"
#include "TROOT.h"
#include "TSystem.h"
#include <iostream>
using namespace nptool;
using namespace root;
////////////////////////////////////////////////////////////////////////////////
RootTreeClient::RootTreeClient(std::string address, unsigned int port) : m_address(address), m_port(port) {
  static int counter = 0;
  m_Socket = new TSocket(address.data(), port);
  m_Tree = nullptr;
  // Request initial tree
  bool success = false;
  int retry = 0;
  while (!success) {
    if (m_Socket->IsValid()) {
      TMessage* mess;
      m_Socket->Send("tree");
      if (m_Socket->Recv(mess) <= 0) {
        message("red", "root", "RootTreeClient", "error receiving message ... retrying");
      }

      else if (mess->GetClass()->InheritsFrom(TTree::Class())) {
        m_Tree = (TTree*)mess->ReadObject(mess->GetClass());
        delete mess;
        success = true;
      }

      else {
        message("red", "root", "RootTreeClient", "unknown message type from server ... retrying");
      }
    }
    else {
      if (!retry) {
        message("red", "root", "RootTreeClient", "invalid socket, retrying to connect every 1s ...");
        retry++;
      }
      auto socket = new TSocket(m_address.data(), m_port);
      delete m_Socket;
      m_Socket = socket;
      if (!m_Socket->IsValid()) {
        // magic line to go back one line in the terminal
        std::cout << "\033[F\r Attempt: " << retry++ << "   " << std::flush;
        sleep(1);
      }
      else {
        std::cout << std::endl;
        message("green", "root", "RootTreeClient", "successful reconnection to " + m_address);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
RootTreeClient::~RootTreeClient() {
  if (m_Socket)
    delete m_Socket;
  if (m_Tree)
    delete m_Tree;
};
////////////////////////////////////////////////////////////////////////////////
void RootTreeClient::RequestNewTree() {
  auto old_tree = m_Tree;
  bool success = false;
  int retry = 0;
  while (!success) {

    if (!m_Socket->IsValid()) {
      if (!retry) {
        message("red", "root", "RootTreeClient", "invalid socket, retrying to connect every 1s ...");
        retry++;
      }
      auto socket = new TSocket(m_address.data(), m_port);
      delete m_Socket;
      m_Socket = socket;
      if (!m_Socket->IsValid()) {
        // magic line to go back one line in the terminal
        std::cout << "\033[F\r Attempt: " << retry++ << "   " << std::flush;
        sleep(1);
      }
      else {
        std::cout << std::endl;
        message("green", "root", "RootTreeClient", "successful reconnection to " + m_address);
      }
    }
    else {
      TMessage* mess;
      m_Socket->Send("tree");
      if (m_Socket->Recv(mess) <= 0) {
        message("red", "root", "RootTreeClient", "error receiving message ... retrying");
      }
      else if (mess->GetClass()->InheritsFrom(TTree::Class())) {
        m_Tree = (TTree*)mess->ReadObject(mess->GetClass());
        // Reattach the branches of the old tree to the new one:
        if (old_tree) {
          auto branches = old_tree->GetListOfBranches();
          TIter next(branches);
          TBranch* b;
          while ((b = (TBranch*)next())) {
            m_Tree->SetBranchAddress(b->GetName(), (void*)b->GetAddress());
          }
        }
        thread_local bool first = true;
        if (first)
          first = false;
        else
          delete old_tree;

        delete mess;
        success = true;
      }
      else {
        message("red", "root", "RootTreeClient", "unknown message type from server ... retrying");
      }
    }
  }
}
