#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

//function to see if labelname mismatches label in 2 files

    ifstream f1("/home/sharmap/Workplace/np4/e843/action/ACTIONS_e843_for_np4.txt", ios::in);
    ifstream f2("/home/sharmap/ACTIONS_zdd.CHC_PAR.run_0572.dat.05-06-23_16h01m50s", ios::in);
    int         Label1, c1, d1;
    int        Label2, c2, d2;
  std::string LabelName1, LabelName2;
  std::map<int, std::string> fLabelMap1, fLabelMap2;


  void load_maps(){
        while (f1 >> LabelName1 >> Label1 >> c1 >> d1) {
    fLabelMap1[Label1] = LabelName1;}
    while (f2 >> LabelName2 >> Label2 >> c2 >> d2) {
    fLabelMap2[Label2] = LabelName2;}
  }

int compare_Action_files() {

    if(!f1.is_open() || !f2.is_open()) {
        cout << "Error opening files!" << endl;
        return 1;
    }


    load_maps();


    for (const auto& pair : fLabelMap1) {
        int label = pair.first;
        const string& name1 = pair.second;

        auto it = fLabelMap2.find(label);
        if (it != fLabelMap2.end()) {
            const string& name2 = it->second;
            if (name1 != name2) {
                cout << "Mismatch for label " << label << ": "
                     << name1 << " (file1) vs " << name2 << " (file2)" << endl;
            }
        } else {
            cout << "Label " << label << " found in file1 but not in file2." << endl;
        }
    }
return 0;

}

int change_action_file() {
    
    load_maps();

    if(!f1.is_open()) {
        cout << "Error opening files!" << endl;
        return 1;
    }

    ofstream fout("Corrected_ACTIONS_file_e843.dat", ios::out);
    //increment of label by 1 in file1 when label is between 2 and 2305
    for (const auto& pair : fLabelMap1) {
        int label = pair.first;
        const string& name1 = pair.second;

        if(label >= 2 && label <= 2305) {
            label += 1; // increment label by 1
        }

        fout << name1 << "\t" << label << "\t" << c1 << "\t" << d1 << endl;
    }


    return 0;
}