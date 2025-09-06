rootl NPRaw_501_0.root NPRaw_502_0.root NPRaw_503_0.root NPRaw_509_0.root NPRaw_510_0.root NPRaw_511_0.root NPRaw_512_0.root NPRaw_514_0.root NPRaw_519_0.root NPRaw_522_0.root NPRaw_523_0.root

TFile *outFile = new TFile("Eff_CHIO_merging_window.root", "Update");
string hist_name_array[11] = {"h1","h2","h3","h4","h5","h6","h7","h8","h9","h10","h11"};
string hist_name_array2[11] = {"he1","he2","he3","he4","he5","he6","he7","he8","he9","he10","he11"};
for (int set = 0; set < 3; ++set) {
        // Create a directory (branch) for each set
        std::string dirName = std::to_string(set + 3)+ "us" ;
        TDirectory *setDir = outFile->mkdir(dirName.c_str());
        }
outFile->cd("3us");

TTree *tr[11];

tr[0] = (TTree*)_file0->Get("RawTree");
tr[1] = (TTree*)_file1->Get("RawTree");
tr[2] = (TTree*)_file2->Get("RawTree");
tr[3] = (TTree*)_file3->Get("RawTree");
tr[4] = (TTree*)_file4->Get("RawTree");
tr[5] = (TTree*)_file5->Get("RawTree");
tr[6] = (TTree*)_file6->Get("RawTree");
tr[7] = (TTree*)_file7->Get("RawTree");
tr[8] = (TTree*)_file8->Get("RawTree");
tr[9] = (TTree*)_file9->Get("RawTree");
tr[10] = (TTree*)_file10->Get("RawTree");


tr[0]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[0]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[0].c_str());
hictcp->SetName(hist_name_array2[0].c_str());
outFile->cd("3us");
htac->Write();
hictcp->Write();

tr[1]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[1]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[1].c_str());
hictcp->SetName(hist_name_array2[1].c_str());
outFile->cd("3us");
htac->Write();
hictcp->Write();

tr[2]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[2]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[2].c_str());
hictcp->SetName(hist_name_array2[2].c_str());
outFile->cd("3us");
htac->Write();
hictcp->Write();

tr[3]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[3]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[3].c_str());
hictcp->SetName(hist_name_array2[3].c_str());
outFile->cd("3us");
htac->Write();
hictcp->Write();

tr[4]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[4]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[4].c_str());
hictcp->SetName(hist_name_array2[4].c_str());
outFile->cd("3us");
htac->Write();
hictcp->Write();

tr[5]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[5]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[5].c_str());
hictcp->SetName(hist_name_array2[5].c_str());
outFile->cd("3us");
htac->Write();
hictcp->Write();

tr[6]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[6]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[6].c_str());
hictcp->SetName(hist_name_array2[6].c_str());
outFile->cd("3us");
htac->Write();
hictcp->Write();

tr[7]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[7]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[7].c_str());
hictcp->SetName(hist_name_array2[7].c_str());
outFile->cd("3us");
htac->Write();
hictcp->Write();

tr[8]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[8]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[8].c_str());
hictcp->SetName(hist_name_array2[8].c_str());
outFile->cd("3us");
htac->Write();
hictcp->Write();

tr[9]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[9]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[9].c_str());
hictcp->SetName(hist_name_array2[9].c_str());
outFile->cd("3us");
htac->Write();
hictcp->Write();

tr[10]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[10]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[10].c_str());
hictcp->SetName(hist_name_array2[10].c_str());
outFile->cd("3us");
htac->Write();
hictcp->Write();


outFile->Close();






/************************************4us*********************************** */

TFile *outFile = new TFile("Eff_CHIO_merging_window.root", "Update");
string hist_name_array[11] = {"h1","h2","h3","h4","h5","h6","h7","h8","h9","h10","h11"};
string hist_name_array2[11] = {"he1","he2","he3","he4","he5","he6","he7","he8","he9","he10","he11"};

outFile->cd("4us");

TTree *tr[11];

tr[0] = (TTree*)_file0->Get("RawTree");
tr[1] = (TTree*)_file1->Get("RawTree");
tr[2] = (TTree*)_file2->Get("RawTree");
tr[3] = (TTree*)_file3->Get("RawTree");
tr[4] = (TTree*)_file4->Get("RawTree");
tr[5] = (TTree*)_file5->Get("RawTree");
tr[6] = (TTree*)_file6->Get("RawTree");
tr[7] = (TTree*)_file7->Get("RawTree");
tr[8] = (TTree*)_file8->Get("RawTree");
tr[9] = (TTree*)_file9->Get("RawTree");
tr[10] = (TTree*)_file10->Get("RawTree");


tr[0]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[0]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[0].c_str());
hictcp->SetName(hist_name_array2[0].c_str());
outFile->cd("4us");
htac->Write();
hictcp->Write();

tr[1]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[1]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[1].c_str());
hictcp->SetName(hist_name_array2[1].c_str());
outFile->cd("4us");
htac->Write();
hictcp->Write();

