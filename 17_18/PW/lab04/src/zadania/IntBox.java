package zadania;

public class IntBox {
    private Integer value;

    public IntBox(Integer a) {
        this.value = a;
    }
    public IntBox() { this(0); }

    public void setValue(Integer a) {
        this.value = a;
    }

    public Integer getValue() {
        return this.value;
    }
}
