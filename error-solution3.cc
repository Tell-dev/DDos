/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc: In function ‘int main(int, char**)’:
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:81:13: error: ‘extraDeviceContainer’ was not declared in this scope; did you mean ‘botDeviceContainer’?
   81 |             extraDeviceContainer[i] = pp1.Install(nodes.Get(0), extraNodes.Get(i));
      |             ^~~~~~~~~~~~~~~~~~~~
      |             botDeviceContainer
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:85:13: error: ‘extraDeviceContainer’ was not declared in this scope; did you mean ‘botDeviceContainer’?
   85 |             extraDeviceContainer[i] = pp1.Install(nodes.Get(1), extraNodes.Get(i));
      |             ^~~~~~~~~~~~~~~~~~~~
      |             botDeviceContainer
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:89:13: error: ‘extraDeviceContainer’ was not declared in this scope; did you mean ‘botDeviceContainer’?
   89 |             extraDeviceContainer[i] = pp1.Install(nodes.Get(2), extraNodes.Get(i));
      |             ^~~~~~~~~~~~~~~~~~~~
      |             botDeviceContainer
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:106:44: error: ‘extraDeviceContainer’ was not declared in this scope; did you mean ‘botDeviceContainer’?
  106 |         extraInterfaces[i] = ipv4_n.Assign(extraDeviceContainer[i]);
      |                                            ^~~~~~~~~~~~~~~~~~~~
      |                                            botDeviceContainer
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:145:23: error: cannot declare variable ‘routingHelper’ to be of abstract type ‘ns3::Ipv4RoutingHelper’
  145 |     Ipv4RoutingHelper routingHelper;
      |                       ^~~~~~~~~~~~~
In file included from /home/ddos/ns-allinone-3.42/ns-3.42/src/internet/helper/ipv4-global-routing-helper.h:22,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/ipv4-global-routing-helper.h:1,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/internet-module.h:9,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:5:
/home/ddos/ns-allinone-3.42/ns-3.42/src/internet/helper/ipv4-routing-helper.h:45:7: note:   because the following virtual functions are pure within ‘ns3::Ipv4RoutingHelper’:
   45 | class Ipv4RoutingHelper
      |       ^~~~~~~~~~~~~~~~~
/home/ddos/ns-allinone-3.42/ns-3.42/src/internet/helper/ipv4-routing-helper.h:60:32: note:     ‘virtual ns3::Ipv4RoutingHelper* ns3::Ipv4RoutingHelper::Copy() const’
   60 |     virtual Ipv4RoutingHelper* Copy() const = 0;
      |                                ^~~~
/home/ddos/ns-allinone-3.42/ns-3.42/src/internet/helper/ipv4-routing-helper.h:66:38: note:     ‘virtual ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::Ipv4RoutingHelper::Create(ns3::Ptr<ns3::Node>) const’
   66 |     virtual Ptr<Ipv4RoutingProtocol> Create(Ptr<Node> node) const = 0;
      |                                      ^~~~~~
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:146:58: error: ‘class ns3::Ipv4RoutingHelper’ has no member named ‘GetStaticRouting’; did you mean ‘GetRouting’?
  146 |     Ptr<Ipv4StaticRouting> staticRouting = routingHelper.GetStaticRouting(firewallIpv4);
      |                                                          ^~~~~~~~~~~~~~~~
      |                                                          GetRouting
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:190:16: error: ‘class ns3::Ipv4FlowProbe’ has no member named ‘SerializeToXmlFile’; did you mean ‘SerializeToXmlStream’?
  190 |     flowProbe->SerializeToXmlFile("flow-monitor-results.xml", true, true);
      |                ^~~~~~~~~~~~~~~~~~
      |                SerializeToXmlStream
gmake[3]: *** [scratch/CMakeFiles/scratch_solution3.dir/build.make:76: scratch/CMakeFiles/scratch_solution3.dir/solution3.cc.o] Error 1
gmake[2]: *** [CMakeFiles/Makefile2:17430: scratch/CMakeFiles/scratch_solution3.dir/all] Error 2
gmake[1]: *** [CMakeFiles/Makefile2:17436: scratch/CMakeFiles/scratch_solution3.dir/rule] Error 2
gmake: *** [Makefile:6126: scratch_solution3] Error 2
