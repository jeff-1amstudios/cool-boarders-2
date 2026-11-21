#ifndef CB2_BP_H
#define CB2_BP_H

#include "main.h"

typedef struct BpPlayer {
    unsigned char unknown_0000[0x1F4];
    void *buffer_1f4;
    void *buffer_1f8;
    void *buffer_1fc;
    unsigned char unknown_0200[0x470C];
    void *effects;
    unsigned char unknown_4910[12];
} BpPlayer;

int func_8005B4A4(void);

void func_8005AEA8(FileLoadState *state);
void func_8005B720(void);

typedef struct BpViewState {
    short state;
    short unknown_02;
    short previous_state;
    short unknown_06;
    short timer;
    unsigned char unknown_0a[0x19A];
    void *data;
} BpViewState;

typedef struct BpMotion {
    int unknown_00;
    int x, y, z;
    unsigned char unknown_10[4];
    short mode;
    unsigned char unknown_16[0x42];
    unsigned int buttons;
    int camera_x, camera_y, camera_z;
    unsigned char unknown_68[0x48];
    unsigned int previous_buttons;
    int previous_x, previous_y, previous_z;
} BpMotion;

typedef struct BpRenderEntry {
    unsigned char unknown_00[12];
    short related[3];
    unsigned char unknown_12[38];
} BpRenderEntry;

typedef struct BpRenderView {
    unsigned char unknown_000[0x188];
    GsCOORDINATE2 coordinate;
    unsigned char unknown_1d8[12];
    GsDOBJ3 *objects;
    int object_count;
    unsigned char unknown_1ec[0xA60];
    short forward_order;
} BpRenderView;

typedef struct BpSetupValues {
    unsigned int word_00, word_04, word_08;
} BpSetupValues;

typedef struct BpSetupLink {
    unsigned char unknown_00[0x1C];
    int value;
    unsigned char unknown_20[0x28];
    int *progress;
} BpSetupLink;

typedef struct BpSetupSource {
    unsigned char unknown_0000[0x70];
    BpSetupValues values;
    unsigned int word_0c;
    unsigned char unknown_0080[0x1200];
    BpSetupLink *link;
    short value;
} BpSetupSource;

typedef struct BpSetupState {
    BpSetupValues values;
    unsigned int word_0c;
    unsigned char unknown_0010[0x4DC];
    short value;
    unsigned char unknown_04ee[2];
    BpSetupLink *link;
} BpSetupState;

typedef struct BpMotionSample {
    int x, y, z;
    SVECTOR rotation;
    int active;
} BpMotionSample;

typedef struct BpMotionState {
    unsigned char unknown_000[0x84];
    short height;
    short unknown_086;
    int history_a[16];
    int history_b[16];
    short angles[4];
    short *angle_buffer;
    short mode;
    short unknown_116;
    int value;
    int negative_height;
    int timer;
    int word_124;
    int word_128;
    BpMotionSample samples[4];
    int selected;
    int word_190;
    int word_194;
    int word_198;
    int word_19c;
    int word_1a0;
} BpMotionState;

typedef struct BpMotionTransform {
    unsigned char unknown_000[0x2C];
    SVECTOR rotation;
    GsCOORDINATE2 coordinate;
} BpMotionTransform;

typedef struct BpMotionPlayer {
    unsigned char unknown_000[0x2C];
    int position[3];
    unsigned char unknown_038[4];
    short height;
    short airborne;
    unsigned char unknown_040[0x19A];
    short heading;
    unsigned char unknown_1dc[0x4FC];
    BpMotionState *motion;
    unsigned char unknown_6dc[0x28];
    int progress;
    unsigned char unknown_708[0x544];
    short reverse;
    unsigned char unknown_c4e[0x3C96];
    int vertical_speed;
} BpMotionPlayer;

typedef struct BpMotionExtended {
    BpMotionState motion;
    int saved_height;
} BpMotionExtended;

typedef struct BpCameraState {
    unsigned char unknown_000[0xC];
    GsRVIEW2 view;
    unsigned char unknown_02c[0x17C];
    int position[3];
} BpCameraState;

typedef struct BpPoseInput {
    unsigned char unknown_00[0x64];
    short amount;
} BpPoseInput;

typedef struct BpPosePlayer {
    BpMotionPlayer player;
    unsigned char unknown_48e8[0x30];
    BpPoseInput *pose;
} BpPosePlayer;

typedef struct BpPoseState {
    BpMotionTransform transform;
    short height;
    unsigned char unknown_086[0x11E];
    int saved_height;
} BpPoseState;

typedef struct BpSelectionView {
    unsigned char unknown_00[20];
    short course;
    short record;
    int mode;
} BpSelectionView;

