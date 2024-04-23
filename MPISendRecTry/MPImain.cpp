#include "../InputH/input.h"
#include "../MatrixH/matrix_demo.h"

#include <iostream>
#include <mpi.h>
#include <string>
#include <vector>

using namespace std;

void distributeRowField(const int &nrow, const int &world_size,
                        const int &world_rank, int &rowStart, int &rowEnd);

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int world_size = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int world_rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len = 0;
  MPI_Get_processor_name(processor_name, &name_len);

  vector<MPI_Request> sendRequest;
  vector<MPI_Request> recvRequest;

  if (world_rank == 0) {
    Input inputA;
    Input inputB;
    string filenameA = "./testData/randomA.dat";
    string filenameB = "./testData/randomB.dat";
    inputA.init(filenameA);
    inputB.init(filenameB);

    if (inputA.targetConfigs_.at("nrows") !=
            inputB.targetConfigs_.at("nrows") ||
        inputA.targetConfigs_.at("ncols") !=
            inputB.targetConfigs_.at("ncols")) {
      cerr << "Matrix A and Matrix B have different dimensions!" << endl;
      MPI_Abort(MPI_COMM_WORLD, 1);
    }

    Matrix_demo matrixA;
    Matrix_demo matrixB;
    Matrix_demo matrixC;
    matrixA.init(inputA);
    matrixB.init(inputB);

    /* cout << "MatrixA:" << endl;
    matrixA.print();
    cout << endl << "MatrixB:" << endl;
    matrixB.print();
    cout << endl; */

    double *tempBufferC;
    tempBufferC = new double[matrixA.rowNum() * matrixA.colNum()];

    sendRequest.resize((world_size - 1) * 3);

    for (int i = 0; i < world_size; ++i) {
      int startRow = 0;
      int endRow = 0;
      distributeRowField(matrixA.rowNum(), world_size, i, startRow, endRow);
      int itemCount = (endRow - startRow) * matrixA.colNum();

      vector<int> auxIndex;
      auxIndex.push_back(endRow - startRow);
      auxIndex.push_back(matrixA.colNum());

      if(i == 0){
        double *tempBufferA =
          matrixA.matrixToArray(CblasRowMajor, tempBufferA, startRow, endRow);
        double *tempBufferB =
          matrixB.matrixToArray(CblasRowMajor, tempBufferB, startRow, endRow);
        Matrix_demo partA;
        Matrix_demo partB;
        partA.ArrayToMatrix(tempBufferA, auxIndex[0], auxIndex[1], CblasRowMajor);
        partB.ArrayToMatrix(tempBufferB, auxIndex[0], auxIndex[1], CblasRowMajor);
        Matrix_demo partC;
        partC.init(auxIndex[0], auxIndex[1]);
        partC = partA + partB;

        /* cout << "PartA:" << endl;
        partA.print();
        cout << endl << "PartB:" << endl;
        partB.print();
        cout << endl << "PartC:" << endl;
        partC.print();
        cout << endl; */

        double* tempBufferPartC = nullptr;
        tempBufferPartC = partC.matrixToArray(CblasRowMajor, tempBufferC);
        for(int i = 0; i < itemCount; ++i) {
          tempBufferC[i] = tempBufferPartC[i];
        }
      } else {
        double *tempBufferA = nullptr;
        tempBufferA = matrixA.matrixToArray(CblasRowMajor, tempBufferA, startRow, endRow);
        MPI_Isend(tempBufferA, itemCount, MPI_DOUBLE, i, 1, MPI_COMM_WORLD,
                  &sendRequest[1 + 3 * (i - 1)]);

        MPI_Isend(auxIndex.data(), 2, MPI_INT, i, 0, MPI_COMM_WORLD,
                  &sendRequest[0 + 3 * (i - 1)]);

        double *tempBufferB = nullptr;
        tempBufferB = matrixB.matrixToArray(CblasRowMajor, tempBufferB, startRow, endRow);
        MPI_Isend(tempBufferB, itemCount, MPI_DOUBLE, i, 2, MPI_COMM_WORLD,
                  &sendRequest[2 + 3 * (i - 1)]);
      }
    }
    MPI_Waitall((world_size - 1) * 3, sendRequest.data(), MPI_STATUS_IGNORE);


    recvRequest.resize(world_size - 1);
    for (int i = 1; i < world_size; ++i) {
        int startRow = 0;
        int endRow = 0;
        distributeRowField(matrixA.rowNum(), world_size, i, startRow, endRow);
        int itemCount = (endRow - startRow) * matrixA.colNum();
        double *auxPtr = tempBufferC + startRow * matrixA.colNum();
        MPI_Irecv(auxPtr, itemCount, MPI_DOUBLE, i, 3, MPI_COMM_WORLD,
                  &recvRequest[i - 1]);
    }

    MPI_Waitall(world_size - 1, recvRequest.data(), MPI_STATUS_IGNORE);

    matrixC.ArrayToMatrix(tempBufferC, matrixA.rowNum(), matrixA.colNum(),
                          CblasRowMajor);
    matrixC.print();
    matrixC.writeToFile("./testData/output.dat");

    delete[] tempBufferC;
  }
  else
  {
    Matrix_demo partA;
    Matrix_demo partB;

    double *tempBufferA = nullptr;
    double *tempBufferB = nullptr;
    vector<int> auxIndex;
    auxIndex.resize(2);
    sendRequest.resize(3);
    recvRequest.resize(3);

    MPI_Irecv(auxIndex.data(), 2, MPI_INT, 0, 0, MPI_COMM_WORLD,
              &recvRequest[0]);
    MPI_Wait(&recvRequest[0], MPI_STATUS_IGNORE);
    tempBufferA = new double[auxIndex[0] * auxIndex[1]];
    MPI_Irecv(tempBufferA, auxIndex[0] * auxIndex[1], MPI_DOUBLE, 0, 1,
              MPI_COMM_WORLD, &recvRequest[1]);

    tempBufferB = new double[auxIndex[0] * auxIndex[1]];
    MPI_Irecv(tempBufferB, auxIndex[0] * auxIndex[1], MPI_DOUBLE, 0, 2,
              MPI_COMM_WORLD, &recvRequest[2]);

    MPI_Waitall(3, &recvRequest[0], MPI_STATUS_IGNORE);
    partA.ArrayToMatrix(tempBufferA, auxIndex[0], auxIndex[1], CblasRowMajor);
    partB.ArrayToMatrix(tempBufferB, auxIndex[0], auxIndex[1], CblasRowMajor);

    Matrix_demo partC;
    partC.init(auxIndex[0], auxIndex[1]);
    partC = partA + partB;

    /* cout << "PartA:" << endl;
    partA.print();
    cout << endl << "PartB:" << endl;
    partB.print();
    cout << endl << "PartC:" << endl;
    partC.print();
    cout << endl; */

    delete[] tempBufferA;
    delete[] tempBufferB;

    double *tempBufferC = partC.matrixToArray(CblasRowMajor, tempBufferC);

    MPI_Send(tempBufferC, auxIndex[0] * auxIndex[1], MPI_DOUBLE, 0, 3,
             MPI_COMM_WORLD);
  }
  MPI_Finalize();

  return 0;
}

void distributeRowField(const int &nrow, const int &world_size,
                        const int &world_rank, int &rowStart, int &rowEnd) {
  if (nrow < world_size) {
    rowStart = world_rank < nrow ? world_rank : nrow - 1;
    rowEnd = rowStart + 1;
    return;
  }
  int rowPerProc = nrow / world_size;
  rowStart = world_rank * rowPerProc;
  rowEnd = rowStart + rowPerProc;
  if (world_rank == world_size - 1) {
    rowEnd = nrow;
  }
  return;
}
