package padrao;

import java.util.*;

class Soma {
  private int numElementos;
  private int nThreads;
  private Vetor A;

  Soma(int numElementos, int numberOfThreads, Vetor A) {
    this.numElementos = numElementos;
    this.nThreads = numberOfThreads;
    this.A = A;
  }

   public synchronized void opSoma(int id) {
    int ini, end;
    int blockSize = this.numElementos / this.nThreads;
    ini = id * blockSize;
    end = id == nThreads - 1 ? numElementos : ini + blockSize;

    for(int idx = ini; idx < end; idx++) {
      int value = this.A.getItem(idx) + this.B.getItem(idx);
    }
  }
}