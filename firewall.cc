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
#define NUMBER_OF_EXTRA_NODES 6

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("DDoSAttack");

// ฟังก์ชันสำหรับกรอง packet ที่เข้ามา
void FirewallDropPackets(Ptr<Packet> packet, Ipv4InterfaceContainer botInterfaces, Ptr<Socket> firewallSocket)
{
    Ipv4Header ipv4Header;
    packet->PeekHeader(ipv4Header);

    // เช็คว่า packet มาจาก bot nodes หรือไม่
    for (int i = 0; i < NUMBER_OF_BOTS; ++i)
    {
        if (ipv4Header.GetSource() == botInterfaces[i].GetAddress(0))
        {
            // ถ้าเป็น bot node ให้ drop packet
            return;
        }
    }

    // ส่ง packet ต่อไปที่ node 3
    firewallSocket->Send(packet);
}

int main(int argc, char *argv[])
{
    CommandLine cmd;
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    // Legitimate connection nodes (4 nodes)
    NodeContainer nodes;
    nodes.Create(4);

    // Nodes for attack bots
    NodeContainer botNodes;
    botNodes.Create(NUMBER_OF_BOTS);

    // Nodes for extra users
    NodeContainer extraNodes;
    extraNodes.Create(NUMBER_OF_EXTRA_NODES);

    // Define the Point-To-Point Links and their Parameters
    PointToPointHelper pp1, pp2, pp3, ppFirewall;
    pp1.SetDeviceAttribute("DataRate", StringValue("500Kbps"));
    pp1.SetChannelAttribute("Delay", StringValue("10ms"));
    pp3.SetDeviceAttribute("DataRate", StringValue("1Mbps"));
    pp3.SetChannelAttribute("Delay", StringValue("1ms"));

    // Install the Point-To-Point Connections between Nodes
    NetDeviceContainer d02, d12, d23, d33, d43;
    d02 = pp1.Install(nodes.Get(0), nodes.Get(1));
    d12 = pp1.Install(nodes.Get(0), nodes.Get(2));
    d23 = pp1.Install(nodes.Get(1), nodes.Get(3));
    d33 = pp1.Install(nodes.Get(2), nodes.Get(3));
    d43 = pp1.Install(nodes.Get(1), nodes.Get(2));

    // Bot nodes connect to legitimate node 0
    NetDeviceContainer botDeviceContainer[NUMBER_OF_BOTS];
    for (int i = 0; i < NUMBER_OF_BOTS; ++i)
    {
        botDeviceContainer[i] = pp3.Install(botNodes.Get(i), nodes.Get(0));
    }

    // Extra nodes connect to legitimate nodes
    NetDeviceContainer extraDeviceContainer[NUMBER_OF_EXTRA_NODES];
    for (int i = 0; i < NUMBER_OF_EXTRA_NODES; ++i)
    {
        if (i < 2)
        {
            extraDeviceContainer[i] = pp3.Install(nodes.Get(0), extraNodes.Get(i));
        }
        else if (i >= 2 && i < 4)
        {
            extraDeviceContainer[i] = pp3.Install(nodes.Get(1), extraNodes.Get(i));
        }
        else if (i >= 4)
        {
            extraDeviceContainer[i] = pp3.Install(nodes.Get(2), extraNodes.Get(i));
        }
    }

    // Assign IP addresses to legitimate nodes and bots
    InternetStackHelper stack;
    stack.Install(nodes);
    stack.Install(botNodes);
    stack.Install(extraNodes);

    Ipv4AddressHelper ipv4_n;
    ipv4_n.SetBase("10.0.0.0", "255.255.255.252");

    Ipv4InterfaceContainer extraInterfaces[NUMBER_OF_EXTRA_NODES];
    for (int i = 0; i < NUMBER_OF_EXTRA_NODES; ++i)
    {
        extraInterfaces[i] = ipv4_n.Assign(extraDeviceContainer[i]);
        ipv4_n.NewNetwork();
    }

    Ipv4InterfaceContainer botInterfaces[NUMBER_OF_BOTS];
    for (int j = 0; j < NUMBER_OF_BOTS; ++j)
    {
        botInterfaces[j] = ipv4_n.Assign(botDeviceContainer[j]);
        ipv4_n.NewNetwork();
    }

    // Assign IP addresses to legitimate nodes
    Ipv4AddressHelper a02, a12, a23, a33, a43;
    a02.SetBase("10.1.1.0", "255.255.255.0");
    a12.SetBase("10.1.2.0", "255.255.255.0");
    a23.SetBase("10.1.3.0", "255.255.255.0");
    a33.SetBase("10.1.4.0", "255.255.255.0");
    a43.SetBase("10.1.5.0", "255.255.255.0");

    Ipv4InterfaceContainer i02, i12, i23, i33, i43;
    i02 = a02.Assign(d02);
    i12 = a12.Assign(d12);
    i23 = a23.Assign(d23);
    i33 = a33.Assign(d33);
    i43 = a43.Assign(d43);

    // สร้าง Firewall Node
    NodeContainer firewallNode;
    firewallNode.Create(1);

    // เชื่อมต่อ Firewall กับ Node 0 และ Node 3
    ppFirewall.SetDeviceAttribute("DataRate", StringValue("500Kbps"));
    ppFirewall.SetChannelAttribute("Delay", StringValue("10ms"));

    NetDeviceContainer firewallToNode0Device = ppFirewall.Install(firewallNode.Get(0), nodes.Get(0));
    NetDeviceContainer firewallToNode3Device = ppFirewall.Install(firewallNode.Get(0), nodes.Get(3));

    // กำหนด IP Address ให้กับ Firewall Node
    Ipv4AddressHelper ipv4Firewall;
    ipv4Firewall.SetBase("10.1.6.0", "255.255.255.0");
    Ipv4InterfaceContainer firewallInterfaces = ipv4Firewall.Assign(firewallToNode0Device);
    ipv4Firewall.Assign(firewallToNode3Device);

    // สร้าง socket สำหรับ firewall
    Ptr<Socket> firewallSocket = Socket::CreateSocket(firewallNode.Get(0), UdpSocketFactory::GetTypeId());
    firewallSocket->Bind(InetSocketAddress(Ipv4Address::GetAny(), UDP_SINK_PORT));
    firewallSocket->SetRecvCallback(MakeCallback(&FirewallDropPackets, botInterfaces, firewallSocket));

    // DDoS Application Behaviour
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

    // BulkSend on extra nodes to send TCP data to node3
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
    ApplicationContainer UDPSinkApp = UDPsink.Install(nodes.Get(3));
    UDPSinkApp.Start(Seconds(0.0));
    UDPSinkApp.Stop(Seconds(MAX_SIMULATION_TIME));

    // TCP Sink on the receiver side
    PacketSinkHelper TCPsink("ns3::TcpSocketFactory", Address(InetSocketAddress(Ipv4Address::GetAny(), TCP_SINK_PORT)));
    ApplicationContainer TCPSinkApp = TCPsink.Install(nodes.Get(3));
    TCPSinkApp.Start(Seconds(0.0));
    TCPSinkApp.Stop(Seconds(MAX_SIMULATION_TIME));

    // สร้าง flow monitor
    FlowMonitorHelper flowHelper;
    Ptr<FlowMonitor> flowMonitor = flowHelper.InstallAll();

    // Run simulation
    Simulator::Stop(Seconds(MAX_SIMULATION_TIME));
    Simulator::Run();
    
    // Print statistics
    flowMonitor->SerializeToXmlFile("DDoSFlowMonitor.xml", true, true);
    
    Simulator::Destroy();
    return 0;
}
