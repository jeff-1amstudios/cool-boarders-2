#ifndef CB2_COMB_H
#define CB2_COMB_H

#include "main.h"

void func_8005AEA4(FileLoadState *state);

typedef struct CombPlayer {
    unsigned char unknown_0000[0x1F4];
    void *buffer_1f4;
    void *buffer_1f8;
    void *buffer_1fc;
    unsigned char unknown_0200[0x4714];
    void *effects;
    unsigned char unknown_4918[12];
} CombPlayer;

typedef struct CombSettings {
    short character;
    unsigned char unknown_02[8];
    short second_character;
    unsigned char unknown_0c[8];
    short course;
    unsigned char unknown_16[6];
    short option;
} CombSettings;

int func_8005B568(void);

void func_8005B8D0(void);

void func_8005BB00(GsOT *ot, unsigned char shade);

void func_8005D774(int first, int second, int mode);

int func_8005DEE4(int mode, int value);

typedef struct CombSceneEntry {
    unsigned char unknown_00[12];
    short linked_entries[3];
    unsigned char unknown_12[38];
} CombSceneEntry;

void func_8005DF88(GsOT *ot, void *view, void *position, int mode, int entry);

typedef struct CombRenderView {
    unsigned char unknown_000[0x188];
    GsCOORDINATE2 coordinate;
    unsigned char unknown_1d8[12];
    GsDOBJ3 *objects;
    int object_count;
    unsigned char unknown_1ec[0xA68];
    short forward_order;
} CombRenderView;

void func_8005E7A4(GsOT *ot, CombPlayer *player, int index);

void func_8005E9E0(GsOT *ot, int shift, int kind, int entry);

typedef struct CombPlayerEffectView {
    unsigned char unknown_00[0x2C];
    VECTOR position;
    short entry;
} CombPlayerEffectView;

void func_8005EB18(GsOT *ot, CombPlayer *player);

int func_8005EC58(VECTOR *origin, MATRIX *matrix, VECTOR *normal);

typedef struct CombPlayerHeadingView {
    unsigned char unknown_0000[0x96];
    short state_96;
    unsigned char unknown_0098[0xB0];
    int state_148;
    unsigned char unknown_014c[0x8E];
    short heading;
    unsigned char unknown_01dc[0x4FC];
    unsigned short *surface;
    unsigned char unknown_06dc[0x48];
    int value_724;
    unsigned char unknown_0728[6];
    short mode;
    unsigned char unknown_0730[0x41B8];
    long motion_x;
    unsigned char unknown_48ec[4];
    long motion_z;
} CombPlayerHeadingView;

void func_8005EDC4(CombPlayerHeadingView *view, int value);

typedef struct CombHudEntry {
    unsigned char unknown[0x34];
} CombHudEntry;

typedef struct CombHudState {
    int unknown_00;
    int value_04;
    int value_08;
    int value_0c;
    int value_10;
    int value_14;
    int unknown_18;
    int value_1c;
    int value_20;
    unsigned char unknown_24[0x18];
    int value_3c;
    int unknown_40;
    int value_44;
    int value_48;
    int value_4c;
    int value_50;
    int countdown;
    int value_58;
    int value_5c;
} CombHudState;

typedef struct CombMovementHudView {
    unsigned char unknown_00[0x1C];
    int angular_speed;
    unsigned char unknown_20[0x18];
    unsigned short speed;
} CombMovementHudView;

typedef struct CombPlayerHudView {
    unsigned char unknown_0000[0x3C];
    short state;
    unsigned char unknown_003e[0x69A];
    short *surface;
    unsigned char unknown_06dc[0x28];
    int progress;
    unsigned char unknown_0708[0x2C];
    CombMovementHudView movement;
    unsigned char unknown_0770[0x4A];
    short display_position;
    unsigned char unknown_07bc[4];
    short display_side;
    unsigned char unknown_07c2[6];
    CombHudState hud;
    unsigned char unknown_0828[0x28];
    CombHudEntry *entries;
    unsigned char unknown_0854[0x36];
    short entry_count;
    unsigned char unknown_088c[0x42A];
    short value_cb6;
    unsigned char unknown_0cb8[0x1E];
    short value_cd6;
    unsigned char unknown_0cd8[0x138];
    unsigned char display_e10[0x3AE8];
    int previous_progress;
    unsigned char unknown_48fc[0x20];
    short value_491c;
} CombPlayerHudView;

