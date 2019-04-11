/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"

using namespace ns3;
int main (int argc, char *argv[])
{
    // Create the network nodes
    NodeContainer hosts;
    NodeContainer routers;

    hosts.Create(3);
    routers.Create(4);
    
    // Install internet stack on each node 
    InternetStackHelper stack;
    stack.Install(hosts);
    stack.Install(routers);
    
    // Create point to point helper
    PointToPointHelper p2p;
    
    // Create an address helper
    Ipv4AddressHelper address;  
    
    
    //  Network Configuration
    
    std::string speed = "10Mbps";
    CommandLine cmd;
    cmd.AddValue("DefaultRate", "Default data rate to be used on network devices", speed);
    cmd.Parse(argc, argv);
    std::cout << "Speed =" << speed << std::endl;
    
    
    DataRate rate(speed);
    Time delay("2ms");
    
    Config::SetDefault("ns3::PointToPointNetDevice::DataRate", DataRateValue(rate));
    Config::SetDefault("ns3::PointToPointChannel::Delay", TimeValue(delay));
    Config::SetDefault("ns3::CsmaNetDevice::DataRate", DataRateValue(rate));
    Config::SetDefault("ns3:CsmaChannel::Delay", TimeValue(delay));
    
    // Configuring individual subnets
    
    // Configure subnet 1
    NodeContainer subnet1;
    subnet1.Add(hosts.Get(0));
    subnet1.Add(hosts.Get(1));
    subnet1.Add(hosts.Get(2));
    subnet1.Add(routers.Get(0));

    // Create a device container 
    NetDeviceContainer subnet1Devices = p2p.Install(subnet1);  
    
    // Configure the subnet address and mask
    address.SetBase("137.158.127.191", "255.255.255.224");
    
    Ipv4InterfaceContainer subnet1Interfaces = address.Assign(subnet1Devices);
    
    
    // Configure subnet2    
    NodeContainer subnet2;
    subnet2.Add(routers.Get(0));
    subnet2.Add(routers.Get(1));
    
    DataRate rate_subnet2("50Mbps");
    Time delay_subnet2(MilliSeconds(4));    
    
    // Create device container for subnet2
    NetDeviceContainer subnet2Devices = p2p.Install(subnet2);
    
    address.SetBase("137.158.128.191", "255.255.255.224");
    
    Ipv4InterfaceContainer subnet2Interfaces = address.Assign(subnet2Devices);
    
    
    // Subnet 2: 2 Config
    
    Config::Set("/NodeList/1/DeviceList/2/$ns3::PointToPointNetDevice/DataRate", DataRateValue(rate_subnet2));
    
    Config::Set("/NodeList/2/DeviceList/1/$ns3::PointToPointNetDevice/DataRate", DataRateValue(rate_subnet2));
    
    Config::Set("/ChannelList/1/$ns3::PointToPointChannel/Delay", TimeValue(delay_subnet2));
    
    // Configure subnet3
    NodeContainer subnet3;
    subnet3.Add(routers.Get(1));
    subnet3.Add(routers.Get(2));
    
    // Create device container for subnet3
    NetDeviceContainer subnet3Devices = p2p.Install(subnet3);
    
    address.SetBase("137.158.129.191", "255.255.255.224");
    
    Ipv4InterfaceContainer subnet3Interfaces = address.Assign(subnet3Devices);
 
 
 
        
    // Configure subnet4
    NodeContainer subnet4;
    subnet4.Add(routers.Get(1));
    subnet4.Add(routers.Get(3));
     

    // Create device container for subnet4
    NetDeviceContainer subnet4Devices = p2p.Install(subnet4);
    
         Ptr<NetDevice> deviceA = subnet4Devices.Get(0);
    Ptr<NetDevice> deviceB = subnet4Devices.Get(1);

    NetDevice * deviceA_ptr = PeekPointer(deviceA);
    NetDevice * deviceB_ptr = PeekPointer(deviceB);
    
    PointToPointNetDevice* p2pDeviceA = dynamic_cast<PointToPointNetDevice*>(deviceA_ptr);    
        PointToPointNetDevice* p2pDeviceB = dynamic_cast<PointToPointNetDevice*>(deviceB_ptr);
    
    DataRate rate_subnet4("100Mbps");
    
    p2pDeviceA->SetAttribute("DataRate", DataRateValue(rate_subnet4));
    p2pDeviceB->SetAttribute("DataRate", DataRateValue(rate_subnet4));
    
    Ptr<Channel> channel = ChannelList::GetChannel(3);
    Channel * channel_ptr = PeekPointer(channel);
    Time delay_subnet4("5ms");
    PointToPointChannel* p2pChannel = dynamic_cast<PointToPointChannel*>(channel_ptr);
    p2pChannel->SetAttribute("Delay", TimeValue(delay_subnet4));
   
    address.SetBase("137.158.130.191", "255.255.255.224");
    
    Ipv4InterfaceContainer subnet4Interfaces = address.Assign(subnet4Devices);
    
   
    // Create branch 
    NodeContainer branch;
    
    // Create 3 branch nodes
    branch.Create(3);
    
    // Install the TCP/IP stack on the nodes
    stack.Install(branch);
    
    // Create CSMA helper
    CsmaHelper csma;
 
    csma.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
        
    // Configure subnet5
    NodeContainer subnet5 ;
    subnet5.Add(routers.Get(2));
    subnet5.Add(branch);
    
    
    NetDeviceContainer subnet5Devices = csma.Install(subnet5);
    
    address.SetBase("137.158.131.191", "255.255.255.224");
    
    Ipv4InterfaceContainer subnet5Interfaces = address.Assign(subnet5Devices);
    Simulator::Run();
    Simulator::Destroy();

    return 0;   
}
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"

