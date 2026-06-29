#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include <iostream>
#include <string>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WifiPerformanceEval");

int main (int argc, char *argv[])
{
  // Default values
  uint32_t numSTA = 4;
  std::string macMechanism = "EDCA";
  uint32_t totalLoadPercent = 50;

  // Set up command line parameters
  CommandLine cmd (__FILE__);
  cmd.AddValue ("numSTA", "Number of STA nodes (4, 8, 12, or 20)", numSTA);
  cmd.AddValue ("macMechanism", "MAC mechanism ('EDCA' or 'RTS/CTS')", macMechanism);
  cmd.AddValue ("totalLoadPercent", "Total offered load percent (50, 80, or 90)", totalLoadPercent);
  cmd.Parse (argc, argv);

  // Validate parameters
  if (numSTA != 4 && numSTA != 8 && numSTA != 12 && numSTA != 20) {
      std::cerr << "Invalid numSTA: " << numSTA << ". Must be 4, 8, 12, or 20." << std::endl;
      return 1;
  }
  if (macMechanism != "EDCA" && macMechanism != "RTS/CTS") {
      std::cerr << "Invalid macMechanism: " << macMechanism << ". Must be 'EDCA' or 'RTS/CTS'." << std::endl;
      return 1;
  }
  if (totalLoadPercent != 50 && totalLoadPercent != 80 && totalLoadPercent != 90) {
      std::cerr << "Invalid totalLoadPercent: " << totalLoadPercent << ". Must be 50, 80, or 90." << std::endl;
      return 1;
  }

  // 1. Configure RTS/CTS Threshold globally
  // If "RTS/CTS", threshold = 0 (enabled for all). If "EDCA", threshold = 999999 (disabled).
  if (macMechanism == "RTS/CTS") {
      Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue (0));
  } else {
      Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue (999999));
  }

  // 2. Enable Short Guard Interval globally
  Config::SetDefault ("ns3::HtConfiguration::ShortGuardIntervalSupported", BooleanValue (true));

  // 3. Enable maximum Frame Aggregation sizes for QoS Best Effort
  Config::SetDefault ("ns3::WifiMac::BE_MaxAmpduSize", UintegerValue (65535));
  Config::SetDefault ("ns3::WifiMac::BE_MaxAmsduSize", UintegerValue (7935));

  // Create Nodes
  NodeContainer apNode;
  apNode.Create (1);

  NodeContainer staNodes;
  staNodes.Create (nAumST);

  // 4. Configure Wi-Fi standard and rate manager (Minstrel HT)
  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211n);
  wifi.SetRemoteStationManager ("ns3::MinstrelHtWifiManager");

  // 5. Configure Physical Layer (2.4 GHz, 20 MHz bandwidth, and 1x1 MIMO)
  YansWifiPhyHelper phy;
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  phy.SetChannel (channel.Create ());
  
  // Use the standard ChannelSettings attribute: {ChannelNumber, Width, Band, Primary20Index}
  phy.Set ("ChannelSettings", StringValue ("{0, 20, BAND_2_4GHZ, 0}"));

  // Enforce 1x1 MIMO (1 antenna, 1 spatial stream)
  phy.Set ("Antennas", UintegerValue (1));
  phy.Set ("MaxSupportedTxSpatialStreams", UintegerValue (1));
  phy.Set ("MaxSupportedRxSpatialStreams", UintegerValue (1));

  // 6. Configure MAC layer with QoS SSID
  Ssid ssid = Ssid ("wifi-80211n");
  WifiMacHelper mac;

  // AP MAC
  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid),
               "QosSupported", BooleanValue (true));
  NetDeviceContainer apDevice = wifi.Install (phy, mac, apNode);

  // STA MAC
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "QosSupported", BooleanValue (true));
  NetDeviceContainer staDevices = wifi.Install (phy, mac, staNodes);

  // 7. Configure mobility: AP at (0,0,0) and STAs extremely close at (1,0,0) for maximum signal strength
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0)); // AP Position
  for (uint32_t i = 0; i < numSTA; ++i) {
      positionAlloc->Add (Vector (1.0, 0.0, 0.0)); // STA Positions
  }
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  NodeContainer allNodes;
  allNodes.Add (apNode);
  allNodes.Add (staNodes);
  mobility.Install (allNodes);

  // 8. Configure Internet Stack and IP addresses
  InternetStackHelper stack;
  stack.Install (allNodes);

  Ipv4AddressHelper address;
  address.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer apInterface = address.Assign (apDevice);
  Ipv4InterfaceContainer staInterfaces = address.Assign (staDevices);

  // 9. Calculate traffic load
  // Theoretical raw capacity of 802.11n 20MHz 1x1 MIMO with SGI (MCS7) is 72.2 Mbps.
  double rawCapacityBps = 72.2 * 1e6;
  double targetLoadBps = (totalLoadPercent / 100.0) * rawCapacityBps;
  double staLoadBps = targetLoadBps / numSTA;

  // 10. Install TCP Packet Sink on the AP
  uint16_t port = 9;
  Address apAddress (InetSocketAddress (apInterface.GetAddress (0), port));
  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", apAddress);
  ApplicationContainer sinkApp = packetSinkHelper.Install (apNode.Get (0));
  sinkApp.Start (Seconds (0.0));
  sinkApp.Stop (Seconds (10.0));

  // 11. Install TCP OnOff client applications on each STA
  OnOffHelper onOffHelper ("ns3::TcpSocketFactory", apAddress);
  onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  onOffHelper.SetAttribute ("DataRate", DataRateValue (DataRate (staLoadBps)));
  onOffHelper.SetAttribute ("PacketSize", UintegerValue (1420)); // Optimal payload for standard MTU

  ApplicationContainer clientApps;
  for (uint32_t i = 0; i < numSTA; ++i) {
      clientApps.Add (onOffHelper.Install (staNodes.Get (i)));
  }
  clientApps.Start (Seconds (0.0));
  clientApps.Stop (Seconds (10.0));

  // 12. Run Simulation for exactly 10.0 seconds
  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();

  // 13. Calculate and Print Received Throughput in Mbit/sec
  Ptr<PacketSink> sink = DynamicCast<PacketSink> (sinkApp.Get (0));
  uint64_t totalBytesReceived = sink->GetTotalRx ();
  double throughputMbit = (totalBytesReceived * 8.0) / (10.0 * 1e6);

  std::cout << throughputMbit << std::endl;

  Simulator::Destroy ();
  return 0;
}
