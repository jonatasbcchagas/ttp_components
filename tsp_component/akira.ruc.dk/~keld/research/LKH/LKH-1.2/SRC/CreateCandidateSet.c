#include "LK.h"

/*
   The CreateCandidateSet function determines for each node its set of incident
   candidate edges.

   The Ascent function is called to determine a lower bound on the optimal tour 
   using subgradient optimization. But only if the penalties (the pi-values) is
   not available on file. In the latter case, the penalties is read from the file,
   and the lower bound is computed from a minimum 1-tree.      

   The function GenerateCandidates is called to compute the alpha-values and to 
   associate to each node a set of incident candidate edges.  

   The CreateCandidateSet function itself is called from LKmain.
 */

void CreateCandidateSet()
{
    double Cost;
    long i, Id;
    Node *Na, *Nb;
    double LastTime = GetTime();

    if (C == C_EXPLICIT) {
        Na = FirstNode;
        do {
            for (i = 1; i < Na->Id; i++)
                Na->C[i] *= Precision;
        }
        while ((Na = Na->Suc) != FirstNode);
    }
    if (PiFileName == 0 || (PiFile = fopen(PiFileName, "r")) == 0) {
        /* No PiFile specified or available */
        Cost = Ascent();
        if (PiFileName && (PiFile = fopen(PiFileName, "w"))) {
            Na = FirstNode;
            do
                fprintf(PiFile, "%ld %ld\n", Na->Id, Na->Pi);
            while ((Na = Na->Suc) != FirstNode);
            fclose(PiFile);
        }
    } else {
        /* Read the Pi-values from file */
        fscanf(PiFile, "%ld", &Id);
        FirstNode = Na = &NodeSet[Id];
        fscanf(PiFile, "%ld", &Na->Pi);
        for (i = 2; i <= Dimension; i++) {
            fscanf(PiFile, "%ld", &Id);
            Nb = &NodeSet[Id];
            fscanf(PiFile, "%ld", &Nb->Pi);
            Nb->Pred = Na;
            Na->Suc = Nb;
            Na = Nb;
        }
        FirstNode->Pred = Nb;
        Nb->Suc = FirstNode;
        fclose(PiFile);
        Cost = Minimum1TreeCost(0);
    }
    LowerBound = Cost / Precision;
    printf("\nLower bound = %0.1f, ", LowerBound);
    if (Optimum != -DBL_MAX && Optimum != 0)
        printf("Gap = %0.1f%%, ", 100 * (Optimum - LowerBound) / Optimum);
    printf("Ascent time = %0.0f sec.\n", GetTime() - LastTime);
    if (Norm == 0)
        return;
    GenerateCandidates(MaxCandidates, fabs(Excess * Cost), CandidateSetSymmetric);
    if (C == C_EXPLICIT) {
        Na = FirstNode;
        do {
            Nb = Na;
            while ((Nb = Nb->Suc) != FirstNode) {
                if (Na->Id > Nb->Id)
                    Na->C[Nb->Id] += Na->Pi + Nb->Pi;
                else
                    Nb->C[Na->Id] += Na->Pi + Nb->Pi;
            }
        }
        while ((Na = Na->Suc) != FirstNode);
    }
}