void func_8005EEE0(GsOT *ot, CombPlayerHudView *player, int first, int second);

typedef struct CombRaceStateView {
    unsigned char unknown_00[0x5C];
    short value_5c;
} CombRaceStateView;

typedef struct CombPlayerRaceHudView {
    CombPlayerHudView player;
    CombRaceStateView *race;
} CombPlayerRaceHudView;

typedef struct CombPlayerRenderStateView {
    unsigned char unknown_000[0x1E0];
    int result;
    unsigned char unknown_1e4[0x4F4];
    short *surface;
} CombPlayerRenderStateView;

void func_8005FB48(int mode, unsigned int mask);

int func_8005FC1C(int course, int value, int difference, int penalty);

typedef struct CombSetupValues {
    unsigned int word_00, word_04, word_08;
} CombSetupValues;

typedef struct CombSetupLink {
    unsigned char unknown_00[0x1C];
    int value;
    unsigned char unknown_20[0x28];
    int *progress;
} CombSetupLink;

typedef struct CombSetupSource {
    unsigned char unknown_0000[0x70];
    CombSetupValues values;
    unsigned int word_0c;
    unsigned char unknown_0080[0x1200];
    CombSetupLink *link;
    short value;
} CombSetupSource;

typedef struct CombSetupState {
    CombSetupValues values;
    unsigned int word_0c;
    unsigned char unknown_0010[0x4DC];
    short value;
    unsigned char unknown_04ee[2];
    CombSetupLink *link;
} CombSetupState;

void func_8006009C(CombSetupState *state, CombSetupSource *source, int *progress);

typedef struct CombPlayerCheckpointView {
    unsigned char unknown_0000[0x7E0];
    int checkpoint_reset;
    int comparison_state;
    int time_difference;
    unsigned char unknown_07ec[0x4CA];
    short checkpoint;
    short previous_checkpoint;
    unsigned char unknown_0cba[2];
    int checkpoint_value;
    unsigned char unknown_0cc0[0x12];
    short finished_value;
    unsigned char unknown_0cd4[0x13C];
    int checkpoint_times[4];
    unsigned char unknown_0e20[0x3AFC];
    short side;
} CombPlayerCheckpointView;

unsigned int func_800600E8(unsigned int value, int time);

int func_800605A8(void);

int func_80060660(CombPlayerCheckpointView *player);

void func_800606F0(void);

void func_80060728(void);

typedef struct CombMotionSample {
    int x, y, z;
    SVECTOR rotation;
    int active;
} CombMotionSample;

typedef struct CombMotionState {
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
    CombMotionSample samples[4];
    int selected;
    int word_190;
    int word_194;
    int word_198;
    int word_19c;
    int word_1a0;
} CombMotionState;

void func_800607B4(CombMotionState *state, short mode, int value, int x, int y, int z);

void func_80060BD8(CombMotionState *state, int *position);

int func_80060C24(CombMotionState *state, int sample, int target);

typedef struct CombMotionTransform {
    unsigned char unknown_000[0x2C];
    SVECTOR rotation;
    GsCOORDINATE2 coordinate;
} CombMotionTransform;

typedef struct CombMotionPlayer {
    unsigned char unknown_000[0x2C];
    int position[3];
    unsigned char unknown_038[4];
    short height;
    short airborne;
    unsigned char unknown_040[0x19A];
    short heading;
    unsigned char unknown_1dc[0x4FC];
    CombMotionState *motion;
    unsigned char unknown_6dc[0x28];
    int progress;
    unsigned char unknown_708[0x544];
    short reverse;
    unsigned char unknown_c4e[0x3C96];
    int vertical_speed;
} CombMotionPlayer;

