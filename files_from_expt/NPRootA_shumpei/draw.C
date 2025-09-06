TCanvas *cc = new TCanvas();
cc->cd();
ch->Draw("IC_ZDDRaw[0]>>h123(1000,100,20100)","TAC_CATS_PL>0","");
ch->Draw("IC_ZDDRaw[1]>>h124(1000,100,20100)","TAC_CATS_PL>0","same");
ch->Draw("IC_ZDDRaw[2]>>h125(1000,100,20100)","TAC_CATS_PL>0","same");
ch->Draw("IC_ZDDRaw[3]>>h126(1000,100,20100)","TAC_CATS_PL>0","same");
ch->Draw("IC_ZDDRaw[4]>>h127(1000,100,20100)","TAC_CATS_PL>0","same");



ch->Draw("IC_ZDDRaw[0]>>h128(1000,0,20000)","TAC_PL_1>0 || TAC_PL_2>0 || TAC_PL_3>0 || TAC_PL_4>0 || TAC_PL_5>0","");
ch->Draw("IC_ZDDRaw[1]>>h129(1000,0,20000)","TAC_PL_1>0 || TAC_PL_2>0 || TAC_PL_3>0 || TAC_PL_4>0 || TAC_PL_5>0","same");
ch->Draw("IC_ZDDRaw[2]>>h130(1000,0,20000)","TAC_PL_1>0 || TAC_PL_2>0 || TAC_PL_3>0 || TAC_PL_4>0 || TAC_PL_5>0","same");
ch->Draw("IC_ZDDRaw[3]>>h131(1000,0,20000)","TAC_PL_1>0 || TAC_PL_2>0 || TAC_PL_3>0 || TAC_PL_4>0 || TAC_PL_5>0","same");
ch->Draw("IC_ZDDRaw[4]>>h132(1000,0,20000)","TAC_PL_1>0 || TAC_PL_2>0 || TAC_PL_3>0 || TAC_PL_4>0 || TAC_PL_5>0","same");