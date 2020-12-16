#include <iostream>
#include <cstdio>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>

#include "data.h"

using namespace std;

vector < vector < double > > tab;
vector < vector < vector < int > > > items;

inline void dp_just_profit(const string &outputFileName, vector < int > vi) {
    
    int numItems = Data::getInstance().numItems;
    int cap = Data::getInstance().capacityOfKnapsack;
    
    tab.resize(2);
    tab[0].resize(cap + 1);
    tab[1].resize(cap + 1);
    
    int i, j;

    for(i = 0; i <= cap; i++) {
        tab[0][i] = 0;
    }

    for(i = 1; i <= numItems; i++) {
        for(j = 1; j<= cap; j++) {
            if(Data::getInstance().items[i].weight > j) {
                tab[1][j] = tab[0][j];
            }
            else {
                if(tab[0][j-Data::getInstance().items[i].weight] + Data::getInstance().items[i].profit > tab[0][j]) {
                    tab[1][j] = tab[0][j-Data::getInstance().items[i].weight] + Data::getInstance().items[i].profit;
                }
                else {
                    tab[1][j] = tab[0][j];
                }
            }
        }
        tab[0].swap(tab[1]);
    }
    
    char file[1000]; 
    for(int s = 1; s <= 10; ++s) {    
        sprintf(file, "%s_%02d.opt.kp.value", outputFileName.c_str(), s);
        ofstream fout(file);
        fout << fixed << setprecision(0) << tab[0][vi[s-1]] << endl;
        fout.close();
    }
}

inline void DP(const string &outputFileName) {
    
    int numItems = Data::getInstance().numItems;
    int cap = Data::getInstance().capacityOfKnapsack;
    
    tab.resize(2);
    tab[0].resize(cap + 1);
    tab[1].resize(cap + 1);
    
    items.resize(2);
    items[0].resize(cap + 1);
    items[1].resize(cap + 1); 
    
    int i, j;

    for(i = 0; i <= cap; i++) {
        tab[0][i] = 0;
        items[0][i].clear();
    }

    for(i = 1; i <= numItems; i++) {
        for(j = 1; j<= cap; j++) {
            if(Data::getInstance().items[i].weight > j) {
                tab[1][j] = tab[0][j];
                items[1][j] = items[0][j];
            }
            else {
                if(tab[0][j-Data::getInstance().items[i].weight] + Data::getInstance().items[i].profit > tab[0][j]) {
                    tab[1][j] = tab[0][j-Data::getInstance().items[i].weight] + Data::getInstance().items[i].profit;
                    items[1][j] = items[0][j-Data::getInstance().items[i].weight];
                    items[1][j].push_back(i);
                }
                else {
                    tab[1][j] = tab[0][j];
                    items[1][j] = items[0][j];                    
                }
            }
        }
        tab[0].swap(tab[1]);
        items[0].swap(items[1]);
    }
    
    int totalProfit = tab[0][cap];
    vector < int > stolenItems;
    
    for(i = 0; i < items[0][cap].size(); ++i) {
        stolenItems.push_back(items[0][cap][i]);
    }
    
    sort(stolenItems.begin(), stolenItems.end());
    
    ofstream fout(outputFileName.c_str());
    
    fout << fixed << setprecision(0) << totalProfit << endl;
        
    for(i = 0; i < (int)stolenItems.size(); ++i) {
        fout << stolenItems[i] << ' ';
    }
    fout << endl;
    
    fout.close();
}

