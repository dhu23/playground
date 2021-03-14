import LogAnalysis
import Log

main = do
    print $ parseMessage "This is not in the right format"
    print $ parseMessage "I 29 la la la"
    print $ parseMessage "E 2 562 help help"
    
    logs <- testParse parse 10 "error.log"
    print logs

    errs <- testWhatWentWrong parse whatWentWrong "sample.log"
    print errs

