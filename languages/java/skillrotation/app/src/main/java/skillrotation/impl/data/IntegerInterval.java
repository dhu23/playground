package skillrotation.impl.data;

import com.google.common.base.Preconditions;
import org.immutables.value.Value;

@Value.Immutable
public abstract class IntegerInterval {
    @Value.Parameter
    public abstract int lower();

    @Value.Parameter
    public abstract int upper();

    @Value.Check
    public void check() {
        Preconditions.checkState(lower() <= upper(),
                "lower (%s) cannot be greater than upper (%s)",
                lower(), upper());
    }
}
