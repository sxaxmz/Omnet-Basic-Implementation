#include <omnetpp.h>
using namespace omnetpp; //for Omnet++ ver. 5

class Source : public cSimpleModule
{
	cMessage *send_event; //special message; it reminds about sending the next job

  protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msgin);
};

Define_Module(Source);


void Source::initialize()
{  
	send_event = new cMessage("Send!");                 //create the special message
    scheduleAt(par("interarrival_time"), send_event);   //schedule the first future reminder about sending a job
}


void Source::handleMessage(cMessage *msgin)             //when the reminder arrives ...
{	
    cMessage *job = new cMessage(" Job");               //create and send a job
	send(job, "out" );
	scheduleAt(simTime()+par("interarrival_time"), send_event); //schedule the next future reminder about sending a job
}

