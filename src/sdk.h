#ifndef CB2_SDK_H
#define CB2_SDK_H

/* PsyQ 3.7.0 declarations for accepted SDK entries in config/psyq-sdk.json.
 * Archive SHA-256: 6ef2e0ad9957f7af035a351b1c656af1fb91fe2bdc81a2b0a7ec0207bb7cf370.
 * Types/prototypes follow the archive; matched layouts retain authored field names.
 * Other unnamed fields have valid C identifiers.
 * Duplicate resident names carry address suffixes; overlay addresses are local
 * to the containing image. Names without archive prototypes are omitted.
 */

typedef unsigned char   undefined;

typedef struct VECTOR VECTOR, *PVECTOR;

struct VECTOR {
    long vx;
    long vy;
    long vz;
    long pad;
};

typedef struct MATRIX MATRIX, *PMATRIX;

struct MATRIX {
    short m[3][3];
    long t[3];
};

typedef struct SVECTOR SVECTOR, *PSVECTOR;

struct SVECTOR {
    short vx;
    short vy;
    short vz;
    short pad;
};

typedef unsigned char u_char;

typedef struct CdlLOC CdlLOC, *PCdlLOC;

struct CdlLOC {
    unsigned char minute;
    unsigned char second;
    unsigned char sector;
    unsigned char track;
};

typedef struct CdlATV CdlATV, *PCdlATV;

struct CdlATV {
    unsigned char val0;
    unsigned char val1;
    unsigned char val2;
    unsigned char val3;
};

typedef unsigned long u_long;

typedef void (*CdlCB)(unsigned char status, unsigned char *result);

typedef struct CdlFILE CdlFILE, *PCdlFILE;

struct CdlFILE {
    CdlLOC pos;
    unsigned long size;
    char name[16];
};

typedef struct RECT RECT, *PRECT;

struct RECT {
    short x, y, w, h;
};

typedef struct EVECTOR EVECTOR, *PEVECTOR;

typedef struct DVECTOR DVECTOR, *PDVECTOR;

typedef struct CVECTOR CVECTOR, *PCVECTOR;

struct CVECTOR {
    unsigned char r, g, b, cd;
};

struct DVECTOR {
    short vx;
    short vy;
};

struct EVECTOR {
    struct SVECTOR v;
    struct VECTOR sxyz;
    struct DVECTOR sxy;
    struct CVECTOR rgb;
    short txuv;
    short pad;
    long chx;
    long chy;
};

typedef struct DECDCTENV DECDCTENV, *PDECDCTENV;

struct DECDCTENV {
    u_char iq_y[64];
    u_char iq_c[64];
    short dct[64];
};

typedef unsigned short u_short;

typedef u_short DECDCTTAB[34816];

typedef struct DRAWENV DRAWENV, *PDRAWENV;

typedef struct DR_ENV DR_ENV, *PDR_ENV;

struct DR_ENV {
    unsigned long tag;
    unsigned long code[15];
};

struct DRAWENV {
    RECT clip;
    short ofs[2];
    RECT tw;
    unsigned short tpage;
    unsigned char dtd, dfe;
    unsigned char isbg;
    unsigned char r0, g0, b0;
    DR_ENV dr_env;
};

typedef struct DISPENV DISPENV, *PDISPENV;

struct DISPENV {
    RECT disp;
    RECT screen;
    unsigned char isinter;
    unsigned char isrgb24;
    unsigned char pad0, pad1;
};

typedef struct GsOT GsOT, *PGsOT;

typedef struct GsOT_TAG GsOT_TAG, *PGsOT_TAG;

struct GsOT {
    unsigned long length;
    struct GsOT_TAG *org;
    unsigned long offset;
    unsigned long point;
    struct GsOT_TAG *tag;
};

struct GsOT_TAG {
    unsigned int p:24;
    unsigned char num:8;
};

typedef struct _GsCOORDINATE2 _GsCOORDINATE2, *P_GsCOORDINATE2;

typedef struct _GsCOORDINATE2 GsCOORDINATE2;

typedef struct GsCOORD2PARAM GsCOORD2PARAM, *PGsCOORD2PARAM;

struct GsCOORD2PARAM {
    struct VECTOR scale;
    struct SVECTOR rotate;
    struct VECTOR trans;
};

struct _GsCOORDINATE2 {
    unsigned long flg;
    struct MATRIX coord;
    struct MATRIX workm;
    struct GsCOORD2PARAM *param;
    struct _GsCOORDINATE2 *super;
    struct _GsCOORDINATE2 *sub;
};

typedef struct GsDOBJ3 GsDOBJ3, *PGsDOBJ3;

struct GsDOBJ3 {
    unsigned long attribute;
    GsCOORDINATE2 *coord2;
    unsigned long *pmd;
    unsigned long *base;
    unsigned long *sv;
    unsigned long id;
};

typedef struct GsDOBJ2 GsDOBJ2, *PGsDOBJ2;

struct GsDOBJ2 {
    unsigned long attribute;
    GsCOORDINATE2 *coord2;
    unsigned long *tmd;
    unsigned long id;
};

typedef struct GsF_LIGHT GsF_LIGHT, *PGsF_LIGHT;

