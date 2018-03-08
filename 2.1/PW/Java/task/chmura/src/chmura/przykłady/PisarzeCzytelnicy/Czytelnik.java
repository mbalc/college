package chmura.przykłady.PisarzeCzytelnicy;

import chmura.przykłady.Przesuwacz;

class Czytelnik extends Przesuwacz {
    Czytelnik(int indeks) {
        super(
                Opcje.czytelnikWlasneSprawy,
                Opcje.czytelnikCzytanie,
                new int[]{-1, 0},
                new int[]{-1, 0},
                indeks,
                "Czytelnik",
                "czytać",
                Stan.chmura,
                Stan.mutex,
                () -> {
                },
                () -> {
                    if (Stan.iluPisze + Stan.czekaPis > 0) {
                        ++Stan.czekaCzyt;
                        Stan.V(Stan.mutex);
                        Stan.P(Stan.czytelnicy);
                        --Stan.czekaCzyt;
                    }
                    ++Stan.iluCzyta;
                    if (Stan.czekaCzyt > 0) {
                        Stan.V(Stan.czytelnicy);
                    } else {
                        Stan.V(Stan.mutex);
                    }
                },
                () -> {
                },
                () -> {
                    --Stan.iluCzyta;
                    if ((Stan.iluCzyta == 0) && (Stan.czekaPis > 0)) {
                        Stan.V(Stan.pisarze);
                    } else {
                        Stan.V(Stan.mutex);
                    }
                },
                Opcje.mnożnikCzasu
        );
    }
}
