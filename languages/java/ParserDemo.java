import java.util.Map;
import java.util.Arrays;
import net.sourceforge.argparse4j.ArgumentParsers;
import net.sourceforge.argparse4j.inf.*;


public class ParserDemo {
    public static void main(String[] args) {
        System.out.println("Demo");

        ArgumentParser parser = ArgumentParsers.newFor("Demo").build()
            .defaultHelp(true).description("Demo program for parsing");

        Subparsers subparsers = parser.addSubparsers();
        _createSecuritiesCommand(subparsers.addParser("security"));

        // parser.printHelp();

        // Subparsers subparsers = parser.addSubparsers().title("subcommands");
        // 
        // Subparser printParser = subparsers.addParser("print").help("print something");
        // MutuallyExclusiveGroup group = printParser.addMutuallyExclusiveGroup();
        // group.addArgument("--traded").action(new PrintAction("Traded"));

        try {
            System.out.println("args=" + Arrays.asList(args));
            Namespace ns = parser.parseArgs(args);
            System.out.println("namespace=" + ns);
        } catch (ArgumentParserException e) {
            System.out.println("error=" + e);
            parser.handleError(e);
        }
    }

    private static void _createSecuritiesCommand(Subparser securitiesParser) {
        securitiesParser.help("securities information");
        Subparsers subparsers = securitiesParser.addSubparsers();
        Subparser subparser = subparsers.addParser("print").description("print securities");
        MutuallyExclusiveGroup group = subparser.addMutuallyExclusiveGroup();
        group.addArgument("-traded").help("list all traded")
            .action(new PrintAction("traded"));
    }

    private static class PrintAction implements ArgumentAction {
        private final String message;

        public PrintAction(String message) {
            this.message = message;
        }

        @Override
        public void run(
                ArgumentParser parser, Argument arg, 
                Map<String, Object> attrs, String flag, Object value) 
            throws ArgumentParserException {
            System.out.println("map=" + attrs);
            System.out.println("flag=" + flag);
            System.out.println("value=" + value);
        }

        @Override
        public void onAttach(Argument arg) {
        }

        @Override
        public boolean consumeArgument() {
            return false; // true means the flag takes an argument
        }
    }
}