using namespace ns3;
int main (int argc, char *argv[])
{
    // Create the network nodes
    NodeContainer hosts;
    NodeContainer routers;

    hosts.Create(3);
    routers.Create(4);
    
    // Install internet stack on each node 
    InternetStackHelper stack;
    stack.Install(hosts);
    stack.Install(routers);
    
    // Create point to point helper
    PointToPointHelper p2p;
    
    // Create an address helper
    Ipv4AddressHelper address;  
    
    
    //  Network Configuration
    
    std::string speed = "10Mbps";
    CommandLine cmd;
    cmd.AddValue("DefaultRate", "Default data rate to be used on network devices", speed);
    cmd.Parse(argc, argv);
    std::cout << "Speed =" << speed << std::endl;
    
    
    DataRate rate(speed);
    Time delay("2ms");
    
    Config::SetDefault("ns3::PointToPointNetDevice::DataRate", DataRateValue(rate));
    Config::SetDefault("ns3::PointToPointChannel::Delay", TimeValue(delay));
    Config::SetDefault("ns3::CsmaNetDevice::DataRate", DataRateValue(rate));
    Config::SetDefault("ns3:CsmaChannel::Delay", TimeValue(delay));
    
    // Configuring individual subnets
    
    // Configure subnet 1
    NodeContainer subnet1;
    subnet1.Add(hosts.Get(0));
    subnet1.Add(hosts.Get(1));
    subnet1.Add(hosts.Get(2));
    subnet1.Add(routers.Get(0));

    // Create a device container 
    NetDeviceContainer subnet1Devices = p2p.Install(subnet1);  
    
    // Configure the subnet address and mask
    address.SetBase("137.158.127.191", "255.255.255.224");
    
    Ipv4InterfaceContainer subnet1Interfaces = address.Assign(subnet1Devices);
    
    
    // Configure subnet2    
    NodeContainer subnet2;
    subnet2.Add(routers.Get(0));
    subnet2.Add(routers.Get(1));
    
    DataRate rate_subnet2("50Mbps");
    Time delay_subnet2(MilliSeconds(4));    
    
    // Create device container for subnet2
    NetDeviceContainer subnet2Devices = p2p.Install(subnet2);
    
    address.SetBase("137.158.128.191", "255.255.255.224");
    
    Ipv4InterfaceContainer subnet2Interfaces = address.Assign(subnet2Devices);
    
    
    // Subnet 2: 2 Config
    
    Config::Set("/NodeList/1/DeviceList/2/$ns3::PointToPointNetDevice/DataRate", DataRateValue(rate_subnet2));
    
    Config::Set("/NodeList/2/DeviceList/1/$ns3::PointToPointNetDevice/DataRate", DataRateValue(rate_subnet2));
    
    Config::Set("/ChannelList/1/$ns3::PointToPointChannel/Delay", TimeValue(delay_subnet2));
    
    // Configure subnet3
    NodeContainer subnet3;
    subnet3.Add(routers.Get(1));
    subnet3.Add(routers.Get(2));
    
    // Create device container for subnet3
    NetDeviceContainer subnet3Devices = p2p.Install(subnet3);
    
    address.SetBase("137.158.129.191", "255.255.255.224");
    
    Ipv4InterfaceContainer subnet3Interfaces = address.Assign(subnet3Devices);
 
 
 
        
    // Configure subnet4
    NodeContainer subnet4;
    subnet4.Add(routers.Get(1));
    subnet4.Add(routers.Get(3));
     

    // Create device container for subnet4
    NetDeviceContainer subnet4Devices = p2p.Install(subnet4);
    
         Ptr<NetDevice> deviceA = subnet4Devices.Get(0);
    Ptr<NetDevice> deviceB = subnet4Devices.Get(1);

    NetDevice * deviceA_ptr = PeekPointer(deviceA);
    NetDevice * deviceB_ptr = PeekPointer(deviceB);
    
    PointToPointNetDevice* p2pDeviceA = dynamic_cast<PointToPointNetDevice*>(deviceA_ptr);    
        PointToPointNetDevice* p2pDeviceB = dynamic_cast<PointToPointNetDevice*>(deviceB_ptr);
    
    DataRate rate_subnet4("100Mbps");
    
    p2pDeviceA->SetAttribute("DataRate", DataRateValue(rate_subnet4));
    p2pDeviceB->SetAttribute("DataRate", DataRateValue(rate_subnet4));
    
    Ptr<Channel> channel = ChannelList::GetChannel(3);
    Channel * channel_ptr = PeekPointer(channel);
    Time delay_subnet4("5ms");
    PointToPointChannel* p2pChannel = dynamic_cast<PointToPointChannel*>(channel_ptr);
    p2pChannel->SetAttribute("Delay", TimeValue(delay_subnet4));
   
    address.SetBase("137.158.130.191", "255.255.255.224");
    
    Ipv4InterfaceContainer subnet4Interfaces = address.Assign(subnet4Devices);
    
   
    // Create branch 
    NodeContainer branch;
    
    // Create 3 branch nodes
    branch.Create(3);
    
    // Install the TCP/IP stack on the nodes
    stack.Install(branch);
    
    // Create CSMA helper
    CsmaHelper csma;
 
    csma.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
        
    // Configure subnet5
    NodeContainer subnet5 ;
    subnet5.Add(routers.Get(2));
    subnet5.Add(branch);
    
    
    NetDeviceContainer subnet5Devices = csma.Install(subnet5);
    
    address.SetBase("137.158.131.191", "255.255.255.224");
    
    Ipv4InterfaceContainer subnet5Interfaces = address.Assign(subnet5Devices);
    Simulator::Run();
    Simulator::Destroy();

    return 0;   
}
