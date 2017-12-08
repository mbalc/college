package chmura.przykłady.ProducenciKonsumenci;

import chmura.przykłady.Przesuwacz;

class Producent extends Przesuwacz {
    Producent(int indeks) {
        super(
                Opcje.zakresCzasuProd,
                Opcje.zakresCzasuUmieszczania,
                new int[]{1, 0},
                new int[]{0, 0},
                indeks,
                "Producent",
                "umieszczać produkt w buforze",
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