tr[2]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[2]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[2].c_str());
hictcp->SetName(hist_name_array2[2].c_str());
outFile->cd("4us");
htac->Write();
hictcp->Write();

tr[3]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[3]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[3].c_str());
hictcp->SetName(hist_name_array2[3].c_str());
outFile->cd("4us");
htac->Write();
hictcp->Write();

tr[4]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[4]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[4].c_str());
hictcp->SetName(hist_name_array2[4].c_str());
outFile->cd("4us");
htac->Write();
hictcp->Write();

tr[5]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[5]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[5].c_str());
hictcp->SetName(hist_name_array2[5].c_str());
outFile->cd("4us");
htac->Write();
hictcp->Write();

tr[6]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[6]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[6].c_str());
hictcp->SetName(hist_name_array2[6].c_str());
outFile->cd("4us");
htac->Write();
hictcp->Write();

tr[7]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[7]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[7].c_str());
hictcp->SetName(hist_name_array2[7].c_str());
outFile->cd("4us");
htac->Write();
hictcp->Write();

tr[8]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[8]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[8].c_str());
hictcp->SetName(hist_name_array2[8].c_str());
outFile->cd("4us");
htac->Write();
hictcp->Write();

tr[9]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[9]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[9].c_str());
hictcp->SetName(hist_name_array2[9].c_str());
outFile->cd("4us");
htac->Write();
hictcp->Write();

tr[10]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[10]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[10].c_str());
hictcp->SetName(hist_name_array2[10].c_str());
outFile->cd("4us");
htac->Write();
hictcp->Write();


outFile->Close();


/************************************5us*********************************** */

TFile *outFile = new TFile("Eff_CHIO_merging_window.root", "Update");
string hist_name_array[11] = {"h1","h2","h3","h4","h5","h6","h7","h8","h9","h10","h11"};
string hist_name_array2[11] = {"he1","he2","he3","he4","he5","he6","he7","he8","he9","he10","he11"};

outFile->cd("5us");

TTree *tr[11];

tr[0] = (TTree*)_file0->Get("RawTree");
tr[1] = (TTree*)_file1->Get("RawTree");
tr[2] = (TTree*)_file2->Get("RawTree");
tr[3] = (TTree*)_file3->Get("RawTree");
tr[4] = (TTree*)_file4->Get("RawTree");
tr[5] = (TTree*)_file5->Get("RawTree");
tr[6] = (TTree*)_file6->Get("RawTree");
tr[7] = (TTree*)_file7->Get("RawTree");
tr[8] = (TTree*)_file8->Get("RawTree");
tr[9] = (TTree*)_file9->Get("RawTree");
tr[10] = (TTree*)_file10->Get("RawTree");


tr[0]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[0]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[0].c_str());
hictcp->SetName(hist_name_array2[0].c_str());
outFile->cd("5us");
htac->Write();
hictcp->Write();

tr[1]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[1]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[1].c_str());
hictcp->SetName(hist_name_array2[1].c_str());
outFile->cd("5us");
htac->Write();
hictcp->Write();

tr[2]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[2]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[2].c_str());
hictcp->SetName(hist_name_array2[2].c_str());
outFile->cd("5us");
htac->Write();
hictcp->Write();

tr[3]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[3]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[3].c_str());
hictcp->SetName(hist_name_array2[3].c_str());
outFile->cd("5us");
htac->Write();
hictcp->Write();

tr[4]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[4]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[4].c_str());
hictcp->SetName(hist_name_array2[4].c_str());
outFile->cd("5us");
htac->Write();
hictcp->Write();

tr[5]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[5]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[5].c_str());
hictcp->SetName(hist_name_array2[5].c_str());
outFile->cd("5us");
htac->Write();
hictcp->Write();

tr[6]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[6]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[6].c_str());
hictcp->SetName(hist_name_array2[6].c_str());
outFile->cd("5us");
htac->Write();
hictcp->Write();

tr[7]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[7]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[7].c_str());
hictcp->SetName(hist_name_array2[7].c_str());
outFile->cd("5us");
htac->Write();
hictcp->Write();

tr[8]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[8]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[8].c_str());
hictcp->SetName(hist_name_array2[8].c_str());
outFile->cd("5us");
htac->Write();
hictcp->Write();

tr[9]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[9]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[9].c_str());
hictcp->SetName(hist_name_array2[9].c_str());
outFile->cd("5us");
htac->Write();
hictcp->Write();

tr[10]->Draw("fTAC_Time>>htac(16384,0,65536)","fTAC_Name==\"TAC_CATS_PL\"","");
tr[10]->Draw("ZDD.fZDD_IC_E[0]>>hictcp(1500,0,15000)","fTAC_Time>23000  && fTAC_Time<27000 && fTAC_Name==\"TAC_CATS_PL\"","");
htac->SetName(hist_name_array[10].c_str());
hictcp->SetName(hist_name_array2[10].c_str());
outFile->cd("5us");
htac->Write();
hictcp->Write();


outFile->Close();