inline void HEU_DP(const string &outputFileName, int optValue, int finalSlice) {
    
    int numItems = Data::getInstance().numItems;
    int cap = Data::getInstance().capacityOfKnapsack;
    
    int i, j;
    
    vector < pair < double, int > > vt;
    for(int i = 1; i <= numItems; ++i) {            
        double weight = Data::getInstance().items[i].weight;
        double profit = Data::getInstance().items[i].profit;
        int cityID = Data::getInstance().items[i].idCity;
        vt.push_back(make_pair(-profit/weight, i));
    }
    
    sort(vt.begin(), vt.end());
    
    vector < int > stolenItems;
    double totalWeight = 0.0;
    double totalProfit = 0.0;
    int pos = 0;
    
    for(int i = 0; i < (int)vt.size(); ++i) {        
        double weight = Data::getInstance().items[vt[i].second].weight;
        if(totalWeight + weight <= cap - finalSlice) {
            totalWeight += weight;
            totalProfit += Data::getInstance().items[vt[i].second].profit;
            stolenItems.push_back(vt[i].second);
        }
        else {
            pos = i;
            break;
        }
    }    
    
    cap = cap - totalWeight;    
    
    tab.resize(2);
    tab[0].resize(cap + 1);
    tab[1].resize(cap + 1);
    
    items.resize(2);
    items[0].resize(cap + 1);
    items[1].resize(cap + 1);    
    
    for(i = 0; i <= cap; i++) {
        tab[0][i] = 0;
        items[0][i].clear();
    }
      
    
    for(int k = pos; k < (int)vt.size(); k++) {
        
        i = vt[k].second;
        
        for(j = 1; j<= cap; j++) {
            if(Data::getInstance().items[i].weight > j) {
                tab[1][j] = tab[0][j];
                items[1][j] = items[0][j];
            }
            else {
                if(tab[0][j-Data::getInstance().items[i].weight] + Data::getInstance().items[i].profit > tab[0][j]) {
                    tab[1][j] = tab[0][j-Data::getInstance().items[i].weight] + Data::getInstance().items[i].profit;
                    items[1][j] = items[0][j-Data::getInstance().items[i].weight];
                    items[1][j].push_back(i);
                }
                else {
                    tab[1][j] = tab[0][j];
                    items[1][j] = items[0][j];                    
                }
            }
        }
        tab[0].swap(tab[1]);
        items[0].swap(items[1]);
    }
    
    totalProfit += tab[0][cap];
    
    if(totalProfit != optValue) {
        clog << fixed << setprecision(0) << totalProfit << ' ' << optValue << endl;
        return;
    }
    
    for(i = 0; i < items[0][cap].size(); ++i) {
        stolenItems.push_back(items[0][cap][i]);
    }
    
    sort(stolenItems.begin(), stolenItems.end());
    
    ofstream fout(outputFileName.c_str());
    
    fout << fixed << setprecision(0) << totalProfit << endl;
        
    for(i = 0; i < (int)stolenItems.size(); ++i) {
        fout << stolenItems[i] << endl;
    }
    
    fout.close();
}


vector < int > getSizes(string inputFileName) {
    
    vector < int > vi;
    
    char file[1000];
 
    for(int s = 1; s <= 10; ++s) {
    
        sprintf(file, "%s_%02d.ttp", inputFileName.c_str(), s);
        
        string line;
        int capacityOfKnapsack;
        ifstream fin(file);

        if(!fin) {
            clog << "ERROR!" << endl;
            clog << file << endl;
            exit(0);
        }
        
        getline(fin, line); // PROBLEM NAME:    a280-ThOP
        getline(fin, line); // KNAPSACK DATA TYPE: bounded strongly corr
        getline(fin, line); // DIMENSION:  280
        getline(fin, line); // NUMBER OF ITEMS:    279
        getline(fin, line); // CAPACITY OF KNAPSACK:   25936
        for(unsigned j=0; j < line.length(); ++j) {
            if(line[j] < '0' || line[j] > '9') line[j] = ' ';
        }
        stringstream ss;
        ss.clear();
        ss << line;
        ss >> capacityOfKnapsack;
        vi.push_back(capacityOfKnapsack);
        clog << capacityOfKnapsack << endl;
    }
    
    sort(vi.begin(), vi.end());
    
    return vi;
}


int main(int argc, char **argv) {
    
    if(argc == 3) {        
        string instanceFileName = argv[1]; 
        string outputFileName = argv[2];        
        Data::getInstance().readData(instanceFileName + "_10.ttp");    
        dp_just_profit(outputFileName, getSizes(instanceFileName));
    }
    else if(argc == 5) {
        string instanceFileName = argv[1]; 
        string optValueFileName = argv[2];
        int optValue, finalSlice; 
        ifstream fin(optValueFileName.c_str());
        fin >> optValue;
        fin.close();
        string outputFileName = argv[3];    
        sscanf(argv[4], "%d", &finalSlice);      
        Data::getInstance().readData(instanceFileName);         
        HEU_DP(outputFileName, optValue, finalSlice);        
    }
    
    return 0;
}
