package padrao;

import java.util.*;
class Main {
	public static void main(String[] args) {
    int numElementos = 20;
    
    Vetor A = new Vetor();
    
    A.populaVetor(numElementos);
   
    Thread[] threads = new Thread[numElementos];
    Soma soma = new Soma(numElementos, numElementos, A);

    for(int i = 0; i < threads.length; i++) {
      threads[i] = new T(i, soma);
    }

    for(int i = 0; i < threads.length; i++) {
      threads[i].start();
    }

    for(int i = 0; i < threads.length; i++) {
      try{
        threads[i].join();
      }
      catch(InterruptedException IE){
        return;
      }
    }

    System.out.println("Vetor A: " + A.toString());
	}
}