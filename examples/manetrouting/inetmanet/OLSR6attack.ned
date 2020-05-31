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

package inet.examples.manetrouting.inetmanet;


// import inet.networklayer.ipv4.RoutingTableRecorder;  
import inet.environment.common.*;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.AdhocHost;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;


network OLSR6attack
{
    parameters:
        int numHosts; // number of nodes in the network
    submodules:
        configurator: Ipv4NetworkConfigurator; // network autoconfiguration
        radioMedium: Ieee80211ScalarRadioMedium; // 802.11 physical medium
        host[numHosts]: AdhocHost; // ad-hoc wifi nodes
        physicalEnvironment: PhysicalEnvironment {
            @display("p=580,425");
        }
        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
            @display("p=580,125");
        }
}




