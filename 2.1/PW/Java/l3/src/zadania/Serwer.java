package zadania;

import java.util.LinkedList;
import java.util.PriorityQueue;
import java.util.Queue;

public class Serwer {

    int ileGrup;
    int ileZasobów;

    Integer ilePracuje[]; // ile wątków własnie pracuje na zasobie
    IntBox zasóbGrupy[]; // trzymany zasób dla każdej z grup
    Queue<Integer> zasoby; // taki typ trzeba zwrócić

    public Serwer(int ileGrup, int ileZasobów) {
        this.ileGrup = ileGrup;
        this.ileZasobów = ileZasobów;

        ilePracuje = new Integer[ileGrup];
        zasóbGrupy = new IntBox[ileGrup];
        zasoby = new LinkedList<Integer>();

        for (int i = 0; i < ileGrup; ++i) zasóbGrupy[i] = new IntBox(-1);
        for (int i = 0; i < ileGrup; ++i) ilePracuje[i] = 0;

        for (int i = 0; i < ileZasobów; ++i) zasoby.add(i);
    }

    Integer chcęKorzystać(Integer grupa) throws InterruptedException {
        synchronized (zasóbGrupy[grupa]) {
            if (zasóbGrupy[grupa].getValue() == -2) { // ktoś już czeka na zasób, ten ktoś nas obudzi
                zasóbGrupy[grupa].wait();
            }
            if (zasóbGrupy[grupa].getValue() == -1  || // nie mamy zasobu
                (zasóbGrupy[grupa].getValue() >= 0 && zasoby.size() < 1)) {  // grupa pracuje, a fajnie by było zwolnić zasób
                synchronized (zasoby) {
                    while (zasoby.size() < 1) {
                        zasóbGrupy[grupa].setValue(-2);
//                        System.out.printf("Wątek z grupy nr %02d postanowił zaczekać na zasób\n", grupa);
                        zasoby.wait();
//                        System.out.printf("Wątek z grupy nr %02d obudził się!\n", grupa);
                    }
                }
                zasóbGrupy[grupa].setValue(zasoby.poll());
                zasóbGrupy[grupa].notifyAll();
            }
            synchronized (ilePracuje[grupa]) {
//                System.out.println(ilePracuje[grupa] + "pre");
                ++ilePracuje[grupa];
//                System.out.println(ilePracuje[grupa] + "post");
            }
            return zasóbGrupy[grupa].getValue();
        }
    }

    void skończyłem (Integer grupa, Integer zasób) {
        synchronized (zasoby) {
            synchronized (ilePracuje[grupa]) {
                --ilePracuje[grupa];
//                System.out.println(ilePracuje[grupa]);
                if (ilePracuje[grupa] == 0) { // wszyscy skończyli
//                    System.out.printf("Wątek z grupy nr %02d jest ostatnim wychodzącym, oddaje zasób nr %02d\n", grupa, zasób);
                    zasoby.add(new Integer(zasób));
                    zasoby.notify();
                }
            }
        }
    }
}
