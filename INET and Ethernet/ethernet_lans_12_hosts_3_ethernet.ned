//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package inet.examples.ethernet.lans;


import inet.common.misc.ThruputMeteringChannel;
import inet.linklayer.ethernet.EtherBus;
import inet.linklayer.ethernet.EtherHub;
import inet.node.ethernet.EtherHost;
import inet.node.ethernet.EtherSwitch;
import ned.DatarateChannel;

network TwelveHosts
{
    @display("bgb=871,500");
    submodules:
        hostC: EtherHost {
            parameters:
                @display("p=195.78125,126.875");
        }
        hostD: EtherHost {
            parameters:
                @display("p=267,400");
        }
        hostA: EtherHost {
            parameters:
                @display("p=66,369");
        }
        hostB: EtherHost {
            parameters:
                @display("p=100,200");
        }
        hub1: EtherHub {
            parameters:
                @display("p=278.90625,304.0625");
            gates:
                ethg[5];
        }
        hostF: EtherHost {
            parameters:
                @display("p=490,84");
        }
        hostH: EtherHost {
            parameters:
                @display("p=309.53125,32.8125");
        }
        hostG: EtherHost {
            parameters:
                @display("p=428.75,32.8125");
        }
        hostE: EtherHost {
            parameters:
                @display("p=289.84375,126.875");
        }
        hub2: EtherHub {
            parameters:
                @display("p=402.5,109.375");
            gates:
                ethg[6];
        }
        hostK: EtherHost {
            parameters:
                @display("p=748.125,199.0625");
        }
        hostL: EtherHost {
            parameters:
                @display("p=760.15625,325.9375");
        }
        hostJ: EtherHost {
            parameters:
                @display("p=565.46875,399.21875");
        }
        hostI: EtherHost {
            parameters:
                @display("p=654.0625,126.875");
        }
        hub3: EtherHub {
            parameters:
                @display("p=620.15625,276.71875");
            gates:
                ethg[5];
        }
    connections:
        hub1.ethg[0] <--> {  delay = 0.1us; datarate = 100Mbps; } <--> hub2.ethg[0];
        hub1.ethg[1] <--> {  delay = 0.1us; datarate = 100Mbps; } <--> hostC.ethg;
        hub1.ethg[2] <--> {  delay = 0.3us; datarate = 100Mbps; } <--> hostD.ethg;
        hub1.ethg[3] <--> {  delay = 0.4us; datarate = 100Mbps; } <--> hostA.ethg;
        hub1.ethg[4] <--> {  delay = 0.5us; datarate = 100Mbps; } <--> hostB.ethg;


        hub2.ethg[1] <--> {  delay = 0.1us; datarate = 100Mbps; } <--> hostF.ethg;
        hub2.ethg[2] <--> {  delay = 0.3us; datarate = 100Mbps; } <--> hostH.ethg;
        hub2.ethg[3] <--> {  delay = 0.4us; datarate = 100Mbps; } <--> hostG.ethg;
        hub2.ethg[4] <--> {  delay = 0.2us; datarate = 100Mbps; } <--> hostE.ethg;
        hub2.ethg[5] <--> {  delay = 0.1us; datarate = 100Mbps; } <--> hub3.ethg[0];


        hub3.ethg[1] <--> {  delay = 0.1us; datarate = 100Mbps; } <--> hostK.ethg;
        hub3.ethg[2] <--> {  delay = 0.3us; datarate = 100Mbps; } <--> hostL.ethg;
        hub3.ethg[3] <--> {  delay = 0.4us; datarate = 100Mbps; } <--> hostJ.ethg;
        hub3.ethg[4] <--> {  delay = 0.2us; datarate = 100Mbps; } <--> hostI.ethg;
}
