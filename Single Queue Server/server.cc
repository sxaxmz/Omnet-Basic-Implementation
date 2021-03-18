#include <omnetpp.h>
using namespace omnetpp; //for Omnet++ ver. 5

class Server : public cSimpleModule
{		
  private:
    cQueue queue;	            //the queue of jobs; it is assumed that the first job in the queue is the one being serviced
	cMessage *departure, *measure, *front_job, *finish_msg;        //special message; it reminds about the end of service and the need for job departure
	simtime_t departure_time, T, response_time, interarrival_time, service_time, waiting_time, end_time;
	cLongHistogram hist_queue_size, hist_queue_size_arrival;
	cDoubleHistogram hist_virtual_waiting_time, hist_waiting_time;
	cOutVector lengthq, response_time_vec, interarrival_time_vec; // Queue Length

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msgin);
    virtual void finish();

};

Define_Module(Server);


void Server::initialize()
{	
	departure = new cMessage("Departure");
	measure = new cMessage("Measurement");
	T = 1;
	scheduleAt(T, measure);

	/**
	end_time = 5;
	finish_msg = new cMessage("Finish");
	scheduleAt(end_time, finish_msg);
	**/

	 // Queue
	 queue.setName("queue");

	 // Waiting Time Histogram
	 hist_waiting_time.setName("histogram of waiting time");
	 hist_waiting_time.setRange(0.0, 5.0);
	 hist_virtual_waiting_time.setNumCells(200);

	 // Queue Size At Arrival Histogram
	 hist_queue_size_arrival.setName("histogram of queue size at arrival");
	 hist_queue_size_arrival.setRange(0.0, 5.0);

     // Queue Size Histogram
     hist_queue_size.setName("histogram of queue size");
     hist_queue_size.setRange(0.0, 5.0);

     // Virtual Waiting Time Histogram
     hist_virtual_waiting_time.setName("histogram of virtual waiting time");
     hist_virtual_waiting_time.setRange(0.0, 5.0);
     hist_virtual_waiting_time.setNumCells(200);

	 // Queue Length
	 lengthq.setName("Number of items in the queue");

	 response_time_vec.setName("Responses time");
	 interarrival_time_vec.setName("Interarrival time");
}


void Server::handleMessage(cMessage *msgin)  //two types of messages may arrive: a job from the source, or the special message initiating job departure
{		
    if (msgin == measure)
    {
        // Virtual Time
        if (queue.getLength() == 0){
            hist_virtual_waiting_time.collect(0);
        } else if (queue.getLength() == 1) {
            hist_virtual_waiting_time.collect(departure_time - simTime());
        } else {
            simtime_t workload;
            for(int i = 0; i<queue.getLength(); i++){
                workload += par("service_time");
            }
            hist_virtual_waiting_time.collect(workload + (departure_time - simTime()));
        }

        // Queue Size
        hist_queue_size.collect(queue.getLength()); // Queue Size Collection
        scheduleAt(simTime()+T, measure); // Schedule next measurement
    }

    else if (msgin==departure)   //job departure
	{
		cMessage *msg = (cMessage *)queue.pop();    //remove the finished job from the head of the queue
		lengthq.record(queue.getLength()); // Record deduction
		send(msg,"out");                            //depart the finished job
		if (!queue.empty())                         //if the queue is not empty, initiate the next service, i.e. schedule the next departure event in the future
		{
		    front_job = (cMessage *)queue.front();
		    waiting_time = simTime() - front_job->getTimestamp();
		    interarrival_time_vec.record(msgin->getTimestamp());
		    hist_waiting_time.collect(waiting_time); // Waiting Queue Time Collection
		    service_time = par("service_time");
		    response_time = waiting_time + service_time;
		    response_time_vec.record(response_time);
		    EV << "Response Time: " << response_time;
			departure_time=simTime()+par("service_time");
	        scheduleAt(departure_time,departure);
		}
	}
	else                    //job arrival
	{
	    msgin->setTimestamp();
	    hist_queue_size_arrival.collect(queue.getLength()); // Queue Size at Arrival Times Collection
		if (queue.empty())  //if the queue is empty, the job that has just arrived has to be served immediately, i.e. the departure event of this job has to be scheduled in the future
		{
		    waiting_time = 0;
		    interarrival_time_vec.record(msgin->getTimestamp());
		    hist_waiting_time.collect(waiting_time); // Waiting Queue Time Collection
		    service_time = par("service_time");
		    response_time = waiting_time + service_time;
		    response_time_vec.record(response_time);
		    EV << "Response Time: " << response_time;
			departure_time=simTime()+par("service_time");
            scheduleAt(departure_time,departure);
		}
		queue.insert(msgin); //insert the job at the end of the queue
		lengthq.record(queue.getLength()); // Record Addition
	}
}

void Server::finish(){
    hist_waiting_time.recordAs("hist_waiting");
    hist_queue_size.recordAs("hist_queue_size");
    hist_queue_size_arrival.recordAs("hist_queue_size_arrival");
    hist_virtual_waiting_time.recordAs("hist_virtual_waiting");
    EV << "Mean Waiting Time: " << hist_waiting_time.getMean() << endl;
    EV << "Mean Queue Size: " << hist_queue_size.getMean() << endl;
    EV << "Mean Queue Size At Arrival: " << hist_queue_size_arrival.getMean() << endl;
    EV << "Mean Virtual Waiting Time: " << hist_virtual_waiting_time.getMean() << endl;
}