void func_80061A64(CombMotionPlayer *player, int *position);

void func_80061AB0(CombMotionPlayer *player, int *position);

typedef struct CombCameraState {
    unsigned char unknown_000[0xC];
    GsRVIEW2 view;
    unsigned char unknown_02c[0x17C];
    int position[3];
} CombCameraState;

void func_80061AFC(CombMotionPlayer *player, int *position);

typedef struct CombPoseInput {
    unsigned char unknown_00[0x64];
    short amount;
} CombPoseInput;

typedef struct CombPosePlayer {
    CombMotionPlayer player;
    unsigned char unknown_48e8[0x38];
    CombPoseInput *pose;
} CombPosePlayer;

typedef struct CombPoseState {
    CombMotionTransform transform;
    short height;
    unsigned char unknown_086[0x11E];
    int saved_height;
} CombPoseState;

void func_80061C20(CombPosePlayer *player);

typedef struct CombSurfaceState {
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
} CombSurfaceState;

typedef struct CombSurfaceMotionPlayer {
    unsigned char unknown_0000[0x80];
    CombSurfaceState surface;
    unsigned char unknown_00d8[0x58];
    CombSurfaceState alternate_surface;
    unsigned char unknown_0188[0xAD4];
    int slow;
    unsigned char unknown_0c60[0x3C78];
    int motion_x;
    int motion_y;
    int motion_z;
    unsigned char unknown_48e4[8];
    int vertical_speed;
} CombSurfaceMotionPlayer;

void func_800622D8(CombSurfaceMotionPlayer *player);

typedef struct CombSurfaceMotionView {
    short state;
    unsigned char unknown_002[6];
    short reset;
    unsigned char unknown_00a[0x10E];
    int heading;
} CombSurfaceMotionView;

typedef struct CombPlayerSurfaceMotionView {
    unsigned char unknown_000[0x1DA];
    short heading;
    unsigned char unknown_1dc[0x4FC];
    CombSurfaceMotionView *motion;
    unsigned char unknown_6dc[0x578];
    short reverse;
} CombPlayerSurfaceMotionView;

void func_800608D4(CombPlayerSurfaceMotionView *player);

typedef struct CombCameraModeView {
    short camera_mode;
    unsigned char unknown_002[8];
    unsigned short projection;
    GsRVIEW2 view;
    SVECTOR rotation;
    GsCOORDINATE2 camera_coordinate;
    short height;
    short unknown_086;
} CombCameraModeView;

typedef struct CombPlayerCameraView {
    int unknown_0000;
    int x;
    int y;
    int z;
    unsigned char unknown_0010[0x20];
    int target_y;
    unsigned char unknown_0034[0x6A4];
    CombCameraModeView *camera;
    unsigned char unknown_06dc[0x4242];
    signed char secondary_view;
} CombPlayerCameraView;

void func_80060940(CombPlayerCameraView *player, int mode);

typedef struct CombCameraControl {
    short mode;
    unsigned char unknown_002[6];
    short transition;
    unsigned char unknown_00a[2];
    GsRVIEW2 view;
    SVECTOR rotation;
    GsCOORDINATE2 coordinate;
    short height;
    short previous_height;
    unsigned char unknown_088[0x108];
    int word_190;
    unsigned char unknown_194[0x10];
    int saved_height;
} CombCameraControl;

typedef struct CombCameraPlayerView {
    unsigned char unknown_000[0x24];
    int lean;
    unsigned char unknown_028[0x14];
    short height;
    unsigned char unknown_03e[0x19A];
    SVECTOR rotation;
    unsigned char unknown_1e0[0x4E4];
    int state;
    unsigned char unknown_6c8[0x10];
    CombMotionState *motion;
    unsigned char unknown_6dc[0x64];
    short angle_a;
    short angle_b;
    unsigned char unknown_744[0x510];
    short direction;
    unsigned char unknown_c56[0x5A];
    short yaw;
    short camera_yaw;
    unsigned char unknown_cb4[0x3C34];
    VECTOR velocity;
    unsigned char unknown_48f8[0x28];
    CombPoseInput *pose;
} CombCameraPlayerView;