struct GsF_LIGHT {
    int vx;
    int vy;
    int vz;
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

typedef struct GsFOGPARAM GsFOGPARAM, *PGsFOGPARAM;

struct GsFOGPARAM {
    short dqa;
    long dqb;
    unsigned char rfc;
    unsigned char gfc;
    unsigned char bfc;
};

typedef struct GsRVIEW2 GsRVIEW2, *PGsRVIEW2;

struct GsRVIEW2 {
    long vpx;
    long vpy;
    long vpz;
    long vrx;
    long vry;
    long vrz;
    long rz;
    GsCOORDINATE2 *super;
};

typedef unsigned char PACKET;

typedef struct TMD_P_F3G TMD_P_F3G, *PTMD_P_F3G;

struct TMD_P_F3G {
    u_char out;
    u_char in;
    u_char dummy;
    u_char cd;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char dummy1;
    u_char r2;
    u_char g2;
    u_char b2;
    u_char dummy2;
    u_short n0;
    u_short sdk_unnamed_2;
    u_short sdk_unnamed_3;
    u_short v2;
};

typedef struct VERT VERT, *PVERT;

struct VERT {
    short vx;
    short vy;
    short vz;
    u_char tu;
    u_char tv;
};

typedef struct TMD_P_F4G TMD_P_F4G, *PTMD_P_F4G;

struct TMD_P_F4G {
    u_char out;
    u_char in;
    u_char dummy;
    u_char cd;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char dummy1;
    u_char r2;
    u_char g2;
    u_char b2;
    u_char dummy2;
    u_char r3;
    u_char g3;
    u_char b3;
    u_char dummy3;
    u_short n0;
    u_short sdk_unnamed_2;
    u_short sdk_unnamed_3;
    u_short v2;
    u_short v3;
    u_short dummy4;
};

typedef struct TMD_P_G3G TMD_P_G3G, *PTMD_P_G3G;

struct TMD_P_G3G {
    u_char out;
    u_char in;
    u_char dummy;
    u_char cd;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char dummy1;
    u_char r2;
    u_char g2;
    u_char b2;
    u_char dummy2;
    u_short n0;
    u_short sdk_unnamed_2;
    u_short n1;
    u_short sdk_unnamed_3;
    u_short n2;
    u_short v2;
};

typedef struct TMD_P_G4G TMD_P_G4G, *PTMD_P_G4G;

struct TMD_P_G4G {
    u_char out;
    u_char in;
    u_char dummy;
    u_char cd;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char dummy1;
    u_char r2;
    u_char g2;
    u_char b2;
    u_char dummy2;
    u_char r3;
    u_char g3;
    u_char b3;
    u_char dummy3;
    u_short n0;
    u_short sdk_unnamed_2;
    u_short n1;
    u_short sdk_unnamed_3;
    u_short n2;
    u_short v2;
    u_short n3;
    u_short v3;
};

typedef struct TIM_IMAGE TIM_IMAGE, *PTIM_IMAGE;

struct TIM_IMAGE {
    u_long mode;
    struct RECT *crect;
    u_long *caddr;
    struct RECT *prect;
    u_long *paddr;
};

typedef struct TMD_PRIM TMD_PRIM, *PTMD_PRIM;

struct TMD_PRIM {
    u_long id;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char p0;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char p1;
    u_char r2;
    u_char g2;
    u_char b2;
    u_char p2;
    u_char r3;
    u_char g3;
    u_char b3;
    u_char p3;
    u_short tpage;
    u_short clut;
    u_char u0;
    u_char sdk_unnamed_2;
    u_char u1;
    u_char sdk_unnamed_3;
    u_char u2;
    u_char v2;
    u_char u3;
    u_char v3;
    struct SVECTOR x0;
    struct SVECTOR x1;
    struct SVECTOR x2;
    struct SVECTOR x3;
    struct SVECTOR n0;
    struct SVECTOR n1;
    struct SVECTOR n2;
    struct SVECTOR n3;
    struct SVECTOR *v_ofs;
    struct SVECTOR *n_ofs;
    u_short vert0;
    u_short vert1;
    u_short vert2;
    u_short vert3;
    u_short norm0;
    u_short norm1;
    u_short norm2;
    u_short norm3;
};

typedef struct DR_AREA DR_AREA, *PDR_AREA;

struct DR_AREA {
    u_long tag;
    u_long code[2];
};

typedef struct DR_LOAD DR_LOAD, *PDR_LOAD;

struct DR_LOAD {
    u_long tag;
    u_long code[3];
    u_long p[13];
};

typedef struct DR_MODE DR_MODE, *PDR_MODE;

struct DR_MODE {
    u_long tag;
    u_long code[2];
};

typedef struct DR_MOVE DR_MOVE, *PDR_MOVE;

struct DR_MOVE {
    u_long tag;
    u_long code[5];
};

typedef struct DR_OFFSET DR_OFFSET, *PDR_OFFSET;

struct DR_OFFSET {
    u_long tag;
    u_long code[2];
};

typedef struct DR_TPAGE DR_TPAGE, *PDR_TPAGE;

struct DR_TPAGE {
    unsigned long tag;
    unsigned long code[1];
};

typedef struct LINE_F2 LINE_F2, *PLINE_F2;

struct LINE_F2 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    short x1;
    short y1;
};

