#include "LK.h"

/*
   The ReadParameters function reads the name of a parameter file from standard
   input and reads the problem parameters from this file.

   The order of specifications in the file is arbitrary.

   The format is as follows: 

   PROBLEM_FILE = <string>
   Specifies the name of the problem file.

   Additional control information may be supplied in the following format:

   RUNS = <integer>
   The total number of runs. 
   Default: 10.

   MAX_TRIALS = <integer>
   The maximum number of trials in each run. 
   Default: number of nodes (DIMENSION, given in the problem file).

   TOUR_FILE = <string>
   Specifies the name of a file to which the best tour is to be written.

   OPTIMUM = <real>
   Known optimal tour length. A run will be terminated as soon as a tour 
   length less than or equal to optimum is achieved.
   Default: -DBL_MAX.

   MAX_CANDIDATES = <integer> { SYMMETRIC }
   The maximum number of candidate edges to be associated with each node.
   The integer may be followed by the keyword SYMMETRIC, signifying that 
   the candidate set is to be complemented such that every candidate edge 
   is associated with both its two end nodes. 
   Default: 5.

   ASCENT_CANDIDATES = <integer>
   The number of candidate edges to be associated with each node during 
   the ascent. The candidate set is complemented such that every candidate 
   edge is associated with both its two end nodes.
   Default: 50.

   COMMENT : <string>
   A comment.

   EOF
   Terminates the input data. The entry is optional.

   EXCESS = <real>
   The maximum alpha-value allowed for any candidate edge is set to EXCESS 
   times the absolute value of the lower bound of a solution tour (determined 
   by the ascent).
   Default: 1.0/DIMENSION.

   INITIAL_PERIOD = <integer>
   The length of the first period in the ascent.
   Default: DIMENSION/2 (but at least 100). 

   INITIAL_STEP_SIZE = <integer>
   The initial step size used in the ascent.
   Default: 1.

   PI_FILE = <string>
   Specifies the name of a file to which penalties (pi-values determined 
   by the ascent) is to be written. If the file already exits, the penalties 
   are read from the file, and the ascent is skipped. 

   PRECISION = <integer>
   The internal precision in the representation of transformed distances: 
   d[i][j] = PRECISION*c[i][j] + pi[i] + pi[j], where d[i][j], c[i][j], 
   pi[i] and pi[j] are all integral. 
   Default: 100 (which corresponds to 2 decimal places).

   SEED = <integer>
   Specifies the initial seed for random number generation.
   Default: 1.

   SUBGRADIENT: [ YES | NO ]
   Specifies whether the pi-values should be determined by subgradient 
   optimization.
   Default: YES.

   INPUT_TOUR_FILE = <string>
   Specifies the name of a file containing a tour. The tour is given by a 
   list of integers giving the sequence in which the nodes are visited in 
   the tour. The tour is terminated by a -1. The tour is used to limit the 
   search (the last edge to be removed in a non-gainful move must not belong to 
   the tour). In addition, the Alpha field of its edges is set to zero.

   MOVE_TYPE = <integer>
   Specifies the move type to be used in local search. The value can be 
   2, 3, 4 or 5 which signifies that a 2-opt, 3-opt, 4-opt or 5-opt move 
   is to be used.
   Default: 5.  

   TRACE_LEVEL = <integer>
   Specifies the level of detail of the output given during the solution 
   process. The value 0 signifies a minimum amount of output. The higher 
   the value is the more information is given.
   Default: 1.                   
 */

static char Delimiters[] = " =\n\t\r\f\v";
static char *GetFileName(char *Line);

