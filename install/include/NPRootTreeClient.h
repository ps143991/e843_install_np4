#ifndef NPRootTreeClient_h
#define NPRootTreeClient_h
#include "NPException.h"
#include "NPFunction.h"
#include "TMessage.h"
#include "TMonitor.h"
#include "TSocket.h"
#include "TTree.h"
#include <string>

namespace nptool {
  namespace root {
    class RootTreeClient {
     public:
      RootTreeClient(std::string address, unsigned int port);
      ~RootTreeClient();

     public:
      // the tree change at each server request
      inline TTree* GetCurrentTree() { return m_Tree; };
      inline unsigned long long GetEntries() { return m_Tree->GetEntries(); };
      void RequestNewTree();

     private:
      TSocket* m_Socket; // server socket
      TTree* m_Tree;     // circular in memory tree
      std::string m_address;
      unsigned int m_port;
    };
  } // namespace root
} // namespace nptool

#endif
