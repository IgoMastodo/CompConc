import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

class V {
  private List<Integer> vetor;
	
  public V() {
    this.vetor = new ArrayList<Integer>();      
  }

  public V(int n) {
    this.vetor = Arrays.asList(new Integer[n]);
    for(int i = 0; i < n; i++) {
      this.vetor.set(i, 1);
    } 
  }
  
  public int pegaItem(int idx) {
	  return this.vetor.get(idx);
  }
  
  public void setaItem(int idx, int item) {
	  this.vetor.set(idx, item);
  }
 
}
  
class Soma {
  private int numElementos;
  private int nThreads;
  private V A;

  Soma(int numElementos, int numberOfThreads, V A) {
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
      this.A.setaItem(idx, this.A.pegaItem(idx) + 1);
    }
  }
}
  
class Th extends Thread {
	  private int id;
	  private Soma soma;

	  public Th(int id, Soma soma) {
	    this.id = id;
	    this.soma = soma;
	  }
	  public void run(){
		System.out.println("Rodando Thread: "+id);
		this.soma.opSoma(this.id);
  }
 
}

class SomaNoVetor {
	
	static final int N = 15;
	
	public static void main (String[] args) {
		
		int numElementos = N;
	    
	    V A = new V(numElementos);
	
	    System.out.println("Vetor inicialmente: ");		
    for(int i=0; i<N; i++) {
    	System.out.print("["+A.pegaItem(i)+"] ");	
    }
    System.out.println();	
    System.out.println();	
    Thread[] threads = new Thread[numElementos];
    Soma soma = new Soma(numElementos, numElementos, A);

    for(int i = 0; i < threads.length; i++) {
      threads[i] = new Th(i, soma);
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
    System.out.println();	
    System.out.println("Vetor apos o incremento: ");	
    
    for(int i=0; i<numElementos; i++) {
    	System.out.print("["+A.pegaItem(i)+"] ");	
	    }
			
	}
	
}


