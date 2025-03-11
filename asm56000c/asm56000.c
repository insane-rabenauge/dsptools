/*
 * Motorola DSP56000 Assembler Version 4.1.1
 * 68000 to C conversion
 * by insane/Rabenauge^tSCc
 * https://insane.tscc.de
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MEMSIZE 0x1000000

#include "asm56000_emu68k.h"

#include "asm56000_data.h"
#include "asm56000_code.h"

#define MAXFILES 0x20
#define FILEHDL  0x10

#define EFILNF	-33L	// File not found
#define ENHNDL	-35L	// No more handles available
#define EIHNDL	-37L    // Invalid file handle

int memptr=0;
int membot=0x010000;

FILE *fileptrs[MAXFILES];
char fileused[MAXFILES];
char filemode[MAXFILES];

void host_pterm(int exitcode) {
  exit(exitcode);
};

void host_conout(int fd, int ch) {
  if (fd==1) fputc(ch,stdout);
  if (fd==2) fputc(ch,stderr);
};

uint32_t host_malloc(int32_t num) {
  if ((memptr+num)<MEMSIZE) {
    uint32_t mem=memptr;
    memptr+=num;
    return(mem);
  } else return(0);
};

int32_t host_fopen(uint32_t fnamptr,uint16_t mode) {
  int handle=-1;
  for (int i=0;i<MAXFILES;i++) {
    if (!fileused[i]) {
      handle=i;
      break;
    };
  };
  if (handle==-1) return(ENHNDL);
  switch(mode) {
    case 0: fileptrs[handle]=fopen(m68k_pb(fnamptr),"rb"); break;
    case 1: fileptrs[handle]=fopen(m68k_pb(fnamptr),"wb"); break;
    case 2: fileptrs[handle]=fopen(m68k_pb(fnamptr),"r+b"); break;
  };
  if (fileptrs[handle]==NULL) return(EFILNF);
  filemode[handle]=mode;
  fileused[handle]=1;
  handle+=FILEHDL;
  return(handle);
};

int32_t host_fseek(int32_t offset, int16_t handle, int16_t seekmode) {
  int fpos=0;
  if ((handle>=FILEHDL)&&(handle<(FILEHDL+MAXFILES))) {
    handle-=FILEHDL;
    switch (seekmode) {
      case 0: seekmode=SEEK_SET; break;
      case 1: seekmode=SEEK_CUR; break;
      case 2: seekmode=SEEK_END; break;
    };
    if (fileused[handle]) {
      fseek(fileptrs[handle],offset,seekmode);
      fpos=ftell(fileptrs[handle]);
    };
  };
  return(fpos);
};

int32_t host_fread(int16_t handle,int32_t count,uint32_t ptr) {
  if ((handle>=FILEHDL)&&(handle<(FILEHDL+MAXFILES))) {
    handle-=FILEHDL;
    if (fileused[handle]) {
      return(fread(m68k_pb(ptr),1,count,fileptrs[handle]));
    };
  };
  return(EIHNDL);
};

int32_t host_fwrite(int16_t handle,int32_t count,uint32_t ptr) {
  if ((handle>=FILEHDL)&&(handle<(FILEHDL+MAXFILES))) {
    handle-=FILEHDL;
    if (fileused[handle]) {
      return(fwrite(m68k_pb(ptr),1,count,fileptrs[handle]));
    };
  };
  return(EIHNDL);
};

int32_t host_fclose(int16_t handle) {
  if ((handle>=FILEHDL)&&(handle<(FILEHDL+MAXFILES))) {
    handle-=FILEHDL;
    fclose(fileptrs[handle]);
    fileused[handle]=0;
    return(0);
  } else {
    return(EIHNDL);
  };
};

uint32_t host_gettime() {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  return((tm.tm_hour<<11)+(tm.tm_min<<5)+(tm.tm_sec>>1));
};

uint32_t host_getdate() {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  return(((tm.tm_year-80)<<9)+((tm.tm_mon+1)<<5)+(tm.tm_mday));
};

void m68k_jsr_reg(int reg) {
  uint32_t proc=m68k_rll(reg);
  switch(proc) {
    case ptr_bad_op    : m68k_jsr(func_bad_op    );break;
    case ptr_do_add    : m68k_jsr(func_do_add    );break;
    case ptr_do_sub    : m68k_jsr(func_do_sub    );break;
    case ptr_do_mul    : m68k_jsr(func_do_mul    );break;
    case ptr_do_div    : m68k_jsr(func_do_div    );break;
    case ptr_do_and    : m68k_jsr(func_do_and    );break;
    case ptr_do_or     : m68k_jsr(func_do_or     );break;
    case ptr_do_eor    : m68k_jsr(func_do_eor    );break;
    case ptr_do_mod    : m68k_jsr(func_do_mod    );break;
    case ptr_do_shftl  : m68k_jsr(func_do_shftl  );break;
    case ptr_do_shftr  : m68k_jsr(func_do_shftr  );break;
    case ptr_do_ltha   : m68k_jsr(func_do_ltha   );break;
    case ptr_do_gtha   : m68k_jsr(func_do_gtha   );break;
    case ptr_do_equl   : m68k_jsr(func_do_equl   );break;
    case ptr_do_lteq   : m68k_jsr(func_do_lteq   );break;
    case ptr_do_gteq   : m68k_jsr(func_do_gteq   );break;
    case ptr_do_nequ   : m68k_jsr(func_do_nequ   );break;
    case ptr_do_loga   : m68k_jsr(func_do_loga   );break;
    case ptr_do_logo   : m68k_jsr(func_do_logo   );break;
    case ptr_fabs      : m68k_jsr(libm_fabs      );break;
    case ptr_acos      : m68k_jsr(libm_acos      );break;
    case ptr_asin      : m68k_jsr(libm_asin      );break;
    case ptr_atan2     : m68k_jsr(libm_atan2     );break;
    case ptr_atan      : m68k_jsr(libm_atan      );break;
    case ptr_ceil      : m68k_jsr(libm_ceil      );break;
    case ptr_cosh      : m68k_jsr(libm_cosh      );break;
    case ptr_cos       : m68k_jsr(libm_cos       );break;
    case ptr_floor     : m68k_jsr(libm_floor     );break;
    case ptr_log10     : m68k_jsr(libm_log10     );break;
    case ptr_log       : m68k_jsr(libm_log       );break;
    case ptr_pow       : m68k_jsr(libm_pow       );break;
    case ptr_sin       : m68k_jsr(libm_sin       );break;
    case ptr_sinh      : m68k_jsr(libm_sinh      );break;
    case ptr_sqrt      : m68k_jsr(libm_sqrt      );break;
    case ptr_tan       : m68k_jsr(libm_tan       );break;
    case ptr_tanh      : m68k_jsr(libm_tanh      );break;
    case ptr_exp       : m68k_jsr(libm_exp       );break;
    case ptr_scs_bre   : m68k_jsr(func_scs_bre   );break;
    case ptr_scs_con   : m68k_jsr(func_scs_con   );break;
    case ptr_scs_els   : m68k_jsr(func_scs_els   );break;
    case ptr_scs_end   : m68k_jsr(func_scs_end   );break;
    case ptr_scs_end_0 : m68k_jsr(func_scs_end_0 );break;
    case ptr_scs_end_1 : m68k_jsr(func_scs_end_1 );break;
    case ptr_scs_end_2 : m68k_jsr(func_scs_end_2 );break;
    case ptr_scs_for   : m68k_jsr(func_scs_for   );break;
    case ptr_scs_if    : m68k_jsr(func_scs_if    );break;
    case ptr_scs_loo   : m68k_jsr(func_scs_loo   );break;
    case ptr_scs_rep   : m68k_jsr(func_scs_rep   );break;
    case ptr_scs_unt   : m68k_jsr(func_scs_unt   );break;
    case ptr_scs_whi   : m68k_jsr(func_scs_whi   );break;
    case ptr_cond_cm   : m68k_jsr(func_cond_cm   );break;
    case ptr_sungetc   : m68k_jsr(libc_sungetc   );break;
    case ptr_sgetc     : m68k_jsr(libc_sgetc     );break;
    case ptr_fnc_cmp   : m68k_jsr(func_fnc_cmp   );break;
    case ptr_mne_cmp   : m68k_jsr(func_mne_cmp   );break;
    case ptr_psu_cmp   : m68k_jsr(func_psu_cmp   );break;
    case ptr_opt_cmp   : m68k_jsr(func_opt_cmp   );break;
    case ptr_rev_cmp   : m68k_jsr(func_rev_cmp   );break;
    case ptr_dbg_cmp   : m68k_jsr(func_dbg_cmp   );break;
    case ptr_scs_cmp_0 : m68k_jsr(func_scs_cmp_0 );break;
    case ptr_cmp_def   : m68k_jsr(func_cmp_def   );break;
    case ptr_cmp_mac   : m68k_jsr(func_cmp_mac   );break;
    case ptr_cmp_sec   : m68k_jsr(func_cmp_sec   );break;
    case ptr_cmp_sym   : m68k_jsr(func_cmp_sym   );break;
    case ptr_cmp_mu    : m68k_jsr(func_cmp_mu    );break;

    default:
      printf("%s: %08X\n",__func__,proc);
      exit(-1);
  };
};

#include "asm56000_code.c"
#include "asm56000_data.c"

int main(int argc, char* argv[]) {
  uint32_t strptr=0x400; uint32_t adrptr=0x4;

  memptr=membot+asm56000_data_bin_len;
  m68k_rsl(A7,membot);

  srand(time(NULL));
  char* DSPASMOPT=getenv("DSPASMOPT");
  if (DSPASMOPT) {
    m68k_esl(IDB(A7),strptr);
    strcpy(m68k_pb(strptr),DSPASMOPT);
    strptr+=strlen(DSPASMOPT)+1;
  } else {
    m68k_esl(IDB(A7),0);
  };

  m68k_esl(IDB(A7),adrptr); //ARGV ARRAY PTR
  for (int i=0;i<argc;i++) {
    m68k_sl(adrptr,strptr);adrptr+=4;
    strcpy(m68k_pb(strptr),argv[i]);
    strptr+=strlen(argv[i])+1;
  };
  m68k_esl(IDB(A7),argc);

  memcpy(m68k_pb(membot),asm56000_data_bin,asm56000_data_bin_len);

  m68k_jsr(libc_main);

};
