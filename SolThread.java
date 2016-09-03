public class SolThread extends Thread {
    Lambda lambda;
    SolThread(Lambda lambda) { this.lambda = lambda; }
    public void run() {
	try {
	    try {
		lambda.eval();
	    } catch(EvaluationRequest evaluationRequest) {
		evaluationRequest.eval();
	    }
	} catch(Exception e) {
	    e.printStackTrace();
	}
    }
}

