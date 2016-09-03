import java.util.*;

public class Evaluator {
    public static Sol eval(Atom atom) throws Exception {
	return eval(atom, null, null, null);
    }
    public static Sol eval(Atom atom, Environment environment) throws Exception {
	return eval(atom, null, null, environment);
    }
    public static Sol eval(Atom atom, Sol self, Sol contextClass, Environment environment) throws Exception {
	while(true) 
	    if(atom == null)
		return null;
	    else if(atom instanceof MessageAtom) {
		MessageAtom messageAtom = (MessageAtom)atom;
		Atom messageReceiver = messageAtom.getReceiver();
		Sol receiver, clazz;
		if(messageReceiver instanceof IdentifierAtom &&
		   ((IdentifierAtom)messageReceiver).getIdentifier().equals("super")) {
		    if(contextClass == null)
			throw new RuntimeException("Cannot refer to super while not in a valid context.");
		    receiver = self;	    
		    clazz = contextClass.getVariable("superclass");
		} else {
		    receiver = eval(messageReceiver, self, contextClass, environment);
		    clazz = receiver == null ? Classes.Nothing.get() : receiver.getClazz();
		}

		Atom[] messageArguments = messageAtom.getArguments();
		Sol[] arguments = new Sol[messageArguments.length];
		for(int i=0; i<messageArguments.length; i++)
		    arguments[i] = eval(messageArguments[i], self, contextClass, environment);

		Message message = messageAtom.getMessage();
		try {
		    return Sol.sendWER(receiver, message, arguments, clazz);
		} catch(EvaluationRequest request) {
		    atom = request.atom;
		    self = request.self;
		    contextClass = request.contextClass;
		    environment = request.environment;
		    continue;
		}
	    } else if(atom instanceof IdentifierAtom) {
		IdentifierAtom identifierAtom = (IdentifierAtom)atom;
		String identifier = identifierAtom.getIdentifier();
		if(identifier.equals("self"))
		    if(self == null)
			throw new RuntimeException(atom, "Cannot refer to self while not in a valid context.");
		    else
			return self;
		else if(identifier.equals("super"))
		    throw new RuntimeException("Cannot refer to super as a standalone variable.");
		else if(identifier.equals("nothing"))
		    return null;
		else if(environment.has(identifier))
		    return environment.get(identifier);
		else if(self != null && self.hasVariable(identifier))
		    return self.getVariable(identifier);
		else
		    throw new RuntimeException(atom, "Undefined variable \"" + identifier + "\".");
	    } else if(atom instanceof SequenceAtom) {
		SequenceAtom sequenceAtom = (SequenceAtom)atom;
		Atom[] sequence = sequenceAtom.getSequence();
		for(int i=0; i<sequence.length-1; i++)
		    eval(sequence[i], self, contextClass, environment);

		atom = sequence[sequence.length-1];
		continue;
	    } else if(atom instanceof AssignmentAtom) {
		AssignmentAtom assignmentAtom = (AssignmentAtom)atom;
		String variable = assignmentAtom.getVariable();
		Atom expression = assignmentAtom.getExpression();
		Sol value = eval(expression, self, contextClass, environment);
		if(self != null && self.hasVariable(variable))
		    self.setVariable(variable, value);
		else if(environment.has(variable))
		    environment.set(variable, value);
		else
		    environment.define(variable, value);
		return self;
	    } else if(atom instanceof LambdaAtom) {
		LambdaAtom lambda = (LambdaAtom)atom;
		Atom body = lambda.getBody();
		String[] parameters = lambda.getParameters();
		return new Sol(Classes.Lambda, new Lambda(body, parameters,
							  self, environment));
	    } else if(atom instanceof VectorAtom) {
		VectorAtom vectorAtom = (VectorAtom)atom;
		Atom[] vector = vectorAtom.getVector();
		Sol[] elements = new Sol[vector.length];
		for(int i=0; i<vector.length; i++)
		    elements[i] = eval(vector[i], self, contextClass, environment);
		return new Sol(Classes.Vector, new ArrayList<Sol>(Arrays.asList(elements)));
	    } else if(atom instanceof IntegerAtom) {
		return new Sol(Classes.Integer, ((IntegerAtom)atom).getInteger());
	    } else if(atom instanceof DoubleAtom) {
		return new Sol(Classes.Double, ((DoubleAtom)atom).getDouble());
	    } else if(atom instanceof CharacterAtom) {
		return new Sol(Classes.Character, ((CharacterAtom)atom).getCharacter());
	    } else if(atom instanceof StringAtom) {
		return new Sol(Classes.String, ((StringAtom)atom).getString());
	    } else if(atom instanceof MessageSymbolAtom) {
	        return new Sol(Classes.Message, ((MessageSymbolAtom)atom).getMessage());
	    } else {
		System.out.println("Fatal error, unrecognized Atom.");
	    }
    }
}