void ReadParameters()
{
    char *Line, *Keyword, *Token;
    int i;

    ProblemFileName = PiFileName = InputTourFileName = TourFileName = 0;
    Runs = 10;
    Seed = 1;
    MaxTrials = 0;
    MaxCandidates = 5;
    AscentCandidates = 50;
    Optimum = -DBL_MAX;
    InitialPeriod = InitialStepSize = 0;
    Precision = 100;
    Subgradient = 1;
    Excess = 0.0;
    TraceLevel = 1;
    MoveType = 5;

    printf("PARAMETER_FILE = ");
    fflush(stdout);
    if (!(ParameterFileName = GetFileName(ReadLine(stdin)))) {
        printf("PROBLEM_FILE = ");
        fflush(stdout);
        ProblemFileName = GetFileName(ReadLine(stdin));
        return;
    }
    if (!(ParameterFile = fopen(ParameterFileName, "r")))
        eprintf("Cannot open %s", ParameterFileName);
    while (Line = ReadLine(ParameterFile)) {
        if (!(Keyword = strtok(Line, Delimiters)))
            continue;
        for (i = 0; i < strlen(Keyword); i++)
            Keyword[i] = (char) toupper(Keyword[i]);
        if (!strcmp(Keyword, "ASCENT_CANDIDATES")) {
            if (sscanf(strtok(0, Delimiters), "%ld", &AscentCandidates)) {
                if (AscentCandidates <= 0)
                    eprintf("(ASCENT_CANDIDATES): positive integer expected");
            }
        } else if (!strcmp(Keyword, "COMMENT"));
        else if (!strcmp(Keyword, "EOF"))
            break;
        else if (!strcmp(Keyword, "EXCESS")) {
            if (!sscanf(strtok(0, Delimiters), "%lf", &Excess))
                eprintf("(EXCESS): real expected");
        } else if (!strcmp(Keyword, "INITIAL_PERIOD")) {
            if (sscanf(strtok(0, Delimiters), "%ld", &InitialPeriod)) {
                if (InitialPeriod <= 0)
                    eprintf("(INITIAL_PERIOD): positive integer expected");
            } else
                eprintf("(INITIAL_PERIOD): integer expected");
        } else if (!strcmp(Keyword, "INITIAL_STEP_SIZE")) {
            if (sscanf(strtok(0, Delimiters), "%ld", &InitialStepSize)) {
                if (InitialStepSize <= 0)
                    eprintf("(INITIAL_STEP_SIZE): positive integer expected");
            } else
                eprintf("(INITIAL_STEP_SIZE): integer expected");
        } else if (!strcmp(Keyword, "INPUT_TOUR_FILE")) {
            if (!(InputTourFileName = GetFileName(0)))
                eprintf("(INPUT_TOUR_FILE): string expected");
        } else if (!(strcmp(Keyword, "MAX_CANDIDATES"))) {
            if (sscanf(strtok(0, Delimiters), "%ld", &MaxCandidates)) {
                if (MaxCandidates <= 0)
                    eprintf("(MAX_CANDIDATES): positive integer expected");
                if (Token = strtok(0, Delimiters)) {
                    for (i = 0; i < strlen(Token); i++)
                        Token[i] = (char) toupper(Token[i]);
                    if (!strcmp(Token, "SYMMETRIC"))
                        CandidateSetSymmetric = 1;
                    else
                        eprintf("(MAXCANDIDATES) illegal symmetry specification");
                }
            }
        } else if (!strcmp(Keyword, "MAX_TRIALS")) {
            if (sscanf(strtok(0, Delimiters), "%ld", &MaxTrials)) {
                if (MaxTrials <= 0)
                    eprintf("(MAX_TRIALS): positive integer expected");
            } else
                eprintf("(MAX_TRIALS): integer expected");
        } else if (!strcmp(Keyword, "MOVE_TYPE")) {
            if (!sscanf(strtok(0, Delimiters), "%d", &MoveType))
                eprintf("(MOVE_TYPE): integer expected");
            if (MoveType < 2 || MoveType > 5)
                eprintf("(MOVE_TYPE): 2, 3, 4 or 5 expected");
        } else if (!strcmp(Keyword, "OPTIMUM")) {
            if (!sscanf(strtok(0, Delimiters), "%lf", &Optimum))
                eprintf("(OPTIMUM): real expected");
            Optimum = floor(Optimum + 0.5);
        } else if (!strcmp(Keyword, "PI_FILE")) {
            if (!(PiFileName = GetFileName(0)))
                eprintf("(PI_FILE): string expected");
        } else if (!strcmp(Keyword, "PRECISION")) {
            if (!sscanf(strtok(0, Delimiters), "%ld", &Precision))
                eprintf("(PRECISION): integer expected");
        } else if (!strcmp(Keyword, "PROBLEM_FILE")) {
            if (!(ProblemFileName = GetFileName(0)))
                eprintf("(PROBLEM_FILE): string expected");
        } else if (!strcmp(Keyword, "RUNS")) {
            if (sscanf(strtok(0, Delimiters), "%ld", &Runs)) {
                if (Runs <= 0)
                    eprintf("(RUNS): positive integer expected");
            } else
                eprintf("(RUNS): integer expected");
        } else if (!strcmp(Keyword, "SEED")) {
            if (!sscanf(strtok(0, Delimiters), "%u", &Seed))
                eprintf("(SEED): integer expected");
        } else if (!strcmp(Keyword, "SUBGRADIENT")) {
            if (Token = strtok(0, Delimiters)) {
                for (i = 0; i < strlen(Token); i++)
                    Token[i] = (char) toupper(Token[i]);
                if (!strcmp(Token, "YES"))
                    Subgradient = 1;
                else if (!strcmp(Token, "NO"))
                    Subgradient = 0;
                else
                    Token = 0;
            }
            if (!Token)
                eprintf("(SUBGRADIENT): YES or NO expected");
        } else if (!strcmp(Keyword, "TOUR_FILE")) {
            if (!(TourFileName = GetFileName(0)))
                eprintf("(TOUR_FILE): string expected");
        } else if (!strcmp(Keyword, "TRACE_LEVEL")) {
            if (!sscanf(strtok(0, Delimiters), "%d", &TraceLevel))
                eprintf("(TRACE_LEVEL): integer expected");
        } else
            eprintf("Unknown Keyword: %s", Keyword);
    }
    if (!ProblemFileName)
        eprintf("Problem file name is missing.");
    fclose(ParameterFile);
}

static char *
 GetFileName(char *Line)
{
    char *Rest = strtok(Line, "\n\t\r\f"), *t;
    if (!Rest)
        return 0;
    while (*Rest == ' ' || *Rest == '=')
        Rest++;
    for (t = Rest + strlen(Rest) - 1; *t == ' '; t--)
        *t = '\0';
    if (!strlen(t))
        return 0;
    assert(t = (char *) malloc(strlen(Rest) + 1));
    strcpy(t, Rest);
    return t;
}
