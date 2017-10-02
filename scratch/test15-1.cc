#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/gnuplot.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("DynamicQoS");

class QoS
{
public:
  QoS();
  void AddNode();
  void DropNode();
  void Initialize();
  void InstallApps();
  void InstantThroughput(double time);
  void Run();
  void AddApp(uint8_t AC, uint8_t node, Time stopTime);
  void UpdateCW();
  void RemoveApp(uint8_t AC, uint8_t node);

  Gnuplot2dDataset data_VO_S;
  Gnuplot2dDataset data_VI_S;
  Gnuplot2dDataset data_BE_S;
  Gnuplot2dDataset data_BK_S;

  Gnuplot2dDataset data_VO_Avg_S;
  Gnuplot2dDataset data_VI_Avg_S;
  Gnuplot2dDataset data_BE_Avg_S;
  Gnuplot2dDataset data_BK_Avg_S;
  
  Gnuplot2dDataset data_VO_D;
  Gnuplot2dDataset data_VI_D;
  Gnuplot2dDataset data_BE_D;
  Gnuplot2dDataset data_BK_D; 

  Gnuplot2dDataset data_VO_Avg_D;
  Gnuplot2dDataset data_VI_Avg_D;
  Gnuplot2dDataset data_BE_Avg_D;
  Gnuplot2dDataset data_BK_Avg_D; 

  Gnuplot2dDataset data_VO_CWmin; 
  Gnuplot2dDataset data_VI_CWmin; 
  Gnuplot2dDataset data_BE_CWmin; 
  Gnuplot2dDataset data_BK_CWmin; 
 
  Gnuplot2dDataset data_VO_Coll; 
  Gnuplot2dDataset data_VI_Coll; 
  Gnuplot2dDataset data_BE_Coll; 
  Gnuplot2dDataset data_BK_Coll; 

  void PhyStateRxError(std::string context, Ptr<const Packet> packet, double snr);
  void PhyStateRxOk (std::string context, Ptr<const Packet> packet, double snr, WifiMode mode, enum WifiPreamble preamble);
  void PhyStateTx (std::string context, Ptr<const Packet> packet, WifiMode mode, WifiPreamble preamble, uint8_t txPower);
  
  void MacRxOkTrace (std::string context, Ptr<const Packet> packet);
  void MacTxOkTrace (std::string context, Ptr<const Packet> packet);
  
  void PhyTxBegin (std::string context, Ptr<const Packet> packet);
  void PhyRxEnd (std::string context, Ptr<const Packet> packet);
  void PhyRxDrop (std::string context, Ptr<const Packet> packet);

  void VODelay (std::string context, Ptr<const Packet> packet, const Address &adr);
  void VIDelay (std::string context, Ptr<const Packet> packet, const Address &adr);
  void BEDelay (std::string context, Ptr<const Packet> packet, const Address &adr);
  void BKDelay (std::string context, Ptr<const Packet> packet, const Address &adr);

  void CalcLimits();
  void IncCW(uint8_t AC);
  void DecCW(uint8_t AC);
private:
  double VO_Sn = 0; //Current throughputs
  double VI_Sn = 0;
  double BE_Sn = 0;
  double BK_Sn = 0;
  double VO_Sn0 = 0; //past  throughputs
  double VI_Sn0 = 0;
  double BE_Sn0 = 0;
  double BK_Sn0 = 0;
  double VO_S_lim = 0.6;  //lower throughput limits 
  double VI_S_lim = 40;
  double BE_S_lim = 50;
  double BK_S_lim = 20;

  size_t m_phystateRxOk=0;  //tracing packets
  size_t m_phystateRxErr=0;
  size_t m_phystateTx=0;
  size_t m_macRxOk=0;
  size_t m_macTxOk=0;
  size_t m_phyTxBegin=0;
  size_t m_phyRxEnd=0;
  size_t m_phyRxDrop=0;
  
  uint16_t VO_CW = 3;
  uint16_t VI_CW = 7;
  uint16_t BE_CW = 15;
  uint16_t BK_CW = 15;
  uint32_t nWifi = 20;
  double simulationTime =100; //seconds
  double distance = 1.0; //meters
  uint16_t mcs = 23;
  uint8_t channelWidth = 10; //MHz
  bool useShortGuardInterval = true;
  bool useRts = false;
  NodeContainer wifiStaNodes;
  NodeContainer wifiApNode;
  Ipv4InterfaceContainer staNodeInterfaces;
  Ipv4InterfaceContainer apNodeInterface;
  ApplicationContainer sinkApplications;
  ApplicationContainer sourceApplications;
  ApplicationContainer sourceApplications2;
  
  double m_VO_packets=0;
  double m_VI_packets=0;
  double m_BE_packets=0;
  double m_BK_packets=0;

  uint32_t m_VO_sinkPackets=0;
  uint32_t m_VI_sinkPackets=0;
  uint32_t m_BE_sinkPackets=0;
  uint32_t m_BK_sinkPackets=0;

  double m_VO_sinkDelay=0;
  double m_VI_sinkDelay=0;
  double m_BE_sinkDelay=0;
  double m_BK_sinkDelay=0;

  uint32_t m_VO_sinkPacketsN0=0;
  uint32_t m_VI_sinkPacketsN0=0;
  uint32_t m_BE_sinkPacketsN0=0;
  uint32_t m_BK_sinkPacketsN0=0;

  double m_VO_sinkDelayN0=0;
  double m_VI_sinkDelayN0=0;
  double m_BE_sinkDelayN0=0;
  double m_BK_sinkDelayN0=0;

  double m_VO_Davg = 0;
  double m_VI_Davg = 0;
  double m_BE_Davg = 0;
  double m_BK_Davg = 0;

  uint32_t m_VO_PhyColl=0; //incremented by trace source
  uint32_t m_VI_PhyColl=0; //current collision count
  uint32_t m_BE_PhyColl=0;
  uint32_t m_BK_PhyColl=0;
  uint32_t m_VO_PhyRx=0;  //rx count incremented by trace
  uint32_t m_VI_PhyRx=0;
  uint32_t m_BE_PhyRx=0;  
  uint32_t m_BK_PhyRx=0;
  uint32_t m_VO_PhyColln0=0; //collision count n-1
  uint32_t m_VI_PhyColln0=0;
  uint32_t m_BE_PhyColln0=0;
  uint32_t m_BK_PhyColln0=0;
  uint32_t m_VO_PhyRxn0=0; //rx count n-1
  uint32_t m_VI_PhyRxn0=0;
  uint32_t m_BE_PhyRxn0=0;
  uint32_t m_BK_PhyRxn0=0;
  double m_VO_favg=0;  //collision rate
  double m_VI_favg=0;
  double m_BE_favg=0;
  double m_BK_favg=0;

