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
#include "NPMFMInput.h"

#include <unistd.h>

using namespace mfm;

#ifdef ZMQ_FOUND
zmq::context_t context(1);
#endif

////////////////////////////////////////////////////////////////////////////////
MFMInput::MFMInput() {
  bytes_read = 0;
  buf_size = 0;
  fLun = 0;  // Logical Unit Number
  fMaxdump = 128;
  m_CurrentEntry = 0;
  framesize = 0;
}

////////////////////////////////////////////////////////////////////////////////
void MFMInput::Init(std::vector<std::string> arg) {
  // possible arg --input mfm,run_XXX
  // possible arg --input mfm,host:port

  fDumpsize = 16;
  fCount_elseframe = 0;
  fNbFramesRead = -1;
  fMaxdump = 128;
  fVerbose = 5;
  m_ReadSize = 0;
  m_Entries = -1;

  app = nptool::Application::GetApplication();
  fFrame = std::make_shared<MFMCommonFrame>();
  m_detectors = app->GetAllDetectors();

  if (arg[1].find(":") == std::string::npos) {
    file_mode = true;

    std::string filename = arg[1];
    strcpy(fCurrentFilename, filename.c_str());
    fLun = open(fCurrentFilename, (O_RDONLY));
    util_vector =
        std::make_shared<UtilVector_c>(MFM_BLOB_HEADER_SIZE);  // min size =8

    ifstream testFile(arg[1], ios::binary);
    const auto begin = testFile.tellg();
    testFile.seekg(0, ios::end);
    const auto end = testFile.tellg();
    m_FileSize = (end - begin);
    testFile.close();

    if (fLun <= 0) {
      std::cout << "Error opening file: " << fCurrentFilename << std::endl;
      exit(1);
    }
  } else {
    file_mode = false;

#ifdef ZMQ_FOUND
    int split_pos = arg[1].find(":");
    std::string host = arg[1].substr(0, split_pos);
    std::string str_port = arg[1].substr(split_pos + 1, arg[1].size());
    unsigned int port = stoi(str_port);

    zmq_port = "tcp://";

    path_to_host = host;
    int host_port = port;

    zmq_port = zmq_port + path_to_host + ":" + std::to_string(host_port);

    zmqsub = 0;
    try {
      zmqsub = new zmq::socket_t(context, ZMQ_SUB);
    } catch (zmq::error_t &e) {
      std::cout << "[SPY2DB] : ERROR: "
                << "process_start: failed to start ZeroMQ event spy: "
                << e.what() << std::endl;
    }

    int timeout = 100;  // milliseconds
    zmqsub->set(zmq::sockopt::rcvtimeo, timeout);
    try {
      zmqsub->connect(zmq_port.c_str());
    } catch (zmq::error_t &e) {
      std::cout << "[SPY2DB] : ERROR"
                << "process_start: failed to bind ZeroMQ endpoint " << zmq_port
                << ": " << e.what() << std::endl;
    }
    std::cout << "[SPY2DB] : Connected to spy " << zmq_port << std::endl;
    zmqsub->set(zmq::sockopt::subscribe, "");

    time_t current_time;
    time(&current_time);
    struct tm *timeinfo = localtime(&current_time);

    std::cout << "[SPY2DB] : beginning at " << asctime(timeinfo);
#endif
  }
}

////////////////////////////////////////////////////////////////////////////////
void MFMInput::Attach(std::string token, std::string, void *ptr) {
  for (auto it = m_label_name.begin(), end = m_label_name.end(); it != end;
       it++) {
    if (it->second.rfind(token, 0) == 0)
      m_label_detector[it->first] = (nptool::VDetector *)ptr;
  }
};

