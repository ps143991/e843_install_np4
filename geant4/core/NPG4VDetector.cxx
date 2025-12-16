#include "NPG4VDetector.h"

// Define the static member
// Required to avoid linker error
InteractionCoordinates* nptool::geant4::VDetector::ms_InterCoord = nullptr;

nptool::geant4::VDetector::VDetector() {
  if (ms_InterCoord == 0)
    ms_InterCoord = new InteractionCoordinates();

  nptool::Application* app = nptool::Application::GetApplication().get();
  for (auto& out : app->GetOutputs()) {
    auto p = out.second.lock();
    if (p)
      p->Attach("InteractionCoordinates",
                "InteractionCoordinates", &ms_InterCoord);
  }
}
