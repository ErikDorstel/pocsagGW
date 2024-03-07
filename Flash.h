#include <Preferences.h>
#include <nvs_flash.h>

Preferences flash;

void getFlash() {
  flash.begin("SX1278FSK",true);
  Log.print(0,"Center Frequency: %s MHz\r\n",String(flash.getDouble("centerFreq",439.9875),5).c_str());
  Log.print(0,"Rx Frequency Offset: %s kHz\r\n",String(flash.getDouble("rxOffset",7.446),3).c_str());
  Log.print(0,"Bitrate: %s bps\r\n",String(flash.getDouble("bitrate",1.2)*1000,0).c_str());
  Log.print(0,"Shift Frequency: +/- %s Hz\r\n",String(flash.getDouble("shift",4.5)*1000,0).c_str());
  Log.print(0,"Rx Bandwidth: %s kHz\r\n",String(flash.getDouble("rxBandwidth",5.2),1).c_str());
  Log.print(0,"AFC Bandwidth: %s kHz\r\n",String(flash.getDouble("afcBandwidth",25),1).c_str());
  Log.print(0,"BOS Mode: %i\r\n",flash.getBool("isBOS",false));
  Log.print(0,"DAU Filter: %s\r\n",String(flash.getString("daufilter","")).c_str());
  Log.print(0,"Eth DHCP: %i\r\n",flash.getBool("ethDHCP",true));
  Log.print(0,"Eth IP: %s\r\n",flash.getString("ethIP","192.168.100.100").c_str());
  Log.print(0,"Eth GW: %s\r\n",flash.getString("ethGW","192.168.100.1").c_str());
  Log.print(0,"Eth DNS: %s\r\n",flash.getString("ethDNS","192.168.100.1").c_str());
  Log.print(0,"Eth NM: %s\r\n",flash.getString("ethNM","255.255.255.0").c_str());
  Log.print(0,"Gateway URL: %s\r\n",String(flash.getString("gwURL","")).c_str());
  Log.print(0,"TELNET Password: xxxx\r\n");
  flash.end(); }

void readFlash() {
  flash.begin("SX1278FSK",true);
  modem.stopSequencer();
  modem.setFrequency(flash.getDouble("centerFreq",439.9875),flash.getDouble("rxOffset",7.446));
  modem.setBitrate(flash.getDouble("bitrate",1.2));
  modem.setShift(flash.getDouble("shift",4.5));
  modem.setRxBandwidth(flash.getDouble("rxBandwidth",5.2));
  modem.setAfcBandwidth(flash.getDouble("afcBandwidth",25));
  modem.startSequencer();
  modem.restartRx(true);
  modem.isBOS=flash.getBool("isBOS",false);
  modem.daufilter=flash.getString("daufilter","");
  ethDHCP=flash.getBool("ethDHCP",true);
  ethIP=flash.getString("ethIP","192.168.100.100");
  ethGW=flash.getString("ethGW","192.168.100.1");
  ethDNS=flash.getString("ethDNS","192.168.100.1");
  ethNM=flash.getString("ethNM","255.255.255.0");
  gwURL=flash.getString("gwURL","");
  telnetPass=flash.getString("telnetPass","pocsag");
  flash.end();
  Log.print(0,"Flash: read\r\n");
  if (checkEth()>0) { configEth(); } }

void writeFlash() {
  flash.begin("SX1278FSK",false);
  flash.putDouble("centerFreq",modem.centerFreq);
  flash.putDouble("rxOffset",modem.rxOffset);
  flash.putDouble("bitrate",modem.bitrate);
  flash.putDouble("shift",modem.shift);
  flash.putDouble("rxBandwidth",modem.rxBandwidth);
  flash.putDouble("afcBandwidth",modem.afcBandwidth);
  flash.putBool("isBOS",modem.isBOS);
  flash.putString("daufilter",modem.daufilter);
  flash.putBool("ethDHCP",ethDHCP);
  flash.putString("ethIP",ethIP);
  flash.putString("ethGW",ethGW);
  flash.putString("ethDNS",ethDNS);
  flash.putString("ethNM",ethNM);
  flash.putString("gwURL",gwURL);
  flash.putString("telnetPass",telnetPass);
  flash.end();
  Log.print(0,"Flash: written\r\n"); }

void eraseFlash() {
  nvs_flash_erase(); nvs_flash_init();
  Log.print(0,"Flash: erased\r\n"); }
