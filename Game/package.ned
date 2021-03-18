simple Game					//simple module Game
{
    parameters:
        int num_sticks; 	// starting number of sticks
        int first_move; 	// 1=Player1, 2=Player2
    gates:
        input from_player1; // input and output gates for connecting player1 and player2
        input from_player2;  
        input from_player3;
        output to_player3;
        output to_player1;
        output to_player2;
}


simple Player		//simple module Player
{
	parameters:
    gates:
        input in; 	// gates for connecting to Game
        output out;
}
simple Cheater		//simple module Player
{
	parameters:
    gates:
        input in_game; 	// gates for connecting to Game
        output out_game;
        input from_advisor; 	// gates for connecting to Game
        output to_advisor;

}
simple Advisor		//simple module Player
{
	parameters:
    gates:
        input in; 	// gates for connecting to Game
        output out;
}



module Nim			// compound module Nim - game and two players interconnected. The whole simulator is a compound module! 
{
	parameters:		
    submodules:
        game: Game//create instance 'game' of module type 'Game'. Unspecified parameters (num_sticks and first_move) will be taken from omnetpp.ini file. Or asked for before the simulation, if not present in the .ini file.  
        {
            parameters:
                @display("i=abstract/table");
        }
        player1: Player		// create instance 'player1' of module type 'Player'
        {
            parameters:
                @display("i=abstract/person");
        }
        player2: Player 
        {
            parameters:
                @display("i=abstract/person,black");
        }
        player3: Cheater
        {
            parameters:
                @display("i=abstract/person,black");
        }
        advisor: Advisor
        {
            parameters:
                @display("i=abstract/person,black");
        }
    connections:
        player1.out --> game.from_player1;
        player1.in <-- game.to_player1;

        player2.out --> game.from_player2;
        player2.in <-- game.to_player2;

        player3.out_game --> game.from_player3;
        player3.in_game <-- game.to_player3;

        player3.to_advisor --> advisor.in;
        player3.from_advisor <-- advisor.out;


}


network nim extends Nim		// create instance 'nim' of module type 'Nim'. 
{
    parameters:
}




