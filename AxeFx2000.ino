/**
     Axe-FX-BCR-2000-Control-Bridge
     Name: AxeFx2000.ino
     Purpose: This is an Arduino based project for communication translation between Behringer BCR2000 and
              Fractal Audio Axe-FX II. You are able to control your Axe-FX with a Behringer BCR2000

     @author Bastian Bührig
     @version 0.1 02/23/18

     This proect uses this Arduino MIDILibrary: https://github.com/FortySevenEffects/arduino_midi_library

     Wiring:
     -------
     BCR-Midi Out B/Thru -> Axe-FX In -> Axe-FX Out(!) -> Arduino In -> Arduino Out -> BCR In

     Use Behringer BCR2000-MIDI-Mode S1!

     See in config.h to declare your BCR2000-Axe-FX-Config. You have to declare, which encoder communicates with which Axe-FX-Parameter

     NOTICE: I am using an Arduino MEGA with four serial-ports. My MIDI-Shield ist connected to Serial1 and then
             I am able to send some messages to the serial monitor via Serial.println(...). If you use an Arduino UNO
             you only have one Serial-port. Then you have to remove all Serial.print/Serial.println commands and
             you have to set the MIDI-library to use Serial instead of Serial1-port!
**/

#include <MIDI.h>
#include "constants.h"
#include "config.h"


// Create a 'MIDI' object using MySettings bound to Serial1. Switch to Serial if you are an Ardino UNO user!
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial1, MIDI, midi::DefaultSettings);

// Sysex-Template for Sysex-communication
byte sysexSendDataParam[] = { 0xF0, 0x00, 0x01, 0x74, 0x07, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF7 };

// Global variables for remebering CC-values send by the BCR2000!
unsigned int val1 = 0;
unsigned int val2 = 0;

// We don't want to double incomming CC-messages for setting BCR-settings. We need to pause the CC-handler!
int pauseCCHandler = 0;


/**
   Simple setup your Arduino with MIDI-settings and Serial-Monitor settings if you are on an Arduino-Mega
*/
void setup() {
  // Set MIDI-Channel of BCR-2000 for listening on CCs.
  MIDI.begin(CHANNEL_BCR);
  // we don't won't to echo all incoming data without filtering or translating!
  MIDI.setThruFilterMode(midi::Thru::Off);

  // declare our MIDI-handlers.
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleSystemExclusive(handleSystemExclusive);

  // Serial-Monitor-settings
  Serial.begin(2000000);
  Serial.println("Waiting for incoming CCs and Sysex...");
}


/**
   Start reading MIDI-Inputs...
*/
void loop() {
  MIDI.read();
}


/**
   Handle all incoming ControlChange messages. BCR2000 is sending different CC-messages to the
   Arduino. If you are moving an encoder the BCR will send a 14bit value in two different CC messages.
   E.g. you are moving encoder 3 that is configured to send on CC#3 you will get the High-byte of the
   value on this CC and the Low-byte of the value on CC#3+32 (35).

   By pushing a button on the BCR2000 it will send only one value on the configured CC.

   By config.h the CONFIG-array defines which entry is a button and which an encoder. Then we will remember
   the sent values an resend all to the Axe-FX after we received all data.

   @param channel MIDI channel for incoming MIDI ControlChange message
   @param number  number of controller that was received
   @param value   value of invcoming ControlChange message. Possible values: 0-127
*/
void handleControlChange(byte channel, byte number, byte value) {
  // First we check to pause the CC-Handler because of echoing CCs sent to the BCR!
  if (pauseCCHandler > 0) {
    pauseCCHandler--;
    return;
  }

  // Readout our CONFIG and search for the received CC
  for (int i = 0; i < sizeof(CONFIG) / sizeof(CONFIG[0]); i++) {

    // 1. Check encoders
    if (number == *CONFIG[i][0] && CONFIG[i][3] == ENCODER) {
      // Encoder CC received - first Value
      val1 = value * 128;

    } else if (number == (*CONFIG[i][0]) + 32 && CONFIG[i][3] == ENCODER) {
      // Encoder CC received - second value
      val2 = value;
      setAxeFxParam( CONFIG[i][1], CONFIG[i][2], bcr2axeValue(val1 + val2));

    }
    // 2. Check switches
    else if (number == *CONFIG[i][0] && CONFIG[i][3] == SWITCH) {
      val1 = 0;
      val2 = value;
      setAxeFxParam( CONFIG[i][1], CONFIG[i][2], bcr2axeValue(val1 + val2));

    }
    // 3. Check readout all parameters
    else if (number == 127) {
      // Readout-Button pressed! Get all parameters from the Axe-FX and sync all
      // encoders and buttons on the BCR!
      Serial.println("\nReading all Parameter from Axe-FX!");
      readAllParameters();
    }
  }
}


