ddos@ddos-VirtualBox:~/ns-allinone-3.42/ns-3.42$ ./ns3 run solution3.cc
[  0%] Building CXX object scratch/CMakeFiles/scratch_solution3.dir/solution3.cc.o
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc: In function ‘int main(int, char**)’:
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:159:55: error: ‘NullRoutingProtocol’ was not declared in this scope
  159 |         firewallIpv4->SetRoutingProtocol(CreateObject<NullRoutingProtocol>()); // Use NullRoutingProtocol to block bot IPs
      |                                                       ^~~~~~~~~~~~~~~~~~~
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:159:75: error: no matching function for call to ‘CreateObject<<expression error> >()’
  159 |         firewallIpv4->SetRoutingProtocol(CreateObject<NullRoutingProtocol>()); // Use NullRoutingProtocol to block bot IPs
      |                                          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~
In file included from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/object.h:1,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/src/network/model/net-device.h:29,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/src/network/model/node.h:23,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/node.h:1,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/src/network/helper/node-container.h:22,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/node-container.h:1,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/src/mobility/helper/group-mobility-helper.h:26,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/group-mobility-helper.h:1,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/mobility-module.h:6,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:1:
/home/ddos/ns-allinone-3.42/ns-3.42/src/core/model/object.h:630:1: note: candidate: ‘template<class T, class ... Args> ns3::Ptr<T> ns3::CreateObject(Args&& ...)’
  630 | CreateObject(Args&&... args)
      | ^~~~~~~~~~~~
/home/ddos/ns-allinone-3.42/ns-3.42/src/core/model/object.h:630:1: note:   template argument deduction/substitution failed:
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:159:75: error: template argument 1 is invalid
  159 |         firewallIpv4->SetRoutingProtocol(CreateObject<NullRoutingProtocol>()); // Use NullRoutingProtocol to block bot IPs
      |                                          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~
gmake[3]: *** [scratch/CMakeFiles/scratch_solution3.dir/build.make:76: scratch/CMakeFiles/scratch_solution3.dir/solution3.cc.o] Error 1
gmake[2]: *** [CMakeFiles/Makefile2:17430: scratch/CMakeFiles/scratch_solution3.dir/all] Error 2
gmake[1]: *** [CMakeFiles/Makefile2:17436: scratch/CMakeFiles/scratch_solution3.dir/rule] Error 2
gmake: *** [Makefile:6126: scratch_solution3] Error 2
