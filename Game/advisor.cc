#include <stdio.h>
#include "omnetpp.h"
using namespace omnetpp; //for Omnet++ ver. 5

class Advisor : public cSimpleModule
{
    virtual int calculateMove(int num_sticks);
  protected:
    virtual void handleMessage(cMessage *wiad);
};

Define_Module(Advisor);    //macro that registers a class with OMNeT++. Do not forget this!


void Advisor::handleMessage(cMessage *msgin)
{
    int num_sticks = msgin->getKind();    //read the number of sticks on the table
    delete msgin;

    int move = calculateMove(num_sticks);     //calculate the number of sticks to take

    char msgname[32];                    //create a message with the number of sticks to take
    sprintf(msgname," Taking %d", move);
    cMessage *msgout = new cMessage(msgname);
    msgout->setKind(move);

    sendDelayed(msgout,uniform(0,10), "out");               //send the message to Game
}


int Advisor::calculateMove(int num_sticks)  //calculate the number of sticks to take, based on the number of sticks on the table
                                           //any algorithm can be used, but we are not interested in this part while learning Omnet++
{
    int move = (num_sticks + 4) % 5;
    if (move == 0)  move = 1;
    return move;
}

