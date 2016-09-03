server = ServerSocket port: 60000.
{
   client = server listen.
   {
     "----------------------------------------------------------------------" printLine.
     data = client receiveAll.
     data ifNothing: {#EmptyData throw}.
     http = data asString.
     http ifNothing: {#EmptyRequestError throw}.
     "--- Richiesta ricevuta ---" printLine.
     http print.
     request = Pattern match: "GET ([^?]*)(\\?.*)? HTTP" on: http.
     request ifNothing: {#EmptyRequestError throw}.
     requestedFile = nothing.
     GET = nothing.
     request size == 1 ifTrue: {requestedFile = request}.
     request size == 2 ifTrue: {
        requestedFile = request first.
        parameterString = request second.
	"||| Parameter string: " print.
	parameterString printLine.
        parameterList = Pattern matchVector: "[?&]([^?&]*)" on: parameterString.
	parameterList = parameterList map: {x | Pattern match: "(.*)=(.*)" on: x}.
	"||| Parameter list: " print.
	parameterList printLine.
	GET = StringDictionary new.
	parameterList map: { parameter | GET at: parameter first set: parameter second }.
     }.
     requestedFile == "/" ifTrue: {requestedFile = "index.html"}.
     "---- File richiesto ---- " printLine.
     requestedFile printLine.
     "---- Richiesta GET ----" printLine.
     GET printLine.
     file = InputFile read: "" + requestedFile.
     client send: "HTTP/1.1 200 OK\n\n".
     client send: file.
   } try: #printStackTrace action.
   client close.
} repeat
