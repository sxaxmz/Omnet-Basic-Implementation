#include <stdio.h>
#include "omnetpp.h"
using namespace omnetpp; //for Omnet++ ver. 5

class Game : public cSimpleModule   //derive Game from cSimpleModule
{
  private:
    int num_sticks;
    int player_to_move;
    int next_player[3] = {2,3,1};

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msgin);
    virtual void finish();
};

Define_Module(Game);  //macro that registers a class with OMNeT++. Do not forget this!


void Game::initialize()  //this is executed once, before the actual simulation
{

    num_sticks = par("num_sticks");     // read parameter values from  omnetpp.ini or .ned file.
                                        // additionally, you can use methods: .boolValue(), .longValue(), .doubleValue(), .stringValue()
    player_to_move = par("first_move");

    EV << "\nNUMBER OF STICKS:  " << num_sticks;        //display info on the user interface
    EV << "\nPLAYER "<<player_to_move << " BEGINS...\n\n";

    char msgname[32];           //create the first message with the number of sticks on the table
    sprintf(msgname," %d sticks left",num_sticks);
    cMessage *msg = new cMessage(msgname);
    msg->setKind(num_sticks);

    if (player_to_move == 1)    //send the message with the number of stick on the table to a player, to initiate the first move
        send(msg, "to_player1");
    else if(player_to_move == 2)
        send(msg, "to_player2");
    else
        send(msg, "to_player3");
}


void Game::handleMessage(cMessage *msgin)  //this is executed several times - every time a message arrives to Game
{
    int sticks_taken = msgin->getKind();  //read the number of sticks taken by a player
    delete msgin;

    num_sticks -= sticks_taken;

    EV << "Player took " << sticks_taken << " stick(s).\n";
    EV << "Sticks left: " << num_sticks << "\n";

    if(num_sticks>0)
    {
        std::cout << player_to_move << std::endl;
        player_to_move = next_player[player_to_move-1];    //switch to the other player

        char msgname[32];                       //create the next message with the number of sticks on the table
        sprintf(msgname,"%d sticks left",num_sticks);
        cMessage *msg = new cMessage(msgname);
        msg->setKind(num_sticks);

        if (player_to_move == 1)    //send the message with the number of stick on the table to a player, to initiate the first move
            send(msg, "to_player1");
        else if(player_to_move == 2)
            send(msg, "to_player2");
        else
            send(msg, "to_player3");

    }
}


void Game::finish()  //this is executed once, at the end of the simulation
{
    EV << "\nPLAYER " << player_to_move << " LOSES!!!\n";   //display info on who losses the game
    EV << "End of the game. \n";
}



