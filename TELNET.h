// change C:\Users\xxxx\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.x\cores\esp32\Server.h
// "virtual void begin(uint16_t port=0) =0;" to "virtual void begin() =0;"

#ifndef SX1278_TELNET_H
#define SX1278_TELNET_H

EthernetServer telnetServer(23);
EthernetClient telnetSession;

uint8_t sessionActive=0;
bool isAuth=false;
String telnetPass="pocsag";
String passBuffer="";
int isIAC=0;

void writeTelnet(const char value) { if (telnetSession.connected() && isAuth) { telnetSession.write(value); } }

void printTelnet(const char* value) { if (telnetSession.connected() && isAuth) { telnetSession.print(value); } }

void iacEcho(bool value) { if (value) { telnetSession.print("\xff\xfc\x01"); } else { telnetSession.print("\xff\xfb\x01"); } }

void initTELNET() {
  telnetServer.begin();
  Log.writeTelnet=writeTelnet;
  Log.printTelnet=printTelnet; }

void telnetWorker() {
  static uint64_t telnetTimer=millis()+1111;
  if (millis()>=telnetTimer) { telnetTimer=millis()+1111;
      EthernetClient newSession=telnetServer.accept();

    if (((!telnetSession) || (!isAuth)) && newSession) {
      telnetSession=newSession;
      sessionActive=1; isAuth=false; passBuffer="";
      Log.print(0,"TELNET Session from %s connected\r\n",telnetSession.remoteIP().toString().c_str());
      iacEcho(false); telnetSession.print("Password: "); }
    else { newSession.stop(); }

    if ((sessionActive || telnetSession) && (!telnetSession.connected())) {
      telnetSession.stop();
      sessionActive=0; isAuth=false; passBuffer="";
      Log.print(0,"TELNET Session disconnected\r\n"); } }

  if (telnetSession && (!isAuth) && telnetSession.available()) {
    char telnetByte=telnetSession.read();
    if (isIAC==3) { isIAC=0; }
    if (telnetByte==255 && (!isIAC)) { isIAC++; }
    else if (telnetByte==255 && isIAC) { isIAC=0; }
    else if (isIAC) { isIAC++; }
    else if (telnetByte==127) { telnetSession.write(telnetByte); passBuffer.remove(passBuffer.length()-1); }
    else if (telnetByte==10) {}
    else if (telnetByte==13) {
      if (passBuffer==telnetPass) {
        iacEcho(true); isAuth=true; telnetSession.print("\r\n"); Log.print(0,"TELNET Authentication from %s passed\r\n> ",telnetSession.remoteIP().toString().c_str()); passBuffer=""; Log.needCR=true; }
      else { if (passBuffer!="") { Log.print(0,"TELNET Authentication from %s failed\r\n",telnetSession.remoteIP().toString().c_str()); } passBuffer=""; telnetSession.print("\r\nPassword: "); } }
    else { telnetSession.write("*"); passBuffer+=String(telnetByte); } } }

#endif
