/*****************************************************************************
 * Original Author: V. Girard-Alcindor                                       *
 * contact address: girard-alcindor@ijclab.in2p3.fr                          *
 *                                                                           *
 * Creation Date  :    08/03/24                                              *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#ifndef NPMFMInput_h
#define NPMFMInput_h 1

#include <iostream>
// using namespace std;
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#ifndef NO_MFMXML
// #include <tinyxml.h>
// #include "tinyxml.h"
#endif
#include "Cobo.h"
#include "DataParameters.h"
#include "DataScalers.h"
#include "MError.h"
#include "MFMAllFrames.h"
#include <cstdlib>
#include <cstring>
#include <time.h>
// #include "MFMlib.in.h"
// #include "ArgInterpretor.h"
#include "Version.h"
#include <cmath>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <typeinfo>
#define ANSWER2TYPE_Max_Question 300

#include <fstream>

// nptool
#include "NPApplication.h"
#include "NPInputParser.h"
#include "NPVDataInput.h"
#include "NPVDetector.h"

#ifdef ZMQ_FOUND
#include "zmq.hpp"
#endif

namespace mfm {

class MFMInput : public nptool::VDataInput {

public:
  MFMInput();
  ~MFMInput(){};

  ////////////////////////
  // Managing the input //
  ////////////////////////

public: // inheritted from VDataInput, must be implemented
  // Initialise the input
  void Init(std::vector<std::string> arg);
  // use to attach link an object to the input
  void Attach(std::string, std::string, void*);
  // return false on last entry
  bool GetNextEntry();
  // return the current entry read
  long long GetCurrentEntryNumber() { return m_CurrentEntry; };
  // return the number of entries in the file
  long long GetEntries() { return m_Entries; };
  // return the file size
  long long GetFileSize() { return m_FileSize; };
  // return the size read so far
  long long GetReadSize() { return m_ReadSize; };
  // return true if the entry contain configuration file
  bool HasConfigFile() { return false; };

  // Read input file
  void Init(std::string filename);
  // Necessary for EbyE data
  void ReadActionFile(std::string ActionFile);
  // Necessary for NUMEXO data
  void ReadDetectorFile(std::string DetectorFile) {
    fDetectorFile = DetectorFile;
  };

private:
  std::map<unsigned int, std::string>        m_label_name;
  std::map<unsigned int, nptool::VDetector*> m_label_detector;

private:
  long long m_Entries;
  long long m_CurrentEntry;
  long long m_FileSize;
  long long m_ReadSize;

  void PrintCurrentFile();
  // Return frame type
  std::string                TypeTable(int type_key);
  std::map<int, std::string> fLabelMap;

  std::string        fInputFile;
  std::string        fActionFile;
  std::string        fDetectorFile;
  int                fVerbose;
  unsigned long long fNbFramesRead;
  int                fCount_elseframe;
  char               fCurrentFilename[255];
  int                fMaxdump;
  int                fDumpsize;
  int                framesize;
  int                fLun;
  std::shared_ptr<UtilVector_c> util_vector;

  // mfm treatment
  void TreatFrames(std::shared_ptr<MFMCommonFrame> fFrame);

  long long bytes_read;
  
  std::shared_ptr<nptool::Application> app;

  std::shared_ptr<MFMCommonFrame> fFrame;

  std::map<std::string, std::shared_ptr<nptool::VDetector>> m_detectors;

  bool file_mode;

#ifdef ZMQ_FOUND
  std::string zmq_port;
  std::string path_to_host;
  zmq::socket_t* zmqsub;
  zmq::message_t event;
#endif
  bool continue_treat_event;
  char* buf;
  long long buf_size;
  bool frame_treated = false;

};

} // namespace mfm

#endif
