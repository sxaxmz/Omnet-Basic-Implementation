    #include <stdio.h>
#include "omnetpp.h"
using namespace omnetpp; //for Omnet++ ver. 5

class Cheater : public cSimpleModule
{
    virtual int calculateMove(int num_sticks);
  protected:
    bool waiting_for_advisor = false;
    virtual void handleMessage(cMessage *wiad);
};

Define_Module(Cheater);    //macro that registers a class with OMNeT++. Do not forget this!


void Cheater::handleMessage(cMessage *msgin)
{//read the number of sticks on the table
    if(msgin->arrivedOn("from_advisor")){
        int move = msgin->getKind();
        char msgname[32];                    //create a message with the number of sticks to take
        sprintf(msgname," Taking %d", move);
        cMessage *msgout = new cMessage(msgname);
        msgout->setKind(move);
        waiting_for_advisor = false;
        if (!waiting_for_advisor){
        //
                sendDelayed(msgout,uniform(0,10), "out_game");

            }
    }else{
        int num_sticks = msgin->getKind();
        delete msgin;
        int move = calculateMove(num_sticks);     //calculate the number of sticks to take
        char msgname[32];                    //create a message with the number of sticks to take
        sprintf(msgname," Taking %d", move);
        cMessage *msgout = new cMessage(msgname);
        msgout->setKind(move);
        if (!waiting_for_advisor){
        //
                sendDelayed(msgout,uniform(0,10), "out_game");

            }

    }


               //send the message to Game
}


int Cheater::calculateMove(int num_sticks)  //calculate the number of sticks to take, based on the number of sticks on the table                                          //any algorithm can be used, but we are not interested in this part while learning Omnet++
{  
     if(num_sticks % 2 == 1){
         return 4;
    }else{
        char msgname[32];                       //create the next message with the number of sticks on the table
        sprintf(msgname,"%d sticks left",num_sticks);
        cMessage *msg = new cMessage(msgname);
        msg->setKind(num_sticks);
        sendDelayed(msg,uniform(0,10), "to_advisor");
        waiting_for_advisor = true;
        return -1;
    }
}