int func_80061E48(CombCameraPlayerView *player, int update);

typedef struct CombResultEntry {
    unsigned char unknown_00[0x14];
    int score;
    unsigned char unknown_18[0x1C];
} CombResultEntry;

typedef struct CombResultState {
    unsigned char unknown_00[0x58];
    short rank;
} CombResultState;

typedef struct CombPlayerResultView {
    unsigned char unknown_0000[0x850];
    CombResultEntry *entries;
    unsigned char unknown_0854[0x34];
    short entry_count;
    unsigned char unknown_088a[0x448];
    short finished;
    unsigned char unknown_0cd4[2];
    short penalty;
    unsigned char unknown_0cd8[0x138];
    int start_time;
    unsigned char unknown_0e14[8];
    int finish_time;
    unsigned char unknown_0e20[0x3AFC];
    short rank;
    unsigned char unknown_491e[2];
    CombResultState *result;
} CombPlayerResultView;

void func_80060328(CombPlayerResultView *first, CombPlayerResultView *second, int result);

typedef union CombResultWork {
    CombResultEntry *entry;
    int value;
} CombResultWork;

/* Detailed motion/player views used by func_80060E14. */
typedef struct CombPitchMotionState {
    unsigned char unknown_000[0x10];
    int translation_y;
    unsigned char unknown_014[0x18];
    SVECTOR rotation;
    int word_034;
    MATRIX matrix;
    unsigned char unknown_058[0x24];
    int word_07c;
    unsigned char unknown_080[4];
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
    CombMotionSample samples[4];
    int selected;
    int word_190;
    int word_194;
    int word_198;
    int word_19c;
    int word_1a0;
} CombPitchMotionState;

typedef struct CombMotionExtended {
    CombPitchMotionState motion;
    int saved_height;
} CombMotionExtended;

typedef struct CombPitchMotionPlayer {
    unsigned char unknown_000[0x2C];
    int position[3];
    unsigned char unknown_038[4];
    short height;
    short airborne;
    unsigned char unknown_040[0x19A];
    short heading;
    unsigned char unknown_1dc[0x4FC];
    CombMotionExtended *motion;
    unsigned char unknown_6dc[0x6C];
    int movement;
    unsigned char unknown_74c[0x508];
    short reverse;
    unsigned char unknown_c56[0x5A];
    short rotation_x;
    short unknown_cb2;
    short rotation_y;
} CombPitchMotionPlayer;

int func_80060E14(CombCameraPlayerView *player, int active, int frozen);

typedef union CombPitchWork {
    int value;
    unsigned int bits;
    short *cursor;
} CombPitchWork;

typedef struct CombMotionTransformView {
    unsigned char unknown_00[0x2C];
    SVECTOR rotation;
    int word_34;
    MATRIX matrix;
    unsigned char unknown_58[0x24];
    int word_7c;
} CombMotionTransformView;

typedef struct CombPlayerMotionView {
    unsigned char unknown_0000[0x2C];
    VECTOR position;
    short state;
    short airborne;
    unsigned char unknown_0040[0x19A];
    short heading;
    unsigned char unknown_01dc[0x4FC];
    CombMotionState *motion;
    unsigned char unknown_06dc[0x28];
    int progress;
    unsigned char unknown_0708[0x54C];
    short reverse;
    unsigned char unknown_0c56[0x3C96];
    int vertical_motion;
} CombPlayerMotionView;

void func_80061440(CombCameraPlayerView *player, int progress, int frozen);

typedef struct CombSpeedParameters {
    unsigned char unknown_00[0xE];
    short percent;
} CombSpeedParameters;

