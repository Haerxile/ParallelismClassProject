#include "../../MatrixH/matrix_demo.h"
#include "../../InputH/input.h"

#include <mpi.h>
#include <iostream>

using namespace std;

const int PPLIMIT = 2;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int ppCount = 0;
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    /* int aux_rank = (world_rank + 1) % 2;

    while (ppCount < PPLIMIT)
    {
        if (world_rank == ppCount % 2)
        {
            ppCount++;
            cout << "Processor " << processor_name << ", rank " << world_rank << " is sending a message to processor " << aux_rank << endl;
            MPI_Send(&ppCount, 1, MPI_INT, aux_rank, 0, MPI_COMM_WORLD);
        }
        else
        {
            cout << "Processor " << processor_name << ", rank " << world_rank << " is receiving a message from processor " << aux_rank << endl;
            MPI_Recv(&ppCount, 1, MPI_INT, aux_rank, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (ppCount % 2 == 1)
            {
                cout << aux_rank << ">>>>" << ppCount << ">>>>" << world_rank << endl;
            }
            else
            {
                cout << world_rank << "<<<<" << ppCount << "<<<<" << aux_rank << endl;
            }
        }
    } */

    int rec_rank = (world_rank + world_size - 1) % world_size;
    int send_rank = (world_rank + 1) % world_size;

    int ball;
    if (world_rank == 0)
    {
        ball = 0;
    }
    int count = 0;
    while (count < PPLIMIT)
    {
        MPI_Send(&ball, 1, MPI_INT, send_rank, 0, MPI_COMM_WORLD);
        MPI_Recv(&ball, 1, MPI_INT, rec_rank, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout << "Send ball " << ball << " from " << rec_rank << " to " << send_rank << endl;
        count++;
    }

    MPI_Finalize();

    return 0;
}