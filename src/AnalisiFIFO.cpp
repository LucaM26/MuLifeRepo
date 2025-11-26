#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/*
*
*Meant to be used as root macros.
*DecayTime is an analysis program that basically does this : 
*(1) Imports a file (specified by a path given as an input) assumed to have to columns, that will be converted in CHv (it corresponds to
*the channel that was activated at a given time) and CLKv (the time in which the channel was triggered)
*(2) Loops over CHv until it finds 1 (START signal): if so it increments index until it finds a 2(STOP signal), if the difference is more 
*than 8 clock cycles, it is multiplied by a calibration constant (see Calibration) and registered in an histogram.
*(3) The histogram is then plotted, the user can work with it with various root utilities
*/

void DecayTime(const char* path) {

    ifstream FIFO(path);

    vector<double> CHv, CLKv;

    double CH , CLK;

    TH1F *h = new TH1F("Decay Time", "Histogram MuLife", 100, -1, 25);

    while (FIFO >> CH >> CLK){

        CHv.push_back(CH);

        CLKv.push_back(CLK);

    }

    int N = CLKv.size();

    double a = 4.98892e-3;

    for (int i = 1 ; i < N ; i++){

        if (CHv[i] == 1){

            int j = i+1;
            
            while (j < N && CHv[j] != 1){
                
                double diff = CLKv[j]-CLKv[i];

                if (CHv[j] == 2 && diff > 8){

                    h->Fill(a * diff);

                   break; 

                }

                else {

                    j++;

                }

            }

            i = j+1;

        }

    }

    TCanvas* c = new TCanvas("c_decay", "Canvas Decay Time", 800, 600);

    h->GetXaxis()->SetTitle("Decay Time [us]");

    h->GetYaxis()->SetTitle("Counts [pure]");

    h->Draw();

}

/*
*Meant to be used as root macro.
*I used this to estimate the calibration costant between physical time and clock cycles (it's the clock period). The workflow is the following:
*(1) Import works as above (see DecayTime)
*(2) Loops over CLKv, and calculates difference between two following clock fronts. It is saved inside an histogram.
*(3) The histogram is then plotted. The user can then make usage of various root utilities on it.
*(4) The calibration constant is estimated with reference period of the original signal measured in the Lab.
*/

void Calibration(const char* path) {

    ifstream FIFO(path);

    vector<double> CHv, CLKv;

    double CH , CLK;

    TH1F *Period = new TH1F("Period", "Histogram of period of calibration signal", 100 , 1.86e8 ,1.875e8 );

    while (FIFO >> CH >> CLK){

        CHv.push_back(CH);

        CLKv.push_back(CLK);

    }

    int lenCH = CHv.size();

    int lenCLK = CLKv.size();

    for (int i = 0; i < (lenCH-1) ; i++){

        if (CHv[i] == 1 && CHv[i+1] == 1){

            double T = CLKv[i+1] - CLKv[i];

            Period->Fill(T);

        }
        else{
            continue;
        }
    }

    TCanvas* c = new TCanvas("Period", "Canvas Period of calibration signal", 800, 600);

    Period->GetXaxis()->SetTitle("Period [digits]");

    Period->GetYaxis()->SetTitle("Counts [pure]");

    Period->Draw();

    double T_mean = Period->GetMean();

    double T_std = Period->GetRMS();

    double Entries = Period->GetEntries();

    double T_err = T_std/sqrt(Entries);

    double T_s = 0.932;

    double a = T_s/T_mean;

    double a_err = a * (T_err/T_mean);

    cout << a << "+/-" << a_err <<endl;

}

/*
*This is a bit usesless, but still...
*Meant to be used as root macro.
*It estimates the delay between two (presumably) synchronous square waves. The workflow is the following:
*(1) File import works as above (see DecayTime)
*(2) Loop calculates time difference if the two following signals come from 2 different channels, then saves difference;
*(3) The histogram is then plotted. The user can then make usage of various root utilities on it.
*/

void Delay(const char* path) {

    ifstream FIFO(path);

    vector<double> CHv, CLKv;

    double CH , CLK;

    TH1F *delay = new TH1F("Delay between 0 and 1", "Histogram of delay between channel", 10 , -2, 2);

    while (FIFO >> CH >> CLK){

        CHv.push_back(CH);

        CLKv.push_back(CLK);

    }

    int lenCH = CHv.size();

    int lenCLK = CLKv.size();

    for(int i = 0; i < lenCH ; i++){

        if (CHv[i] == 2 && CHv[i+1] == 1){

            double dt = CLKv[i]-CLKv[i+1];

            delay->Fill(dt);

        }
    }

    TCanvas* c = new TCanvas("Delay 1-0", "Canvas Delay Time between 1-0", 800, 600);

    delay->GetXaxis()->SetTitle("Delay Time [a.u.]");

    delay->GetYaxis()->SetTitle("Counts [pure]");

    delay->Draw();
}