  uint32_t datarate_VO = 64000; //64kbps
  uint32_t datarate_VI = 2500000000; 
  uint64_t datarate_BE = 1000000000;
  uint32_t datarate_BK = 1000000000;

  uint16_t delay_VO = 20; //20ms
  uint16_t delay_VI = 20; 
  uint16_t delay_BE = 1000; //1 second
  uint16_t delay_BK = 5000; //5 second

  uint32_t S_VO_limits [20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
  uint32_t S_VI_limits [20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  uint64_t S_BE_limits [20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  uint32_t S_BK_limits [20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  /*uint16_t D_VO_limits [20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
  uint16_t D_VI_limits [20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
  uint16_t D_BE_limits [20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
  uint16_t D_BK_limits [20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; */

  uint8_t timeout_VO = 0;
  uint8_t timeout_VI = 0;
  uint8_t timeout_BE = 0; 
  uint8_t timeout_BK = 0;		
};

QoS::QoS()
  : data_VO_S ("1VO S"), data_VI_S ("1VI S"), data_BE_S ("1BE S"), data_BK_S ("1BK S"),
    data_VO_Avg_S ("1VO Savg"), data_VI_Avg_S ("1VI Savg"),data_BE_Avg_S ("1BE Savg"), data_BK_Avg_S ("1BK Savg"),
    data_VO_D ("2VO D"),data_VI_D ("2VI D"),data_BE_D ("2BE D"), data_BK_D ("2BK D"),
    data_VO_Avg_D ("2VO Davg"),data_VI_Avg_D ("2VI Davg"),data_BE_Avg_D ("2BE Davg"), data_BK_Avg_D ("2BK Davg"), 
    data_VO_CWmin("3VO CWmin"),data_VI_CWmin("3VI CWmin"),data_BE_CWmin("3BE CWmin"),data_BK_CWmin("3BK CWmin"),
    data_VO_Coll("4VO Coll"), data_VI_Coll("4VI Coll"), data_BE_Coll("4BE Coll"), data_BK_Coll("4BK Coll")
{
  data_VO_S.SetStyle (Gnuplot2dDataset::LINES);
  data_VI_S.SetStyle (Gnuplot2dDataset::LINES);
  data_BE_S.SetStyle (Gnuplot2dDataset::LINES);
  data_BK_S.SetStyle (Gnuplot2dDataset::LINES);
  data_VO_Avg_S.SetStyle (Gnuplot2dDataset::LINES);
  data_VI_Avg_S.SetStyle (Gnuplot2dDataset::LINES);
  data_BE_Avg_S.SetStyle (Gnuplot2dDataset::LINES);
  data_BK_Avg_S.SetStyle (Gnuplot2dDataset::LINES);

  data_VO_D.SetStyle (Gnuplot2dDataset::LINES);
  data_VI_D.SetStyle (Gnuplot2dDataset::LINES);
  data_BE_D.SetStyle (Gnuplot2dDataset::LINES);
  data_BK_D.SetStyle (Gnuplot2dDataset::LINES);
  data_VO_Avg_D.SetStyle (Gnuplot2dDataset::LINES);
  data_VI_Avg_D.SetStyle (Gnuplot2dDataset::LINES);
  data_BE_Avg_D.SetStyle (Gnuplot2dDataset::LINES);
  data_BK_Avg_D.SetStyle (Gnuplot2dDataset::LINES);

  data_VO_CWmin.SetStyle (Gnuplot2dDataset::LINES);
  data_VI_CWmin.SetStyle (Gnuplot2dDataset::LINES);
  data_BE_CWmin.SetStyle (Gnuplot2dDataset::LINES);
  data_BK_CWmin.SetStyle (Gnuplot2dDataset::LINES);

  data_VO_Coll.SetStyle (Gnuplot2dDataset::LINES);
  data_VI_Coll.SetStyle (Gnuplot2dDataset::LINES);
  data_BE_Coll.SetStyle (Gnuplot2dDataset::LINES);
  data_BK_Coll.SetStyle (Gnuplot2dDataset::LINES);
}

void
QoS::Initialize()
{
  wifiApNode.Create (1);
  wifiStaNodes.Create (nWifi);
  
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  //phy.Set ("ShortGuardEnabled", BooleanValue (useShortGuardInterval));

  WifiMacHelper mac;
  WifiHelper wifi;
  //wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
  wifi.SetStandard (WIFI_PHY_STANDARD_80211ow);

  std::ostringstream oss;
  oss << "HtMcs" << mcs;
  //wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue (oss.str ()), "ControlMode", StringValue (oss.str ()));
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OwcRate10Gbps"), "ControlMode", StringValue ("OwcRate10Gbps"));

  Ssid ssid = Ssid ("ns3-80211ow");

  mac.SetType ("ns3::StaWifiMac","Ssid", SsidValue (ssid),"QosSupported", BooleanValue (true));
  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid),"QosSupported", BooleanValue (true));
  NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);

  // mobility
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  for (uint32_t i = 0; i < nWifi; i++)
  {
    positionAlloc->Add (Vector (distance, 0.0, 0.0));
  }
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);
  mobility.Install (wifiStaNodes);

  // Internet stack
  InternetStackHelper stack;
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);
  Ipv4AddressHelper address;

  address.SetBase ("192.168.1.0", "255.255.255.0");
  
  staNodeInterfaces = address.Assign (staDevices);
  apNodeInterface = address.Assign (apDevice);
}