typedef struct BpSurfaceState {
    int kind;
    int x, y, z;
    int word_10;
    short surface;
    short mode;
    int word_18;
    SVECTOR offset;
    unsigned char unknown_24[0x18];
    int normal[3];
    int word_48;
    int height;
    int word_50;
    int word_54;
} BpSurfaceState;

typedef struct BpModelHeader {
    unsigned int unknown_00;
    unsigned int unknown_04;
    unsigned int object_count;
} BpModelHeader;

typedef struct BpCoordinateHeader {
    unsigned int unknown_00;
    unsigned short coordinate_count;
} BpCoordinateHeader;

typedef struct BpLightColor {
    unsigned char r, g, b;
    unsigned char ambient;
} BpLightColor;

typedef struct BpLightingTable {
    BpLightColor colors[16][3];
} BpLightingTable;

typedef struct BpStartOffsets {
    int course[16][3];
} BpStartOffsets;

typedef struct BpStartPlayer {
    unsigned char unknown_00[0x28];
    BpSurfaceState surface;
    BpSurfaceState initial_surface;
} BpStartPlayer;

void func_800633AC(BpStartPlayer *player, int course, int side);

typedef struct BpRiderProfile {
    unsigned char bytes[16];
} BpRiderProfile;

typedef struct BpRiderTuning {
    int index;
    unsigned char unknown_04[0x40];
    int word_44;
    int word_48;
    unsigned char unknown_4c[4];
} BpRiderTuning;

typedef struct BpRider {
    short active;
    unsigned char flags;
    unsigned char character;
    BpRiderTuning tuning;
    BpRiderProfile *profile;
    short variant;
    unsigned char alternate;
    unsigned char unknown_5b;
    short course_value;
    short course_flag;
    short interval;
    short unknown_62;
    short option;
    short unknown_66;
    int scaled_value;
    int unknown_6c;
    BpSetupValues values;
    unsigned int word_7c;
    unsigned char unknown_0080[0x1200];
    BpSetupLink *link;
    short value;
} BpRider;

typedef struct BpRiderCourseValues {
    short values[16];
} BpRiderCourseValues;

typedef struct BpRiderBaseValues {
    int values[2][10][8];
} BpRiderBaseValues;

typedef struct BpRiderCourseFlags {
    short values[2][10][8];
} BpRiderCourseFlags;

typedef struct BpRiderCourseScales {
    short values[10];
} BpRiderCourseScales;

typedef struct BpSetupInputs {
    void *first;
    void *second;
    void *third;
} BpSetupInputs;

void func_80063510(BpRider *rider, int player, int character, int tuning, int option,
                   void *first, void *second, void *third);

typedef struct BpResetEntry {
    int word_00;
    int word_04;
    short value_08;
    short value_0a;
} BpResetEntry;

typedef struct BpPlayerReset {
    unsigned char unknown_0000[0x1A];
    short value_001a;
    short value_001c;
    unsigned char unknown_001e[2];
    int word_0020;
    int word_0024;
    unsigned char unknown_0028[0x1B0];
    SVECTOR rotation;
    int distance;
    unsigned char unknown_01e4[0x550];
    int word_0734;
    int word_0738;
    int word_073c;
    short value_0740;
    short value_0742;
    int word_0744;
    int word_0748;
    int word_074c;
    int word_0750;
    int word_0754;
    int word_0758;
    short value_075c;
    short value_075e;
    short value_0760;
    short value_0762;
    unsigned char unknown_0764[4];
    short value_0768;
    short value_076a;
    short value_076c;
    short value_076e;
    short value_0770;
    short value_0772;
    BpResetEntry entries[2][2];
    int word_07a4;
    int word_07a8;
    unsigned char unknown_07ac[0x4FC];
    short value_0ca8;
    short value_0caa;
    short value_0cac;
    unsigned char unknown_0cae[0x3C22];
    VECTOR vector_48d0;
    VECTOR vector_48e0;
    int word_48f0;
    int word_48f4;
    int word_48f8;
    short value_48fc;
    short value_48fe;
    short value_4900;
    short value_4902;
    short value_4904;
    short value_4906;
    short value_4908;
    unsigned char byte_490a;
    unsigned char byte_490b;
} BpPlayerReset;

void func_8006391C(BpPlayer *player, int reverse);

typedef struct BpAnimationFrame {
    unsigned short word_00;
    unsigned short command_count;
    unsigned int start;
} BpAnimationFrame;

typedef struct BpAnimationHeader {
    unsigned int flags;
    int frame_count;
    BpAnimationFrame *first_frame;
} BpAnimationHeader;

typedef union BpAnimationSlot {
    BpAnimationHeader *header;
    BpAnimationFrame **frames;
} BpAnimationSlot;

typedef struct BpAnimationState {
    GsDOBJ3 *objects;
    int active_count;
    int object_count;
    unsigned long *pmd;
    GsDOBJ3 *object_buffer;
    GsCOORDINATE2 *coordinates;
    GsCOORD2PARAM *parameters;
    BpAnimationSlot animations[101];
    const int *coordinate_indices;
    int frame_counts[101];
    unsigned int first_frame_starts[101];
    int word_4dc;
    int word_4e0;
    int word_4e4;
    int word_4e8;
} BpAnimationState;

