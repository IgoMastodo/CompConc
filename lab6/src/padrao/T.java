package padrao;

class T extends Thread {
	  private int id;
	  private Soma soma;

	  public T(int id, Soma soma) {
	    this.id = id;
	    this.soma = soma;
	  }

	  public void run(){
	    this.soma.opSoma(this.id);
	  }
	 
	}