void
QoS::InstallApps()
{
  uint32_t portVO = 9;
  uint32_t portVI = 10;
  uint32_t portBE = 11;
  uint32_t portBK = 12;

  InetSocketAddress sinkSocketVO (apNodeInterface.GetAddress (0), portVO);
  sinkSocketVO.SetTos (0xc0);
  InetSocketAddress sinkSocketVI (apNodeInterface.GetAddress (0), portVI);
  sinkSocketVI.SetTos (0xb8);
  InetSocketAddress sinkSocketBE (apNodeInterface.GetAddress (0), portBE);
  sinkSocketBE.SetTos (0x70);
  InetSocketAddress sinkSocketBK (apNodeInterface.GetAddress (0), portBK);
  sinkSocketBK.SetTos (0x28);

  uint8_t VO = 0;
  uint8_t VI = 10;
  uint8_t BE = 10;
  uint8_t BK = 15;
  
  uint8_t VO_per = 0; 
  uint8_t VI_per = 10;
  uint8_t BE_per = 10;
  uint8_t BK_per = 15;

  for (uint8_t index = 0; index < nWifi; ++index)
  {
    if(VO <= index && VO_per > index)
    { 	
      OnOffHelper onOffHelperVO ("ns3::UdpSocketFactory", sinkSocketVO);
      //onOffHelperVO.SetAttribute ("OnTime", StringValue ("ns3::ExponentialRandomVariable[Mean=1.0]"));
      onOffHelperVO.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.01]"));
      onOffHelperVO.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.01]"));
      //onOffHelperVO.SetAttribute ("OffTime", StringValue ("ns3::ExponentialRandomVariable[Mean=0.1]"));
      onOffHelperVO.SetAttribute ("DataRate", DataRateValue (datarate_VO)); //64kbps voice
      onOffHelperVO.SetAttribute ("PacketSize", UintegerValue (80)); //80 bytes packet
      sourceApplications.Add (onOffHelperVO.Install (wifiStaNodes.Get (index)));
      std::cout << "VO " << std::endl;
      S_VO_limits[index] = datarate_VO/2;
    }
    if(VI<= index && VI_per > index)
    { 	
      OnOffHelper onOffHelperVI ("ns3::UdpSocketFactory", sinkSocketVI);
      onOffHelperVI.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.005]"));
      onOffHelperVI.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.005]"));
      onOffHelperVI.SetAttribute ("DataRate", DataRateValue (datarate_VI));
      onOffHelperVI.SetAttribute ("PacketSize", UintegerValue (1500)); //bytes
      sourceApplications.Add (onOffHelperVI.Install (wifiStaNodes.Get (index)));
      std::cout << "VI " << std::endl;
      S_VI_limits[index] = datarate_VI/2;
    }
    if(BE <= index && BE_per > index)
    { 	
      OnOffHelper onOffHelperBE ("ns3::UdpSocketFactory", sinkSocketBE);
      onOffHelperBE.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.01]"));
      onOffHelperBE.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.01]"));
      onOffHelperBE.SetAttribute ("DataRate", DataRateValue (datarate_BE));
      onOffHelperBE.SetAttribute ("PacketSize", UintegerValue (1500)); //bytes
      sourceApplications.Add (onOffHelperBE.Install (wifiStaNodes.Get (index)));
      std::cout << "BE " << std::endl;
      S_BE_limits[index] = datarate_BE/2;
    }
    if(BK <= index && BK_per > index)
    { 	
      OnOffHelper onOffHelperBK ("ns3::UdpSocketFactory", sinkSocketBK);
      onOffHelperBK.SetAttribute ("OnTime", StringValue ("ns3::ExponentialRandomVariable[Mean=1.0]"));
      onOffHelperBK.SetAttribute ("OffTime", StringValue ("ns3::ExponentialRandomVariable[Mean=1.0]"));
      onOffHelperBK.SetAttribute ("DataRate", DataRateValue (datarate_BK));
      onOffHelperBK.SetAttribute ("PacketSize", UintegerValue (1472)); //bytes
      sourceApplications.Add (onOffHelperBK.Install (wifiStaNodes.Get (index)));
      std::cout << "BK " << std::endl;
      S_BK_limits[index] = datarate_BK/2;
    }
  }
  PacketSinkHelper packetSinkHelperVO ("ns3::UdpSocketFactory", sinkSocketVO);
  sinkApplications.Add (packetSinkHelperVO.Install (wifiApNode.Get (0)));
  
  PacketSinkHelper packetSinkHelperVI ("ns3::UdpSocketFactory", sinkSocketVI);
  sinkApplications.Add (packetSinkHelperVI.Install (wifiApNode.Get (0)));
 
  PacketSinkHelper packetSinkHelperBE ("ns3::UdpSocketFactory", sinkSocketBE);
  sinkApplications.Add (packetSinkHelperBE.Install (wifiApNode.Get (0)));

  PacketSinkHelper packetSinkHelperBK ("ns3::UdpSocketFactory", sinkSocketBK);
  sinkApplications.Add (packetSinkHelperBK.Install (wifiApNode.Get (0)));   

  //sourceApplications.Stop(Seconds(40));
}

void
QoS::AddApp(uint8_t AC, uint8_t node, Time stopTime)
{
  uint32_t portVO = 9;
  uint32_t portVI = 10;
  uint32_t portBE = 11;
  uint32_t portBK = 12;

  InetSocketAddress sinkSocketVO (apNodeInterface.GetAddress (0), portVO);
  sinkSocketVO.SetTos (0xc0);
  InetSocketAddress sinkSocketVI (apNodeInterface.GetAddress (0), portVI);
  sinkSocketVI.SetTos (0xb8);
  InetSocketAddress sinkSocketBE (apNodeInterface.GetAddress (0), portBE);
  sinkSocketBE.SetTos (0x70);
  InetSocketAddress sinkSocketBK (apNodeInterface.GetAddress (0), portBK);
  sinkSocketBK.SetTos (0x28);

  if(AC==0)
  {
    OnOffHelper onOffHelperVO ("ns3::UdpSocketFactory", sinkSocketVO);
    onOffHelperVO.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.01]"));
    onOffHelperVO.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.01]"));
    onOffHelperVO.SetAttribute ("DataRate", DataRateValue (datarate_VO)); //64kbps voice
    onOffHelperVO.SetAttribute ("PacketSize", UintegerValue (80)); //80 bytes packet
    sourceApplications2.Add (onOffHelperVO.Install (wifiStaNodes.Get (node)));
    std::cout << "VO " << std::endl;
    S_VO_limits[node] = datarate_VO/2;
  }
  if(AC==1)
  {
    OnOffHelper onOffHelperVI ("ns3::UdpSocketFactory", sinkSocketVI);
    onOffHelperVI.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.005]"));
    onOffHelperVI.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.005]"));
    onOffHelperVI.SetAttribute ("DataRate", DataRateValue (datarate_VI)); //64kbps voice
    onOffHelperVI.SetAttribute ("PacketSize", UintegerValue (1500)); //80 bytes packe
    sourceApplications2.Add (onOffHelperVI.Install (wifiStaNodes.Get (node)));
    std::cout << "VI " << std::endl;
    S_VI_limits[node] = datarate_VI/2;
  }
  if(AC==2)
  {
    OnOffHelper onOffHelperBE ("ns3::UdpSocketFactory", sinkSocketBE);
    onOffHelperBE.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.01]"));
    onOffHelperBE.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.01]"));
    onOffHelperBE.SetAttribute ("DataRate", DataRateValue (datarate_BE));
    onOffHelperBE.SetAttribute ("PacketSize", UintegerValue (1500)); //bytes
    sourceApplications2.Add (onOffHelperBE.Install (wifiStaNodes.Get (node)));
    std::cout << "BE " << std::endl;
    S_BE_limits[node] = datarate_BE/2;
  }
  if(AC==3)
  {
    OnOffHelper onOffHelperBK ("ns3::UdpSocketFactory", sinkSocketBK);
    onOffHelperBK.SetAttribute ("OnTime", StringValue ("ns3::ExponentialRandomVariable[Mean=1.0]"));
    onOffHelperBK.SetAttribute ("OffTime", StringValue ("ns3::ExponentialRandomVariable[Mean=1.0]"));
    onOffHelperBK.SetAttribute ("DataRate", DataRateValue (datarate_BK));
    onOffHelperBK.SetAttribute ("PacketSize", UintegerValue (1472)); //bytes
    sourceApplications2.Add (onOffHelperBK.Install (wifiStaNodes.Get (node)));
    std::cout << "BK " << std::endl;
    S_BK_limits[node] = datarate_BK/2;
  }
  sourceApplications2.Stop(stopTime);
  
}


