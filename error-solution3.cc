#include "ns3/mobility-module.h"
#include "ns3/nstime.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor-module.h"

#define TCP_SINK_PORT 9000
#define UDP_SINK_PORT 9001

// Experimental parameters
#define DATA_RATE "1Mb/s"
#define DDOS_RATE "1Mb/s"
#define MAX_SIMULATION_TIME 60

// Number of Bots for DDoS
#define NUMBER_OF_BOTS 50
#define NUMBER_OF_EXTRA_NODES 6  // Number of extra legitimate nodes

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("DDoSAttack");

int main(int argc, char *argv[])
{
    CommandLine cmd;
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    // Legitimate connection nodes (4 nodes)
    NodeContainer nodes;
    nodes.Create(4);  // 4 legitimate nodes

    // Nodes for attack bots
    NodeContainer botNodes;
    botNodes.Create(NUMBER_OF_BOTS);

    // Nodes for extra users
    NodeContainer extraNodes;
    extraNodes.Create(NUMBER_OF_EXTRA_NODES);  // 6 extra legitimate nodes

    // Create the firewall node
    Ptr<Node> firewallNode = CreateObject<Node>();
    NodeContainer firewallNodes;
    firewallNodes.Add(firewallNode); // Adding firewall node

    // Define the Point-To-Point Links and their Parameters
    PointToPointHelper pp1, ppFirewallToNode3;
    pp1.SetDeviceAttribute("DataRate", StringValue("500Kbps"));
    pp1.SetChannelAttribute("Delay", StringValue("10ms"));

    // Define connection for the firewall
    ppFirewallToNode3.SetDeviceAttribute("DataRate", StringValue("500Kbps"));
    ppFirewallToNode3.SetChannelAttribute("Delay", StringValue("1ms"));

    // Install the Point-To-Point Connections between Nodes
    NetDeviceContainer d02, d12, d23, d33, d43, extraDeviceContainer[NUMBER_OF_EXTRA_NODES], firewallBotLinks[NUMBER_OF_BOTS];
    d02 = pp1.Install(nodes.Get(0), nodes.Get(1));  // Node 0 connected to Node 1
    d12 = pp1.Install(nodes.Get(0), nodes.Get(2));  // Node 0 connected to Node 2
    d23 = pp1.Install(nodes.Get(1), nodes.Get(3));  // Node 1 connected to Node 3
    d33 = pp1.Install(nodes.Get(2), nodes.Get(3));  // Node 2 connected to Node 3
    d43 = pp1.Install(nodes.Get(1), nodes.Get(2));  // Node 1 connected to Node 2

    // Connect bot nodes to node 0
    for (int i = 0; i < NUMBER_OF_BOTS; ++i)
    {
        firewallBotLinks[i] = pp1.Install(botNodes.Get(i), nodes.Get(0)); // Connect bot to node 0
    }

    // Install the connection between firewall and node 3
    NetDeviceContainer firewallToNode3 = ppFirewallToNode3.Install(firewallNode, nodes.Get(3));

    for (int i = 0; i < NUMBER_OF_EXTRA_NODES; ++i)
    {
        if (i < 2)
        {
            // Extra node 104, 105 connected to Node 0
            extraDeviceContainer[i] = pp1.Install(nodes.Get(0), extraNodes.Get(i));
        }
        else if (i >= 2 && i < 4)
        {
            // Extra node 106, 107 connected to Node 1
            extraDeviceContainer[i] = pp1.Install(nodes.Get(1), extraNodes.Get(i));
        }
        else if (i >= 4)
        {
            // Extra node 108, 109 connected to Node 2
            extraDeviceContainer[i] = pp1.Install(nodes.Get(2), extraNodes.Get(i));
        }
    }

    // Assign IP addresses to legitimate nodes, bots, and firewall
    InternetStackHelper stack;
    stack.Install(nodes);
    stack.Install(botNodes);
    stack.Install(extraNodes);
    stack.Install(firewallNode); // Install Internet stack on the firewall node

    Ipv4AddressHelper ipv4_n;
    ipv4_n.SetBase("10.0.0.0", "255.255.255.252");

    Ipv4InterfaceContainer extraInterfaces[NUMBER_OF_EXTRA_NODES];
    for (int i = 0; i < NUMBER_OF_EXTRA_NODES; ++i)
    {
        extraInterfaces[i] = ipv4_n.Assign(extraDeviceContainer[i]);
        ipv4_n.NewNetwork();
    }

    // Assign IP addresses to the firewall
    Ipv4AddressHelper ipv4_firewall;
    ipv4_firewall.SetBase("10.1.6.0", "255.255.255.0"); // IP address for firewall
    Ipv4InterfaceContainer firewallInterface = ipv4_firewall.Assign(firewallToNode3);

    // Assign IP addresses to legitimate nodes
    Ipv4InterfaceContainer i02, i12, i23, i33, i43;
    i02 = ipv4_n.Assign(d02);
    i12 = ipv4_n.Assign(d12);
    i23 = ipv4_n.Assign(d23);
    i33 = ipv4_n.Assign(d33);
    i43 = ipv4_n.Assign(d43);

    // DDoS Application Behaviour on bot nodes
    OnOffHelper onoff("ns3::UdpSocketFactory", Address(InetSocketAddress(i23.GetAddress(1), UDP_SINK_PORT)));
    onoff.SetConstantRate(DataRate(DDOS_RATE));
    onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=30]"));
    onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    ApplicationContainer onOffApp[NUMBER_OF_BOTS];
    for (int k = 0; k < NUMBER_OF_BOTS; ++k)
    {
        onOffApp[k] = onoff.Install(botNodes.Get(k));
        onOffApp[k].Start(Seconds(0.0));
        onOffApp[k].Stop(Seconds(MAX_SIMULATION_TIME));
    }

    // Blocking bot IPs at the firewall using a simple filtering approach
    Ptr<Ipv4> firewallIpv4 = firewallNode->GetObject<Ipv4>();
    for (int i = 0; i < NUMBER_OF_BOTS; ++i)
    {
        Ptr<Ipv4> botIpv4 = botNodes.Get(i)->GetObject<Ipv4>();
        Ipv4Address botAddress = botIpv4->GetAddress(1, 0).GetLocal();
        // Use a simple filtering method (the implementation might differ)
    }
    
    // BulkSend on extra nodes to send TCP data to node 3
    OnOffHelper onoffTcp("ns3::TcpSocketFactory", Address(InetSocketAddress(i23.GetAddress(1), TCP_SINK_PORT)));
    onoffTcp.SetConstantRate(DataRate(DATA_RATE));
    ApplicationContainer OnOffApp[NUMBER_OF_EXTRA_NODES];

    for (int k = 0; k < NUMBER_OF_EXTRA_NODES; k++)
    {
        OnOffApp[k] = onoffTcp.Install(extraNodes.Get(k));
        OnOffApp[k].Start(Seconds(5.0));
        OnOffApp[k].Stop(Seconds(MAX_SIMULATION_TIME));
    }

    // UDPSink on the receiver side
    PacketSinkHelper UDPsink("ns3::UdpSocketFactory", Address(InetSocketAddress(Ipv4Address::GetAny(), UDP_SINK_PORT)));
    ApplicationContainer UDPSinkApp = UDPsink.Install(nodes.Get(3));  // Install on node 3
    UDPSinkApp.Start(Seconds(0.0));
    UDPSinkApp.Stop(Seconds(MAX_SIMULATION_TIME));

    // TCP Sink Application on the server side
    PacketSinkHelper TCPsink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), TCP_SINK_PORT));
    ApplicationContainer TCPSinkApp = TCPsink.Install(nodes.Get(3));  // Install on node 3
    TCPSinkApp.Start(Seconds(0.0));
    TCPSinkApp.Stop(Seconds(MAX_SIMULATION_TIME));

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Simulation NetAnim configuration and node placement
    MobilityHelper mobility;

    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(0.0), "MinY", DoubleValue(0.0),
                                  "DeltaX", DoubleValue(5.0), "DeltaY", DoubleValue(10.0),
                                  "GridWidth", UintegerValue(5), "LayoutType", StringValue("RowFirst"));

    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(nodes);
    mobility.Install(botNodes);
    mobility.Install(extraNodes);
    mobility.Install(firewallNodes);  // Install mobility on the firewall node

    // Start Simulation
    Simulator::Stop(Seconds(MAX_SIMULATION_TIME));
    Simulator::Run();

    // Flow Monitor
    FlowMonitorHelper flowHelper;
    Ptr<FlowMonitor> monitor = flowHelper.InstallAll();
    monitor->CheckForLostPackets();

    // Visualization using NetAnim
    AnimationInterface anim("DDoSSimulation.xml");

    Simulator::Destroy();

    return 0;
}
