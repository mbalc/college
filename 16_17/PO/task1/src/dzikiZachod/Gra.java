package dzikiZachod;

import dzikiZachod.Akcje.Akcja;
import dzikiZachod.Akcje.PulaAkcji;
import dzikiZachod.Akcje.TaliaAkcji;
import dzikiZachod.Akcje.WykonanieAkcji;
import dzikiZachod.Gracze.Gracz;
import dzikiZachod.Powiadomienia.DziennikGrySI;
import dzikiZachod.Powiadomienia.Informator;
import dzikiZachod.Powiadomienia.Powiadomienie;
import dzikiZachod.Powiadomienia.Wydarzenie;
import dzikiZachod.Strategie.StrategiaGracza;
import dzikiZachod.Widoki.WidokBandyty;
import dzikiZachod.Widoki.WidokGraczy;
import dzikiZachod.Widoki.WidokPomocnikaSzeryfa;
import dzikiZachod.Widoki.WidokSzeryfa;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class Gra {

    private List <Gracz> uczestnicy;

    public static final Random losuj = new Random();
    public static final int dolnyZakresPunktowZycia = 3;
    public static final int gornyZakresPunktowZycia = 4;
    public static final int liczbaPunktowZyciaSzeryfa = 5;
    private static final int liczbaOczekNaKostce = 6;
    private static final int limitTur = 42;
    public static final int limitKartWRece = 5;
    private static final int obrazeniaZadawanePrzezDynamit = 3;
    public static final int poczatkowyZasiegGracza = 1;
    public static final int zasiegLeczenia = 1;

    private static int pozycjaSzeryfa;

    public static int getPozycjaSzeryfa () {
        return pozycjaSzeryfa;
    }

    private int licznikBandytow;
    private boolean dynamitWGrze;
    private boolean dynamitJuzWybuchl;

    private List <Integer> odlegloscOdPierwszegoGracza;

    private Informator obserwator;

    private TaliaAkcji taliaAkcji;

    private WidokGraczy widok;

    public Gra () {
    }

    private void zainicjalizuj (List <Gracz> gracze, PulaAkcji pulaAkcji) throws IllegalStateException {
        uczestnicy = new ArrayList <>();
        obserwator = new Informator();
        obserwator.dodajObserwujacego(new DziennikGrySI(uczestnicy));
        for (Gracz g : uczestnicy) {
            obserwator.dodajObserwujacego(g);
        }
        uczestnicy.addAll(gracze);
        Collections.shuffle(uczestnicy, losuj);
        {
            int licznikSzeryfow = 0;
            licznikBandytow = 0;
            for (int i = 0; i < uczestnicy.size(); ++i) {
                if (uczestnicy.get(i).czySzeryf()) {
                    ++licznikSzeryfow;
                    pozycjaSzeryfa = i;
                }
                if (uczestnicy.get(i).czyBandyta()) {
                    ++licznikBandytow;
                }
            }
            if (licznikSzeryfow > 1) {
                throw new IllegalStateException("To miasto jest zbyt małe na więcej niż jednego szeryfa!");
            }
            if (licznikSzeryfow < 1) {
                throw new IllegalStateException("W tym mieście nie ma szeryfów!");
            }
            if (licznikBandytow < 1) {
                throw new IllegalStateException("Nie ma z kim walczyć - brak bandytów!");
            }
        }
        odlegloscOdPierwszegoGracza = new ArrayList <>();
        Gracz g;
        for (int i = 0; i < uczestnicy.size(); ++i) {
            g = uczestnicy.get(i);
            if (g.czyBandyta()) widok = new WidokBandyty(uczestnicy, odlegloscOdPierwszegoGracza);
            else if (g.czySzeryf()) widok = new WidokSzeryfa(uczestnicy, odlegloscOdPierwszegoGracza);
            else widok = new WidokPomocnikaSzeryfa(uczestnicy, odlegloscOdPierwszegoGracza);
            g.inicjalizujGracza(i, widok);
            odlegloscOdPierwszegoGracza.add(i);
        }

        taliaAkcji = pulaAkcji.dajTalieAkcji();
        dynamitWGrze = false;
        dynamitJuzWybuchl = false;

    }

    private void obsluzSmierc (int zabojca, int ofiara) throws Powiadomienie {
        taliaAkcji.dodaj(Akcja.ULECZ, uczestnicy.get(ofiara).getReka().ileUlecz());
        taliaAkcji.dodaj(Akcja.STRZEL, uczestnicy.get(ofiara).getReka().ileStrzel());
        taliaAkcji.dodaj(Akcja.ZASIEG_PLUS_DWA, uczestnicy.get(ofiara).getReka().ileZasiegDwa());
        taliaAkcji.dodaj(Akcja.ZASIEG_PLUS_JEDEN, uczestnicy.get(ofiara).getReka().ileZasiegJeden());
        taliaAkcji.dodaj(Akcja.DYNAMIT, uczestnicy.get(ofiara).getReka().ileDynamit());
        uczestnicy.get(ofiara).getReka().odrzucWszystkieKarty();
        obserwator.powiadom(new Powiadomienie(Wydarzenie.ZABOJSTWO, zabojca, ofiara));
        if (uczestnicy.get(ofiara).czyBandyta()) --licznikBandytow;
        if (ofiara != 0)
            for (int i = ofiara; i < uczestnicy.size(); ++i) {
                odlegloscOdPierwszegoGracza.set(i, odlegloscOdPierwszegoGracza.get(i) - 1);
            }
        if (ofiara == pozycjaSzeryfa) throw new Powiadomienie(Wydarzenie.ZWYCIESTWO_BANDYTOW);
        if (licznikBandytow <= 0) throw new Powiadomienie(Wydarzenie.ZWYCIESTWO_SZERYFA);
    }

    private void obsluzDynamit (int indeks) throws Powiadomienie {
        Gracz g = uczestnicy.get(indeks);
        if (1 + losuj.nextInt(liczbaOczekNaKostce) == 1) { //wyrzucono jedynke
            dynamitJuzWybuchl = true;
            for (int i = 0; i < obrazeniaZadawanePrzezDynamit; ++i) {
                if (g.otrzymajObrazenia()) { //gracz umarł w wyniku wybuchu
                    obsluzSmierc(indeks, indeks);
                    break;
                }
            }
        }
    }

    private void obsluzWykonanieAkcji (Akcja a, int wykonawca, int cel) throws IllegalArgumentException, Powiadomienie {
        if (a == Akcja.ULECZ) {
            if (widok.odlegloscMiedzyGraczami(wykonawca, cel) <= zasiegLeczenia) {
                obserwator.powiadom(new Powiadomienie(Wydarzenie.LECZENIE, wykonawca, cel));
                if (uczestnicy.get(cel).getPunktyZycia() < uczestnicy.get(cel).limitPunktowZycia) {
                    uczestnicy.get(cel).otrzymajLeczenie();
                }
            } else throw new IllegalArgumentException("Cel leczenia nie sąsiaduje z graczem!");
        }
        if (a == Akcja.ZASIEG_PLUS_DWA) {
            obserwator.powiadom(new Powiadomienie(Wydarzenie.ZWIEKSZENIE_ZASIEGU_O_DWA, wykonawca, wykonawca));
            uczestnicy.get(wykonawca).otrzymajZwiekszenieZasiegu();
            uczestnicy.get(wykonawca).otrzymajZwiekszenieZasiegu();
        }
        if (a == Akcja.ZASIEG_PLUS_JEDEN) {
            obserwator.powiadom(new Powiadomienie(Wydarzenie.ZWIEKSZENIE_ZASIEGU, wykonawca, wykonawca));
            uczestnicy.get(wykonawca).otrzymajZwiekszenieZasiegu();
        }
        if (a == Akcja.STRZEL) {
            if (widok.odlegloscMiedzyGraczami(wykonawca, cel) <= uczestnicy.get(wykonawca).getZasieg()) {
                obserwator.powiadom(new Powiadomienie(Wydarzenie.STRZAL, wykonawca, cel));
                if (uczestnicy.get(cel).otrzymajObrazenia()) obsluzSmierc(wykonawca, cel);
            } else throw new IllegalArgumentException("Cel poza zasięgiem strzału gracza!");
        }
        if (a == Akcja.DYNAMIT) {
            obserwator.powiadom(new Powiadomienie(Wydarzenie.WYPUSZCZENIE_DYNAMITU, wykonawca, wykonawca));
            dynamitWGrze = true;
        } else taliaAkcji.dodaj(a);
    }

    private void obsluzRuchyRozgrywajacego (int indeks) throws Powiadomienie {
        Gracz rozgrywajacy = uczestnicy.get(indeks);
        WykonanieAkcji bufor;
        while (rozgrywajacy.czyWGrze()) {
            bufor = rozgrywajacy.wykonajRuch();
            if (bufor.getCel() == StrategiaGracza.anulowanieAkcji) break;
            rozgrywajacy.getReka().zwrocAkcje(bufor.getAkcja());
            obsluzWykonanieAkcji(bufor.getAkcja(), indeks, bufor.getCel());
        }
    }

    private void przeprowadzZagrywke (int indeks) throws Powiadomienie {
        Gracz rozgrywajacy = uczestnicy.get(indeks);
        obserwator.powiadom(new Powiadomienie(Wydarzenie.ZAGRYWKA, indeks));
        try {
            if (rozgrywajacy.czyWGrze()) {
                while (rozgrywajacy.getReka().size() < limitKartWRece) {
                    rozgrywajacy.getReka().przyjmijKarte(taliaAkcji.dajAkcje());
                }
                obserwator.powiadom(new Powiadomienie(Wydarzenie.DOBRANIE_KART, indeks));
                if (dynamitWGrze) {
                    try {
                        obsluzDynamit(indeks);
                    } finally {
                        if (dynamitJuzWybuchl) {
                            dynamitWGrze = false;
                            obserwator.powiadom(new Powiadomienie(Wydarzenie.DYNAMIT_WYBUCHA, indeks));
                        } else {
                            obserwator.powiadom(new Powiadomienie(Wydarzenie.DYNAMIT_PRZECHODZI, indeks));
                        }
                    }
                }
            }
            obserwator.powiadom(new Powiadomienie(Wydarzenie.POCZATEK_RUCHOW, indeks));
            if (rozgrywajacy.czyWGrze()) {
                obsluzRuchyRozgrywajacego(indeks);
            }
        } finally {
            obserwator.powiadom(new Powiadomienie(Wydarzenie.KONIEC_RUCHOW, indeks));
            if (rozgrywajacy.czyWGrze()) {
                obserwator.powiadom(new Powiadomienie(Wydarzenie.KONIEC_ZAGRYWKI, indeks));
            }
        }
    }

    private void przeprowadzTure (int numerTury) throws Powiadomienie {
        obserwator.powiadom(new Powiadomienie(Wydarzenie.NOWA_TURA, numerTury));
        for (int i = pozycjaSzeryfa; i < uczestnicy.size(); ++i) {
            przeprowadzZagrywke(i);
        }
        for (int i = 0; i < pozycjaSzeryfa; ++i) {
            przeprowadzZagrywke(i);
        }
    }

    private Powiadomienie przeprowadzRozgrywke () {
        obserwator.powiadom(new Powiadomienie(Wydarzenie.ROZPOCZECIE_GRY));
        try {
            for (int i = 1; i <= limitTur; ++i) {
                przeprowadzTure(i);
            }
        } catch (Powiadomienie w) {
            return w;
        }
        return new Powiadomienie(Wydarzenie.REMIS);
    }

    private void zakonczRozgrywke (Powiadomienie wynik) {
        assert (wynik.getRodzajPowiadomienia() == Wydarzenie.ZWYCIESTWO_SZERYFA ||
                wynik.getRodzajPowiadomienia() == Wydarzenie.ZWYCIESTWO_BANDYTOW ||
                wynik.getRodzajPowiadomienia() == Wydarzenie.REMIS);

        obserwator.powiadom(new Powiadomienie(Wydarzenie.KONIEC_GRY));
        obserwator.powiadom(wynik);
    }

    public void rozgrywka (List <Gracz> gracze, PulaAkcji pulaAkcji) {
        zainicjalizuj(gracze, pulaAkcji);
        zakonczRozgrywke(przeprowadzRozgrywke());
    }

}
