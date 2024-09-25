home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc: In function ‘int main(int, char**)’:
/home/ddos/ns-allinone-3.42/ns-3.42/scratch/solution3.cc:153:65: error: ‘GetStaticRouting’ is not a member of ‘ns3::Ipv4RoutingHelper’
  153 |     Ptr<Ipv4StaticRouting> firewallRouting = Ipv4RoutingHelper::GetStaticRouting(firewallNode->GetObject<Ipv4>());
      |                                                                 ^~~~~~~~~~~~~~~~
gmake[3]: *** [scratch/CMakeFiles/scratch_solution3.dir/build.make:76: scratch/CMakeFiles/scratch_solution3.dir/solution3.cc.o] Error 1
gmake[2]: *** [CMakeFiles/Makefile2:17430: scratch/CMakeFiles/scratch_solution3.dir/all] Error 2
gmake[1]: *** [CMakeFiles/Makefile2:17436: scratch/CMakeFiles/scratch_solution3.dir/rule] Error 2
gmake: *** [Makefile:6126: scratch_solution3] Error 2
