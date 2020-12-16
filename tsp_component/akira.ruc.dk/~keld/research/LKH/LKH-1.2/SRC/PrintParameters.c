#include "LK.h"

/*
   The PrintParameters function prints the problem parameters to standard output. 
 */

void PrintParameters()
{
    printf("\nPARAMETER_FILE = %s\n", ParameterFileName ? ParameterFileName : "");
    printf("PROBLEM_FILE = %s\n", ProblemFileName ? ProblemFileName : "");
    printf("PI_FILE = %s\n", PiFileName ? PiFileName : "");
    printf("TOUR_FILE = %s\n", TourFileName ? TourFileName : "");
    printf("RUNS = %ld\n", Runs);
    printf("MAX_TRIALS = %ld\n", MaxTrials);
    printf("MAX_CANDIDATES = %ld", MaxCandidates);
    if (CandidateSetSymmetric)
        printf(" SYMMETRIC");
    printf("\n");
    if (Optimum == -DBL_MAX)
        printf("OPTIMUM = -DBL_MAX\n");
    else
        printf("OPTIMUM = %0.0f\n", Optimum);
    printf("SEED = %u\n", Seed);
    printf("PRECISION = %ld\n", Precision);
    printf("EXCESS = %0.6f\n", Excess);
    printf("SUBGRADIENT = %s\n", Subgradient ? "YES" : "NO");
    printf("INITIAL_STEP_SIZE = %ld\n", InitialStepSize);
    printf("INITIAL_PERIOD = %ld\n", InitialPeriod);
    printf("ASCENT_CANDIDATES = %ld\n", AscentCandidates);
    printf("INPUT_TOUR_FILE = %s\n", InputTourFileName ? InputTourFileName : "");
    printf("MOVE_TYPE = %d\n", MoveType);
    printf("TRACE_LEVEL = %d\n", TraceLevel);
}