typedef struct BpAnimationPlayback {
    BpAnimationState animation;
    short saved_y;
    GsCOORDINATE2 *root_coordinate;
} BpAnimationPlayback;

typedef struct BpCourseSoundBanks {
    short banks[7];
} BpCourseSoundBanks;

typedef struct BpModeSoundBanks {
    short banks[16];
} BpModeSoundBanks;

void func_8006474C(void);

void func_80064944(BpAnimationPlayback *playback, int animation, int mode, int end_frame);

typedef struct BpPlayerCoordinates {
    unsigned char unknown_0000[0x188];
    GsCOORDINATE2 body_coordinate;
    SVECTOR body_rotation;
    unsigned char unknown_01e0[0xA78];
    GsCOORDINATE2 secondary_coordinate;
    short secondary_pitch;
    short unknown_0caa;
    short secondary_heading;
} BpPlayerCoordinates;

void func_80068770(BpPlayerCoordinates *player);

typedef struct BpStartLocation {
    BpRenderEntry *entry;
    int x, y, z;
    unsigned char unknown_10[4];
    short related[3];
} BpStartLocation;

int func_8006889C(BpStartPlayer *player, int start, short x, short y, short z);

typedef struct BpScoreLimit {
    unsigned char unknown_00[0x20];
    int maximum;
} BpScoreLimit;

typedef struct BpScoreMotion {
    int unknown_00;
    int reduced_score;
    unsigned char unknown_08[0xC];
    int heading_active;
    int second_rotation;
    int first_rotation;
    int unknown_20;
    int move_duration;
} BpScoreMotion;

typedef struct BpScoreMove {
    unsigned char unknown_00[6];
    short id;
    unsigned char unknown_08[0x40];
} BpScoreMove;

typedef struct BpScoreSelection {
    short fallback;
    short override;
} BpScoreSelection;

typedef struct BpScorePlayer {
    unsigned char unknown_0000[0x2C];
    int position[3];
    unsigned char unknown_0038[0x1A0];
    SVECTOR rotation;
    unsigned char unknown_01e0[0x4FC];
    BpScoreLimit limit;
    unsigned char unknown_0700[0x34];
    BpScoreMotion motion;
    unsigned char unknown_075c[0x12C];
    BpScoreMove moves[13];
    unsigned char unknown_0c30[0xA];
    BpScoreSelection selections[2];
    unsigned char unknown_0c42[0x3C9E];
    VECTOR velocity;
} BpScorePlayer;

typedef struct BpScore {
    unsigned char height_bonus;
    unsigned char reverse;
    unsigned char reverse_bonus;
    unsigned char duration_bonus;
    unsigned char move_bonus[2];
    unsigned char heading_bonus;
    unsigned char unknown_07;
    unsigned char unknown_08[2];
    short rotation_bonus[2];
    short combined_bonus;
    unsigned short distance_bonus;
    unsigned char unknown_12[2];
    int total;
    int height;
    short move_index[2];
    short rotations[2];
    short combined_flag;
    short unknown_26;
    int origin_x;
    int origin_z;
} BpScore;

int func_80068BA0(BpScorePlayer *player, BpScore *score);

typedef struct BpMoveInput {
    unsigned char timer;
    unsigned char timeout;
    unsigned char step;
    unsigned char step_count;
    short state;
    short id;
    unsigned int masks[16];
} BpMoveInput;

typedef struct BpMovePlayer {
    unsigned char unknown_000[0x28];
    unsigned int input;
    unsigned char unknown_02c[0x12];
    short airborne;
    unsigned char unknown_040[0x40];
    unsigned int previous_input;
    unsigned char unknown_084[0x804];
    BpMoveInput moves[13];
} BpMovePlayer;

typedef struct BpMoveAlias {
    short id;
    short alias;
} BpMoveAlias;

int func_80069028(BpMovePlayer *player);

int func_800694E0(BpResetEntry *entry, int amount, unsigned int positive_input,
                  unsigned int negative_input, unsigned int held);

void func_80069614(BpMotionPlayer *player);

void func_80069648(int mode);

typedef struct BpDistancePlayer {
    unsigned char unknown_00[0x84];
    int x;
    int y;
    int z;
    unsigned char unknown_90[0x4C];
    int previous_x;
    int previous_y;
    int previous_z;
} BpDistancePlayer;

int func_8006978C(GsOT *ot, BpDistancePlayer *player, int draw, int x, int y);

int func_800699B4(GsOT *ot, int move_left, int position, int x, int y);

void func_8006ACC0(GsOT *ot, int *counter, int different, int value, int direction);

#endif
