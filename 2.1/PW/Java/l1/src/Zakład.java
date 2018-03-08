import java.util.ArrayList;
import java.util.List;

public class Zakład {


    public static void main(String[] args) {
        new Fryzjer();
        for (int i = 0; i < Dane.getInstance().liczbaKlientow; ++i) {
            new Klient(i + 1);
        }
    }



}