////////////////////////////////////////////////////////////////////////////////
bool MFMInput::GetNextEntry() {
  // std::cout<<"**********************************"<<std::endl;
  // std::cout<<"step 1: this one ran!"<<std::endl;
  if (file_mode == true) {
    if (m_CurrentEntry != fNbFramesRead) {
      framesize = fFrame->ReadInFile(&fLun, util_vector.get());
      fFrame->SetAttributs();

      if (framesize <= 0) return false;

      bytes_read += framesize;
      m_ReadSize = bytes_read;

      TreatFrames(fFrame);
      return true;
    } else {
      fLun = close(fLun);
      return false;
    }
  } else if (file_mode == false) {
#ifdef ZMQ_FOUND
    if (bytes_read >= buf_size || buf_size == 0 || buf_size == 32) {
      bytes_read = 0;
      try {
        if (!zmqsub->recv(event)) {
          std::this_thread::sleep_for(std::chrono::milliseconds(1000));
          return true;
        }
      } catch (zmq::error_t &e) {
        std::cout << "[SPY2DB] : timeout on ZeroMQ endpoint: " << e.what()
                  << std::endl;
        return true;
      }

      buf = (char *)event.data();
      buf_size = event.size();
      if (buf_size > 32)
        std::cout << "Buffer incoming of size: " << buf_size << std::endl;
    } else {
      std::shared_ptr<nptool::VDataOutput> output;
      if (buf_size > 32) {
        if (framesize = fFrame->ReadInMem(&buf)) {
          bytes_read += framesize;
          m_ReadSize = bytes_read;

          if (bytes_read >= buf_size) {
            buf_size = 0;
            bytes_read = 0;
            return true;
          }

          if (framesize <= 0) {
            buf_size = 0;
            bytes_read = 0;
            return true;
          }

          TreatFrames(fFrame);

          return true;
        } else {
          buf_size = 0;
          bytes_read = 0;
          return true;
        }
        return true;
      }
    }
#endif
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////
// bool MFMInput::GetNextEntry() {
//   if(file_mode == true){
//     if (m_CurrentEntry != fNbFramesRead){
//       framesize = fFrame->ReadInFile(&fLun, util_vector.get());
//       fFrame->SetAttributs();

//       if (framesize <= 0)
//         return false;

//       bytes_read += framesize;
//       m_ReadSize = bytes_read;

//       TreatFrames(fFrame);
//       return true;
//     }
//     else{
//       fLun = close(fLun);
//       return false;
//     }
//   }
// else if(file_mode == false){
// if(bytes_read >= buf_size || buf_size == 0 || buf_size == 32){
//   try{
//     if(!zmqsub->recv(event)){
//       std::this_thread::sleep_for(std::chrono::milliseconds(100));
//       frame_treated = false;
//     }
//   }catch(zmq::error_t &e) {
//     std::cout << "[SPY2DB] : timeout on ZeroMQ endpoint: " << e.what() <<
//     std::endl; frame_treated = false;
//   }

//   buf = (char*)event.data();
//   buf_size = event.size() ;
//   if(buf_size > 32)
//     std::cout << "Buffer incoming of size: " << buf_size << std::endl;
// } else{
//   if (buf_size > 32){
//     if(bytes_read < buf_size){
//       while(framesize = fFrame->ReadInMem(&buf)){
//         std::cout << framesize << std::endl;
//         bytes_read += framesize;
//         m_ReadSize = bytes_read;

//         TreatFrames(fFrame);
//         std::cout << "OUT OF TREATMENT" << std::endl;
//         return true;
//       }
//     }
//     else{
//       buf_size = 0;
//       bytes_read = 0;
//       return true;
//     }
//   }
// }
// }

// /*
// // while(!frame_treated){
// // while(true){
// // std::cout << "Stuck In WHILE" << std::endl;
// if(bytes_read >= buf_size || buf_size == 0 || buf_size == 32){
// // std::cout << "Stuck In if" << std::endl;
// frame_treated = false;
// while(buf_size == 0 || buf_size == 32){
// std::cout << "while(buf_size == 0 || buf_size == 32){" << std::endl;

// buf = (char*)event.data();
// buf_size = event.size() ;
// if(buf_size > 32)
// std::cout << "Buffer incoming of size: " << buf_size << std::endl;

// }
// frame_treated = true;
// } else {
// // std::cout << ">>>>>>>>>> DEBUG : " << 6 << std::endl;
// // bytes_read = 0;
// std::shared_ptr<nptool::VDataOutput> output;
// if (buf_size > 32){
// if(bytes_read < buf_size){
// if(framesize = fFrame->ReadInMem(&buf)){
// std::cout << ">>>>>>>>>> DEBUG : " << 7 << std::endl;
// bytes_read += framesize;
// m_ReadSize = bytes_read;

// if(bytes_read > buf_size){
// buf_size = 0;
// bytes_read = 0;
// std::cout << ">>>>>>>>>> DEBUG : " << 0 << std::endl;
// frame_treated = false;
// return true;
// }

// if (framesize <= 0){
// buf_size = 0;
// bytes_read=0;
// frame_treated = false;
// return true;
// std::cout << ">>>>>>>>>> DEBUG : " << 1 << std::endl;
// }

// TreatFrames(fFrame);
// frame_treated = true;
// // std::cout << ">>>>>>>>>> DEBUG : " << 2 << std::endl;
// return true;
// } else{
// buf_size = 0;
// bytes_read = 0;
// frame_treated = false;
// return true;
// }
// }
// // frame_treated = false;
// std::cout << ">>>>>>>>>> DEBUG : " << 3 << std::endl;
// return true;
// }
// }
// }
// */
// // std::cout << ">>>>>>>>>> DEBUG : " << 4 << std::endl;
// return true;
// }

void MFMInput::TreatFrames(std::shared_ptr<MFMCommonFrame> fFrame) {
  // std::cout<<"step 2: this is the step running now"<<std::endl;
  std::shared_ptr<MFMMergeFrame> MergeFrame = std::make_shared<MFMMergeFrame>();
  std::shared_ptr<MFMCommonFrame> InsideFrame =
      std::make_shared<MFMCommonFrame>();
  unsigned int type_key = fFrame->GetFrameType();
  if (type_key && type_key != MFM_XML_DATA_DESCRIPTION_FRAME_TYPE &&
      type_key != MFM_XML_FILE_HEADER_FRAME_TYPE &&
      type_key != MFM_HELLO_FRAME_TYPE && type_key != 65535 &&
      type_key != 34645 && type_key != 22016 && type_key != 255 &&
      type_key != 2595) {
    // std::cout << "TypeKey: " <<TypeTable(type_key) << std::endl;
    if (type_key == MFM_MERGE_EN_FRAME_TYPE ||
        type_key == MFM_MERGE_TS_FRAME_TYPE) {
      MergeFrame->SetAttributs(fFrame->GetPointHeader());
      unsigned int nbInsideFrame = MergeFrame->GetNbItems();
      for (unsigned int i = 0; i < nbInsideFrame; i++) {
        MergeFrame->ReadInFrame(InsideFrame.get());
        int subtype_key = InsideFrame->GetFrameType();
        if (subtype_key != MFM_COMMON_FRAME_TYPE) {
          app->BuildRawEvent("mfm", "", InsideFrame.get());
        }
      }
    } else {
      app->BuildRawEvent("mfm", "", fFrame.get());
    }
  }
  m_CurrentEntry++;
}

////////////////////////////////////////////////////////////////////////////////
std::string MFMInput::TypeTable(int type_key) {
  std::string FrameType = "";
  if (type_key == 0x00) {
    FrameType = "MFM_COMMON_FRAME_TYPE";
  } else if (type_key == 0x01) {
    FrameType = "MFM_COBO_FRAME_TYPE";
  } else if (type_key == 0x02) {
    FrameType = "MFM_COBOF_FRAME_TYPE";
  } else if (type_key == 0x07) {
    FrameType = "MFM_COBOT_FRAME_TYPE";
  } else if (type_key == 0x08) {
    FrameType = "MFM_MUTANT_FRAME_TYPE";
  } else if (type_key == 0x09) {
    FrameType = "MFM_MUTANT1_FRAME_TYPE";
  } else if (type_key == 0x0A) {
    FrameType = "MFM_MUTANT2_FRAME_TYPE";
  } else if (type_key == 0x0B) {
    FrameType = "MFM_MUTANT3_FRAME_TYPE";
  } else if (type_key == 0x10) {
    FrameType = "MFM_EXO2_FRAME_TYPE";
  } else if (type_key == 0x11) {
    FrameType = "MFM_OSCI_FRAME_TYPE";
  } else if (type_key == 0x12) {
    FrameType = "MFM_NEDA_FRAME_TYPE";
  } else if (type_key == 0x13) {
    FrameType = "MFM_NEDACOMP_FRAME_TYPE";
  } else if (type_key == 0x14) {
    FrameType = "MFM_VAMOSIC_FRAME_TYPE";
  } else if (type_key == 0x15) {
    FrameType = "MFM_VAMOSPD_FRAME_TYPE";
  } else if (type_key == 0x16) {
    FrameType = "MFM_DIAMANT_FRAME_TYPE";
  } else if (type_key == 0x17) {
    FrameType = "MFM_S3_BAF2_FRAME_TYPE";
  } else if (type_key == 0x18) {
    FrameType = "MFM_S3_ALPHA_FRAME_TYPE";
  } else if (type_key == 0x19) {
    FrameType = "MFM_S3_RUTH_FRAME_TYPE";
  } else if (type_key == 0x1A) {
    FrameType = "MFM_S3_EGUN_FRAME_TYPE";
  } else if (type_key == 0x1B) {
    FrameType = "MFM_S3_SYNC_FRAME_TYPE";
  } else if (type_key == 0x1C) {
    FrameType = "MFM_REA_GENE_FRAME_TYPE";
  } else if (type_key == 0x1D) {
    FrameType = "MFM_VAMOSTAC_FRAME_TYPE";
  } else if (type_key == 0x1E) {
    FrameType = "MFM_BOX_DIAG_FRAME_TYPE";
  } else if (type_key == 0x20) {
    FrameType = "MFM_EBY_EN_FRAME_TYPE";
  } else if (type_key == 0x21) {
    FrameType = "MFM_EBY_TS_FRAME_TYPE";
  } else if (type_key == 0x22) {
    FrameType = "MFM_EBY_EN_TS_FRAME_TYPE";
  } else if (type_key == 0x23) {
    FrameType = "MFM_MATACQ_FRAME_TYPE";
  } else if (type_key == 0x24) {
    FrameType = "MFM_SCALER_DATA_FRAME_TYPE";
  } else if (type_key == 0x30) {
    FrameType = "MFM_RIBF_DATA_FRAME_TYPE";
  } else if (type_key == 0x40) {
    FrameType = "MFM_FAZIA_DATA_FRAME_TYPE";
  } else if (type_key == 0x60) {
    FrameType = "MFM_CHIMERA_DATA_FRAME_TYPE";
  } else if (type_key == 0x70) {
    FrameType = "MFM_SIRIUS_FRAME_TYPE";
  } else if (type_key == 0x71) {
    FrameType = "MFM_REA_TRACE_FRAME_TYPE";
  } else if (type_key == 0x80) {
    FrameType = "MFM_S3_DEFLECTOR_FRAME_TYPE";
  } else if (type_key == 0x90) {
    FrameType = "MFM_PARIS_FRAME_TYPE";
  } else if (type_key == 0xFF00) {
    FrameType = "MFM_HELLO_FRAME_TYPE";
  } else if (type_key == 0xFF01) {
    FrameType = "MFM_MERGE_EN_FRAME_TYPE";
  } else if (type_key == 0xFF02) {
    FrameType = "MFM_MERGE_TS_FRAME_TYPE";
  } else if (type_key == 0x4ADF) {
    FrameType = "MFM_MESYTEC_FRAME_TYPE";
  } else if (type_key == 0xFF10) {
    FrameType = "MFM_XML_DATA_DESCRIPTION_FRAME_TYPE";
  } else if (type_key == 0xFF11) {
    FrameType = "MFM_XML_FILE_HEADER_FRAME_TYPE";
  } else if (type_key == 0xFF11) {
    FrameType = "MFM_MAX_TYPE";
  } else {
    FrameType = "UNKOWN FRAME TYPE!!!!!!!!";
  }

  return FrameType;
}

////////////////////////////////////////////////////////////////////////////////
extern "C" {
std::shared_ptr<nptool::VDataInput> ConstructDataInput() {
  return std::make_shared<MFMInput>();
};
}
