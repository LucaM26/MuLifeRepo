#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// Mu meanlife estimate

int main() {

    ifstream FIFO("FIFOread_Take1.txt");

    vector<double> CHv, CLKv;

    double CH , CLK;

    TH1F *h = new TH1F("Decay Time", "Histogram MuLife", 100, -1, 25);

    while (FIFO >> CH >> CLK){

        CHv.push_back(CH);

        CLKv.push_back(CLK);

    }

    int lenCH = CHv.size();

    int lenCLK = CLKv.size();

    double a = 4.98892e-3;


    for (int i = 1 ; i < lenCLK ; i++){

        CLKv[i] *= a;

    }

    for (int i = 1 ; i < lenCLK ; i++){

        if (CHv[i] == 1){

            double t_start = CLKv[i];

            int j = i + 1;

            while (j < lenCH && CHv[j] == 1) {

                j++;

            }

            if (j < lenCH && CHv[j] == 2){

                double tau = CLKv[j] - CLKv[i];

                h->Fill(tau);

            }

            i = j+1;

        }

    }

    TCanvas* c = new TCanvas("c_decay", "Canvas Decay Time", 800, 600);

    h->GetXaxis()->SetTitle("Decay Time [us]");

    h->GetYaxis()->SetTitle("Counts [pure]");

    h->Draw();

    return 0;

}

// Calibration costant

int Calibration() {

    ifstream FIFO("FIFOread_Cal2.txt");

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

    return 0;

}

// Delay estimate

int Delay() {

    ifstream FIFO("FIFOread_CalDoppia.txt");

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

    return 0;
}

