simple Source
{
    parameters:
        volatile double interarrival_time;   //the 'volatile' modifier indicates that a parameter is re-read every time a value is needed during simulation. Important for random numbers! 		 					  
    gates:
        output out;
}


simple Server
{
    parameters:
        volatile double service_time;   
        volatile double N;   
    gates:
        input in;
        output out;
}


simple Sink
{
    gates:
        input in;
}


module GG1
{
	parameters:
	
    submodules:
        source: Source 
        {
            parameters:
                @display("i=block/source");
        }
        server: Server 
        {
            parameters:
                @display("i=block/queue");
        }
        sink: Sink 
        {
            parameters:
                @display("i=block/sink");
        }
    connections:
        source.out --> server.in;
        server.out --> sink.in;
}


network gg1 extends GG1
{
    parameters:
}
