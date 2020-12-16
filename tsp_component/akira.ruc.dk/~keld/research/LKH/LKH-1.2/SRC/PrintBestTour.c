#include "LK.h"

/*
   The PrintBestTour function prints the tour in TSPLIB format to the file 
   specified by TourFileName. 

   Nothing happens if TourFileName is 0.        
 */

void PrintBestTour()
{
    Node *N;

    if (TourFileName == 0)
        return;
    assert(TourFile = fopen(TourFileName, "w"));
    fprintf(TourFile, "NAME : %s.tour\n", Name);
    fprintf(TourFile, "COMMENT : Length = %0.0f\n", BestCost);
    fprintf(TourFile, "TYPE : TOUR\n");
    fprintf(TourFile, "DIMENSION : %ld\n", Dimension);
    fprintf(TourFile, "TOUR_SECTION  \n");
    N = FirstNode;
    do
        fprintf(TourFile, "%ld\n", N->Id);
    while ((N = N->BestSuc) != FirstNode);
    fprintf(TourFile, "-1\nEOF\n");
    fclose(TourFile);
}
