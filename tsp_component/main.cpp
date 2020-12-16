#include <iostream>
#include <cstdio>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <fstream>

#include "data.h"

#define INF 987654321

extern "C" {
	#include <concorde.h>
}

using namespace std;

void usage() { 
    clog << "Usage: ./concorde --inputfile input_file_name" << endl;
    exit(0);
}

int calculateTotalDistance(int* tour, int len) {

    int totalDistance = 0;
    for(int i = 0; i < len; ++i) {
        totalDistance += Data::getInstance().getDistance(tour[i], tour[(i+1)%len]);
    }    
    return totalDistance;
}

void solveTSPUsingConcordeSolver() {
    
    string line;
    
    string fileName = Data::getInstance().problemName;
    fileName.erase(--fileName.end());
    fileName.erase(--fileName.end());
    fileName.erase(--fileName.end());
    fileName.erase(--fileName.end());
    fileName = "akira.ruc.dk/~keld/research/LKH/LKH-1.2/TOURS/" + fileName + ".opt.tour";
    
    vector < int > inTour;
    
    ifstream fin(fileName.c_str());
        
    if(fin) {
        clog << "\"" << fileName << "\" okay" << endl;
        vector < int > optTourEuc;    
        getline(fin, line);
        getline(fin, line);
        getline(fin, line);
        getline(fin, line);
        getline(fin, line);
        int tmp;
        while(fin >> tmp) {
            if(tmp == -1) break;
            optTourEuc.push_back(tmp);
        }        
        int p = 0;
        for(; p < (int)optTourEuc.size(); ++p) if(optTourEuc[p] == 1) break;        
        for(int i = p; i < (int)optTourEuc.size(); ++i) inTour.push_back(optTourEuc[i]);
        for(int i = 0; i < p; ++i) inTour.push_back(optTourEuc[i]);
    } 
    else {        
        fileName = Data::getInstance().problemName;
        fileName.erase(--fileName.end());
        fileName.erase(--fileName.end());
        fileName.erase(--fileName.end());
        fileName.erase(--fileName.end());
        fileName = "akira.ruc.dk/~keld/research/LKH/LKH-1.2/TOURS/" + fileName + ".nopt.tour"; 
      
        fin.open(fileName.c_str());
        
        if(fin) {
            clog << "\"" << fileName << "\" okay" << endl;
            vector < int > optTourEuc;    
            getline(fin, line);
            getline(fin, line);
            getline(fin, line);
            getline(fin, line);
            getline(fin, line);
            int tmp;
            while(fin >> tmp) {
                if(tmp == -1) break;
                optTourEuc.push_back(tmp);
            }        
            int p = 0;
            for(; p < (int)optTourEuc.size(); ++p) if(optTourEuc[p] == 1) break;        
            for(int i = p; i < (int)optTourEuc.size(); ++i) inTour.push_back(optTourEuc[i]);
            for(int i = 0; i < p; ++i) inTour.push_back(optTourEuc[i]);
        } 
        else {        
            clog << "\"" << fileName << "\" not found" << endl;
            for(int i = 0; i < Data::getInstance().numCities; i++) {
                inTour.push_back(i+1);
            } 
        }
    }
    
    if(Data::getInstance().numCities > 4 ){ //TSP only for more than 4 elements
    
        int rval = 0;
        int semente = rand();
        double szeit, optval, *mybnd, *mytimebound;
        int ncount, success, foundtour, hit_timebound = 0;
        
        int *in_tour = new int[inTour.size()];
        for(int i = 0; i < (int)inTour.size(); ++i) in_tour[i] = inTour[i] - 1;
        
        int *out_tour = (int *) NULL;
        CCrandstate rstate;
        char *probname = (char *) NULL;
        static int run_silently = 1;
        CCutil_sprand(semente, &rstate);
        mybnd = (double *) NULL;
        mytimebound = (double *) NULL;
        ncount = Data::getInstance().numCities;
        int ecount = (ncount * (ncount - 1)) / 2;
        int *elist = new int[ecount * 2];
        int *elen = new int[ecount];
        int edge = 0;
        int edgeWeight = 0;
        
        for(int i = 0; i < ncount; i++) {
            for(int j = i + 1; j < ncount; j++) {
                if(i != j) {
                    elist[edge] = i;
                    elist[edge + 1] = j;
                    elen[edgeWeight] = Data::getInstance().getDistance(i, j);
                    edgeWeight++;
                    edge = edge + 2;
                }
            }
        }
        
        out_tour = CC_SAFE_MALLOC (ncount, int);
        probname = CCtsp_problabel(" ");
        
        rval = CCtsp_solve_sparse(ncount, ecount, elist, elen, in_tour, out_tour, mybnd, &optval, &success, &foundtour, probname, mytimebound, &hit_timebound, run_silently, &rstate);

        string fileName = Data::getInstance().problemName;
        fileName.erase(--fileName.end());
        fileName.erase(--fileName.end());
        fileName.erase(--fileName.end());
        fileName.erase(--fileName.end());
            
        char output[1000];    
        sprintf(output, "tsp_opt_sols/%s.opt.tsp.sol", fileName.c_str()); 
        
        clog << output << endl;
        
        ofstream fout(output);    
    
        fout << "NAME : " << fileName <<  endl;
        fout << "COMMENT : " << " optimal TSP tour (length = " << calculateTotalDistance(out_tour, inTour.size()) << ")" << endl;
        fout << "TYPE : TOUR CEIL_2D" << endl;
        fout << "DIMENSION : " << Data::getInstance().numCities << endl;
        fout << "TOUR_SECTION" << endl;
        
        for(int i = 0; i < inTour.size(); ++i) {
            fout << out_tour[i]+1 << endl;
        }
        fout << "-1" << endl << "EOF" << endl;    
        fout.close();
        
        szeit = CCutil_zeit();
        CC_IFFREE (elist, int);
        CC_IFFREE (elen, int);
        CC_IFFREE (out_tour, int);
        CC_IFFREE (probname, char);
    }
}


int main(int argc, char **argv) {

    if(argc < 3) usage();
    
    char inputFileName[1000];
    
    int check_parameters = 0;
    for(int i = 1; i < argc; i += 2) {
        if(strcmp(argv[i], "--inputfile") == 0) {
            sscanf(argv[i+1],"%s", inputFileName);
            check_parameters += 1;
        }
    }
    
    if(check_parameters != 1) usage();
    
    Data::getInstance().readData(inputFileName); 
    
    solveTSPUsingConcordeSolver();

    return 0;
}
