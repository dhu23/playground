package yieldcpu;

public class Record {
    public final String data;
    public final long nano;

    public Record(String data) {
        this.data = data;
        this.nano = System.nanoTime();
    }
}
