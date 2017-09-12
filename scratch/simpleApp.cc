#include "ns3/core-module.h"
#include "ns3/propagation-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/wifi-module.h"

NS_LOG_COMPONENT_DEFINE ("simpleApp");

using namespace ns3;

int main (int argc, char *argv[])
{ 

  
  //uint32_t m_packetSize;              
  //uint32_t m_bytesTotal;  
  double simulationTime=10;
  uint32_t payloadSize = 1500;
  NodeContainer wifiStaNode;
  NodeContainer wifiApNode;
  NetDeviceContainer apDevice;
  NetDeviceContainer staDevice;

  //Create nodes
  wifiStaNode.Create (1);
  wifiApNode.Create (1);

  //Create wireless channel
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  //LogComponentEnable ("WifiPhy", LOG_LEVEL_FUNCTION);


  //Default IEEE 802.11n (2.4 GHz)
  WifiHelper wifi;
  //wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
  wifi.SetStandard (WIFI_PHY_STANDARD_80211ow);
  //wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode",    StringValue ("HtMcs7"), "ControlMode", StringValue ("HtMcs7"));
  
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OwcRate10Gbps"), "ControlMode", StringValue ("OwcRate10Gbps"));


  WifiMacHelper mac;

  //Install PHY and MAC
  Ssid ssid = Ssid ("ns3-wifi");
  mac.SetType ("ns3::StaWifiMac", "Ssid", SsidValue (ssid),"QosSupported", BooleanValue (true), "BE_MaxAmpduSize", UintegerValue (32768), "BE_MaxAmsduSize", UintegerValue (3839));
 /* mac.SetType ("ns3::StaWifiMac",
               "QosSupported", BooleanValue (false),
               "Ssid", SsidValue (ssid));*/



  //NetDeviceContainer staDevice;
  staDevice = wifi.Install (phy, mac, wifiStaNode);

  mac.SetType ("ns3::ApWifiMac", "Ssid", SsidValue (ssid),"QosSupported", BooleanValue (true), "BE_MaxAmpduSize", UintegerValue (32768), "BE_MaxAmsduSize", UintegerValue (3839));
  /*mac.SetType ("ns3::ApWifiMac",
               "QosSupported", BooleanValue (false),
               "Ssid", SsidValue (ssid));*/

  //NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);
  

  //Mobility
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();

  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (1.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  mobility.Install (wifiApNode);
  mobility.Install (wifiStaNode);

  //Internet stack
  InternetStackHelper stack;
  stack.Install (wifiApNode);
  stack.Install (wifiStaNode);

  Ipv4AddressHelper address;

  address.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer StaInterface;
  StaInterface = address.Assign (staDevice);
  Ipv4InterfaceContainer ApInterface;
  ApInterface = address.Assign (apDevice);

  //staNodeInterface = address.Assign (staDevice);
  //apNodeInterface = address.Assign (apDevice);

  //Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/Sifs", TimeValue (MicroSeconds (1)));
  //Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/Slot", TimeValue (MicroSeconds (1)));	
  //Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/Rifs", TimeValue (MicroSeconds (1)));	

  uint16_t port = 5001;
  UdpServerHelper server (port);
  ApplicationContainer serverApp = server.Install (wifiApNode.Get (0));
  serverApp.Start (Seconds (0.0));
  serverApp.Stop (Seconds (simulationTime + 1));

  InetSocketAddress dest (ApInterface.GetAddress (0), port);
  //dest.SetTos (0x70); //AC_BE

  OnOffHelper client ("ns3::UdpSocketFactory", dest);
  client.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
  client.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"));
  //client.SetAttribute ("OnTime", StringValue ("ns3::ExponentialRandomVariable[Mean=1.0]"));
  //client.SetAttribute ("OffTime", StringValue ("ns3::ExponentialRandomVariable[Mean=0.1]"));
  client.SetAttribute ("DataRate", StringValue ("5000Mb/s"));
  //client.SetAttribute ("DataRate", StringValue ("600000000"));
  client.SetAttribute ("PacketSize", UintegerValue (payloadSize));

  ApplicationContainer clientApp = client.Install (wifiStaNode);
  clientApp.Start (Seconds (1.0));
  clientApp.Stop (Seconds (simulationTime + 1));
  
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  
  Simulator::Stop (Seconds (simulationTime + 1));
  Simulator::Run ();
  monitor->SerializeToXmlFile("TOS.xml", true, true);
  Simulator::Destroy ();

  uint64_t totalPacketsThrough = DynamicCast<UdpServer> (serverApp.Get (0))->GetReceived ();
  double throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0);
  std::cout << "Total Throughput: " << throughput << " Mbit/s" << '\n' << '\n';
 
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  double accumulatedThroughput = 0;
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i=stats.begin(); 
        i!=stats.end(); ++i)
  {    
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
    std::cout << "Flow " << i->first<< " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
    std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
    std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
    std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
    std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
    std::cout << "  Lost Packets: " << i->second.lostPackets << "\n";
    std::cout << "  Pkt Lost Ratio: " << ((double)i->second.txPackets-(double)i->second.rxPackets)/(double)i->second.txPackets << "\n";
    std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / simulationTime / 1024 / 1024  << " Mbps\n";
    accumulatedThroughput+=(i->second.rxBytes*8.0/simulationTime/1024/1024);
  } 


  

  return 0;
}
