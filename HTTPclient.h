//#include <WiFi.h>
//#include <WiFiClientSecure.h>
//#include <HTTPClient.h>

//WiFiClientSecure client;
//HTTPClient https;

String gwURL="";
int httpStatus;
int httpMaxRetries=2;
uint32_t httpRetried;
uint32_t httpFailed;

//void postHTTPS(String postData) {
//  uint8_t attempt=0; while (attempt<=httpMaxRetries) {
//    if (checkEth()==2) { client.connect(gwURL.c_str(),443); https.begin(client,gwURL);
//      https.addHeader("Content-Type","application/x-www-form-urlencoded");
//      httpStatus=https.POST(postData);
//      Log.print(2,"    HTTP POST: %s\r\n",postData.c_str());
//      Log.print(1,"    HTTP Attempt: %i    Status: %i\r\n",attempt+1,httpStatus);
//      https.end(); client.stop(); } else { httpStatus=0; }
//    attempt++; if (httpStatus==200) { break; } }
//  if (attempt>httpMaxRetries && httpStatus!=200) { httpFailed++; }
//  else if (attempt>1) { httpRetried++; } }

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