typedef struct CombSpeedPlayer {
    unsigned char unknown_0000[0x6FC];
    int speed_limit;
    unsigned char unknown_0700[0x34];
    int mode;
    int active;
    unsigned char unknown_073c[0x30];
    unsigned short timer;
    unsigned char unknown_076e[0x4CA];
    CombSpeedParameters *parameters;
    unsigned char unknown_0c3c[0x20];
    int slow;
    unsigned char unknown_0c60[0x3C78];
    int acceleration_x;
    unsigned char unknown_48dc[4];
    int acceleration_z;
    unsigned char unknown_48e4[4];
    int velocity_x;
    unsigned char unknown_48ec[4];
    int velocity_z;
    unsigned char unknown_48f4[8];
    int deceleration;
} CombSpeedPlayer;

void func_80062694(CombSpeedPlayer *player, int braking, int half_speed,
                   int strong, int timer_mode, int scale);

int func_80062978(CombPlayerHeadingView *player, int turn_step, int divisor);

typedef struct CombPlayerSlopeView {
    unsigned char unknown_000[0x28];
    CombSurfaceState surface;
    unsigned char unknown_080[0xB0];
    CombSurfaceState alternate_surface;
    unsigned char unknown_188[0x52];
    short heading;
    unsigned char unknown_1dc[0x4FC];
    CombMotionState *motion;
    unsigned char unknown_6dc[0x5E4];
    int history_index;
} CombPlayerSlopeView;

unsigned int func_80062B2C(CombPlayerSlopeView *player);

typedef struct CombImpulseParameters {
    int scale;
} CombImpulseParameters;

typedef struct CombPlayerImpulseView {
    unsigned char unknown_0000[0xC38];
    CombImpulseParameters *parameters;
    unsigned char unknown_0c3c[0x3CA0];
    int acceleration_y;
} CombPlayerImpulseView;

void func_80062C94(CombPlayerImpulseView *player, int alternate);

typedef struct CombPlayerDirectionalImpulseView {
    unsigned char unknown_0000[0xC54];
    short direction;
    unsigned char unknown_0c56[0x3C82];
    int acceleration_x;
    int acceleration_y;
    int acceleration_z;
} CombPlayerDirectionalImpulseView;

void func_80062CF8(CombPlayerDirectionalImpulseView *player, int mode);

void func_80062E8C(CombPlayerDirectionalImpulseView *player, int mode);

typedef struct CombSessionSettingsView {
    CombSettings settings;
    unsigned char unknown_1e[6];
    short first_flag;
    short second_flag;
} CombSessionSettingsView;

typedef struct CombSessionPlayerView {
    unsigned char unknown_0000[0xCBC];
    int progress;
    unsigned char unknown_0cc0[0x14];
    short duration;
    unsigned char unknown_0cd6[0x3C48];
    signed char secondary_view;
} CombSessionPlayerView;

typedef struct CombSceneCountView {
    unsigned char unknown_00[4];
    unsigned short count;
} CombSceneCountView;

int func_80063020(FileLoadState *files, int mode);

typedef struct CombPlayerNameView {
    unsigned char unknown_0000[0xCC4];
    unsigned char name[4];
} CombPlayerNameView;

void func_80063460(CombSettings *settings, int state, int update, int alternate);

void func_80063DDC(int course);

void func_8006406C(GsOT *ot, GsOT_TAG *tags, unsigned long length);

void func_80064094(CombPlayerCameraView *player);

typedef struct CombStartOffsets {
    int course[16][3];
} CombStartOffsets;

typedef struct CombStartPlayer {
    unsigned char unknown_00[0x28];
    CombSurfaceState surface;
    CombSurfaceState initial_surface;
} CombStartPlayer;

void func_80064418(CombStartPlayer *player, int course, int side);

typedef struct CombObjectCourseValues {
    short course[16];
} CombObjectCourseValues;

typedef struct CombObjectSpeeds {
    int values[2][10][8];
} CombObjectSpeeds;

typedef struct CombObjectLimits {
    short values[2][10][8];
} CombObjectLimits;

typedef struct CombObjectPercentages {
    short course[10];
} CombObjectPercentages;

typedef struct CombObjectTables {
    CombObjectCourseValues course;
    CombObjectSpeeds speeds;
    CombObjectLimits limits;
    CombObjectPercentages percentages;
} CombObjectTables;