typedef struct LINE_F3 LINE_F3, *PLINE_F3;

struct LINE_F3 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
    u_long pad;
};

typedef struct LINE_F4 LINE_F4, *PLINE_F4;

struct LINE_F4 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
    short x3;
    short y3;
    u_long pad;
};

typedef struct LINE_G2 LINE_G2, *PLINE_G2;

struct LINE_G2 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char p1;
    short x1;
    short y1;
};

typedef struct LINE_G3 LINE_G3, *PLINE_G3;

struct LINE_G3 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char p1;
    short x1;
    short y1;
    u_char r2;
    u_char g2;
    u_char b2;
    u_char p2;
    short x2;
    short y2;
    u_long pad;
};

typedef struct LINE_G4 LINE_G4, *PLINE_G4;

struct LINE_G4 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char p1;
    short x1;
    short y1;
    u_char r2;
    u_char g2;
    u_char b2;
    u_char p2;
    short x2;
    short y2;
    u_char r3;
    u_char g3;
    u_char b3;
    u_char p3;
    short x3;
    short y3;
    u_long pad;
};

typedef struct POLY_F3 POLY_F3, *PPOLY_F3;

struct POLY_F3 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
};

typedef struct POLY_F4 POLY_F4, *PPOLY_F4;

struct POLY_F4 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
    short x3;
    short y3;
};

typedef struct POLY_FT3 POLY_FT3, *PPOLY_FT3;

struct POLY_FT3 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    u_char u0;
    u_char sdk_unnamed_2;
    u_short clut;
    short x1;
    short y1;
    u_char u1;
    u_char sdk_unnamed_3;
    u_short tpage;
    short x2;
    short y2;
    u_char u2;
    u_char v2;
    u_short pad1;
};

typedef struct POLY_FT4 POLY_FT4, *PPOLY_FT4;

struct POLY_FT4 {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
};

typedef struct POLY_G3 POLY_G3, *PPOLY_G3;

struct POLY_G3 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char pad1;
    short x1;
    short y1;
    u_char r2;
    u_char g2;
    u_char b2;
    u_char pad2;
    short x2;
    short y2;
};

typedef struct POLY_G4 POLY_G4, *PPOLY_G4;

struct POLY_G4 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char pad1;
    short x1;
    short y1;
    u_char r2;
    u_char g2;
    u_char b2;
    u_char pad2;
    short x2;
    short y2;
    u_char r3;
    u_char g3;
    u_char b3;
    u_char pad3;
    short x3;
    short y3;
};

typedef struct POLY_GT3 POLY_GT3, *PPOLY_GT3;

struct POLY_GT3 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    u_char u0;
    u_char sdk_unnamed_2;
    u_short clut;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char p1;
    short x1;
    short y1;
    u_char u1;
    u_char sdk_unnamed_3;
    u_short tpage;
    u_char r2;
    u_char g2;
    u_char b2;
    u_char p2;
    short x2;
    short y2;
    u_char u2;
    u_char v2;
    u_short pad2;
};

typedef struct POLY_GT4 POLY_GT4, *PPOLY_GT4;

struct POLY_GT4 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    u_char u0;
    u_char sdk_unnamed_2;
    u_short clut;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char p1;
    short x1;
    short y1;
    u_char u1;
    u_char sdk_unnamed_3;
    u_short tpage;
    u_char r2;
    u_char g2;
    u_char b2;
    u_char p2;
    short x2;
    short y2;
    u_char u2;
    u_char v2;
    u_short pad2;
    u_char r3;
    u_char g3;
    u_char b3;
    u_char p3;
    short x3;
    short y3;
    u_char u3;
    u_char v3;
    u_short pad3;
};

typedef struct SPRT SPRT, *PSPRT;

struct SPRT {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
};

typedef struct SPRT_16 SPRT_16, *PSPRT_16;

struct SPRT_16 {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
};

typedef struct SPRT_8 SPRT_8, *PSPRT_8;

struct SPRT_8 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    u_char u0;
    u_char sdk_unnamed_2;
    u_short clut;
};

typedef struct DR_TWIN DR_TWIN, *PDR_TWIN;

struct DR_TWIN {
    u_long tag;
    u_long code[2];
};

typedef struct TILE TILE, *PTILE;

struct TILE {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
    short w;
    short h;
};

typedef struct TILE_1 TILE_1, *PTILE_1;

struct TILE_1 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
};

typedef struct TILE_16 TILE_16, *PTILE_16;

struct TILE_16 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
};

typedef struct TILE_8 TILE_8, *PTILE_8;

struct TILE_8 {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
    short x0;
    short y0;
};

typedef struct SpuCommonAttr SpuCommonAttr, *PSpuCommonAttr;

typedef struct SpuVolume SpuVolume, *PSpuVolume;

typedef struct SpuExtAttr SpuExtAttr, *PSpuExtAttr;

struct SpuVolume {
    short left;
    short right;
};

struct SpuExtAttr {
    SpuVolume volume;
    long reverb;
    long mix;
};

struct SpuCommonAttr {
    unsigned long mask;
    SpuVolume mvol;
    SpuVolume mvolmode;
    SpuVolume mvolx;
    SpuExtAttr cd;
    SpuExtAttr ext;
};

