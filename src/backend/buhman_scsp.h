#ifndef __BUHMAN_SATURN_SCSP_H__
#define __BUHMAN_SATURN_SCSP_H__


// Taken from https://github.com/buhman/saturn/blob/main/scsp.h

#include "buhman_type.h"

// Because the main CPU cannot access in units of 8 bits, so read and write in
// 16 bit units.
//
// (presumably m68k can do 32-byte accesses)

typedef union scsp_ram {
  uint16_t u16[0x080000 / 2];
  uint32_t u32[0x080000 / 4];
} scsp_ram;

static_assert((sizeof (union scsp_ram)) == 0x080000);

typedef unsigned char scsp_res0[0x080000];

// Formal register names are poorly documented; these are as faithfully as
// possible derived from "4.2 Sound Source Register"
typedef struct scsp_slot {
  struct {     // Loop control
    union {
      struct {
	union {
	  reg16 LOOP;
	  reg16 SAH;  // start address high
	};
	reg16 SAL;  // start address low
      };
      reg32 SA; // start address (requires mask)
    };
    reg16 LSA; // loop start address
    reg16 LEA; // loop end address
  };
  union {      // EG register
    struct {
      reg16 EGU;
      reg16 EGL;
    };
    reg32 EG;
  };
  union {
    struct {
      reg16 FMU; // Sound volume
      reg16 FML; // FM modulation control
    };
    reg32 FM;
  };
  reg16 PITCH;  // FM pitch control
  reg16 LFO;    // LFO (low-freqency oscillator)
  union {       // slot mixer
    struct {
      reg16 MIXERU;
      reg16 MIXERL;
    };
    reg32 MIXER;
  };
  reg16 _res[0x8 / 2];
} scsp_slot;

static_assert((sizeof (struct scsp_slot)) == 0x20);
static_assert((offsetof (struct scsp_slot, EGU)) == 0x08);
static_assert((offsetof (struct scsp_slot, EGL)) == 0x0a);
static_assert((offsetof (struct scsp_slot, MIXER)) == 0x14);

// Formal register names are poorly documented; these are as faithfully as
// possible derived from "4.2 Sound Source Register"
typedef struct scsp_ctrl {
  reg16 MIXER;     // master mixer
  reg16 DSP;       // DSP memory control
  union {          // MIDI
    struct {
      reg16 MIDIU;
      reg16 MIDIL;
    };
    reg32 MIDI;
  };
  reg16 STATUS;    // slot status
  reg16 _res[(8 / 2)];
  struct {
    reg16 DMAL;
    reg16 DMAH;
    reg16 DMAX;
  };
  struct {
    reg16 TIMA;
    reg16 TIMB;
    reg16 TIMC;
  };
  struct {
    reg16 SCIEB;
    reg16 SCIPD;
    reg16 SCIRE;
  };
  struct {
    reg16 SCILV0;
    reg16 SCILV1;
    reg16 SCILV2;
  };
  struct {
    reg16 MCIEB;
    reg16 MCIPD;
    reg16 MCIRE;
  };
} scsp_ctrl;

static_assert((sizeof (struct scsp_ctrl)) == 0x30);
static_assert((offsetof (struct scsp_ctrl, STATUS)) == 0x08);
static_assert((offsetof (struct scsp_ctrl, DMAL)) == 0x12);
static_assert((offsetof (struct scsp_ctrl, MCIRE)) == 0x2e);

typedef struct scsp_dsp_lh {
  reg16 L;
  reg16 H;
} scsp_dsp_lh;

typedef struct scsp_dsp_internal {
  scsp_dsp_lh TEMP[128];
  scsp_dsp_lh MEMS[32];
  scsp_dsp_lh MIXS[16];
  reg16 EFREG[16];
  reg16 _res1[2];
} scsp_dsp_internal;

static_assert((sizeof (struct scsp_dsp_internal)) == 0x2e4);
static_assert((offsetof (struct scsp_dsp_internal, EFREG)) == 0x2c0);

typedef struct scsp_dsp {
  reg16 COEF[64];
  reg16 MADRS[32];
  reg16 _res[0x40 / 2];
  struct {
    reg16 MPRO[4];
  } STEP[128];
  scsp_dsp_internal internal;
} scsp_dsp;

static_assert((sizeof (struct scsp_dsp)) == 0x7e4);
static_assert((offsetof (struct scsp_dsp, STEP)) == 0x100);
static_assert((offsetof (struct scsp_dsp, internal)) == 0x500);

