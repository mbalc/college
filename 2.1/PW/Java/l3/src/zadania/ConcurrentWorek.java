package zadania;


import java.util.HashMap;
import java.util.Map;


public class ConcurrentWorek<T> implements Worek<T> {

    private Map<T, Zbiornik<T>> grupy = new HashMap<>();

    @Override
    public void włóż(T wartość) {
        synchronized (grupy) {
            grupy.putIfAbsent(wartość, new Zbiornik<>(wartość));
        }
        grupy.get(wartość).push();
    }

    @Override
    public void wyjmij(T wartość) throws InterruptedException {
        synchronized (grupy) {
            grupy.putIfAbsent(wartość, new Zbiornik<>(wartość));
        }
        grupy.get(wartość).pull();
    }
}
