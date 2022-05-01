#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <string.h>





int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    int rank,size,number;
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    if (rank == 0){
        char chArr[255]  = "5 60 45 20 30 5 4 77 8 15 98 65 22 1 0 35";
        char* token = strtok(chArr," ");
        int arr[16];
        int essArro[4][4];
        int row = 0;
        int col = 0;
        int i = 0;
        int partialArr[4];
        while(token!= NULL){
            if (i < 16){
                arr[i] = atoi(token);
                i++;
            }
            if (i < 17){
                if(col == 4){
                    col = 0;
                    row = row + 1;
                }
            essArro[row][col] = atoi(token);
            //printf("last token which is assigned is -> %d \n",essArro[row][col]);
            col++;
            }
            token = strtok(NULL," ");
            
        }
        
        for(int i = 0; i < 4; i++){
            for(int j = 0; j<4; j++){
                printf("%d ",essArro[i][j]);
            }
            printf("\n");
        }
        
        int index_arr = 0;
        int essArr[4][4];
        for (int i = 0 ; i < 4; i++){
            for(int j = 0 ; j < 4 ; j++){
                essArr[i][j] = arr[index_arr];
                index_arr++;
            }
        }
        
        int iterationNumber = 4;
        int block_nums = 0;
        for (int i = 0; i < iterationNumber; i++){
            MPI_Send(essArr + block_nums , 4,MPI_INT,i,13,MPI_COMM_WORLD);
            block_nums = block_nums + 1;
        }
        double from_third;
        double from_first;
        double from_second;
        double from_zero;
        double avg = 0;
        MPI_Recv(partialArr,4,MPI_INT,0,13,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        for(int i = 0; i < 4 ; i++){
            avg = avg + partialArr[i]; 
        }
        avg = avg /16;
        MPI_Send(&avg,1,MPI_DOUBLE,0,15,MPI_COMM_WORLD);
        MPI_Recv(&from_zero,1,MPI_DOUBLE,0,15,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&from_third,1,MPI_DOUBLE,3,13,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&from_first,1,MPI_DOUBLE,1,13,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&from_second,1,MPI_DOUBLE,2,14,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        
        printf("> i am rank %d and from zero i gave %f \n",rank,from_zero);
        printf("> i am rank %d and from third i gave %f \n",rank,from_third);
        printf("> i am rank %d and from first i gave %f \n",rank,from_first);
        printf("> i am rank %d and from second i gave %f \n",rank, from_second);
        double avg_matrix = (from_first + from_second + from_third + from_zero);
        printf("---\nThe average of the matrix is %f \n---\n",avg_matrix);
        printf("P%d = ",rank);
        for (int i = 0 ; i < 4; i++){
            printf("%d ",partialArr[i]);
        }
        printf("\n");
    }
    else if (rank == 1){
        int partialArr [4] ;
        double avg = 0;
        MPI_Recv(partialArr , 4, MPI_INT,0,13,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for (int i = 0 ; i < 4 ; i++){
            avg = avg + partialArr[i];
        }
        avg = avg/16;
        MPI_Send(&avg,1,MPI_DOUBLE,0,13,MPI_COMM_WORLD);
        printf("P%d = ",rank);
        for (int i = 0 ; i < 4; i++){
            printf("%d ",partialArr[i]);
        }
        printf("\n");
        
    }
    else if (rank == 2){
        int parr[4];
        MPI_Recv(parr,4,MPI_INT,0,13,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        double avg = 0;
        for (int i = 0 ; i < 4 ; i++){
            avg = avg + parr[i];
        }
        avg = avg /16 ;
        MPI_Send(&avg, 1, MPI_DOUBLE, 0, 14,MPI_COMM_WORLD);
        printf("P%d = ",rank);
        for (int i = 0; i < 4; i++){
            printf("%d ",parr[i]);
        }
        printf("\n");
    }
    else{
        int parr[4];
        MPI_Recv(parr,4,MPI_INT,0,13,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        //calculate avg
        double avg = 0;
        for(int i = 0; i< 4 ;i++){
            avg = avg + parr[i];
        }
        avg = avg/16;
        printf("P%d =",rank);
        for (int i =0 ; i < 4 ; i++){
            printf("%d ", parr[i]);
        }
        printf("\n");
        MPI_Send(&avg,1,MPI_DOUBLE,0, 13,MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}