typedef struct SpuVoiceAttr SpuVoiceAttr, *PSpuVoiceAttr;

struct SpuVoiceAttr {
    unsigned long voice;
    unsigned long mask;
    SpuVolume volume;
    SpuVolume volmode;
    SpuVolume volumex;
    unsigned short pitch;
    unsigned short note;
    unsigned short sample_note;
    short envx;
    unsigned long addr;
    unsigned long loop_addr;
    long a_mode;
    long s_mode;
    long r_mode;
    unsigned short ar;
    unsigned short dr;
    unsigned short sr;
    unsigned short rr;
    unsigned short sl;
    unsigned short adsr1;
    unsigned short adsr2;
};

/* Callback function types used by archive prototypes. */
typedef undefined CB2SdkCallback__func_49(void);
typedef void CB2SdkCallback_func(void);
typedef void CB2SdkCallback_func1(void);
typedef void CB2SdkCallback_func2(void);
typedef void CB2SdkCallback_f(void);

/* Available SDK entry points; addresses are in config/sdk/*.ld. */
extern void AddPrim(void * ot, void * p);
extern void AddPrims(void * ot, void * p0, void * p1);
extern VECTOR * ApplyMatrixLV(MATRIX * m, VECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3);
extern SVECTOR * ApplyMatrixSV(MATRIX * m, SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3);
extern long AverageZ3(long sz0, long sz1, long sz2);
extern long AverageZ4(long sz0, long sz1, long sz2, long sz3);
extern void CatPrim(void * p0, void * p1);
extern char * CdComstr(u_char com);
extern int CdControl(u_char com, u_char * param, u_char * result);
extern int CdControlB(u_char com, u_char * param, u_char * result);
extern int CdControlF(u_char com, u_char * param);
extern CB2SdkCallback__func_49 * CdDataCallback(CB2SdkCallback_func * callback);
extern int CdDataSync(int mode);
extern void CdFlush(void);
extern int CdGetSector(void * madr, int size);
extern int CdInit(void);
extern CdlLOC * CdIntToPos(int i, CdlLOC * p);
extern char * CdIntstr(u_char intr);
extern int CdLastCom(void);
extern CdlLOC * CdLastPos(void);
extern int CdMix(CdlATV * vol);
extern int CdMode(void);
extern int CdPosToInt(CdlLOC * p);
extern int CdRead(int sectors, u_long * buf, int mode);
extern int CdRead2(long mode);
extern void CdReadBreak(void);
extern CdlCB CdReadCallback(CdlCB callback);
extern int CdReadSync(int mode, u_char * result);
extern int CdReady(int mode, u_char * result);
extern CdlCB CdReadyCallback(CdlCB callback);
extern int CdReset(int mode);
extern CdlFILE * CdSearchFile(CdlFILE * sdk_unnamed_30, char * name);
extern int CdSetDebug(int level);
extern int CdStatus(void);
extern int CdSync(int mode, u_char * result);
extern CdlCB CdSyncCallback(CdlCB callback);
extern int CheckCallback(void);
extern int ClearImage(RECT * rect, u_char r, u_char g, u_char b);
extern int ClearImage2(RECT * rect, u_char r, u_char g, u_char b);
extern u_long * ClearOTag(u_long * ot, int n);
extern u_long * ClearOTagR(u_long * ot, int n);
extern long Clip3F(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, EVECTOR * * evmx);
extern long Clip3FP(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, EVECTOR * * evmx);
extern long Clip3FT(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, short * uv0, short * uv1, short * uv2, EVECTOR * * evmx);
extern long Clip3FTP(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, short * uv0, short * uv1, short * uv2, EVECTOR * * evmx);
extern long Clip3G(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, CVECTOR * rgb0, CVECTOR * rgb1, CVECTOR * rgb2, EVECTOR * * evmx);
extern long Clip3GP(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, CVECTOR * rgb0, CVECTOR * rgb1, CVECTOR * rgb2, EVECTOR * * evmx);
extern long Clip3GT(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, short * uv0, short * uv1, short * uv2, CVECTOR * rgb0, CVECTOR * rgb1, CVECTOR * rgb2, EVECTOR * * evmx);
extern long Clip3GTP(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, short * uv0, short * uv1, short * uv2, CVECTOR * rgb0, CVECTOR * rgb1, CVECTOR * rgb2, EVECTOR * * evmx);
extern long Clip4FT(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, SVECTOR * v3, short * uv0, short * uv1, short * uv2, short * uv3, EVECTOR * * evmx);
extern long Clip4FTP(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, SVECTOR * v3, short * uv0, short * uv1, short * uv2, short * uv3, EVECTOR * * evmx);
extern long Clip4G(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, SVECTOR * v3, CVECTOR * rgb0, CVECTOR * rgb1, CVECTOR * rgb2, CVECTOR * rgb3, EVECTOR * * evmx);
extern long Clip4GP(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, SVECTOR * v3, CVECTOR * rgb0, CVECTOR * rgb1, CVECTOR * rgb2, CVECTOR * rgb3, EVECTOR * * evmx);
extern long Clip4GT(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, SVECTOR * v3, short * uv0, short * uv1, short * uv2, short * uv3, CVECTOR * rgb0, CVECTOR * rgb1, CVECTOR * rgb2, CVECTOR * rgb3, EVECTOR * * evmx);
extern long Clip4GTP(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, SVECTOR * v3, short * uv0, short * uv1, short * uv2, short * uv3, CVECTOR * rgb0, CVECTOR * rgb1, CVECTOR * rgb2, CVECTOR * rgb3, EVECTOR * * evmx);
extern void ColorCol(VECTOR * sdk_unnamed_2, CVECTOR * sdk_unnamed_3, CVECTOR * v2);
extern void ColorDpq(VECTOR * sdk_unnamed_2, CVECTOR * sdk_unnamed_3, long p, CVECTOR * v2);
extern int DecDCTBufSize(u_long * bs);
extern DECDCTENV * DecDCTGetEnv(DECDCTENV * env);
extern DECDCTENV * DecDCTPutEnv(DECDCTENV * env);
extern void DecDCTReset(int mode);
extern void DecDCTin(u_long * buf, int mode);
extern int DecDCTinCallback(CB2SdkCallback_func * callback);
extern int DecDCTinSync(int mode);
extern void DecDCTout(u_long * buf, int size);
extern int DecDCToutCallback(CB2SdkCallback_func * callback);
extern int DecDCToutSync(int mode);
extern int DecDCTvlc2(u_long * bs, u_long * buf, DECDCTTAB table);
extern void DecDCTvlcBuild(u_short * table);
extern int DecDCTvlcSize2(int size);
extern void DpqColor(CVECTOR * sdk_unnamed_2, long p, CVECTOR * sdk_unnamed_3);
extern void DpqColor3(CVECTOR * sdk_unnamed_2, CVECTOR * sdk_unnamed_3, CVECTOR * v2, long p, CVECTOR * v3, CVECTOR * v4, CVECTOR * v5);
extern void DpqColorLight(VECTOR * sdk_unnamed_2, CVECTOR * sdk_unnamed_3, long p, CVECTOR * v2);
extern void DrawOTag(u_long * p);
extern void DrawOTagEnv(u_long * p, DRAWENV * env);
extern void DrawPrim(void * p);
extern int DrawSync(int mode);
extern u_long DrawSyncCallback(CB2SdkCallback_func * callback);
extern void DumpClut(u_short clut);
extern void DumpDispEnv(DISPENV * env);
extern void DumpDrawEnv(DRAWENV * env);
extern void DumpTPage(u_short tpage);
extern u_long * FntFlush(int id);
extern void FntLoad(int tx, int ty);
extern int FntOpen(int x, int y, int w, int h, int isbg, int n);
extern int FntPrint(); /* Archive omits parameter types. */
extern u_short GetClut(int x, int y);
extern DISPENV * GetDispEnv(DISPENV * env);
extern DRAWENV * GetDrawEnv(DRAWENV * env);
extern int GetGraphDebug(void);
extern int GetODE(void);
extern u_short GetTPage(int tp, int abr, int x, int y);
extern void GsClearOt(unsigned short offset, unsigned short point, GsOT * otp);
extern void GsDrawOt(GsOT * ot);
extern int GsGetActiveBuff(void);
extern void GsGetLs(GsCOORDINATE2 * m, MATRIX * out);
extern void GsGetLw(GsCOORDINATE2 * m, MATRIX * out);
extern void GsGetLws(GsCOORDINATE2 * m, MATRIX * outw, MATRIX * outs);
extern void GsInit3D(void);
extern void GsInitCoord2param(GsCOORD2PARAM * );
extern void GsInitCoordinate2(GsCOORDINATE2 * super, GsCOORDINATE2 * base);
extern void GsInitGraph(unsigned short x, unsigned short y, unsigned short intmode, unsigned short dith, unsigned short varmmode);
extern void GsInitGraph2(u_short x, u_short y, u_short intmode, u_short dith, u_short vrammode);
extern u_long GsLinkObject3(unsigned long pmd_base, GsDOBJ3 * objp);
extern void GsLinkObject4(unsigned long tmd_base, GsDOBJ2 * objp, int n);
extern void GsMapModelingData(unsigned long * p);
extern void GsMulCoord0(MATRIX * m1, MATRIX * m2, MATRIX * m3);
extern void GsMulCoord2(MATRIX * m1, MATRIX * m2);
extern void GsMulCoord3(MATRIX * m1, MATRIX * m2);
extern void GsScaleScreen(SVECTOR * scale);
extern void GsSetAmbient(long r, long g, long b);
extern void GsSetClip(RECT * clip);
extern void GsSetDrawBuffClip(void);
extern void GsSetDrawBuffOffset(void);
extern int GsSetFlatLight(int id, GsF_LIGHT * lt);
extern void GsSetFogParam(GsFOGPARAM * fogparm);
extern void GsSetLightMatrix(MATRIX * mp);
extern void GsSetLightMatrix2(MATRIX * mp);
extern void GsSetLightMode(int mode);
extern void GsSetLsMatrix(MATRIX * mp);
extern void GsSetOffset(long x, long y);
extern int GsSetRefView2(GsRVIEW2 * pv);
extern int GsSetRefView2L(GsRVIEW2 * pv);
extern void GsSortClear(unsigned char , unsigned char , unsigned char , GsOT * );
extern void GsSortObject3(GsDOBJ3 * objp, GsOT * ot, int shift);
extern void GsSortObject4(GsDOBJ2 * objp, GsOT * ot, int shift, u_long * scratch);
extern void GsSwapDispBuff(void);
extern PACKET * GsTMDfastF3GL(TMD_P_F3G * op, VERT * vp, VERT * np, PACKET * pk, int n, int shift, GsOT * ot, u_long * scratch);
extern PACKET * GsTMDfastF3GLFG(TMD_P_F3G * op, VERT * vp, VERT * np, PACKET * pk, int n, int shift, GsOT * ot, u_long * scratch);
extern PACKET * GsTMDfastF3GNL(TMD_P_F3G * op, VERT * vp, VERT * np, PACKET * pk, int n, int shift, GsOT * ot, u_long * scratch);
extern PACKET * GsTMDfastF4GL(TMD_P_F4G * op, VERT * vp, VERT * np, PACKET * pk, int n, int shift, GsOT * ot, u_long * scratch);
extern PACKET * GsTMDfastF4GLFG(TMD_P_F4G * op, VERT * vp, VERT * np, PACKET * pk, int n, int shift, GsOT * ot, u_long * scratch);
extern PACKET * GsTMDfastF4GNL(TMD_P_F4G * op, VERT * vp, VERT * np, PACKET * pk, int n, int shift, GsOT * ot, u_long * scratch);
extern PACKET * GsTMDfastG3GL(TMD_P_G3G * op, VERT * vp, VERT * np, PACKET * pk, int n, int shift, GsOT * ot, u_long * scratch);
extern PACKET * GsTMDfastG3GLFG(TMD_P_G3G * op, VERT * vp, VERT * np, PACKET * pk, int n, int shift, GsOT * ot, u_long * scratch);
extern PACKET * GsTMDfastG3GNL(TMD_P_G3G * op, VERT * vp, VERT * np, PACKET * pk, int n, int shift, GsOT * ot, u_long * scratch);
extern PACKET * GsTMDfastG4GL(TMD_P_G4G * op, VERT * vp, VERT * np, PACKET * pk, int n, int shift, GsOT * ot, u_long * scratch);
extern PACKET * GsTMDfastG4GLFG(TMD_P_G4G * op, VERT * vp, VERT * np, PACKET * pk, int n, int shift, GsOT * ot, u_long * scratch);
extern PACKET * GsTMDfastG4GNL(TMD_P_G4G * op, VERT * vp, VERT * np, PACKET * pk, int n, int shift, GsOT * ot, u_long * scratch);
extern void Gssub_make_matrix(MATRIX * mp, short s, short c, char rotate);
extern void InitClip(EVECTOR * evbfad, long hw, long vw, long h, long , long );
extern void InitHeap(unsigned long * , unsigned long );
extern void Intpl(VECTOR * sdk_unnamed_2, long p, CVECTOR * sdk_unnamed_3);
extern void InvSquareRoot(long a, long * b, long * c);
extern int IsEndPrim(void * p);
extern void LightColor(VECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3);
extern void LoadAverage0(VECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3, long p0, long p1, VECTOR * v2);
extern void LoadAverage12(VECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3, long p0, long p1, VECTOR * v2);
extern void LoadAverageByte(u_char * sdk_unnamed_2, u_char * sdk_unnamed_3, long p0, long p1, u_char * v2);
extern void LoadAverageCol(u_char * sdk_unnamed_2, u_char * sdk_unnamed_3, long p0, long p1, u_char * v2);
extern void LoadAverageShort0(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, long p0, long p1, SVECTOR * v2);
extern void LoadAverageShort12(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, long p0, long p1, SVECTOR * v2);
extern u_short LoadClut(u_long * clut, int x, int y);
extern u_short LoadClut2(u_long * clut, int x, int y);
extern int LoadImage(RECT * rect, u_long * p);
extern u_short LoadTPage(u_long * pix, int tp, int abr, int x, int y, int w, int h);
extern void LocalLight(SVECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3);
extern long Lzc(long data);
extern int MargePrim(void * p0, void * p1);
extern void MatrixNormal(MATRIX * m, MATRIX * n);
extern int MoveImage(RECT * rect, int x, int y);
extern MATRIX * MulMatrix(MATRIX * m0, MATRIX * m1);
extern MATRIX * MulMatrix0(MATRIX * m0, MATRIX * m1, MATRIX * m2);
extern MATRIX * MulMatrix2(MATRIX * m0, MATRIX * m1);
extern void * NextPrim(void * p);
extern void NormalColor(SVECTOR * sdk_unnamed_2, CVECTOR * sdk_unnamed_3);
extern void NormalColor3(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, CVECTOR * v3, CVECTOR * v4, CVECTOR * v5);
extern void NormalColorCol(SVECTOR * sdk_unnamed_2, CVECTOR * sdk_unnamed_3, CVECTOR * v2);
extern void NormalColorCol3(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, CVECTOR * v3, CVECTOR * v4, CVECTOR * v5, CVECTOR * v6);
extern void NormalColorDpq(SVECTOR * sdk_unnamed_2, CVECTOR * sdk_unnamed_3, long p, CVECTOR * v2);
extern void NormalColorDpq3(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, CVECTOR * v3, long p, CVECTOR * v4, CVECTOR * v5, CVECTOR * v6);
extern int OpenTIM(u_long * addr);
extern int OpenTMD(u_long * tmd, int obj_no);
extern void OuterProduct0(VECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3, VECTOR * v2);
extern void OuterProduct12(VECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3, VECTOR * v2);
extern int PCcreat(char * name, int perms);
extern int PCread(int fd, char * buff, int len);
extern void PadInit(int mode);
extern u_long PadRead(int id);
extern void PadStop(void);
extern void PopMatrix(void);
extern void PushMatrix(void);
extern DISPENV * PutDispEnv(DISPENV * env);
extern DRAWENV * PutDrawEnv(DRAWENV * env);
extern void ReadGeomOffset(long * ofx, long * ofy);
extern TIM_IMAGE * ReadTIM(TIM_IMAGE * timimg);
extern TMD_PRIM * ReadTMD(TMD_PRIM * tmdprim);
extern int ResetCallback(void);
extern int ResetGraph(int mode);
extern int RestartCallback(void);
extern long RotAverage3(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, long * sxy0, long * sxy1, long * sxy2, long * p, long * flag);
extern long RotAverage4(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, SVECTOR * v3, long * sxy0, long * sxy1, long * sxy2, long * sxy3, long * p, long * flag);
extern long RotAverageNclip3(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, long * sxy0, long * sxy1, long * sxy2, long * p, long * otz, long * flag);
extern long RotAverageNclip4(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, SVECTOR * v3, long * sxy0, long * sxy1, long * sxy2, long * sxy3, long * p, long * otz, long * flag);
extern long RotAverageNclipColorCol3(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, SVECTOR * v3, SVECTOR * v4, SVECTOR * v5, CVECTOR * v6, long * sxy0, long * sxy1, long * sxy2, CVECTOR * v7, CVECTOR * v8, CVECTOR * v9, long * otz, long * flag);
extern long RotAverageNclipColorDpq3(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, SVECTOR * v3, SVECTOR * v4, SVECTOR * v5, CVECTOR * v6, long * sxy0, long * sxy1, long * sxy2, CVECTOR * v7, CVECTOR * v8, CVECTOR * v9, long * otz, long * flag);
extern MATRIX * RotMatrix(SVECTOR * r, MATRIX * m);
extern MATRIX * RotMatrixX(long r, MATRIX * m);
extern MATRIX * RotMatrixY(long r, MATRIX * m);
extern MATRIX * RotMatrixZ(long r, MATRIX * m);
extern void RotPMD_F3(long * pa, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_F4(long * pa, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_FT3(long * pa, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_FT4(long * pa, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_G3(long * pa, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_G4(long * pa, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_GT3(long * pa, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_GT4(long * pa, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_SV_F3(long * pa, long * va, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_SV_F4(long * pa, long * va, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_SV_FT3(long * pa, long * va, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_SV_FT4(long * pa, long * va, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_SV_G3(long * pa, long * va, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_SV_G4(long * pa, long * va, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_SV_GT3(long * pa, long * va, u_long * ot, int otlen, int id, int backc);
extern void RotPMD_SV_GT4(long * pa, long * va, u_long * ot, int otlen, int id, int backc);
extern void RotTrans(SVECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3, long * flag);
extern long RotTransPers(SVECTOR * sdk_unnamed_2, long * sxy, long * p, long * flag);
extern long RotTransPers4(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, SVECTOR * v2, SVECTOR * v3, long * sxy0, long * sxy1, long * sxy2, long * sxy3, long * p, long * flag);
extern void RotTransSV(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3, long * flag);
extern MATRIX * ScaleMatrix(MATRIX * m, VECTOR * v);
extern void SetBackColor(long rbk, long gbk, long bbk);
extern void SetColorMatrix(MATRIX * m);
extern DISPENV * SetDefDispEnv(DISPENV * env, int x, int y, int w, int h);
extern DRAWENV * SetDefDrawEnv(DRAWENV * env, int x, int y, int w, int h);
extern void SetDispMask(int mask);
extern void SetDrawArea(DR_AREA * p, RECT * r);
extern void SetDrawEnv(DR_ENV * dr_env, DRAWENV * env);
extern void SetDrawLoad(DR_LOAD * p, RECT * rect);
extern void SetDrawMode(DR_MODE * p, int dfe, int dtd, int tpage, RECT * tw);
extern void SetDrawMove(DR_MOVE * p, RECT * rect, int x, int y);
extern void SetDrawOffset(DR_OFFSET * p, u_short * ofs);
extern void SetDrawTPage(DR_TPAGE * p, int dfe, int dtd, int tpage);
extern void SetDumpFnt(int id);
extern void SetFarColor(long rfc, long gfc, long bfc);
extern int SetGraphDebug(int level);
extern int SetGraphQueue(int mode);
extern int SetGraphReverse(int mode);
extern void SetLightMatrix(MATRIX * m);
extern void SetLineF2(LINE_F2 * p);
extern void SetLineF3(LINE_F3 * p);
extern void SetLineF4(LINE_F4 * p);
extern void SetLineG2(LINE_G2 * p);
extern void SetLineG3(LINE_G3 * p);
extern void SetLineG4(LINE_G4 * p);
extern void SetPolyF3(POLY_F3 * p);
extern void SetPolyF4(POLY_F4 * p);
extern void SetPolyFT3(POLY_FT3 * p);
extern void SetPolyFT4(POLY_FT4 * p);
extern void SetPolyG3(POLY_G3 * p);
extern void SetPolyG4(POLY_G4 * p);
extern void SetPolyGT3(POLY_GT3 * p);
extern void SetPolyGT4(POLY_GT4 * p);
extern void SetRotMatrix(MATRIX * m);
extern void SetSemiTrans(void * p, int abe);
extern void SetShadeTex(void * p, int tge);
extern void SetSprt(SPRT * p);
extern void SetSprt16(SPRT_16 * p);
extern void SetSprt8(SPRT_8 * p);
extern void SetTexWindow(DR_TWIN * p, RECT * tw);
extern void SetTile(TILE * p);
extern void SetTile1(TILE_1 * p);
extern void SetTile16(TILE_16 * p);
extern void SetTile8(TILE_8 * p);
extern void SetTransMatrix(MATRIX * m);
extern long SpuGetKeyStatus(unsigned long voice_bit);
extern void SpuInit(void);
extern long SpuIsTransferCompleted(long flag);
extern unsigned long SpuRead(unsigned char * addr, unsigned long size);
extern void SpuSetCommonAttr(SpuCommonAttr * attr);
extern void SpuSetKey(long on_off, unsigned long voice_bit);
extern void SpuSetKeyOnWithAttr(SpuVoiceAttr * attr);
extern long SpuSetMute(long on_off);
extern long SpuSetNoiseClock(long n_clock);
extern unsigned long SpuSetNoiseVoice(long on_off, unsigned long voice_bit);
extern long SpuSetTransferMode(long mode);
extern unsigned long SpuSetTransferStartAddr(unsigned long addr);
extern void SpuSetVoiceAttr(SpuVoiceAttr * arg);
extern void SpuStart(void);
extern unsigned long SpuWritePartly(unsigned char * addr, unsigned long size);
extern VECTOR * Square0(VECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3);
extern VECTOR * Square12(VECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3);
extern long SquareRoot0(long a);
extern long SquareRoot12(long a);
extern VECTOR * SquareSL0(SVECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3);
extern VECTOR * SquareSL12(SVECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3);
extern SVECTOR * SquareSS0(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3);
extern SVECTOR * SquareSS12(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3);
extern void StCdInterrupt(void);
extern void StClearRing(void);
extern u_long StFreeRing(u_long * base);
extern int StGetBackloc(CdlLOC * loc);
extern u_long StGetNext(u_long * * addr, u_long * * header);
extern void StSetRing(u_long * ring_addr, u_long ring_size);
extern void StSetStream(u_long mode, u_long start_frame, u_long end_frame, CB2SdkCallback_func1 * callback1, CB2SdkCallback_func2 * callback2);
extern int StopCallback(void);
extern int StoreImage(RECT * rect, u_long * p);
extern void TermPrim(void * p);
extern MATRIX * TransposeMatrix(MATRIX * m0, MATRIX * m1);
extern int VSync(int mode);
extern int VSyncCallback(CB2SdkCallback_f * callback);
extern long VectorNormal(VECTOR * sdk_unnamed_2, VECTOR * sdk_unnamed_3);
extern long VectorNormalS(VECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3);
extern long VectorNormalSS(SVECTOR * sdk_unnamed_2, SVECTOR * sdk_unnamed_3);
extern int bcmp(unsigned char * , unsigned char * , int );
extern void * bzero(unsigned char * , int );
extern int catan(int a);
extern int csqrt(int a);
extern void exit(void);
extern void gte_init(void);
extern void gte_rotate_z_matrix(MATRIX * m, long rz);
extern void * memchr(unsigned char * , unsigned char , int );
extern void * memcpy_80042E68(); /* Archive omits parameter types. */
extern void * memcpy_800449D8(); /* Archive omits parameter types. */
extern void * memmove(unsigned char * , unsigned char * , int );
extern void * memset_8002C390(unsigned char * , unsigned char , int );
extern void * memset_800449E8(unsigned char * , unsigned char , int );
extern int printf(char * fmt, ...);
extern int rand(void);
extern long ratan2(long y, long x);
extern int rcos(int a);
extern int rsin(int a);
extern int sprintf(char * buffer, char * fmt, ...);
extern void srand(unsigned int );
extern char * strcat(char * , char * );
/* Archive omits parameter types; keep this declaration unprototyped. */
extern int strcmp();
/* Archive omits parameter types; keep this declaration unprototyped. */
extern char * strcpy();
/* Archive omits parameter types; keep this declaration unprototyped. */
extern int strlen();
extern int strncmp(char * , char * , int );

/* Previously matched declarations without same-name archive prototypes. */
extern int GetGraphType(void);
extern void StopPAD(void);
extern int ChangeClearPAD(int mode);

#endif
