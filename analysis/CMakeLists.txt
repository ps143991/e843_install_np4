#parent analysis file
install(DIRECTORY ./ DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY} FILES_MATCHING PATTERN "*.h" )
add_library(NPUserAnalysis SHARED Analysis.h Analysis.cxx )
#non root analysis
target_link_libraries(NPUserAnalysis PUBLIC nptool )


# TAC analysis file
#target_link_libraries(NPUserAnalysis PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt ROOT::Hist ROOT::Gpad nptool )
add_library(NPUserAnalysisTAC SHARED Analysis_TAC.h Analysis_TAC.cxx )
target_link_libraries(NPUserAnalysisTAC PUBLIC nptool )

# BeamSpot Analysis file
#target_link_libraries(NPUserAnalysis PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt ROOT::Hist ROOT::Gpad nptool )
add_library(NPUserAnalysisBEAMSPOT SHARED Analysis_Beam_Spot.h Analysis_Beam_Spot.cxx )
target_link_libraries(NPUserAnalysisBEAMSPOT PUBLIC nptool )

# ZDD Analysis file
#target_link_libraries(NPUserAnalysis PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt ROOT::Hist ROOT::Gpad nptool )
add_library(NPUserAnalysisZDD SHARED Analysis_ZDD.h Analysis_ZDD.cxx )
target_link_libraries(NPUserAnalysisZDD PUBLIC nptool )