typedef struct CombObjectParameters {
    int kind;
    unsigned char unknown_04[0x40];
    int value_44;
    int value_48;
    int unknown_4c;
} CombObjectParameters;

typedef struct CombObjectEntry {
    unsigned char unknown[16];
} CombObjectEntry;

typedef struct CombObjectState {
    short active;
    unsigned char flag;
    unsigned char index;
    CombObjectParameters parameters;
    CombObjectEntry *entry;
    short count;
    unsigned char reverse;
    unsigned char flag_5b;
    short course_value;
    short limit;
    short timer;
    short value_62;
    short value_64;
    short value_66;
    int speed;
    int value_6c;
    CombSetupValues values;
    unsigned int word_0c;
    unsigned char unknown_0080[0x1200];
    CombSetupLink *link;
    short value;
} CombObjectState;

void func_8006457C(CombObjectState *object, int player, int index, int kind,
                   int value, int x, int y, int z);

typedef struct CombResetEntry {
    int word_00;
    int word_04;
    short value_08;
    short value_0a;
} CombResetEntry;

typedef struct CombPlayerReset {
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
    CombResetEntry entries[2][2];
    int word_07a4;
    int word_07a8;
    unsigned char unknown_07ac[0x504];
    short value_0cb0;
    short value_0cb2;
    short value_0cb4;
    unsigned char unknown_0cb6[0x3C22];
    VECTOR vector_48d8;
    VECTOR vector_48e8;
    int word_48f8;
    int word_48fc;
    int word_4900;
    short value_4904;
    short value_4906;
    short value_4908;
    short value_490a;
    short value_490c;
    short value_490e;
    short value_4910;
    unsigned char byte_4912;
    unsigned char byte_4913;
} CombPlayerReset;

void func_80064984(CombPlayerReset *state, int reverse);

void func_80065604(int flip_first, int flip_second);

typedef struct CombCharacterBanks {
    short values[7];
} CombCharacterBanks;

typedef struct CombCourseBanks {
    short values[16];
} CombCourseBanks;

void func_800657DC(void);

void func_800658FC(int first_bank, int second_bank);

#include "bp.h"

void func_80065968(BpAnimationPlayback *playback, int animation, int mode, int end_frame);

typedef struct CombTuningParameters {
    int kind;
    int speed_steps[7];
    int speed;
    int value_24;
    int value_28;
    int response;
    int value_30;
    int response_scale;
    int value_38;
    int turn;
    int weight;
    int value_44;
    int value_48;
    int unknown_4c;
} CombTuningParameters;

typedef struct CombTuningEntry {
    unsigned char speed[3];
    unsigned char response[3];
    unsigned char turn[3];
    signed char weight;
} CombTuningEntry;

int func_80065BA8(CombObjectParameters *parameters, int mode);

typedef struct CombPlaybackPlayerView {
    unsigned char unknown_0000[0x28];
    CombSurfaceState surfaces[3];
    unsigned char unknown_0130[0xB4];
    BpAnimationPlayback playback;
    unsigned char unknown_06d8[0x57C];
    short direction;
    unsigned char unknown_0c56[0x66];
    int history_count;
    int frame_count;
    unsigned char unknown_0cc4[0x15C];
    unsigned short input_history[7500];
    unsigned char unknown_48b8[0x30];
    int velocity_x;
    int velocity_y;
    int velocity_z;
    unsigned char unknown_48f4[0x20];
    void *effects;
} CombPlaybackPlayerView;

int func_8006963C(CombPlaybackPlayerView *player, int frame);

typedef struct CombPlayerCoordinates {
    unsigned char unknown_0000[0x188];
    GsCOORDINATE2 body_coordinate;
    SVECTOR body_rotation;
    unsigned char unknown_01e0[0xA80];
    GsCOORDINATE2 secondary_coordinate;
    short secondary_pitch;
    short unknown_0cb2;
    short secondary_heading;
} CombPlayerCoordinates;

void func_800699A0(CombPlayerCoordinates *player);

