abstract public class SolFunction {
    SolFunction() {}
    public abstract Sol call(Sol self, Sol[] arguments) throws Exception;
    Sol call() throws Exception { return call(null, null); }
    Sol call(Sol self) throws Exception { return call(self, null); }
    Sol call(Sol[] arguments) throws Exception { return call(null, arguments); }
}
