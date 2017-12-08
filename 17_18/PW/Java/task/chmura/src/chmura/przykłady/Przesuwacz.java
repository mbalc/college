package chmura.przykłady;

import chmura.Byt;
import chmura.Chmura;

import java.util.Collection;
import java.util.Collections;

public class Przesuwacz {
    private int[] zakresCzekania;
    private int[] zakresPracy;
    private int[] wektorPrzed;
    private int[] wektorPo;
    private int indeks;
    private String nazwa;
    private String czynność;
    private Chmura chmura;
    private Byt byt;
    private Runnable chcęZacząć;
    private Runnable zaczynam;
    private Runnable kończę;
    private Runnable odpoczywam;
    private double mnożnikCzasu;

    public Przesuwacz(int[] zakresCzekania, int[] zakresPracy, int[] wektorPrzed, int[] wektorPo, int indeks, String nazwa, String czynność, Chmura chmura, Byt byt, Runnable chcęZacząć, Runnable zaczynam, Runnable kończę, Runnable odpoczywam, double mnożnikCzasu) {
        this.zakresCzekania = zakresCzekania;
        this.zakresPracy = zakresPracy;
        this.wektorPrzed = wektorPrzed;
        this.wektorPo = wektorPo;
        this.indeks = indeks;
        this.nazwa = nazwa;
        this.czynność = czynność;
        this.chmura = chmura;
        this.byt = byt;
        this.chcęZacząć = chcęZacząć;
        this.zaczynam = zaczynam;
        this.kończę = kończę;
        this.odpoczywam = odpoczywam;
        this.mnożnikCzasu = mnożnikCzasu;
    }

    private String id () {
        return nazwa + " #" + indeks + " ";
    }

    private String poz (int[] pl) {
        return " (" + pl[0] + ", " + pl[1] + ")";
    }

    public void odpal () {
        new Thread(() -> {
            try {
                Collection<Byt> przes = Collections.singletonList(byt);
                int[] pl;
                while (true) {
                    pl = chmura.miejsce(byt);
                    System.out.println(id() + "idzie odpocząć " + poz(pl));
                    Thread.sleep(Czas.losowyCzas(zakresCzekania, mnożnikCzasu));
                    System.out.println(id() + "chce " + czynność);
                    chcęZacząć.run();
                    chmura.przestaw(przes, wektorPrzed[0], wektorPrzed[1]);
                    pl = chmura.miejsce(byt);
                    zaczynam.run();
                    System.out.println(id() + "zaczyna " + czynność + poz(pl));
                    Thread.sleep(Czas.losowyCzas(zakresPracy, mnożnikCzasu));
                    System.out.println(id() + "kończy " + czynność);
                    kończę.run();
                    chmura.przestaw(przes, wektorPo[0], wektorPo[1]);
                    odpoczywam.run();
                }
            }
            catch (InterruptedException e) {
                System.out.println(id() + "przerywa pracę");
            }
        }).start();
    }
}
