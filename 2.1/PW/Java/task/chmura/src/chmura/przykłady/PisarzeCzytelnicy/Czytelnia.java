package chmura.przykłady.PisarzeCzytelnicy;

import chmura.przykłady.Przesuwacz;

import java.util.ArrayList;
import java.util.List;

public class Czytelnia {

    public static void main(String[] args) {
        try {
            new Stan();
        } catch (InterruptedException e) {
            System.out.println("No teraz to mnie Pan(i) zaskoczył(a)!"); // zależy od testującego
        }

        List<Przesuwacz> l = new ArrayList<>();

        for (int i = 0; i < Opcje.iluCzytelnikow; i++) {
            l.add(new Czytelnik(i));
        }
        for (int i = 0; i < Opcje.iluPisarzy; i++) {
            l.add(new Pisarz(i));
        }

        for (Przesuwacz p : l) {
            p.odpal();
        }

    }
}
