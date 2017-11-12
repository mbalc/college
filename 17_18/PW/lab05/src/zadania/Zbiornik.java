package zadania;

public class Zbiornik<T> {
    public T elem;
    public int count = 0;

    Zbiornik(T elem) {
        this.elem = elem;
    }

    synchronized void push() {
        ++count;
        this.notify();
    }

    synchronized void pull() throws InterruptedException {
        if (count < 1) this.wait();
        --count;
    }
}
