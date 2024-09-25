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
    LogComponentEnable("DDoSAttack", LOG_LEVEL_INFO);

    // สร้างโหนดในระบบจำลอง
    NodeContainer nodes;
    nodes.Create(4);  // 4 legitimate nodes

    // สร้าง bot nodes
    NodeContainer botNodes;
    botNodes.Create(NUMBER_OF_BOTS);

    // สร้าง extra nodes
    NodeContainer extraNodes;
    extraNodes.Create(NUMBER_OF_EXTRA_NODES);  // 6 extra legitimate nodes

    // สร้าง firewall node
    Ptr<Node> firewallNode = CreateObject<Node>();
    NodeContainer firewallNodes;
    firewallNodes.Add(firewallNode); // Adding firewall node

    // ติดตั้ง Internet stack
    InternetStackHelper stack;
    stack.Install(nodes);
    stack.Install(botNodes);
    stack.Install(extraNodes);
    stack.Install(firewallNode); // Install Internet stack on the firewall node

    // กำหนด Point-To-Point Links และพารามิเตอร์
    PointToPointHelper pp1, ppFirewall;
    pp1.SetDeviceAttribute("DataRate", StringValue("500Kbps"));
    pp1.SetChannelAttribute("Delay", StringValue("10ms"));

    ppFirewall.SetDeviceAttribute("DataRate", StringValue("500Kbps"));
    ppFirewall.SetChannelAttribute("Delay", StringValue("1ms"));

    // ติดตั้งการเชื่อมต่อ Point-To-Point ระหว่าง Nodes
    NetDeviceContainer d02, d12, d23, d33, d43;
    d02 = pp1.Install(nodes.Get(0), nodes.Get(1));  // Node 0 connected to Node 1
    d12 = pp1.Install(nodes.Get(0), nodes.Get(2));  // Node 0 connected to Node 2
    d23 = pp1.Install(nodes.Get(1), nodes.Get(3));  // Node 1 connected to Node 3
    d33 = pp1.Install(nodes.Get(2), nodes.Get(3));  // Node 2 connected to Node 3
    d43 = pp1.Install(nodes.Get(1), nodes.Get(2));  // Node 1 connected to Node 2

    // เชื่อมต่อ bot nodes กับ firewall
    NetDeviceContainer botDeviceContainer[NUMBER_OF_BOTS];
    NetDeviceContainer firewallBotLinks[NUMBER_OF_BOTS];
    for (int i = 0; i < NUMBER_OF_BOTS; ++i)
    {
        firewallBotLinks[i] = ppFirewall.Install(botNodes.Get(i), firewallNode);
    }

    // เชื่อมต่อ extra nodes กับ nodes หลัก
    NetDeviceContainer extraDeviceContainer[NUMBER_OF_EXTRA_NODES];
    for (int i = 0; i < NUMBER_OF_EXTRA_NODES; ++i)
    {
        if (i < 2)
        {
            // Extra node 0, 1 connected to Node 0
            extraDeviceContainer[i] = pp1.Install(nodes.Get(0), extraNodes.Get(i));
        }
        else if (i >= 2 && i < 4)
        {
            // Extra node 2, 3 connected to Node 1
            extraDeviceContainer[i] = pp1.Install(nodes.Get(1), extraNodes.Get(i));
        }
        else
        {
            // Extra node 4, 5 connected to Node 2
            extraDeviceContainer[i] = pp1.Install(nodes.Get(2), extraNodes.Get(i));
        }
    }

    // กำหนดที่อยู่ IP
    Ipv4AddressHelper ipv4_n;
    ipv4_n.SetBase("10.0.0.0", "255.255.255.252");

    Ipv4InterfaceContainer extraInterfaces[NUMBER_OF_EXTRA_NODES];
    for (int i = 0; i < NUMBER_OF_EXTRA_NODES; ++i)
    {
        extraInterfaces[i] = ipv4_n.Assign(extraDeviceContainer[i]);
        ipv4_n.NewNetwork();
    }

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

    // กำหนด IP สำหรับ bot-to-firewall connections
    for (int j = 0; j < NUMBER_OF_BOTS; ++j)
    {
        ipv4_n.Assign(firewallBotLinks[j]);
        ipv4_n.NewNetwork();
    }

    // ตั้งค่า DDoS Application บน bot nodes
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

    // บล็อก IP ของ bot ที่ firewall โดยใช้ Static Routing
    Ipv4StaticRoutingHelper staticRoutingHelper;
    Ptr<Ipv4StaticRouting> firewallRouting = staticRoutingHelper.GetStaticRouting(firewallNode->GetObject<Ipv4>());

    for (int i = 0; i < NUMBER_OF_BOTS; ++i)
    {
        Ptr<Ipv4> botIpv4 = botNodes.Get(i)->GetObject<Ipv4>();
        Ipv4Address botAddress = botIpv4->GetAddress(1, 0).GetLocal();
        // เพิ่มเส้นทางไปยัง bot IP โดยไม่กำหนด gateway เพื่อบล็อก traffic
        firewallRouting->AddHostRouteTo(botAddress, Ipv4Address("0.0.0.0"), 1);
    }

    // ตั้งค่า BulkSend บน extra nodes เพื่อส่ง TCP data ไปยัง node 3
    OnOffHelper onoffTcp("ns3::TcpSocketFactory", Address(InetSocketAddress(i23.GetAddress(1), TCP_SINK_PORT)));
    onoffTcp.SetConstantRate(DataRate(DATA_RATE));
    ApplicationContainer OnOffAppExtra[NUMBER_OF_EXTRA_NODES];

    for (int k = 0; k < NUMBER_OF_EXTRA_NODES; k++)
    {
        OnOffAppExtra[k] = onoffTcp.Install(extraNodes.Get(k));
        OnOffAppExtra[k].Start(Seconds(5.0));
        OnOffAppExtra[k].Stop(Seconds(MAX_SIMULATION_TIME));
    }

    // ติดตั้ง UDPSink บน receiver (node 3)
    PacketSinkHelper UDPsink("ns3::UdpSocketFactory", Address(InetSocketAddress(Ipv4Address::GetAny(), UDP_SINK_PORT)));
    ApplicationContainer UDPSinkApp = UDPsink.Install(nodes.Get(3));  // ติดตั้งบนโหนดที่ 3
    UDPSinkApp.Start(Seconds(0.0));
     // ติดตั้ง TCP Sink Application บน server (node 3)
    PacketSinkHelper TCPsink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), TCP_SINK_PORT));
    ApplicationContainer TCPSinkApp = TCPsink.Install(nodes.Get(3));  // ติดตั้งบนโหนดที่ 3
    TCPSinkApp.Start(Seconds(0.0));
    TCPSinkApp.Stop(Seconds(MAX_SIMULATION_TIME));

    // กำหนดเส้นทางแบบ Global
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // การตั้งค่า NetAnim และการวางตำแหน่งโหนด
    MobilityHelper mobility;

    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(0.0), "MinY", DoubleValue(0.0),
                                  "DeltaX", DoubleValue(5.0), "DeltaY", DoubleValue(10.0),
                                  "GridWidth", UintegerValue(5), "LayoutType", StringValue("RowFirst"));

    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(nodes);
    mobility.Install(botNodes);
    mobility.Install(extraNodes);  // ติดตั้งตำแหน่งโหนด user ใหม่

    AnimationInterface anim("test.xml");

    // โหลดไอคอนเข้าสู่ NetAnim
    uint32_t node0Icon = anim.AddResource("ns-allinone-3.42/ns-3.42/icon/internet.png");
    uint32_t node1Icon = anim.AddResource("ns-allinone-3.42/ns-3.42/icon/router.png");
    uint32_t node2Icon = anim.AddResource("ns-allinone-3.42/ns-3.42/icon/router.png");
    uint32_t node3Icon = anim.AddResource("ns-allinone-3.42/ns-3.42/icon/web_server.png");
    uint32_t botIcon = anim.AddResource("ns-allinone-3.42/ns-3.42/icon/bot.png");
    uint32_t extraNodeIcon = anim.AddResource("ns-allinone-3.42/ns-3.42/icon/computer.png");  // ไอคอนสำหรับโหนด user ใหม่

    // กำหนดไอคอนให้กับแต่ละโหนด
    anim.UpdateNodeImage(nodes.Get(0)->GetId(), node0Icon);
    anim.UpdateNodeImage(nodes.Get(1)->GetId(), node1Icon);
    anim.UpdateNodeImage(nodes.Get(2)->GetId(), node2Icon);
    anim.UpdateNodeImage(nodes.Get(3)->GetId(), node3Icon);  // กำหนด icon ให้กับโหนดที่ 3

    // กำหนดไอคอนให้กับ bot nodes
    for (int i = 0; i < NUMBER_OF_BOTS; ++i)
    {
        anim.UpdateNodeImage(botNodes.Get(i)->GetId(), botIcon);
    }

    // กำหนดไอคอนให้กับ extra nodes
    for (int i = 0; i < NUMBER_OF_EXTRA_NODES; ++i)
    {
        anim.UpdateNodeImage(extraNodes.Get(i)->GetId(), extraNodeIcon);
    }

    // ตั้งตำแหน่งให้กับโหนดหลัก
    anim.SetConstantPosition(nodes.Get(0), 50, 45);
    anim.SetConstantPosition(nodes.Get(1), 100, 30);
    anim.SetConstantPosition(nodes.Get(2), 110, 50);
    anim.SetConstantPosition(nodes.Get(3), 160, 35);  // วางตำแหน่งของโหนดที่ 3

    // ตั้งตำแหน่งให้กับ extra nodes
    for (int i = 0; i < NUMBER_OF_EXTRA_NODES; ++i)
    {
        if (i < 2)
        {
            // Extra node 0, 1 connected to Node 0
            anim.SetConstantPosition(extraNodes.Get(i), 70 + (i * 10), 60);
        }
        else if (i >= 2 && i < 4)
        {
            // Extra node 2, 3 connected to Node 1
            anim.SetConstantPosition(extraNodes.Get(i), 80 + (i * 10), 20);
        }
        else
        {
            // Extra node 4, 5 connected to Node 2
            anim.SetConstantPosition(extraNodes.Get(i), 90 + (i * 10), 60);
        }
    }

    // ตั้งค่า Flow Monitor
    FlowMonitorHelper flowHelper;
    Ptr<FlowMonitor> flowMonitor = flowHelper.InstallAll();

    Simulator::Stop(Seconds(MAX_SIMULATION_TIME));

    // เริ่มการจำลอง
    Simulator::Run();

    // บันทึกข้อมูล Flow Monitor เป็นไฟล์ XML
    flowMonitor->SerializeToXmlFile("solution-3-bot50.xml", true, true);

    Simulator::Destroy();
    return 0;
}
