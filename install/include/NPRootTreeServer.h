#ifndef NPRootTreeServer_h
#define NPRootTreeServer_h
#include "NPException.h"
#include "NPFunction.h"
#include "TMessage.h"
#include "TMonitor.h"
#include "TServerSocket.h"
#include "TSocket.h"
#include "TTree.h"

namespace nptool {
  namespace root {
    class RootTreeServer {
     public:
      RootTreeServer(unsigned long long size = 1000, unsigned int port = 9999);
      ~RootTreeServer();

     public:
      void Fill();
      TTree* GetTree() { return m_Tree; };

     private:
      void HandleSocket(TSocket* s);
      TServerSocket* m_Server;      // server socket
      TMonitor* m_Monitor;          // socket monitor
      TList m_Sockets;              // list of open spy sockets
      TTree* m_Tree;                // circular in memory tree
      unsigned long long m_entries; // max number of entry
    };

  } // namespace root

} // namespace nptool

#endif
