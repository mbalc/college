import java.util.*;
import java.util.function.IntFunction;


public class WektorTest {

    public static void main(String[] args) {
        
        final int BIG_SIZE = 90000;

        Wektor mod23 = new Wektor(BIG_SIZE, i -> i % 23);
        Wektor mod37 = new Wektor(BIG_SIZE, i -> i % 37);
        Wektor add = mod37.dodaj(mod23);

        System.out.println(add.toString());

    }

}


class Adder extends Thread {

    Wektor a;
    Wektor b;
    Wektor w; 
    int dex;

    Adder (Wektor a, Wektor b, int dex, Wektor w) {
        this.a = a;
        this.b = b;
        this.w = w;
        this.dex = dex;
    }
    
    public void run() {
        int suma = this.a.daj(this.dex) + this.b.daj(this.dex);
        this.w.ustaw(this.dex, suma);
    }

}


class Wektor {
    
    List<Integer> body;
    int dim;

    Wektor (int N, IntFunction<Integer> f) {
        this.dim = N;
        this.body = Collections.synchronizedList(new ArrayList<Integer>());
        for (int i=0; i<N; ++i) {
            this.body.add(f.apply(i));
        }
    }

    Wektor (int N, int [] skladowe) {
        this(N, i -> skladowe[i]);
    }
    
    Wektor (int N) {
        this(N, i -> 0);
    }



    Wektor dodaj(Wektor d) {
        if (this.dim != d.dim) throw new IllegalArgumentException();

        Wektor w = new Wektor(this.dim);
        Thread[] threads = new Thread[this.dim];
        
        for (int i = 0; i < this.dim; i++) {
            threads[i] = new Adder(this, d, i, w);
        }

        for (int i = 0; i < this.dim; i++) {
            threads[i].run();
        }

        for (int i = 0; i < this.dim; i++) {
            while (threads[i].isAlive()) {
                try {
                    Thread.sleep(100); // Sleep for 10 milliseconds
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    System.err.println("GĹĂłwny wÄtek przerwany");
                }
            }
        }
        return w;
    }

    int daj(int dex) {
        return this.body.get(dex);
    }

    int ustaw(int dex, int war) {
        return this.body.set(dex, war);
    }

    public String toString() {
        return this.body.toString();
    }

}