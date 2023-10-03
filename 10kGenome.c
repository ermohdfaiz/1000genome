#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int b;

int main()
{
    
	/*if(argc < 4) {

		printf("Very few arguements passed\n");
		exit(1);
	}*/
    
    //long int alt_lines = 0, filter_lines = 0, 
    long int r_lines = 0;

    //int columns = 0;

    char *chrom = "22";

    float min = 10.0, max = 20.0;

    FILE *file = fopen("chr22.vcf", "r");
    FILE *smat = fopen("smat.dat", "r");
    FILE *gids = fopen("gids.txt", "r");
  
    if (file == NULL && smat == NULL) {

        printf("Unable to open the file\n");
        exit(1);
    }

    char arr[100];              // to store every rows alt and ref
    double sm[4][4];


    int itr = 0;
    int sm_i = 0;
    int sm_j = 0;
    float svalue = 0;

    char *gidm[3000];            // for storing given gids
    int gid_ind[3000];           // for storing gids index
    char gids_line[30];

    long columns = 0;

    b = 21000;
    char smat_line[100];
    char buffer_line[b];       //for storing a line from file
    char *token;               // for storing a word from buffer_line

    

    // storing the provided scoring matrix
    while (fgets(smat_line, sizeof(smat_line), smat)) {

        sm_j = 0;
        token = strtok(smat_line, " ");   
        while (token != NULL) {

            sm[sm_i][sm_j] = atof(token);
            sm_j++;
            token = strtok(NULL, " ");
        }
        sm_i++;

    }

    // to get values from scoring matrix
    float coordinate(int x, int y, int *arr)
    {
        char arr1[4] = {'A', 'C', 'G', 'T'}; 
        
        char a = arr[x];   
        char b = arr[y];    
        
        for (int i = 0; i < 4; i++) {
            if (a == arr1[i]) {
                x = i;
            }
            if (b == arr1[i]) {
                y = i;
            }
        }
        
        //printf("\n%i,%i ", x, y);
    }

        return sm[x][y];
    }

    // storing the gids
    sm_j = 0;
    while (fgets(gids_line, sizeof(gids_line), gids)) {

        gidm[sm_j] = (char *)malloc(12);
        strcpy(gidm[sm_j++], gids_line);
    }
    // sm_j tells us the number of gids in the given file

    //checking gids
    /*for (int i = 0; i < sm_j; i++) {
        printf("%s", gidm[i]);
    }
    printf("\n");*/

    // getting gids token index
    while (fgets(buffer_line, sizeof(buffer_line), file)) {
        
        int token_index = 1;
        if (buffer_line != NULL && (buffer_line[1] != 'C')) {
        
            continue;
        }

        b = strlen(buffer_line);
        b = (b > 12000) ? b : 12000;

        token = strtok(buffer_line, "\t");

        while (token != NULL) {

            columns++;
            if (token_index >= 10) {
                for (int i = 0; i < sm_j; i++) {
                    if (strncmp(gidm[i], token, 7) == 0) {
                        gid_ind[i] = token_index;
                        //printf("%i ", token_index);
                    }
                }
            }
            token_index++;
            token = strtok(NULL, "\t");

        }
        break;
    }
    

    //checking gids index
    /*for (int i = 0; i < sm_j; i++) {
        printf("%i ", gid_ind[i]);
    }
    printf("\n");*/

    // checking scoring matrix
    /*for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%lf ", sm[i][j]);
        }
        printf("\n");
    }
    */

    double matrix[2000][3000];
    int rows = 0;

    while (fgets(buffer_line, sizeof(buffer_line), file)) {

        int flag_alt = 1, flag_ref = 0, flag_filter = 1, total = 0, mutation_count = 0, token_index = 1;
        float percent;
        
    
        if (buffer_line != NULL && (buffer_line[0] == '#')) {
        
            continue;
        }

        for (int i = 0; i < 100; i++) {
            arr[i] = '0';
        }
        int arr_ind = 0;
        b = strlen(buffer_line);
        b = (b > 12000) ? b : 12000;

        token = strtok(buffer_line, "\t");

        while (token != NULL) {

            if (token_index == 1) {         // to chk CHROM no. 

                if (strcmp(chrom, token) != 0) { 

                    break;
                }
            }

            token_index++;
            token = strtok(NULL, "\t");

            if (token == NULL) {

                break;
            }
            
            if (token_index == 4) {         // to chk REF

                if ((token[0] == 'A' || token[0] == 'T' || token[0] == 'G' || token[0] == 'C') && token[1] == '\0') {

                    flag_ref = 1;
                    arr[arr_ind] = token[0];
                    arr_ind++;

                } else {

                    break;
                }
            }
            if (token_index == 5 ) {        // to chk ALT

                int l = strlen(token);

                if (l % 2 == 0) {

                    break;
                }

                for (int i = 0; i < l; i++) {

                    if (i % 2 == 0) {

                        if (token[i] != 'A' && token[i] != 'T' && token[i] != 'G' && token[i] != 'C') {

                            flag_alt = 0;
                            break;

                        } else {

                            arr[arr_ind] = token[i];
                            arr_ind++;
                            continue;
                        }

                    } else if (i % 2 != 0) {

                        if (token[i] != ',') {

                            flag_alt = 0;
                            break;

                        } else {
                            continue;
                        }
                    } 
                }
            }

           if (token_index == 7 && flag_alt == 1) {             // to chk FILTER

                if (strcmp(token, "PASS") != 0) {

                    flag_filter = 0;
                    break;
                }
                  
            }
			// calculating mutation count in a row
            if (token_index >= 10  && flag_ref == 1 && flag_alt == 1 && flag_filter == 1) {

                if (token[0] == '0' && token[2] == '0') {

                    total++;

                    for (int i = 0; i < sm_j; i++) {
                        if (gid_ind[i] == token_index) {
                            char a = token[0];
                            char b = token[2];
                            int x = (int)a-48;
                            int y = (int)b-48;
                            svalue = coordinate(x, y, arr);
                            //printf("%lf ", svalue);
                            matrix[rows][token_index] = svalue;     //value from scoring matrix]
                        }
                    }
                    continue;

                } else {

                    mutation_count++;
                    total++;
                    for (int i = 0; i < sm_j; i++) {
                        if (gid_ind[i] == token_index) {
                            char a = token[0];
                            char b = token[2];
                            int x = (int)a-48;
                            int y = (int)b-48;
                            svalue = coordinate(x, y, arr);
                            //printf("%lf ", svalue);
                            matrix[rows][token_index] = svalue;     //value from scoring matrix]
                        }
                    }
                    
                }
            }
            
            
            
        }
        percent = (mutation_count * 100.0) / total ;

        if (percent >= min && percent <= max) {

            r_lines++;
            rows++;
        }

        //matrix
        
    }        
    printf("%ld %ld\n", r_lines, columns);
    
    fclose(file);

    return 0;
}