typedef struct CombStartLocation {
    CombSceneEntry *entry;
    int x, y, z;
    unsigned char unknown_10[4];
    short related[3];
} CombStartLocation;

typedef struct CombScorePlayer {
    unsigned char unknown_0000[0x2C];
    int position[3];
    unsigned char unknown_0038[0x1A0];
    SVECTOR rotation;
    unsigned char unknown_01e0[0x4FC];
    BpScoreLimit limit;
    unsigned char unknown_0700[0x34];
    BpScoreMotion motion;
    unsigned char unknown_075c[0x134];
    BpScoreMove moves[13];
    unsigned char unknown_0c38[0xA];
    BpScoreSelection selections[2];
    unsigned char unknown_0c4a[0x3C9E];
    VECTOR velocity;
} CombScorePlayer;

int func_80069E00(CombScorePlayer *player, BpScore *score);

typedef struct CombMoveState {
    unsigned char timer;
    unsigned char timeout;
    unsigned char step;
    unsigned char step_count;
    short complete;
    short id;
    unsigned int masks[16];
} CombMoveState;

typedef struct CombMovePlayer {
    unsigned char unknown_000[0x28];
    unsigned int buttons;
    unsigned char unknown_02c[0x12];
    short airborne;
    unsigned char unknown_040[0x40];
    unsigned int previous_buttons;
    unsigned char unknown_084[0x80C];
    CombMoveState moves[13];
} CombMovePlayer;

int func_8006A288(CombMovePlayer *player);

typedef union CombMoveWork {
    unsigned int mask;
    int count;
    int id;
} CombMoveWork;

typedef struct CombInputMotion {
    unsigned char unknown_00[0x10];
    int x;
    int y;
} CombInputMotion;

typedef struct CombInputPlayer {
    unsigned char unknown_000[0x28];
    unsigned int buttons;
    unsigned char unknown_02c[0x12];
    short airborne;
    unsigned char unknown_040[0x6F4];
    CombInputMotion motion;
    unsigned char unknown_74c[0x28];
    CombResetEntry entries[2][2];
} CombInputPlayer;

void func_8006A4F8(CombInputPlayer *player, unsigned int side);

typedef struct CombInputResults {
    int first;
    int second;
} CombInputResults;

void func_8006A874(CombMotionPlayer *player);

void func_8006B2D0(GsOT *ot, CombHudEntry *entry, int *state, int direction, int shift, int extra);

typedef struct CombScoreLabels {
    char *text[14];
} CombScoreLabels;

typedef struct CombHudFont {
    unsigned char unknown_00[4];
    unsigned char red, green, blue;
    unsigned char unknown_07[0x1B];
    short height;
    unsigned char unknown_24[0x10];
} CombHudFont;

void func_8006B528(GsOT *ot, CombHudEntry *entry, int *state, int frozen, int duration);

typedef struct CombTrickLabelView {
    unsigned char unknown_00;
    unsigned char fakie;
    unsigned char backside;
    unsigned char unknown_03[0x19];
    short first_trick;
    short second_trick;
    union {
        int combined;
        struct {
            short spin;
            short flip;
        } parts;
    } rotation;
    short twist;
} CombTrickLabelView;

void func_8006BB20(GsOT *ot, int *data, int different);

void func_8006BB5C(GsOT *ot, int value, int *state, int shift, int extra);

void func_8006BBB4(GsOT *ot, int mode, int *state, int shift, int value);

void func_8006BC04(GsOT *ot, int *state, int shift);

void func_8006BC40(GsOT *ot, int *state, int shift, int *value);

void func_8006BC98(GsOT *ot, int *data, int different);

void func_8006BCD4(GsOT *ot, int value, int mode, int first, int second);

void func_8006BE3C(GsOT *ot, int *state, int frozen, int duration, int angle);

void func_8006C2C8(GsOT *ot, int *state, int frozen, int value);

void func_8006C358(GsOT *ot, int *counter, int different, int value, int direction);

void func_8006C4AC(GsOT *ot, int first, int second, int unused);

void func_8006C8F8(int course, int direction);

#endif
