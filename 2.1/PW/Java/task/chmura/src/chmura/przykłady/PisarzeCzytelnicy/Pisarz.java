package chmura.przykłady.PisarzeCzytelnicy;

import chmura.przykłady.Przesuwacz;

class Pisarz extends Przesuwacz {
    Pisarz(int indeks) {
        super(
                Opcje.pisarzWlasneSprawy,
                Opcje.pisarzPisanie,
                new int[]{-1, 0},
                new int[]{-1, 0},
                indeks,
                "Pisarz",
                "pisać",
                Stan.chmura,
                Stan.mutex,
                () -> {
                },
                () -> {
                    if (Stan.iluPisze + Stan.iluCzyta > 0) {
                        ++Stan.czekaPis;
                        Stan.V(Stan.mutex);
                        Stan.P(Stan.pisarze);
                        --Stan.czekaPis;
                    }
                    ++Stan.iluPisze;
                    Stan.V(Stan.mutex);
                },
                () -> {
                },
                () -> {
                    --Stan.iluPisze;
                    if (Stan.czekaCzyt > 0) {
                        Stan.V(Stan.czytelnicy);
                    } else if (Stan.czekaPis > 0) {
                        Stan.V(Stan.pisarze);
                    } else {
                        Stan.V(Stan.mutex);
                    }
                },
                Opcje.mnożnikCzasu
        );
    }
}
