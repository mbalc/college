package chmura.przykłady.ProducenciKonsumenci;

import chmura.przykłady.Przesuwacz;

class Konsument extends Przesuwacz {
    Konsument(int indeks) {
        super(
                Opcje.zakresCzasuKons,
                Opcje.zakresCzasuPobierania,
                new int[]{0, 1},
                new int[]{0, 0},
                indeks,
                "Konsument",
                "pobierać produkt z bufora",
                Fabryka.chmura,
                Fabryka.byt,
                () -> {
                },
                () -> {
                },
                () -> {
                },
                () -> {
                },
                Opcje.mnożnikCzasu
        );
    }
}
