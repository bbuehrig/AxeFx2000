/**
     Axe-FX-BCR-2000-Control-Bridge
     Name: config.h
     Purpose: This header-file defines your config. Here you are defining which parameter
              will change if you move a BCR2000-encoder. All possible Axe-FX effect-blocks and
              parameter are defined in constants.h. Maybe you have to add them in the header-file.

     @author Bastian Bührig
     @version 0.1 02/23/18
**/


// This is the CONFIG-array. Here you define what parameter will change
// if you move an encoder on the BCR2000. If you like to modify this config
// you have to add or replace a line in this array.
//
// One line has the following structure:
// [BCR-ELEMENT], [AXE-FX-BLOCK], [AXE-FX-PARAMETER], [SWITCH/ENCODER]
//
//
// Maybe not all BLOCK- or PARAMETER-IDs are defined in constants.h. Maybe you
// have to add it there!
static byte *CONFIG[][4] = {
  
  // AMP1
  { ENCODER1_GROUP1,    AMP1,      AMP_PARAM_GAIN,        ENCODER },
  { ENCODER2_GROUP1,    AMP1,      AMP_PARAM_DRIVE,       ENCODER },
  { ENCODER3_GROUP1,    AMP1,      AMP_PARAM_BASS,        ENCODER },
  { ENCODER4_GROUP1,    AMP1,      AMP_PARAM_MID,         ENCODER },
  { ENCODER5_GROUP1,    AMP1,      AMP_PARAM_TREBLE,      ENCODER },
  { ENCODER6_GROUP1,    AMP1,      AMP_PARAM_PRESENCE,    ENCODER },
  { ENCODER7_GROUP1,    AMP1,      AMP_PARAM_MASTER,      ENCODER },
  { ENCODER8_GROUP1,    AMP1,      AMP_PARAM_LEVEL,       ENCODER },

  { BUTTON1_ROW1,       AMP1,      AMP_PARAM_SW_BOOST,    SWITCH },
  { BUTTON2_ROW1,       AMP1,      AMP_PARAM_SW_CUT,      SWITCH },
  { BUTTON3_ROW1,       AMP1,      AMP_PARAM_SW_FAT,      SWITCH },
  { BUTTON4_ROW1,       AMP1,      AMP_PARAM_SW_BRIGHT,   SWITCH },


  // AMP2
  { ENCODER1_GROUP2,    AMP2,      AMP_PARAM_GAIN,        ENCODER },
  { ENCODER2_GROUP2,    AMP2,      AMP_PARAM_DRIVE,       ENCODER },
  { ENCODER3_GROUP2,    AMP2,      AMP_PARAM_BASS,        ENCODER },
  { ENCODER4_GROUP2,    AMP2,      AMP_PARAM_MID,         ENCODER },
  { ENCODER5_GROUP2,    AMP2,      AMP_PARAM_TREBLE,      ENCODER },
  { ENCODER6_GROUP2,    AMP2,      AMP_PARAM_PRESENCE,    ENCODER },
  { ENCODER7_GROUP2,    AMP2,      AMP_PARAM_MASTER,      ENCODER },
  { ENCODER8_GROUP2,    AMP2,      AMP_PARAM_LEVEL,       ENCODER },

  { BUTTON1_ROW2,       AMP2,      AMP_PARAM_SW_BOOST,    SWITCH },
  { BUTTON2_ROW2,       AMP2,      AMP_PARAM_SW_CUT,      SWITCH },
  { BUTTON3_ROW2,       AMP2,      AMP_PARAM_SW_FAT,      SWITCH },
  { BUTTON4_ROW2,       AMP2,      AMP_PARAM_SW_BRIGHT,   SWITCH },
};


// MIDI-config: Please change this numbers for your MIDI-environment! Channel 1 is the standard
// Axe-FX-Channel. If you didn't change the settings on your Axe-FX you don't have to change the
// number here!
// BCR-Channel number is the sending and receiving MIDI-channel for encoder movements. If you
// use the given BCR-Layout (for BC-Manager) you don't need to change anything!
static const int CHANNEL_AXEFX = 1;             // Channel Number of the AxeFX II
static const int CHANNEL_BCR = 16;              // Channel Number for CCs configured at the BCR

