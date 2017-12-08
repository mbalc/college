package chmura;

import java.util.function.BiPredicate;

class Miejsce {

    private int x;
    private int y;

    @Override
    public String toString() {
        return "<{" + x + "; " + y + "}>";
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Miejsce miejsce = (Miejsce) o;

        if (x != miejsce.x) return false;
        return y == miejsce.y;
    }

    @Override
    public int hashCode() {
        int result = x;
        result = 31 * result + y;
        return result;
    }

    Miejsce(int x, int y) {
        this.x = x;
        this.y = y;
    }

    boolean sprawdź(BiPredicate<Integer, Integer> predykat) {
        return predykat.test(x, y);
    }

    Miejsce przesuń(int dx, int dy) {
        return new Miejsce (this.x + dx, this.y + dy);
    }

    int[] dajNormal() {
        return new int[]{x, y};
    }
}

