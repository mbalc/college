package zadania.zadanie6;

import zadania.Macierz;

import java.util.concurrent.Callable;

public class LiczMacierz implements Callable<Integer> {

    Integer ID_WIERSZA;
    Integer ID_KOLUMNY;

    LiczMacierz(Integer idWiersza, Integer idKolumny) {
        this.ID_WIERSZA = idWiersza;
        this.ID_KOLUMNY = idKolumny;
    }

    @Override
    public Integer call() throws Exception {
        return Macierz.wartość(ID_WIERSZA, ID_KOLUMNY);
    }
}
