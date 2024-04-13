#ifdef __MPI__
#include <mpi.h>
#endif

#include "../InputH/input.h"
#include "../TimerH/timer.h"
#include "../MatrixH/matrix_demo.h"
#include <cstdlib>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
    Timer_demo::tick("Matrix_demo", "init");
    Input input("./TestData/InputTest.dat");
    Matrix_demo matrix;
    matrix.init(input);
    int print_mpi_log;
    try {
        print_mpi_log = stoi(input.targetConfigs_.at("print_mpi_log"));
    } catch (const std::invalid_argument &e) {
        cerr << "Invalid argument: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    Timer_demo::tick("Matrix_demo", "init");

    #ifdef __MPI__
    Timer_demo::tick("MPI", "Init");
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    Timer_demo::tick("MPI", "Init");

    Timer_demo::tick("MPI", "printf");
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);
    Timer_demo::tick("MPI", "printf");

    Timer_demo::writeResToFile(world_rank, print_mpi_log);

    MPI_Finalize();
    #endif

    #ifndef __MPI__
    Timer_demo::writeResToFile();
    #endif

    return 0;
}