void
QoS::InstantThroughput(double time)
{
  std::cout << "Time: " << time << std::endl;
  CalcLimits();
  double VO_S_Inst=0;
  double VI_S_Inst=0;
  double BE_S_Inst=0;
  double BK_S_Inst=0;
  double a = 0.5;
  for (unsigned index = 0; index < sinkApplications.GetN (); ++index)
  {
      uint64_t totalPacketsThrough = DynamicCast<PacketSink> (sinkApplications.Get (index))->GetTotalRx ();//total bytes NOT PACKETS
      std::cout<< "packets: " << totalPacketsThrough/1500 <<std::endl;
      if(index==0)
      {
        VO_S_Inst = (totalPacketsThrough-m_VO_packets)*8/(1*1000000.0); //# of packets in last 1 second
	VO_Sn0 = (1-a)*VO_S_Inst + a*VO_Sn0;
        m_VO_packets = totalPacketsThrough;
        data_VO_Avg_S.Add(time, VO_Sn0);  //average S
      }
      if(index==1)
      {
        VI_S_Inst = (totalPacketsThrough-m_VI_packets)*8/(1*1000000.0); //# of packets in last second
	VI_Sn0 = (1-a)*VI_S_Inst + a*VI_Sn0;
        m_VI_packets = totalPacketsThrough;
        data_VI_Avg_S.Add(time, VI_Sn0);  //average S
	data_VI_S.Add(time, VI_S_Inst);
      }
      if(index==2)
      {
        BE_S_Inst = (totalPacketsThrough-m_BE_packets)*8/(1*1000000.0); //# of packets in last second
	BE_Sn0 = (1-a)*BE_S_Inst + a*BE_Sn0;
        m_BE_packets = totalPacketsThrough;
        data_BE_Avg_S.Add(time, BE_Sn0);  //average S
	data_BE_S.Add(time, BE_S_Inst); 
//std::cout << "Throughput be: " << BE_S_Inst << " | " << BE_Sn0 << " | " << m_BE_packets  << " | " << "" << std::endl;
      }
      if(index==3)
      {
        BK_S_Inst = (totalPacketsThrough-m_BK_packets)*8/(1*1000000.0); //# of packets in last second
	BK_Sn0 = (1-a)*BK_S_Inst + a*BK_Sn0;
        m_BK_packets = totalPacketsThrough;
        data_BK_Avg_S.Add(time, BK_Sn0);  //average S
	data_BK_S.Add(time, BK_S_Inst); 
      }
      
  }
  
  std::cout << "Throughput: " << VO_Sn0 << " | " << VI_Sn0 << " | " << BE_Sn0  << " | " << BK_Sn0 << std::endl;

  uint32_t m_VO_Coll_Inst = m_VO_PhyColl - m_VO_PhyColln0;
  uint32_t m_VI_Coll_Inst = m_VI_PhyColl - m_VI_PhyColln0;
  uint32_t m_BE_Coll_Inst = m_BE_PhyColl - m_BE_PhyColln0;
  uint32_t m_BK_Coll_Inst = m_BK_PhyColl - m_BK_PhyColln0;

  uint32_t m_VO_Rx_Inst = m_VO_PhyRx - m_VO_PhyRxn0;
  uint32_t m_VI_Rx_Inst = m_VI_PhyRx - m_VI_PhyRxn0;
  uint32_t m_BE_Rx_Inst = m_BE_PhyRx - m_BE_PhyRxn0;
  uint32_t m_BK_Rx_Inst = m_BK_PhyRx - m_BK_PhyRxn0;

  uint32_t m_VO_sinkPackets_Inst = m_VO_sinkPackets - m_VO_sinkPacketsN0;
  uint32_t m_VI_sinkPackets_Inst = m_VI_sinkPackets - m_VI_sinkPacketsN0;
  uint32_t m_BE_sinkPackets_Inst = m_BE_sinkPackets - m_BE_sinkPacketsN0;
  uint32_t m_BK_sinkPackets_Inst = m_BK_sinkPackets - m_BK_sinkPacketsN0;

  uint32_t m_VO_sinkDelay_Inst = m_VO_sinkDelay - m_VO_sinkDelayN0; 
  uint32_t m_VI_sinkDelay_Inst = m_VI_sinkDelay - m_VI_sinkDelayN0; 
  uint32_t m_BE_sinkDelay_Inst = m_BE_sinkDelay - m_BE_sinkDelayN0; 
  uint32_t m_BK_sinkDelay_Inst = m_BK_sinkDelay - m_BK_sinkDelayN0;  
  //VO Delay
  if(m_VO_sinkPackets_Inst<=0){
    m_VO_Davg = a*m_VO_Davg;
  }
  else{
    m_VO_Davg = (1-a)*(float(m_VO_sinkDelay_Inst)/(m_VO_sinkPackets_Inst)) + a*m_VO_Davg;
  }
  //VI Delay
  if(m_VI_sinkPackets_Inst<=0){
    m_VI_Davg = a*m_VI_Davg;
  }
  else{
    m_VI_Davg = (1-a)*(float(m_VI_sinkDelay_Inst)/(m_VI_sinkPackets_Inst)) + a*m_VI_Davg;
  }
  //BE Delay
  if(m_BE_sinkPackets_Inst<=0){
    m_BE_Davg = a*m_BE_Davg;
  }
  else{
    m_BE_Davg = (1-a)*(float(m_BE_sinkDelay_Inst)/(m_BE_sinkPackets_Inst)) + a*m_BE_Davg;
  }
  //BK Delay
  if(m_BK_sinkPackets_Inst<=0){
    m_BK_Davg = a*m_BK_Davg;
  }
  else{
    m_BK_Davg = (1-a)*(float(m_BK_sinkDelay_Inst)/(m_BK_sinkDelay_Inst)) + a*m_BK_Davg;
  }




  if(m_VO_Rx_Inst<=0){
    m_VO_favg = a*m_VO_favg;
  }
  else{
    m_VO_favg = (1-a)*(float(m_VO_Coll_Inst)/m_VO_Rx_Inst) + a*m_VO_favg;
  }

  if(m_VI_Rx_Inst<=0){
    m_VI_favg = a*m_VI_favg;
  }
  else{
    m_VI_favg = (1-a)*(float(m_VI_Coll_Inst)/m_VI_Rx_Inst) + a*m_VI_favg;
  }

  if(m_BE_Rx_Inst<=0){
    m_BE_favg = a*m_BE_favg;
  }
  else{
    m_BE_favg = (1-a)*(float(m_BE_Coll_Inst)/m_BE_Rx_Inst) + a*m_BE_favg;
  }

  if(m_BK_Rx_Inst<=0){
    m_BK_favg = a*m_BK_favg;
  }
  else{
    m_BK_favg = (1-a)*(float(m_BK_Coll_Inst)/m_BK_Rx_Inst) + a*m_BK_favg;
  }

  m_VO_PhyColln0 = m_VO_PhyColl;
  m_VI_PhyColln0 = m_VI_PhyColl;
  m_BE_PhyColln0 = m_BE_PhyColl;
  m_BK_PhyColln0 = m_BK_PhyColl;
  m_VO_PhyRxn0 = m_VO_PhyRx;
  m_VI_PhyRxn0 = m_VI_PhyRx;
  m_BE_PhyRxn0 = m_BE_PhyRx;
  m_BK_PhyRxn0 = m_BK_PhyRx;
  
  m_VO_sinkPacketsN0 = m_VO_sinkPackets;
  m_VI_sinkPacketsN0 = m_VI_sinkPackets;
  m_BE_sinkPacketsN0 = m_BE_sinkPackets;
  m_BK_sinkPacketsN0 = m_BK_sinkPackets;  

  m_VO_sinkDelayN0 = m_VO_sinkDelay;
  m_VI_sinkDelayN0 = m_VI_sinkDelay;
  m_BE_sinkDelayN0 = m_BE_sinkDelay;
  m_BK_sinkDelayN0 = m_BK_sinkDelay;  

  
  std::cout << "Coll:  " << m_VO_favg << " | " << m_VI_favg << " | " << m_BE_favg << " | " << m_BK_favg <<std::endl;
  std::cout << "Delay: " << m_VO_Davg << " | " << m_VI_Davg << " | " << m_BE_Davg << " | " << m_BK_Davg <<std::endl;
  
  data_VO_Coll.Add(time, m_VO_favg);
  data_VI_Coll.Add(time, m_VI_favg);
  data_BE_Coll.Add(time, m_BE_favg);
  data_BK_Coll.Add(time, m_BK_favg);
  data_VO_Avg_D.Add(time, m_VO_Davg);
  data_VI_Avg_D.Add(time, m_VI_Davg);
  data_BE_Avg_D.Add(time, m_BE_Davg);
  data_BK_Avg_D.Add(time, m_BK_Davg);

  data_VO_CWmin.Add(time, VO_CW);
  data_VI_CWmin.Add(time, VI_CW);
  data_BE_CWmin.Add(time, BE_CW);
  data_BK_CWmin.Add(time, BK_CW);
  std::cout<<std::endl;
}
void 
QoS::IncCW(uint8_t AC)
{
  //uint16_t CWmin [4] = {3, 7, 15, 15}; 
  uint16_t CWmax [4] = {15, 63, 1023, 2047}; 
  if(AC==0){
    VO_CW = (VO_CW+1)*2 - 1;
    if(VO_CW<CWmax[0]){
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VO_EdcaTxopN/MinCw",  UintegerValue (VO_CW));
      timeout_VO = 5;
      std::cout << "VO CW: " << VO_CW << std::endl;
    }
    else{
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VO_EdcaTxopN/MinCw",  UintegerValue (CWmax[0]));
      VO_CW = CWmax[0];
      timeout_VO = 5;
      std::cout << "VO CW: " << VO_CW << std::endl;
    }
  }
  if(AC==1)
  {
    VI_CW = (VI_CW+1)*2 - 1;
    if(VI_CW<CWmax[1]){
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VI_EdcaTxopN/MinCw",  UintegerValue (VI_CW));
      timeout_VI = 5;
      std::cout << "VI CW: " << VI_CW << std::endl;
    }
    else{
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VI_EdcaTxopN/MinCw",  UintegerValue (CWmax[1]));
      VI_CW = CWmax[1];
      timeout_VI = 5;
      std::cout << "VI CW: " << VI_CW << std::endl;
    }
  }
  if(AC==2)
  {
    BE_CW = (BE_CW+1)*2 - 1;
    if(BE_CW<CWmax[2]){
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BE_EdcaTxopN/MinCw",  UintegerValue (BE_CW));
      timeout_BE = 5;
      std::cout << "BE CW: " << BE_CW << std::endl;
    }
    else{
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BE_EdcaTxopN/MinCw",  UintegerValue (CWmax[2]));
      BE_CW = CWmax[2];
      timeout_BE = 5;
      std::cout << "BE CW: " << BE_CW << std::endl;
    }
  }
  if(AC==3)
  {
    BK_CW = (BK_CW+1)*2 - 1;
    if(BK_CW<CWmax[3]){
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BK_EdcaTxopN/MinCw",  UintegerValue (BK_CW));
      timeout_BK = 5;
      std::cout << "BK CW: " << BK_CW << std::endl;
    }
    else{
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BK_EdcaTxopN/MinCw",  UintegerValue (CWmax[3]));
      BK_CW = CWmax[3];
      timeout_BK = 5;
      std::cout << "BK CW: " << BK_CW << std::endl;
    }
  }
}
void 
QoS::DecCW(uint8_t AC)
{
  uint16_t CWmin [4] = {3, 7, 15, 15}; 
  //uint16_t CWmax [4] = {15, 63, 1023, 2047}; 
  if(AC==0)
  {
    VO_CW = (VO_CW+1)/2 - 1;
    if(VO_CW>CWmin[0]){
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VO_EdcaTxopN/MinCw",  UintegerValue (VO_CW));
      timeout_VO = 5;
      std::cout << "VO CW: " << VO_CW << std::endl;
    }
    else{
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VO_EdcaTxopN/MinCw",  UintegerValue (CWmin[0]));
      VO_CW = CWmin[0];
      timeout_VO = 5;
      std::cout << "VO CW: " << VO_CW << std::endl;
    }
  }
  if(AC==1)
  {
    VI_CW = (VI_CW+1)/2 - 1;
    if(VI_CW>CWmin[1]){
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VI_EdcaTxopN/MinCw",  UintegerValue (VI_CW));
      timeout_VI = 5;
      std::cout << "VI CW: " << VI_CW << std::endl;
    }
    else{
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VI_EdcaTxopN/MinCw",  UintegerValue (CWmin[1]));
      VI_CW = CWmin[1];
      timeout_VI = 5;
      std::cout << "VI CW: " << VI_CW << std::endl;
    }
  } 
  if(AC==2)
  {
    BE_CW = (BE_CW+1)/2 - 1;
    if(BE_CW>CWmin[2]){
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BE_EdcaTxopN/MinCw",  UintegerValue (BE_CW));
      timeout_BE = 5;
      std::cout << "BE CW: " << BE_CW << std::endl;
    }
    else{
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BE_EdcaTxopN/MinCw",  UintegerValue (CWmin[2]));
      BE_CW = CWmin[2];
      timeout_BE = 5;
      std::cout << "BE CW: " << BE_CW << std::endl;
    }
  }
  if(AC==3)
  {
    BK_CW = (BK_CW+1)/2 - 1;
    if(BK_CW>CWmin[3]){
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BK_EdcaTxopN/MinCw",  UintegerValue (BK_CW));
      timeout_BK = 5;
      std::cout << "BK CW: " << BK_CW << std::endl;
    }
    else{
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BK_EdcaTxopN/MinCw",  UintegerValue (CWmin[3]));
      BK_CW = CWmin[3];
      timeout_BK = 5;
      std::cout << "BK CW: " << BK_CW << std::endl;
    }
  }
}
void
QoS::UpdateCW()
{
  //uint16_t CWmin [4] = {3, 7, 15, 15}; 
  //uint16_t CWmax [4] = {15, 63, 1023, 2047}; 
  
  if(VO_Sn0<VO_S_lim*0.8 && timeout_VO<=0)
  {
    IncCW(0);  //increase because fell below 80% of the throughput
  }
  else if(VO_Sn0>=VO_S_lim*0.9 && timeout_VO<=0)
  {
    if(m_VO_favg<0.05){
      DecCW(0);  //decrease because throughput is above 90% and collisions are low
    }
    else if(m_VO_favg>0.2){
      if(m_VO_Davg>150){
        IncCW(0);  //increase because collisions and delay both are high 
      }
    }
  }
  


  if(VI_Sn0<VI_S_lim*0.8 && timeout_VI<=0)
  {
    //if(m_VI_favg>0.3){
      IncCW(1);  //increase because fell below 80% of the throughput
	//}  //increase because fell below 80% of the throughput
  }
  else if(VI_Sn0>=VI_S_lim*0.9 && timeout_VI<=0)
  {
    if(m_VI_favg<0.05){
      DecCW(1);  //decrease because throughput is above 90% and collisions are low
    }
    else if(m_VI_favg>0.2){
      if(m_VI_Davg>150){
        IncCW(1);  //increase because collisions and delay both are high 
      }
    }
  }


  if(BE_Sn0<BE_S_lim*0.8 && timeout_BE<=0)
  {
    //if(m_BE_favg>0.3){
      IncCW(2);
	//}
  }
  else if(BE_Sn0>=BE_S_lim*0.9 && timeout_BE<=0)
  {
    if(m_BE_favg<0.1){
      DecCW(2);
    }
    else if(m_BE_favg>0.9){
      if(m_BE_Davg>500){
        IncCW(2);
      }
    }
  }

  if(BK_Sn0<BK_S_lim && timeout_BK<=0)
  {
    IncCW(3);
  }
  
  if(timeout_VO>0){ timeout_VO--; }
  if(timeout_VI>0){ timeout_VI--; }
  if(timeout_BE>0){ timeout_BE--; }
  if(timeout_BK>0){ timeout_BK--; }
}

