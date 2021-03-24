#include <omnetpp.h>
using namespace omnetpp; //for Omnet++ ver. 5

class Server : public cSimpleModule
{		
  private:
    cQueue queue;	            //the queue of jobs; it is assumed that the first job in the queue is the one being serviced
	cMessage *departure;        //special message; it reminds about the end of service and the need for job departure
	simtime_t departure_time;   //time of the next departure
	int lost, accepted, consecutive;
	double L, B, CL, K, avg_time_to_buffer, buffer_size;
	cHistogram buffer_overflow_period, avg_time_to_buffer_overflow, number_of_consecutive_losses;
	simtime_t full_queue, time_to_buffer_overflow_begin;
	bool timeToBufferOverflowActive;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msgin);
    bool drop_function(int queue_size);

};

Define_Module(Server);


void Server::initialize()
{	
	departure = new cMessage("Departure");

	buffer_size = par("N");
	lost = 0;
	accepted = 0;
	consecutive = 0;
	L= 0; // Loss Ratio
	B = 0; // Burst Ratio
	CL = 0; //  average value of CL
	K = 0; // calculated using the loss ratio
	avg_time_to_buffer = 0;
	timeToBufferOverflowActive = true;

    WATCH(L);
    WATCH(B);
    WATCH(K);
    WATCH(CL);
    WATCH(avg_time_to_buffer);

    buffer_overflow_period.setName("Buffer Overflow Period");
    buffer_overflow_period.setRange(0.0, 50);
    buffer_overflow_period.setNumBinsHint(100);
    avg_time_to_buffer_overflow.setName("Time to Buffer Overflow");
    number_of_consecutive_losses.setName("Number of Consecutive Losses");
}


void Server::handleMessage(cMessage *msgin)  //two types of messages may arrive: a job from the source, or the special message initiating job departure
{		
    if (msgin==departure)   //job departure
	{
        if (consecutive > 0) {
                    number_of_consecutive_losses.collect(consecutive);
                    L = (double) lost / (lost + accepted);
                    CL = (double) number_of_consecutive_losses.getMean();
                    K = (double) 1 / (1 - L);
                    B = (double) CL / K;
                    consecutive  = 0;
        }
		cMessage *msg = (cMessage *)queue.pop();    //remove the finished job from the head of the queue

		if(queue.getLength() == buffer_size - 1)
		{
		    buffer_overflow_period.collect(simTime() - full_queue);
		}
		else if(queue.isEmpty() && !timeToBufferOverflowActive) // Avoid entering this path if the measurement is already in progress
		{
		    timeToBufferOverflowActive = true;
		    time_to_buffer_overflow_begin = simTime();
		}

		send(msg,"out");                            //depart the finished job

		if (!queue.empty())                         //if the queue is not empty, initiate the next service, i.e. schedule the next departure event in the future
		{
			departure_time=simTime()+par("service_time");
	        scheduleAt(departure_time,departure);
		}
	}
	else if (queue.getLength() < buffer_size)                   //job arrival
	{
	    /**
	    if (drop_function(queue.getLength())){
	        delete msgin;
	        lost ++;
	        consecutive ++;
	    } else { **/

		if (queue.empty())  //if the queue is empty, the job that has just arrived has to be served immediately, i.e. the departure event of this job has to be scheduled in the future
		{
		    time_to_buffer_overflow_begin = simTime();
			departure_time=simTime()+par("service_time");
            scheduleAt(departure_time,departure);
		}

		queue.insert(msgin); //insert the job at the end of the queue

		if(queue.getLength() == buffer_size)
		{
		    full_queue = simTime(); // buffer overflow beginning
		    avg_time_to_buffer_overflow.collect(simTime() - time_to_buffer_overflow_begin);
		    avg_time_to_buffer = avg_time_to_buffer_overflow.getMean();
		    timeToBufferOverflowActive = false;
		}
		accepted++;

	   // }
	}
	else // Queue is full
	{
	    delete msgin;
	    lost ++;
	    consecutive ++;
	}
}

bool Server::drop_function(int queue_size){
    double random = uniform(0, 1);

    // condition n < 20
    double exponent = (double) (queue_size - 20) / 4.f;
    double probability = pow(exp(1.0), exponent);

    EV <<random << probability;

    // condition n >= 20
    if (queue_size >= 20){
        return true;
    } else {
        return random < probability;
    }
}








