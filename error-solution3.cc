ddos@ddos-VirtualBox:~/ns-allinone-3.42/ns-3.42$ ./ns3 run solution3-1.cc
[  0%] Building CXX object scratch/CMakeFiles/scratch_solution3-1.dir/solution3-1.cc.o
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3-1.cc: In function ‘int main(int, char**)’:
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3-1.cc:188:113: error: no matching function for call to ‘ns3::PacketSinkHelper::PacketSinkHelper(ns3::TypeId, ns3::InetSocketAddress)’
  188 |     PacketSinkHelper sink(TcpSocketFactory::GetTypeId(), InetSocketAddress(Ipv4Address::GetAny(), TCP_SINK_PORT));
      |                                                                                                                 ^
In file included from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/packet-sink-helper.h:1,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/applications-module.h:8,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3-1.cc:7:
/home/ddos/ns-allinone-3.42/ns-3.42/src/applications/helper/packet-sink-helper.h:46:5: note: candidate: ‘ns3::PacketSinkHelper::PacketSinkHelper(const std::string&, const ns3::Address&)’
   46 |     PacketSinkHelper(const std::string& protocol, const Address& address);
      |     ^~~~~~~~~~~~~~~~
/home/ddos/ns-allinone-3.42/ns-3.42/src/applications/helper/packet-sink-helper.h:46:41: note:   no known conversion for argument 1 from ‘ns3::TypeId’ to ‘const std::string&’ {aka ‘const std::__cxx11::basic_string<char>&’}
   46 |     PacketSinkHelper(const std::string& protocol, const Address& address);
      |                      ~~~~~~~~~~~~~~~~~~~^~~~~~~~
/home/ddos/ns-allinone-3.42/ns-3.42/src/applications/helper/packet-sink-helper.h:33:7: note: candidate: ‘ns3::PacketSinkHelper::PacketSinkHelper(const ns3::PacketSinkHelper&)’
   33 | class PacketSinkHelper : public ApplicationHelper
      |       ^~~~~~~~~~~~~~~~
/home/ddos/ns-allinone-3.42/ns-3.42/src/applications/helper/packet-sink-helper.h:33:7: note:   candidate expects 1 argument, 2 provided
/home/ddos/ns-allinone-3.42/ns-3.42/src/applications/helper/packet-sink-helper.h:33:7: note: candidate: ‘ns3::PacketSinkHelper::PacketSinkHelper(ns3::PacketSinkHelper&&)’
/home/ddos/ns-allinone-3.42/ns-3.42/src/applications/helper/packet-sink-helper.h:33:7: note:   candidate expects 1 argument, 2 provided
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3-1.cc:202:32: error: no matching function for call to ‘ns3::FlowMonitor::SerializeToXmlFile(const char [16], bool, bool, bool)’
  202 |     monitor->SerializeToXmlFile("flowMonitor.xml", true, true, true);
      |     ~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
In file included from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/flow-monitor.h:1,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3-1.cc:10:
/home/ddos/ns-allinone-3.42/ns-3.42/src/flow-monitor/model/flow-monitor.h:275:10: note: candidate: ‘void ns3::FlowMonitor::SerializeToXmlFile(std::string, bool, bool)’
  275 |     void SerializeToXmlFile(std::string fileName, bool enableHistograms, bool enableProbes);
      |          ^~~~~~~~~~~~~~~~~~
/home/ddos/ns-allinone-3.42/ns-3.42/src/flow-monitor/model/flow-monitor.h:275:10: note:   candidate expects 3 arguments, 4 provided
gmake[3]: *** [scratch/CMakeFiles/scratch_solution3-1.dir/build.make:76: scratch/CMakeFiles/scratch_solution3-1.dir/solution3-1.cc.o] Error 1
gmake[2]: *** [CMakeFiles/Makefile2:17433: scratch/CMakeFiles/scratch_solution3-1.dir/all] Error 2
gmake[1]: *** [CMakeFiles/Makefile2:17439: scratch/CMakeFiles/scratch_solution3-1.dir/rule] Error 2
gmake: *** [Makefile:6126: scratch_solution3-1] Error 2
