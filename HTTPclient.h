// change C:\Users\xxxx\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.x\cores\esp32\Server.h
// "virtual void begin(uint16_t port=0) =0;" to "virtual void begin() =0;"

#include <SPI.h>
#include <Ethernet.h>

String gwURL="";
int httpStatus;
int httpMaxRetries=2;
uint32_t httpRetried;
uint32_t httpFailed;

class httpClient {
  public:
  EthernetClient httpClientRequest;
  int responseStatus;
  String responseHeader;
  String responseBody;
  void get(String host,String path="/",String value="") { send("GET",host,path,value); }
  void post(String host,String path="/",String value="") { send("POST",host,path,value); }
  void send(String type,String host,String path,String value) {
    httpClientRequest.connect(host.c_str(),80);
    httpClientRequest.println(type + " " + path + " HTTP/1.0");
    httpClientRequest.println("Host: " + host);
    httpClientRequest.println("Accept: text/html");
    httpClientRequest.println("Content-Type: application/x-www-form-urlencoded");
    httpClientRequest.println("Content-Length: " + String(value.length()));
    httpClientRequest.println("User-Agent: Mozilla/5.0");
    httpClientRequest.println();
    if (value.length()) { httpClientRequest.println(value); }
    responseHeader=""; responseBody=""; String line=""; bool isHeader=true; responseStatus=-1;
    while (httpClientRequest.connected()) { if (httpClientRequest.available()) {
      char c=httpClientRequest.read(); line+=c; if (c=='\n') {
        if (line.length()<3) { isHeader=false; }
        else { if (isHeader) { responseHeader+=line; } else { responseBody+=line; } } line=""; } } }
    if (responseHeader.startsWith("HTTP/")) {
      int a=responseHeader.indexOf(" ")+1; int b=responseHeader.indexOf(" ",a); responseStatus=responseHeader.substring(a,b).toInt(); }
    httpClientRequest.stop(); } };

httpClient httpClientRequest;

void postHTTP(String postData) {
  uint8_t attempt=0; while (attempt<=httpMaxRetries) {
    if (checkEth()==2) {
      int a=gwURL.indexOf("://")+3; int b=gwURL.indexOf("/",a)+1;
      String httpHost=gwURL.substring(a,b-1);
      String httpPath=gwURL.substring(b-1);
      httpClientRequest.post(httpHost,httpPath,postData);
      httpStatus=httpClientRequest.responseStatus;
      Log.print(2,"    HTTP POST: %s\r\n",postData.c_str());
      Log.print(1,"    HTTP Attempt: %i    Status: %i\r\n",attempt+1,httpStatus); }
    else { httpStatus=0; }
    attempt++; if (httpStatus==200) { break; } }
  if (attempt>httpMaxRetries && httpStatus!=200) { httpFailed++; }
  else if (attempt>1) { httpRetried++; } }

String urlencode(String value) {
  String result=""; char x; char x0; char x1;
  for (int idx=0;idx<value.length();idx++) {
    x=value.charAt(idx);
    if (x==' ') { result+='+'; }
    else if (isalnum(x)) { result+=x; }
    else { x1=(x&0xf)+'0';
      if ((x & 0xf)>9) { x1=(x&0xf)-10+'A'; }
      x=(x>>4)&0xf; x0=x+'0';
      if (x>9) { x0=x-10+'A'; }
      result+='%'; result+=x0; result+=x1; } }
  return result; }