/**
   Iterate through the whole config-array and send for all configured parameters a
   Sysex-Parameter-Get-Function to sync the BCR2000 encoder rings!
*/
void readAllParameters() {
  for (int i = 0; i < sizeof(CONFIG) / sizeof(CONFIG[0]); i++) {
    getAxeFxParam(CONFIG[i][1], CONFIG[i][2]);
  }
}


/**
   Handle all incoming Sysex-Data. First we have to filter out Realtime-Sysex like Tempo and Tuner.
   Then we filter out our Sysex Get- and Set-requests for Axe-FX parameters. We only want the returned
   data from Sysex-Get-Requests to read it out and sync the BCR-encoders.
   Additionally if there is a preset-change on the Axe-FX we receive a Sysex-Program-Chage message. Then
   we want to read out all parameters from CONFIG to sync the BCR2000 encoders!

   @param sysexArray received sysex-data-message to analyze
   @param size length of the sysex-message
*/
void handleSystemExclusive(byte* sysexArray, unsigned size) {
  if (sysexArray[5] == 0x10) {
    // Filter out: SYSEX TEMPO
    return;
  }

  if (sysexArray[5] == 0x0d) {
    // Filter out: SYSEX TUNER
    return;
  }

  if (sysexArray[5] == 0x02 && sysexArray[13] == 0x00 && size == 16) {
    // Filter out: SYSEX GET-REQUEST
    return;
  }
  if (sysexArray[5] == 0x02 && sysexArray[13] == 0x01 && size == 16) {
    // Filter out: SYSEX SET-REQUEST
    return;
  }
  if (sysexArray[5] == 0x14) {
    // Preset-Change on Axe-FX -> read out all parameters defined in CONFIG
    readAllParameters();
    return;
  }


  // get-parameter response-data. Get parameter-value
  unsigned int value = parameterValueBytesToInt(sysexArray);

  // Did we received parameter for a BCR2000-switch?
  if (isSwitch(sysexArray)) {
    // then we will send only one CC to BCR for switch-status
    pauseCCHandler += 1;
    if (value == 1) {
      // switch on
      MIDI.sendControlChange(getControllerNumber(sysexArray), SWITCH_ON, CHANNEL_BCR);
    } else {
      // switch off
      MIDI.sendControlChange(getControllerNumber(sysexArray), SWITCH_OFF, CHANNEL_BCR);
    }
  } else {
    // we received parameter-value for an encoder. We have to send two CCs!
    pauseCCHandler += 2;

    // sending received parameter value to BCR with two CC-messages.
    byte controllerNumber = getControllerNumber(sysexArray);
    MIDI.sendControlChange(controllerNumber, getHiByte(axe2bcrValue(value)), CHANNEL_BCR);
    MIDI.sendControlChange(controllerNumber + 32, getLoByte(axe2bcrValue(value)), CHANNEL_BCR);
  }

}


/**
   Converts a received sysex-parameter-value from Axe-FX to standard Integer-value.

   @param sysexData received sysex-data for analyzing and read out the value-part. The Axe-FX sends two bytes for a possible value (0-65534)
   @return The integer-value from the sysex-data
*/
unsigned int parameterValueBytesToInt(byte *sysexData) {
  byte byte1 = sysexData[10];
  byte byte2 = sysexData[11];
  byte byte3 = sysexData[12];
  return (byte1 & 0x7F) | ((byte2 & 0x7F) << 7) | ((byte3 & 0x7F) << 14);
}


/**
   Converts a BCR2000-value (0-16383) to an Axe-FX parameter value. The Axe-FX understands much
   smoother values than the 14-bit BCR-values. So we have to convert them.

   @param bcrValue The value from BCR to convert to Axe-FX value
   @return The converted value for Axe-FX parameter settings.
*/
unsigned int bcr2axeValue(unsigned int bcrValue) {
  return (double)bcrValue * (double)4.00015;
}


/**
   Converts an Axe-FX value (0-65534) to a BCR-encoder-value. The Axe-FX understands much
   smoother values than the 14-bit BCR-values. So we have to convert them.

   @param bcrValue The value from Axe-FX to convert to BCR-value
   @return The converted value for BCR encoder settings.
*/
unsigned int axe2bcrValue(unsigned int axeValue) {
  return (double)axeValue / (double)4.00015;
}


/**
   Gets the Low-byte from an integer value for sending to the BCR.

   @param integer-value for extracting the Low-byte-part
   @return returns the Low-Byte from the given integer value
*/
byte getLoByte(unsigned int value) {
  return value % 128;
}


/**
   Gets the High-byte from an integer value for sending to the BCR.

   @param integer-value for extracting the High-byte-part
   @return returns the High-Byte from the given integer value
*/
byte getHiByte(unsigned int value) {
  return value / 128;
}