void
QoS::PhyStateRxError(std::string context, Ptr<const Packet> packet, double snr)
{
  Ptr<Packet> m_currentPacket;
  WifiMacHeader hdr;
  m_currentPacket = packet->Copy();
  m_currentPacket->RemoveHeader (hdr);
  if(hdr.IsData())
  {
    m_phystateRxErr++;
  }
}
void
QoS::PhyStateRxOk (std::string context, Ptr<const Packet> packet, double snr, WifiMode mode, enum WifiPreamble preamble)
{
  Ptr<Packet> m_currentPacket;
  WifiMacHeader hdr;
  m_currentPacket = packet->Copy();
  m_currentPacket->RemoveHeader (hdr);  
  if(hdr.IsData()){    
    m_phystateRxOk++;
  }
}
void
QoS::PhyStateTx (std::string context, Ptr<const Packet> packet, WifiMode mode, WifiPreamble preamble, uint8_t txPower)
{
  Ptr<Packet> m_currentPacket;
  WifiMacHeader hdr;
  m_currentPacket = packet->Copy();
  m_currentPacket->RemoveHeader (hdr);
  if(hdr.IsData()){
    m_phystateTx++;
  }
}
void
QoS::MacRxOkTrace (std::string context, Ptr<const Packet> packet)
{
  Ptr<Packet> m_currentPacket;
  WifiMacHeader hdr;
  
  m_currentPacket = packet->Copy();
  m_currentPacket->RemoveHeader (hdr);  
  if(hdr.IsData()){    
    m_macRxOk++;
  }
}
void
QoS::MacTxOkTrace (std::string context, Ptr<const Packet> packet)
{
  Ptr<Packet> m_currentPacket;
  WifiMacHeader hdr;
  m_currentPacket = packet->Copy();
  m_currentPacket->RemoveHeader (hdr);  
  if(hdr.IsData()){    
    m_macTxOk++;
    //std::cout << "GetAddr2 :" << hdr.GetQosControl() << " " << std::endl;
   /* TimestampTag timestamp;
  if (m_currentPacket->FindFirstMatchingByteTag (timestamp)){
    Time tx = timestamp.GetTimestamp ();
    std::cout << "RX timestamp: " << tx << " " << std::endl;
    std::cout << "RX sim now: " << Simulator::Now () << " " << std::endl;
  }*/
  }
}
void
QoS::PhyTxBegin (std::string context, Ptr<const Packet> packet)
{
  Ptr<Packet> m_currentPacket;
  WifiMacHeader hdr;
  m_currentPacket = packet->Copy();
  m_currentPacket->RemoveHeader (hdr);
  if(hdr.IsData()){
    m_phyTxBegin++;
    /*std::cout << "PHY TX :" << m_phyTxBegin << " " << std::endl;
    std::cout << "GetAddr1 :" << hdr.GetAddr1() << " " << std::endl;
    std::cout << "GetAddr2 :" << hdr.GetAddr2() << " " << std::endl;
    std::cout << "type :" << hdr.GetTypeString() << std::endl;
    std::cout << "Time: " << ns3::Simulator::Now()<< std::endl << std::endl;*/
    //std::cout << "ack :" << hdr.IsAck() << std::endl;
    //std::cout << "rts :" << hdr.IsRts() << std::endl;
  }
} 
void
QoS::PhyRxEnd (std::string context, Ptr<const Packet> packet)
{
  Ptr<Packet> m_currentPacket;
  WifiMacHeader hdr;
  m_currentPacket = packet->Copy();
  m_currentPacket->RemoveHeader (hdr);
      //std::cout << "type :" << hdr.GetTypeString() << std::endl;
  if(hdr.IsData()){
    m_phyRxEnd++;
    uint8_t tid = hdr.GetQosTid();
    switch(unsigned(tid)){ 
      case 7: m_VO_PhyRx++;
		break;
      case 6: m_VO_PhyRx++;
		break;
      case 5: m_VI_PhyRx++;
		break;
      case 4: m_VI_PhyRx++;
		break;
      case 3: m_BE_PhyRx++;
		break;
      case 0: m_BE_PhyRx++;
		break;
      case 2: m_BK_PhyRx++;
		break;
      case 1: m_BK_PhyRx++;
		break;
      default: break;
    }
  }
}
void
QoS::PhyRxDrop (std::string context, Ptr<const Packet> packet)
{
  Ptr<Packet> m_currentPacket;
  WifiMacHeader hdr;
  m_currentPacket = packet->Copy();
  m_currentPacket->RemoveHeader (hdr);
  
  if(hdr.IsData()){
    m_phyRxDrop++;
    uint8_t tid = hdr.GetQosTid();
    switch(unsigned(tid)){ 
      case 7: m_VO_PhyColl++;
		break;
      case 6: m_VO_PhyColl++;	
		break;
      case 5: m_VI_PhyColl++;
		break;
      case 4: m_VI_PhyColl++;
		break;
      case 3: m_BE_PhyColl++;
		break;
      case 0: m_BE_PhyColl++;
		break;
      case 2: m_BK_PhyColl++;
		break;
      case 1: m_BK_PhyColl++;
		break;
      default: break;
    }
  }
}

