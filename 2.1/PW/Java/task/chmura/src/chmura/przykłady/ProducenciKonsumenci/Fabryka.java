package chmura.przykłady.ProducenciKonsumenci;

import chmura.Byt;
import chmura.Chmura;
import chmura.przykłady.Przesuwacz;

import java.util.ArrayList;
import java.util.List;
import java.util.function.BiPredicate;


public class Fabryka {

    private static BiPredicate<Integer, Integer> predykat = (x, y) -> x < y || y < x - Opcje.bufor;
    // Kształt
    //
    // ╳╳⬆╳╳╳╳╳╳╳╳╱          ╱╳╳╳╳
    // ╳╳│╳╳╳╳╳╳╳╱          ╱╳╳╳╳╳
    // ╳╳│╳╳╳╳╳╳╱ wielkosc ╱╳╳╳╳╳╳
    // ╳╳│╳╳╳╳╳╱⇽────────⇾╱╳╳╳╳╳╳╳
    // ╳╳│╳╳╳╳╱  bufora  ╱╳╳╳╳╳╳╳╳
    // ╳╳│╳╳╳╱          ╱╳╳╳╳╳╳╳╳╳
    // ╳╳│╳╳╱          ╱╳╳╳╳╳╳╳╳╳╳
    // ╳╳│╳╱          ╱╳╳╳╳╳╳╳╳╳╳╳
    // ╳╳│╱          ╱╳╳╳╳╳╳╳╳╳╳╳╳
    // ──┼──────────/────────────⇾
    // ╳0│         /╳╳╳╳╳╳╳╳╳╳╳╳╳╳
    // / │        /╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
    //

    static Chmura chmura = new Chmura(predykat);
    static Byt byt;

    public static void main(String[] args) {
        try {
            byt = chmura.ustaw(0, 0);
        } catch (InterruptedException e) {
            System.out.println("Nieładnie tak przerywać!");
        }
        List<Przesuwacz> l = new ArrayList<>();

        for (int i = 0; i < Opcje.iluKonsumentow; i++) {
            l.add(new Konsument(i));
        }
        for (int i = 0; i < Opcje.iluProducentow; i++) {
            l.add(new Producent(i));
        }

        for (Przesuwacz p : l) {
            p.odpal();
        }
    }
}
