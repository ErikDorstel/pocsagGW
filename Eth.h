#include <SPI.h>
#include <Ethernet.h>

bool ethDHCP;
String ethIP="";
String ethGW="";
String ethDNS="";
String ethNM="";
uint8_t ethMAC[6];
uint32_t upEvents;
uint32_t downEvents;

void initEth() {
  Ethernet.init(5);
  SPI.setFrequency(10000000);
  uint64_t espid=ESP.getEfuseMac();
  Log.print(0,"ESP ID: %s\r\n",String(espid,HEX).c_str());
  // WiFiSTA: espid+0, WiFiAP: espid+1, Bluetooth: espid+2, Ethernet: espid+3
  espid+=3;
  Log.print(0,"Ethernet MAC: %s\r\n",String(espid,HEX).c_str());
  ethMAC[0]=espid>>40&255; ethMAC[1]=espid>>32&255; ethMAC[2]=espid>>24&255;
  ethMAC[3]=espid>>16&255; ethMAC[4]=espid>>8&255; ethMAC[5]=espid>>0&255; }

uint8_t checkEth() {
  uint8_t value=0;
  if (Ethernet.linkStatus()==LinkON) { value=1;
    if (Ethernet.localIP()!=IPAddress(0,0,0,0)) { value=2; } }
  return value; }

void configEth() {
  if (ethDHCP) { Ethernet.begin(ethMAC); Log.print(0,"Ethernet DHCP IP: %s\r\n",Ethernet.localIP().toString().c_str()); }
  else { int a,b,c; uint8_t ethIP_arr[4]; uint8_t ethGW_arr[4]; uint8_t ethDNS_arr[4]; uint8_t ethNM_arr[4];

    a=ethIP.indexOf(".")+1; b=ethIP.indexOf(".",a)+1; c=ethIP.indexOf(".",b)+1;
    ethIP_arr[0]=ethIP.substring(0,a-1).toInt(); ethIP_arr[1]=ethIP.substring(a,b-1).toInt();
    ethIP_arr[2]=ethIP.substring(b,c-1).toInt(); ethIP_arr[3]=ethIP.substring(c).toInt();

    a=ethGW.indexOf(".")+1; b=ethGW.indexOf(".",a)+1; c=ethGW.indexOf(".",b)+1;
    ethGW_arr[0]=ethGW.substring(0,a-1).toInt(); ethGW_arr[1]=ethGW.substring(a,b-1).toInt();
    ethGW_arr[2]=ethGW.substring(b,c-1).toInt(); ethGW_arr[3]=ethGW.substring(c).toInt();

    a=ethDNS.indexOf(".")+1; b=ethDNS.indexOf(".",a)+1; c=ethDNS.indexOf(".",b)+1;
    ethDNS_arr[0]=ethDNS.substring(0,a-1).toInt(); ethDNS_arr[1]=ethDNS.substring(a,b-1).toInt();
    ethDNS_arr[2]=ethDNS.substring(b,c-1).toInt(); ethDNS_arr[3]=ethDNS.substring(c).toInt();

    a=ethNM.indexOf(".")+1; b=ethNM.indexOf(".",a)+1; c=ethNM.indexOf(".",b)+1;
    ethNM_arr[0]=ethNM.substring(0,a-1).toInt(); ethNM_arr[1]=ethNM.substring(a,b-1).toInt();
    ethNM_arr[2]=ethNM.substring(b,c-1).toInt(); ethNM_arr[3]=ethNM.substring(c).toInt();

    Ethernet.begin(ethMAC,ethIP_arr,ethDNS_arr,ethGW_arr,ethNM_arr); Log.print(0,"Ethernet Static IP: %s\r\n",Ethernet.localIP().toString().c_str()); } }

void ethWorker() {
  static uint64_t ethTimer=millis()+1111; static uint8_t ethOld=0;
  if (millis()>=ethTimer) { ethTimer=millis()+2222;
    uint8_t ethNew=checkEth();
    if (ethNew==0 && ethNew!=ethOld) { ethOld=ethNew; downEvents++; Log.print(0,"Ethernet Link: Down   IP: Down\r\n"); }
    if (ethNew==1 && ethNew!=ethOld) { ethOld=ethNew; Log.print(0,"Ethernet Link: Up   IP: Down\r\n"); }
    if (ethNew==2 && ethNew!=ethOld) { ethOld=ethNew; upEvents++; Log.print(0,"Ethernet Link: Up   IP: Up\r\n"); }
    if (ethNew==1) { configEth(); }
    if (ethDHCP) { Ethernet.maintain(); } } }