typedef struct scsp_reg {
  scsp_slot slot[32];
  scsp_ctrl ctrl;
  reg16 _res0[(0x1d0 / 2)];
  struct {
    reg16 gen_a[32];
    reg16 gen_b[32];
  } direct;
  reg16 _res1[(0x80 / 2)];
  scsp_dsp dsp;
} scsp_reg;

static_assert((sizeof (struct scsp_reg)) == 0x000ee4);
static_assert((offsetof (struct scsp_reg, slot[1])) == 0x20);
static_assert((offsetof (struct scsp_reg, slot[2])) == 0x40);
static_assert((offsetof (struct scsp_reg, slot[31])) == 0x3e0);
static_assert((offsetof (struct scsp_reg, ctrl)) == 0x000400);
static_assert((offsetof (struct scsp_reg, direct)) == 0x000600);
static_assert((offsetof (struct scsp_reg, dsp)) == 0x000700);

struct scsp {
  scsp_ram ram;
  scsp_res0 _res0;
  scsp_reg reg;
};

extern struct scsp scsp __asm("scsp");

static_assert((sizeof (struct scsp)) == 0x100ee4);
static_assert((offsetof (struct scsp, ram)) == 0x000000);
static_assert((offsetof (struct scsp, reg)) == 0x100000);
static_assert((sizeof (scsp.reg.dsp.STEP)) == 0x400);

// bits

enum interrupt_bits {
  INT__SAMPLE = (1 << 10),
  INT__MIDI_OUTPUT = (1 << 9),
  INT__TIMER_C = (1 << 8),
  INT__TIMER_B = (1 << 7),
  INT__TIMER_A = (1 << 6),
  INT__CPU = (1 << 5),
  INT__DMA_TRANSFER_END = (1 << 4),
  INT__MIDI_INPUT = (1 << 3),
  INT__INT2N = (1 << 2),
  INT__INT1N = (1 << 1),
  INT__INT0N = (1 << 0),
};

enum scilv_bits {
  SCILV__TIMER_B__TIMER_C__MIDI_OUT__1F = (1 << 7),
  SCILV__TIMER_A = (1 << 6),
  SCILV__CPU = (1 << 5),
  SCILV__DMA_TRANSFER_END = (1 << 4),
  SCILV__MIDI_IN = (1 << 3),
  SCILV__INT2N = (1 << 2),
  SCILV__INT1N = (1 << 1),
  SCILV__INT0N = (1 << 0),
};

enum loop_bits {
  LOOP__KYONEX = (1 << 12), // (KX) execute KEY_ON
  LOOP__KYONB  = (1 << 11), // (KB) record KEY_ON, KEY_OFF
  LOOP__SBCTL = (0b00 << 9), // source bit control
  LOOP__SSCTL__DRAM  = (0b00 << 7), // sound source control
  LOOP__SSCTL__NOISE = (0b01 << 7), // sound source control
  LOOP__SSCTL__ZERO  = (0b10 << 7), // sound source control
  LOOP__LPCTL__OFF         = (0b00 << 5), // loop control
  LOOP__LPCTL__NORMAL      = (0b01 << 5), // loop control
  LOOP__LPCTL__REVERSE     = (0b10 << 5), // loop control
  LOOP__LPCTL__ALTERNATIVE = (0b11 << 5), // loop control
  LOOP__PCM8B = (1 << 4), // (8B) 8bit signed PCM
};

enum sa_bits {
  SA__KYONEX = (1 << (12 + 16)), // (KX) execute KEY_ON
  SA__KYONB  = (1 << (11 + 16)), // (KB) record KEY_ON, KEY_OFF
  SA__SBCTL = (0b00 << (9 + 16)), // source bit control
  SA__SSCTL__DRAM  = (0b00 << (7 + 16)), // sound source control
  SA__SSCTL__NOISE = (0b01 << (7 + 16)), // sound source control
  SA__SSCTL__ZERO  = (0b10 << (7 + 16)), // sound source control
  SA__LPCTL__OFF         = (0b00 << (5 + 16)), // loop control
  SA__LPCTL__NORMAL      = (0b01 << (5 + 16)), // loop control
  SA__LPCTL__REVERSE     = (0b10 << (5 + 16)), // loop control
  SA__LPCTL__ALTERNATIVE = (0b11 << (5 + 16)), // loop control
  SA__PCM8B = (1 << (4 + 16)), // (8B) 8bit signed PCM
#define SA__SA(n) ((n) & 0xfffff)
};

#define SAH__SA(n) ((((n) >> 16) & 0b1111) << 0) // start address high
#define SAL__SA(n) ((n) & 0xffff) // start address low

