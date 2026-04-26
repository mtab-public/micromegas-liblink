
/* Include required header */
#include "WolframLibrary.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "uuid/uuid.h"

/* Return the version of Library Link */
DLLEXPORT mint WolframLibrary_getVersion( ) {
    return WolframLibraryVersion;
}

/* Initialize Library */
DLLEXPORT int WolframLibrary_initialize( WolframLibraryData libData) {
    return LIBRARY_NO_ERROR;
}

/* Uninitialize Library */
DLLEXPORT void WolframLibrary_uninitialize( WolframLibraryData libData) {
    return;
}

/* Calls micrOMEGAs for complex variables, returns the result */
/* DOES NOT USE ANY PERMANENT I/O */
DLLEXPORT int call_micromegas_c_no_io( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res) {

    MTensor T0;
    T0 = MArgument_getMTensor(Args[0]);

    /* Now create the input file via process substitution for micrOMEGAs */
    char *micromegas_input = NULL;

    micromegas_input = (char*)calloc(1000, sizeof(char));

    /* Begin string for process substitution */
    strcat(micromegas_input, "<( ");

    /* Define model parameters */
    char *model_params[] = {
        "Rel11",
        "Rel12",
        "Rel13",
        "Rel21",
        "Rel22",
        "Rel23",
        "Rel31",
        "Rel32",
        "Rel33",
        "Iml11",
        "Iml12",
        "Iml13",
        "Iml21",
        "Iml22",
        "Iml23",
        "Iml31",
        "Iml32",
        "Iml33",
        "Mchi",
        "mphi1",
        "mphi2",
        "mphi3",
        "lH1",
        "lH2",
        "lH3"
    };


    /* Assign each model parameter the corresponding value from the input tensor */
    mreal tmp;
    mint idx;
    for (idx = 1; idx < 26; ++idx){
        libData->MTensor_getReal(T0, &idx, &tmp);
        
        strcat(micromegas_input, "echo ");
        strcat(micromegas_input, model_params[idx - 1]);
        sprintf(micromegas_input + strlen(micromegas_input), " %f", tmp);
        strcat(micromegas_input, "; ");

    }

    /* End process substitution */
    strcat(micromegas_input, ")");
//    printf("\n\n%s\n\n", micromegas_input);


    /* Now start micrOMEGAs with the just created input file */
    char *cmd_line = NULL;
    cmd_line = (char*)calloc(1500, sizeof(char));

    /* get TMP dir where micrOMEGAs resides in order to save nfs access on cluster usage */
    const char *temp_dir = getenv("TMP");

    strcat(cmd_line, "/bin/bash -c '");
    strcat(cmd_line, temp_dir);
    strcat(cmd_line, "/micromegas/dmfv/main_dmfv ");
    strcat(cmd_line, micromegas_input);
    strcat(cmd_line, "' | grep Omega | awk -F '=' '{print $3}'");
//    printf("\n\n%s\n\n", cmd_line);


    /* Create a pipe in order to read the result from stdout */
    FILE *fp;
    char path[1535];

    fp = popen(cmd_line, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }


    /* Read the output line and assign it to the Mathematica return variable */
    if (fgets(path, sizeof(path), fp) != NULL) {
        MArgument_setReal(Res, atof(path));
    }


    /* Close */
    pclose(fp);

    return LIBRARY_NO_ERROR;

}
