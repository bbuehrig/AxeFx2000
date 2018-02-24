/**
     Axe-FX-BCR-2000-Control-Bridge
     Name: constants.h
     Purpose: This header-file defines some constants for using in the Axe-FX-BCR-2000-Control-Bridge
              main-program. Here you have to define the used MIDI SysEx blocks and parameters IDs for
              the Axe-FX. The ID-list is not complete. You have to add the IDs you like to use!
              See here for more informations on the IDs:
              https://wiki.fractalaudio.com/axefx2/index.php?title=MIDI_SysEx_blocks_and_parameters_IDs#AMP

     @author Bastian Bührig
     @version 0.1 02/23/18
**/

// Axe-FX SYSEX-Constants
static byte AMP1[]                 = { 0x6A, 0x00 };
static byte AMP2[]                 = { 0x6B, 0x00 };

static byte AMP_PARAM_GAIN[]       = { 0x01, 0x00 };
static byte AMP_PARAM_DRIVE[]      = { 0x4a, 0x00 };
static byte AMP_PARAM_TRIM[]       = { 0x2f, 0x00 };
static byte AMP_PARAM_BASS[]       = { 0x02, 0x00 };
static byte AMP_PARAM_MID[]        = { 0x03, 0x00 };
static byte AMP_PARAM_TREBLE[]     = { 0x04, 0x00 };
static byte AMP_PARAM_PRESENCE[]   = { 0x14, 0x00 };
static byte AMP_PARAM_BRIGHT[]     = { 0x61, 0x00 };
static byte AMP_PARAM_MASTER[]     = { 0x05, 0x00 };
static byte AMP_PARAM_LEVEL[]      = { 0x15, 0x00 };

static byte AMP_PARAM_SW_BOOST[]   = { 0x28, 0x00 };
static byte AMP_PARAM_SW_CUT[]     = { 0x2D, 0x00 };
static byte AMP_PARAM_SW_FAT[]     = { 0x4E, 0x00 };
static byte AMP_PARAM_SW_BRIGHT[]  = { 0x27, 0x00 };


// Controller defined in BCR2000
static byte ENCODER1_GROUP1[]        = {0};
static byte ENCODER2_GROUP1[]        = {1};
static byte ENCODER3_GROUP1[]        = {2};
static byte ENCODER4_GROUP1[]        = {3};
static byte ENCODER5_GROUP1[]        = {4};
static byte ENCODER6_GROUP1[]        = {5};
static byte ENCODER7_GROUP1[]        = {6};
static byte ENCODER8_GROUP1[]        = {7};

static byte ENCODER1_GROUP2[]        = {8};
static byte ENCODER2_GROUP2[]        = {9};
static byte ENCODER3_GROUP2[]        = {10};
static byte ENCODER4_GROUP2[]        = {11};
static byte ENCODER5_GROUP2[]        = {12};
static byte ENCODER6_GROUP2[]        = {13};
static byte ENCODER7_GROUP2[]        = {14};
static byte ENCODER8_GROUP2[]        = {15};

static byte ENCODER1_ROW1[]          = {16};
static byte ENCODER2_ROW1[]          = {17};
static byte ENCODER3_ROW1[]          = {18};
static byte ENCODER4_ROW1[]          = {19};
static byte ENCODER5_ROW1[]          = {20};
static byte ENCODER6_ROW1[]          = {21};
static byte ENCODER7_ROW1[]          = {22};
static byte ENCODER8_ROW1[]          = {23};

static byte ENCODER1_ROW2[]          = {24};
static byte ENCODER2_ROW2[]          = {25};
static byte ENCODER3_ROW2[]          = {26};
static byte ENCODER4_ROW2[]          = {27};
static byte ENCODER5_ROW2[]          = {28};
static byte ENCODER6_ROW2[]          = {29};
static byte ENCODER7_ROW2[]          = {30};
static byte ENCODER8_ROW2[]          = {31};

static byte ENCODER1_ROW3[]          = {64};
static byte ENCODER2_ROW3[]          = {65};
static byte ENCODER3_ROW3[]          = {66};
static byte ENCODER4_ROW3[]          = {67};
static byte ENCODER5_ROW3[]          = {68};
static byte ENCODER6_ROW3[]          = {69};
static byte ENCODER7_ROW3[]          = {70};
static byte ENCODER8_ROW3[]          = {71};

static byte BUTTON1_ROW1[]           = {72};
static byte BUTTON2_ROW1[]           = {73};
static byte BUTTON3_ROW1[]           = {74};
static byte BUTTON4_ROW1[]           = {75};
static byte BUTTON5_ROW1[]           = {76};
static byte BUTTON6_ROW1[]           = {77};
static byte BUTTON7_ROW1[]           = {78};
static byte BUTTON8_ROW1[]           = {79};

static byte BUTTON1_ROW2[]           = {80};
static byte BUTTON2_ROW2[]           = {81};
static byte BUTTON3_ROW2[]           = {82};
static byte BUTTON4_ROW2[]           = {83};
static byte BUTTON5_ROW2[]           = {84};
static byte BUTTON6_ROW2[]           = {85};
static byte BUTTON7_ROW2[]           = {86};

static byte BUTTON_TOP_LEFT[]        = {127};
static byte BUTTON_TOP_RIGHT[]       = {126};
static byte BUTTON_BOTTOM_LEFT[]     = {125};
static byte BUTTON_BOTTOM_RIGHT[]    = {124};


// System-Constants
static const byte SWITCH_ON = 0x7F;
static const byte SWITCH_OFF = 0x00;


// Type of Controll-Element
static byte SWITCH[]        = { 0x01 };
static byte ENCODER[]       = { 0x00 };