enum eg_bits {
#define EG__D2R(n) (((n) & 0x1f) << 27)  // Decay 2 rate
#define EG__D1R(n) (((n) & 0x1f) << 22)  // Decay 1 rate
        EG__EGHOLD = (       1 << 21), // (HO) EG hold mode
#define EG__AR(n)  (((n) & 0x1f) << 16)  // Attack rate
        EG__LPSLNK = (       1 << 14), // (LS) Loop start link
#define EG__KRS(n) (((n) & 0xf ) << 10)  // Key rate scaling
#define EG__DL(n)  (((n) & 0x1f) << 5 )  // Decay level
#define EG__RR(n)  (((n) & 0x1f) << 0 )  // Release rate
};

enum fm_bits {
  FM__STWINH = (1 << (9 + 16)), // (SI) Stack write inhibit (FM-related register)
  FM__SDIR   = (1 << (8 + 16)), // (SD) Sound direct
#define FM__TL(n) ((n) << (0 + 16)) // Total level (FM)
  FM__MDL   = (  0b0000 << 12), // Modulation level
  FM__MDXSL = (0b000000 << 6 ), // Select modulation input X
  FM__MDYSL = (0b000000 << 0 ), // Select modulation input Y
};

//enum pitch_bits {
#define PITCH__OCT(n) (((n) & 0xf) << 11)
#define PITCH__FNS(n) (((n) & 0x3ff) << 0 )
//};

enum lfo_bits {
  LFO__LFORE  = (      1 << 15), // (RE) LFO reset
  LFO__LFOF   = (0b00000 << 10), // LFO frequency
  LFO__PLFOWS = (   0b00 << 8 ), // Pitch-LFO wave select
  LFO__PLFOS  = (  0b000 << 5 ), // Pitch-LFO sensitivity
  LFO__ALFOWS = (   0b00 << 3 ), // Amplitude-LFO wave select
  LFO__ALFOS  = (  0b000 << 0 ), // Amplitude-LFO sensitivity
};

enum mixer_bits {
  MIXER__ISEL  = ( 0b0000 << 19),
  MIXER__IMXL  = (  0b000 << 16),
#define MIXER__DISDL(n) (((n) & 0b111) << 13)
  MIXER__DIPAN = (0b00000 << 8 ),
  MIXER__EFSDL = (  0b000 << 5 ),
  MIXER__EFPAN = (0b00000 << 0 ),
};

enum scsp_bits {
  MIXER__MEM4MB = (1 << 9),
  MIXER__DAC18B = (1 << 8),
#define MIXER__MVOL(n) ((n) << 0)
};

enum dsp_bits {
  DSP__RBL__8K  = (0b00 << 7),
  DSP__RBL__16K = (0b01 << 7),
  DSP__RBL__32K = (0b10 << 7),
  DSP__RBL__64K = (0b11 << 7),
#define DSP__RBP(n) (((n) >> 12) & 0x7f)
};

enum midiu_bits {
  MIDIU__MOFULL = (1 << 12), // (R)
  MIDIU__MOEMP = (1 << 11), // (R)
  MIDIU__MIOVF = (1 << 10), // (R)
  MIDIU__MIFULL = (1 << 9), // (R)
  MIDIU__MIEMP = (1 << 8), // (R)
#define MIDIU__MIBUF(i) (((i) >> 0) & 0xff) // getter macro
};

// enum midil_bits {
#define MIDIL__MOBUF(o) (((o) & 0xff) << 0) // setter macro
//};

//enum status_bits {
#define STATUS__MSLC(n) (((n) & 31) << 11)
#define STATUS__CA(reg) (((reg) >> 7) & 15)
//};

enum dma_bits {
#define DMAL__DMEA_L(n) ((n) & 0xfffe)
#define DMAH__DMEA_H(reg) ((((reg) >> 16) & 0b1111) << 12)
#define DMAH__DRGA(reg) ((reg) & 0x7fe)
  DMAX__GA = (1 << 14),
  DMAX__DI = (1 << 13),
  DMAX__EX = (1 << 12),
#define DMAX__DTLG(reg) ((reg) & 0x7fe)
};

// timer bits

//enum tima_bits {
#define TIMA__TACTL(n) (((n) & 0b111) << 8)
#define TIMB__TBCTL(n) (((n) & 0b111) << 8)
#define TIMC__TCCTL(n) (((n) & 0b111) << 8)
#define TIMA__TIMA(n) (((n) & 0xff) << 0)
#define TIMB__TIMB(n) (((n) & 0xff) << 0)
#define TIMC__TIMC(n) (((n) & 0xff) << 0)
//};


#endif