void
QoS::VODelay (std::string context, Ptr<const Packet> packet, const Address &adr)
{
  Ptr<Packet> m_currentPacket; 
  m_currentPacket = packet->Copy();
  TimestampTag timestamp;
  if (m_currentPacket->FindFirstMatchingByteTag (timestamp)){
    Time tx = timestamp.GetTimestamp ();
    Time delay = Simulator::Now () - tx;
    m_VO_sinkDelay = m_VO_sinkDelay + delay.GetNanoSeconds()/1000000;
    m_VO_sinkPackets++;
  }
}
void
QoS::VIDelay (std::string context, Ptr<const Packet> packet, const Address &adr)
{
  Ptr<Packet> m_currentPacket; 
  m_currentPacket = packet->Copy();
  TimestampTag timestamp;
  if (m_currentPacket->FindFirstMatchingByteTag (timestamp)){
    Time tx = timestamp.GetTimestamp ();
    Time delay = Simulator::Now () - tx;
    m_VI_sinkDelay = m_VI_sinkDelay + delay.GetNanoSeconds()/1000000;
    m_VI_sinkPackets++;
  }
}
void
QoS::BEDelay (std::string context, Ptr<const Packet> packet, const Address &adr)
{
  Ptr<Packet> m_currentPacket; 
  m_currentPacket = packet->Copy();
  TimestampTag timestamp;
  if (m_currentPacket->FindFirstMatchingByteTag (timestamp)){
    Time tx = timestamp.GetTimestamp ();
    Time delay = Simulator::Now () - tx;
    m_BE_sinkDelay = m_BE_sinkDelay + delay.GetNanoSeconds()/1000000;
    m_BE_sinkPackets++;
  }
}
void
QoS::BKDelay (std::string context, Ptr<const Packet> packet, const Address &adr)
{
  Ptr<Packet> m_currentPacket; 
  m_currentPacket = packet->Copy();
  TimestampTag timestamp;
  if (m_currentPacket->FindFirstMatchingByteTag (timestamp)){
    Time tx = timestamp.GetTimestamp ();
    Time delay = Simulator::Now () - tx;
    m_BK_sinkDelay = m_BK_sinkDelay + delay.GetNanoSeconds()/1000000;
    m_BK_sinkPackets++;
  }
}

