package dzikiZachod.Powiadomienia;

import dzikiZachod.Gracze.Gracz;

import java.util.List;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class DziennikGrySI extends ObserwatorGry {

    private final List <Gracz> gracze;

    public DziennikGrySI (List <Gracz> g) {
        gracze = g;
    }

    private void wypiszFrakcje (Gracz gracz) {
        if (gracz.czyBandyta()) System.out.print("Bandyta");
        else if (gracz.czySzeryf()) System.out.print("Szeryf");
        else System.out.print("Pomocnik szeryfa");
    }

    private void wypiszStanGraczy () {
        System.out.println("  Gracze:");
        for (Gracz g : gracze) {
            System.out.print("    " + (g.getIndeks() + 1) + ": ");
            if (g.czyWGrze()) {
                wypiszFrakcje(g);
                System.out.println(" (liczba żyć: " + g.getPunktyZycia() + ")");
            } else {
                System.out.print("X (");
                wypiszFrakcje(g);
                System.out.println(")");
            }
        }
        System.out.println();
    }

    private void wypiszReke (int indeks) {
        System.out.println("    Akcje: [" + gracze.get(indeks).getReka().toString() + "]");
    }

    private void wypiszKomunikatORuchu (String komunikat) {
        System.out.println("      " + komunikat);
    }

    private void wypiszKomunikatZOpcjonalnymArgumentem (String komunikat, int wykonawca, int odbiorca) {
        if (wykonawca == odbiorca) wypiszKomunikatORuchu(komunikat);
        else wypiszKomunikatORuchu(komunikat + " " + (odbiorca + 1));
    }

    @Override
    public void powiadom (Powiadomienie p) {
        if (p.getRodzajPowiadomienia() == Wydarzenie.ROZPOCZECIE_GRY)
            powiadomORozpoczeciuGry();
        if (p.getRodzajPowiadomienia() == Wydarzenie.NOWA_TURA)
            powiadomONowejTurze(p.getWykonawca());
        if (p.getRodzajPowiadomienia() == Wydarzenie.ZAGRYWKA)
            powiadomOZagrywce(p.getWykonawca());
        if (p.getRodzajPowiadomienia() == Wydarzenie.DOBRANIE_KART)
            powiadomODobraniuKart(p.getWykonawca());
        if (p.getRodzajPowiadomienia() == Wydarzenie.DYNAMIT_PRZECHODZI)
            powiadomOStanieDynamitu(p.getWykonawca(), false);
        if (p.getRodzajPowiadomienia() == Wydarzenie.DYNAMIT_WYBUCHA)
            powiadomOStanieDynamitu(p.getWykonawca(), true);
        if (p.getRodzajPowiadomienia() == Wydarzenie.POCZATEK_RUCHOW)
            powiadomOPoczatkuRuchow();
        if (p.getRodzajPowiadomienia() == dzikiZachod.Powiadomienia.Wydarzenie.LECZENIE)
            wypiszKomunikatZOpcjonalnymArgumentem("ULECZ", p.getWykonawca(), p.getOdbiorca());
        if (p.getRodzajPowiadomienia() == dzikiZachod.Powiadomienia.Wydarzenie.ZWIEKSZENIE_ZASIEGU_O_DWA)
            wypiszKomunikatORuchu("ZASIEG_PLUS_DWA");
        if (p.getRodzajPowiadomienia() == dzikiZachod.Powiadomienia.Wydarzenie.ZWIEKSZENIE_ZASIEGU)
            wypiszKomunikatORuchu("ZASIEG_PLUS_JEDEN");
        if (p.getRodzajPowiadomienia() == dzikiZachod.Powiadomienia.Wydarzenie.STRZAL)
            wypiszKomunikatZOpcjonalnymArgumentem("STRZEL", p.getWykonawca(), p.getOdbiorca());
        if (p.getRodzajPowiadomienia() == dzikiZachod.Powiadomienia.Wydarzenie.WYPUSZCZENIE_DYNAMITU)
            wypiszKomunikatORuchu("DYNAMIT");
//        if (p.getRodzajPowiadomienia() == dzikiZachod.Powiadomienia.Wydarzenie.ZABOJSTWO)
        if (p.getRodzajPowiadomienia() == dzikiZachod.Powiadomienia.Wydarzenie.KONIEC_ZAGRYWKI)
            wypiszStanGraczy();
        if (p.getRodzajPowiadomienia() == Wydarzenie.KONIEC_RUCHOW)
            powiadomOZakonczeniuRuchow(p.getWykonawca());
        if (p.getRodzajPowiadomienia() == Wydarzenie.KONIEC_GRY)
            powiadomOKoncuGry();
        if (p.getRodzajPowiadomienia() == Wydarzenie.REMIS)
            powiadomORemisie();
        if (p.getRodzajPowiadomienia() == Wydarzenie.ZWYCIESTWO_BANDYTOW)
            powiadomOZwyciestwieBandytow();
        if (p.getRodzajPowiadomienia() == Wydarzenie.ZWYCIESTWO_SZERYFA)
            powiadomOZwyciestwieSzeryfa();
    }

    private void powiadomORozpoczeciuGry () {
        System.out.println("** START");
        wypiszStanGraczy();
    }

    private void powiadomONowejTurze (int numerTury) {
        System.out.println("** TURA " + numerTury);
    }

    private void powiadomOZagrywce (int gracz) {
        System.out.print("  GRACZ " + (gracz + 1) + " (");
        wypiszFrakcje(gracze.get(gracz));
        System.out.println("):");
    }

    private void powiadomODobraniuKart (int gracz) {
        wypiszReke(gracz);
    }

    private void powiadomOPoczatkuRuchow () {
        System.out.println("    Ruchy:");
    }

    private void powiadomOZakonczeniuRuchow (int gracz) {
        if (!gracze.get(gracz).czyWGrze()) { //gracz umarł w trakcie swojej zagrywki
            System.out.println("      MARTWY");
        }
        System.out.println();
    }

    private void powiadomOStanieDynamitu (int ofiara, boolean czyWybuchl) {
        System.out.print("    Dynamit: <");
        if (czyWybuchl) {
            System.out.print("WYBUCHŁ");
        } else System.out.print("PRZECHODZI DALEJ");
        System.out.println(">");
        if (!gracze.get(ofiara).czyWGrze()) { //gracz umarł w wyniku wybuchu dynamitu
            System.out.println("    Ruchy:");
            System.out.println("      MARTWY");
            System.out.println();
        }
    }

    private void powiadomOKoncuGry () {
        System.out.println("** KONIEC");
    }

    private void powiadomORemisie () {
        System.out.println("  REMIS - OSIĄGNIĘTO LIMIT TUR");
    }

    private void powiadomOZwyciestwieBandytow () {
        System.out.println("  WYGRANA STRONA: <bandyci>");
    }

    private void powiadomOZwyciestwieSzeryfa () {
        System.out.println("  WYGRANA STRONA: <szeryf i pomocnicy>");

    }
}