/**
   Builds and sends a sysex-message to Axe-FX for setting Axe-FX-parameter. You only have to
   set Block- and FX-ID. Value to set will be the value will be the  the BCR in two Bytes.
   The routine will convert it to the Axe-FX-Value.

   @param BLOCK_ID Block-ID of the effect-block to set
   @param PARAMETER_ID ID of the FX-Parameter of the Block.
   @param value Value to set for the parameter to set.
*/
void setAxeFxParam(byte *BLOCK_ID, byte *PARAMETER_ID, unsigned int value) {
  memcpy(sysexSendDataParam + 6, BLOCK_ID, 2);
  memcpy(sysexSendDataParam + 8, PARAMETER_ID, 2);
  sysexSendDataParam[13] = 0x01;  // SET-Parameter Function
  // Set-Value
  sysexSendDataParam[10] = value & 0x7F;
  sysexSendDataParam[11] = (value >> 7) & 0x7F;
  sysexSendDataParam[12] = (value >> 14) & 0x7F;

  getCheckSum(sysexSendDataParam, sizeof(sysexSendDataParam));
  MIDI.sendSysEx(16, sysexSendDataParam, false);
}


/**
   Builds and sends a sysex-message to Axe-FX for getting Axe-FX-parameter. You only have to set Block- and FX-ID
   that you want to read out. The sysex-message will be sent to the Axe-FX and it returns another sysex-message with
   all parameter-values.

   @param BLOCK_ID Block-ID of the effect-block to get
   @param PARAMETER_ID ID of the FX-Parameter of the Block.
*/
void getAxeFxParam(byte *BLOCK_ID, byte *PARAMETER_ID) {
  memcpy(sysexSendDataParam + 6, BLOCK_ID, 2);
  memcpy(sysexSendDataParam + 8, PARAMETER_ID, 2);
  sysexSendDataParam[13] = 0x00;  // GET-Parameter Function
  sysexSendDataParam[10] = 0x00;
  sysexSendDataParam[11] = 0x00;
  sysexSendDataParam[12] = 0x00;
  getCheckSum(sysexSendDataParam, sizeof(sysexSendDataParam));
  MIDI.sendSysEx(16, sysexSendDataParam, false);
}


/**
   Calculates checksum that the Axe-FX needs to accept a sysex-message. The function
   returns the calculated checksum and builds it into the given sysex-data.

   @param data sysex-data for checksum-calculation
   @param dataLength length of the given sysex-data
   @return returns the calculated checksum
*/
int getCheckSum(byte *data, int dataLength) {
  int i;
  int XOR;
  int c;

  for (XOR = 0, i = 0; i < dataLength - 2; i++) {
    c = (unsigned char)data[i];
    XOR ^= c;
  }
  XOR &= 0x7F;
  data[dataLength - 2] = XOR;

  return XOR;
}


/**
   Extracts the BLOCK- and FX-ID out of a given sysex-data. The function
   searches in CONFIG-variable to find the fitting entry and returns the
   BCR-MIDI-controller number.

   @param rcvSysex sysex-data to analyze and to extract BLOCK- and FX-ID
   @return if it found a fitting entry in CONFIG it returns the defined CC-number
*/
byte getControllerNumber(byte *rcvSysex) {
  for (int i = 0; i < sizeof(CONFIG) / sizeof(CONFIG[0]); i++) {
    if (*(CONFIG[i][1]) == rcvSysex[6] &&
        *(CONFIG[i][1] + 1) == rcvSysex[7] &&
        *(CONFIG[i][2]) == rcvSysex[8] &&
        *(CONFIG[i][2] + 1) == rcvSysex[9]) {
      return *(CONFIG[i][0]);
    }
  }
}


/**
   Extracts the BLOCK- and FX-ID out of a given sysex-data. The function
   searches in CONFIG-variable to find the fitting entry and returns true
   if the entry is defined as switch.

   @param rcvSysex sysex-data to analyze and to extract BLOCK- and FX-ID
   @return if it found a fitting entry in CONFIG it returns true if it is defined
           as switch, otherwise false.
*/
boolean isSwitch(byte *rcvSysex) {
  for (int i = 0; i < sizeof(CONFIG) / sizeof(CONFIG[0]); i++) {
    if (*(CONFIG[i][1]) == rcvSysex[6] &&
        *(CONFIG[i][1] + 1) == rcvSysex[7] &&
        *(CONFIG[i][2]) == rcvSysex[8] &&
        *(CONFIG[i][2] + 1) == rcvSysex[9]) {
      return *(CONFIG[i][3]) == SWITCH[0];
    }
  }
  return false;
}


/**
   Logs out a given sysex-data to the serial-monitor of the arduino.
   We use it for debugging some parts of the code.

   @param sysexData sysex-data you want to print out
   @param len length of the given sysex-data

*/
void logOutSysex(byte *sysexData, int len) {
  Serial.println("");
  for (int i = 0; i < len; i++) {
    Serial.print(sysexData[i], HEX);
    Serial.print("  ");
  }
  Serial.println("");
}

