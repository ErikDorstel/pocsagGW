#include "SX1278.h"
SX1278FSK modem(false,0);
#include "CLI.h"

void setup() {
  initEth();
  initTELNET();
  modem.initChip();
  modem.beginPOCSAG();
  readFlash(); }

void loop() { modem.pocsagWorker(); cliWorker(); ethWorker(); telnetWorker(); }