void
QoS::Run()
{
  //LogComponentEnable ("OnOffApplication", LOG_LEVEL_INFO);
  //LogComponentEnable ("PacketSink", LOG_LEVEL_INFO);
  //LogComponentEnable ("EdcaTxopN", LOG_LEVEL_ALL);
  //LogComponentEnable ("MacLow", LOG_LEVEL_ALL);
  //LogComponentEnable ("DcaTxop", LOG_LEVEL_ALL);


  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  Config::Connect ("/NodeList/0/DeviceList/*/Phy/State/RxError", MakeCallback (&QoS::PhyStateRxError, this));
  Config::Connect ("/NodeList/0/DeviceList/*/Phy/State/RxOk", MakeCallback (&QoS::PhyStateRxOk, this));
  Config::Connect ("/NodeList/1/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/2/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/3/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/4/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/5/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/6/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/7/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/8/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/9/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/10/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/11/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/12/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/13/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/14/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/15/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/16/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/17/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/18/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/19/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  Config::Connect ("/NodeList/20/DeviceList/*/Phy/State/Tx", MakeCallback (&QoS::PhyStateTx, this));
  
  Config::Connect ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyRxDrop", MakeCallback (&QoS::PhyRxDrop, this));  
  Config::Connect ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyRxEnd", MakeCallback (&QoS::PhyRxEnd, this));  
  Config::Connect ("/NodeList/1/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/2/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/3/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/4/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/5/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/6/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/7/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/8/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/9/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/10/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/11/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/12/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/13/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/14/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/15/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/16/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/17/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/18/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/19/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));
  Config::Connect ("/NodeList/20/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin", MakeCallback (&QoS::PhyTxBegin, this));

  Config::Connect ("/NodeList/0/DeviceList/0/$ns3::WifiNetDevice/Mac/MacRx", MakeCallback (&QoS::MacRxOkTrace, this));
  Config::Connect ("/NodeList/1/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/2/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/3/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/4/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/5/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/6/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/7/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/8/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/9/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/10/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/11/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/12/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/13/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/14/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/15/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/16/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/17/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/18/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/19/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));
  Config::Connect ("/NodeList/20/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", MakeCallback (&QoS::MacTxOkTrace, this));

  Config::Connect ("/NodeList/0/ApplicationList/0/$ns3::PacketSink/Rx", MakeCallback (&QoS::VODelay, this));
  Config::Connect ("/NodeList/0/ApplicationList/1/$ns3::PacketSink/Rx", MakeCallback (&QoS::VIDelay, this));
  Config::Connect ("/NodeList/0/ApplicationList/2/$ns3::PacketSink/Rx", MakeCallback (&QoS::BEDelay, this));
  Config::Connect ("/NodeList/0/ApplicationList/3/$ns3::PacketSink/Rx", MakeCallback (&QoS::BKDelay, this));

  
  Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
  
  
  uint8_t i = 1;
  while(i<simulationTime+1)
  {
    Simulator::Schedule(Seconds(i), &QoS::InstantThroughput,this,i);
    Simulator::Schedule(Seconds(i), &QoS::UpdateCW,this);
    i++;
  }
  for (unsigned index = 0; index <10; ++index)
  {
	double t = 20000000 + lrint(uv->GetValue (0,20));
    Simulator::Schedule(MicroSeconds(t), &QoS::AddApp,this,0,index, Seconds(20)); 
    Simulator::Schedule(Seconds(40), &QoS::RemoveApp,this,0,index); 
  }
  
  for (unsigned index = 10; index <20; ++index)
  {
    double t = 0 + lrint(uv->GetValue (0,20));
	Simulator::Schedule(MicroSeconds(t), &QoS::AddApp,this,2,index, Seconds(100)); 
    Simulator::Schedule(Seconds(100), &QoS::RemoveApp,this,2,index); 
  }
  
  for (unsigned index = 0; index <4; ++index)
  {
	double t = 50000000 + lrint(uv->GetValue (0,10));
	std::cout << t <<std::endl;
    Simulator::Schedule(MicroSeconds(t), &QoS::AddApp,this,1,index, Seconds(30)); 
    Simulator::Schedule(Seconds(80), &QoS::RemoveApp,this,1,index); 
  }
  
  /*Simulator::Schedule(Seconds(50), &QoS::AddApp,this,1,0, Seconds(30));  
  Simulator::Schedule(Seconds(50), &QoS::AddApp,this,1,1, Seconds(30));  
  Simulator::Schedule(Seconds(80), &QoS::RemoveApp,this,1,0);  
  Simulator::Schedule(Seconds(80), &QoS::RemoveApp,this,1,1);  
  
  Simulator::Schedule(Seconds(50), &QoS::AddApp,this,1,2, Seconds(30));  
  Simulator::Schedule(Seconds(50), &QoS::AddApp,this,1,3, Seconds(30));  
  Simulator::Schedule(Seconds(80), &QoS::RemoveApp,this,1,2);  
  Simulator::Schedule(Seconds(80), &QoS::RemoveApp,this,1,3); */
  //Simulator::Schedule(Seconds(5), &QoS::AddApp,this,1,3);  
  //Simulator::Schedule(Seconds(5), &QoS::AddApp,this,1,4);
  //Simulator::Schedule(Seconds(7), &QoS::RemoveApp,this,1,12);  
 


  Simulator::Stop (Seconds (simulationTime));
  Simulator::Run ();
  monitor->SerializeToXmlFile("test15_1.xml", true, true);
  Simulator::Destroy ();

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


  std::cout << "MAC RX :" << m_macRxOk << " " << std::endl;
  std::cout << "MAC TX :" << m_macTxOk << " " << std::endl;
  std::cout << "Phy TX begin :" << m_phyTxBegin << " " << std::endl;
  std::cout << "Phy RX drop :" << m_phyRxDrop << " " << std::endl;
  std::cout << "Phy RX end :" << m_phyRxEnd << " " << std::endl;
  std::cout << "PhyState TX begin :" << m_phystateTx << " " << std::endl;
  std::cout << "PhyState RX drop :" << m_phystateRxErr << " " << std::endl;
  std::cout << "PhyState RX Ok :" << m_phystateRxOk << " " << std::endl << std::endl;
  std::cout << "collision/packets: " << float(m_phyRxDrop)/m_macRxOk << std::endl;
  std::cout << " RX drop :" << m_BE_PhyColl << " " << std::endl << std::endl;
  std::cout << " RX Ok :" << m_BE_PhyRx << " " << std::endl << std::endl;
}

void
QoS::CalcLimits()
{
  VO_S_lim=0;
  VI_S_lim=0;
  BE_S_lim=0;
  BK_S_lim=0;
  for (unsigned index = 0; index <20; ++index)
  {
    VO_S_lim = VO_S_lim + S_VO_limits[index];
    VI_S_lim = VI_S_lim + S_VI_limits[index];
    BE_S_lim = BE_S_lim + S_BE_limits[index];
    BK_S_lim = BK_S_lim + S_BK_limits[index];
  }
  VO_S_lim = VO_S_lim/1000000;
  VI_S_lim = VI_S_lim/1000000;
  BE_S_lim = BE_S_lim/1000000;
  BK_S_lim = BK_S_lim/1000000;
  std::cout << "Limits: " << VO_S_lim << "|" << VI_S_lim << "|" << BE_S_lim << "|" << BK_S_lim << std::endl;
}

void
QoS::RemoveApp(uint8_t AC, uint8_t node)
{
  if(AC==0){
    S_VO_limits[node] = 0;
  }
  if(AC==1){
    S_VI_limits[node] = 0;
  }
  if(AC==2){
    S_BE_limits[node] = 0;
  }
  if(AC==3){
    S_BK_limits[node] = 0;
  }
}

int main (int argc, char *argv[])
{
  std::string fileNameWithNoExtension = "test15_1";
  std::string graphicsFileName        = fileNameWithNoExtension + ".png";
  std::string plotFileNameAll            = fileNameWithNoExtension + ".plt";
  RngSeedManager::SetSeed (1);//do not change
  RngSeedManager::SetRun (3);
  QoS qos;
  qos.Initialize();
  qos.InstallApps();
  qos.Run();
  
  Gnuplot gnuplotAll = Gnuplot ("All.png");  

  gnuplotAll.AddDataset (qos.data_VO_Avg_S);
  gnuplotAll.AddDataset (qos.data_VI_Avg_S);
  gnuplotAll.AddDataset (qos.data_BE_Avg_S);
  gnuplotAll.AddDataset (qos.data_BK_Avg_S);
  gnuplotAll.AddDataset (qos.data_VO_Avg_D);
  gnuplotAll.AddDataset (qos.data_VI_Avg_D);
  gnuplotAll.AddDataset (qos.data_BE_Avg_D);
  gnuplotAll.AddDataset (qos.data_BK_Avg_D);
  gnuplotAll.AddDataset (qos.data_VO_CWmin);
  gnuplotAll.AddDataset (qos.data_VI_CWmin);
  gnuplotAll.AddDataset (qos.data_BE_CWmin);
  gnuplotAll.AddDataset (qos.data_BK_CWmin);
  gnuplotAll.AddDataset (qos.data_VO_Coll);
  gnuplotAll.AddDataset (qos.data_VI_Coll);
  gnuplotAll.AddDataset (qos.data_BE_Coll);
  gnuplotAll.AddDataset (qos.data_BK_Coll);

  std::ofstream plotFileAll (plotFileNameAll.c_str());
  gnuplotAll.GenerateOutput (plotFileAll);
  plotFileAll.close ();
  return 0;
}  
