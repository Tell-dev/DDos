ddos@ddos-VirtualBox:~/ns-allinone-3.42/ns-3.42$ ./ns3 run solution3.cc
[  0%] Building CXX object scratch/CMakeFiles/scratch_solution3.dir/solution3.cc.o
In file included from /home/ddos/ns-allinone-3.42/ns-3.42/src/core/model/attribute.h:22,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/attribute.h:1,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/src/mobility/helper/group-mobility-helper.h:25,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/group-mobility-helper.h:1,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/build/include/ns3/mobility-module.h:6,
                 from /home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:1:
/home/ddos/ns-allinone-3.42/ns-3.42/src/core/model/ptr.h: In instantiation of ‘ns3::Ptr<T>::Ptr(const ns3::Ptr<U>&) [with U = ns3::Ipv4; T = ns3::Ipv4RoutingProtocol]’:
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:155:92:   required from here
/home/ddos/ns-allinone-3.42/ns-3.42/src/core/model/ptr.h:683:24: error: cannot convert ‘ns3::Ipv4*’ to ‘ns3::Ipv4RoutingProtocol*’ in initialization
  683 |     : m_ptr(PeekPointer(o))
      |             ~~~~~~~~~~~^~~
      |                        |
      |                        ns3::Ipv4*
gmake[3]: *** [scratch/CMakeFiles/scratch_solution3.dir/build.make:76: scratch/CMakeFiles/scratch_solution3.dir/solution3.cc.o] Error 1
gmake[2]: *** [CMakeFiles/Makefile2:17430: scratch/CMakeFiles/scratch_solution3.dir/all] Error 2
gmake[1]: *** [CMakeFiles/Makefile2:17436: scratch/CMakeFiles/scratch_solution3.dir/rule] Error 2
gmake: *** [Makefile:6126: scratch_solution3] Error 2
