/*
 * TRANSPILATOR asm2c transpiler runtime v0.3
 * by insane/Rabenauge^tSCc
 * https://insane.tscc.de
 */

#ifndef MEMSIZE
#define MEMSIZE 0x1000000 // needs to be 2^x for MEMMASK, defaults to 16MB
#endif
#ifndef EMUDEBUG
#define EMUDEBUG 0
#endif
#ifndef JSRTRACE
#define JSRTRACE 0
#endif
#ifndef MEMMASK
#define MEMMASK 0 // and mask memory accesses
#endif
#ifndef MEMERR
#define MEMERR 0 // error if memory above memsize is accessed
#endif


#define INLINE static inline __attribute__((always_inline))
#define OPCODE

#define m68k_bswap16(x) __builtin_bswap16(x)
#define m68k_bswap32(x) __builtin_bswap32(x)

#define SETL_NZ(tmp) flag_N=(tmp&0x80000000)?1:0;flag_Z=(tmp&0xffffffff)==0
#define SETW_NZ(tmp) flag_N=(tmp&0x8000)?1:0;flag_Z=(tmp&0xffff)==0
#define SETB_NZ(tmp) flag_N=(tmp&0x80)?1:0;flag_Z=(tmp&0xff)==0
#define SETXL_NZ(tmp) flag_N=(tmp&0x80000000)?1:0;flag_Z&=(tmp&0xffffffff)==0
#define SETXW_NZ(tmp) flag_N=(tmp&0x8000)?1:0;flag_Z&=(tmp&0xffff)==0
#define SETXB_NZ(tmp) flag_N=(tmp&0x80)?1:0;flag_Z&=(tmp&0xff)==0
#define SETADDL_C(src,dst,res) flag_C=(((src&dst)|(~res&(dst|src)))&0x80000000)?1:0
#define SETADDW_C(src,dst,res) flag_C=(res>0xFFFF)?1:0
#define SETADDB_C(src,dst,res) flag_C=(res>0xFF)?1:0
#define SETSUBL_C(src,dst,res) flag_C=(((src&res)|(~dst&(res|src)))&0x80000000)?1:0
#define SETSUBW_C(src,dst,res) flag_C=(res>0xFFFF)?1:0
#define SETSUBB_C(src,dst,res) flag_C=(res>0xFF)?1:0
#define SETADDL_V(src,dst,res) flag_V=((dst^res)&(res^src)&0x80000000)?1:0
#define SETADDW_V(src,dst,res) flag_V=((dst^res)&(res^src)&0x8000)?1:0
#define SETADDB_V(src,dst,res) flag_V=((dst^res)&(res^src)&0x80)?1:0
#define SETSUBL_V(src,dst,res) flag_V=((dst^res)&(dst^src)&0x80000000)?1:0
#define SETSUBW_V(src,dst,res) flag_V=((dst^res)&(dst^src)&0x8000)?1:0
#define SETSUBB_V(src,dst,res) flag_V=((dst^res)&(dst^src)&0x80)?1:0
#define CLR_VC flag_V=flag_C=0

#define GETBASE(BAS,MUL) (MUL*((BAS>=D0_W)?(int16_t)regs[BAS-D0_W].w:regs[BAS].l))
#define ISRC int st,int sp,int sr,int sb,int sm
#define IDST int dt,int dp,int dr,int db,int dm
#define SRC st,sp,sr,sb,sm
#define DST dt,dp,dr,db,dm
#define SIA (st==T_IIA)?T_IND:st,sp,sr,sb,sm
#define SDB (st==T_IDB)?T_IND:st,sp,sr,sb,sm
#define DIA (dt==T_IIA)?T_IND:dt,dp,dr,db,dm
#define DDB (dt==T_IDB)?T_IND:dt,dp,dr,db,dm
#define ABS(DAT) T_ABS,DAT,0,0,0
#define IMM(DAT) T_IMM,DAT,0,0,0
#define IND(REG) T_IND,0,REG,0,0
#define IDB(REG) T_IDB,0,REG,0,0
#define IIA(REG) T_IIA,0,REG,0,0
#define REG(REG) T_REG,0,REG,0,0
#define IDP(OFS,REG) T_IND,OFS,REG,0,0
#define MCCR T_CCR,0,0,0,0
#define MSR T_SR,0,0,0,0
#define PTRB(x) &mem[x]
#define RLB(x) m68k_rlb(x)
#define RLW(x) m68k_rlw(x)
#define RLL(x) m68k_rll(x)
#define RSB(x,y) m68k_rsb(x,y)
#define RSW(x,y) m68k_rsw(x,y)
#define RSL(x,y) m68k_rsl(x,y)

#if EMUDEBUG
#if JSRTRACE
#define m68k_jsr(proc) fprintf(stderr,"%s:%s\n",__func__,#proc);m68k_esl(IDB(A7),0xbad0c0de);proc()
#else
#define m68k_jsr(proc) m68k_esl(IDB(A7),0xbad0c0de);proc()
#endif
#define m68k_rts(void) if(m68k_ell(IIA(A7))!=0xbad0c0de) { printf("rts error %s:%i\n",__func__,__LINE__); exit(-1);}; return
#else
#define m68k_jsr(proc) regs[A7].l-=4;proc()
#define m68k_rts(x) regs[A7].l+=4;return
#endif
#define m68k_jmp(proc) proc();return

#define CCt  1
#define CCf  0
#define CChi (!(flag_C || flag_Z))
#define CCls (flag_C || flag_Z)
#define CCcc (!flag_C)
#define CChs CCcc
#define CCcs (flag_C)
#define CClo CCcs
#define CCne (!flag_Z)
#define CCeq (flag_Z)
#define CCvc (!flag_V)
#define CCvs (flag_V)
#define CCpl (!flag_N)
#define CCmi (flag_N)
#define CCge (!(flag_N ^ flag_V))
#define CClt (flag_N ^ flag_V)
#define CCgt (!((flag_N ^ flag_V) || flag_Z))
#define CCle ((flag_N ^ flag_V) || flag_Z)

#define m68k_bra(x) goto x

#define m68k_bhi(x) if (CChi) goto x
#define m68k_bls(x) if (CCls) goto x
#define m68k_bcc(x) if (CCcc) goto x
#define m68k_bhs(x) if (CChs) goto x
#define m68k_bcs(x) if (CCcs) goto x
#define m68k_blo(x) if (CClo) goto x
#define m68k_bne(x) if (CCne) goto x
#define m68k_beq(x) if (CCeq) goto x
#define m68k_bvc(x) if (CCvc) goto x
#define m68k_bvs(x) if (CCvs) goto x
#define m68k_bpl(x) if (CCpl) goto x
#define m68k_bmi(x) if (CCmi) goto x
#define m68k_bge(x) if (CCge) goto x
#define m68k_blt(x) if (CClt) goto x
#define m68k_bgt(x) if (CCgt) goto x
#define m68k_ble(x) if (CCle) goto x

#define m68k_dbra(x,y) if((--(regs[x].w))!=0xffff) goto y
#define m68k_dbf(x,y)  m68k_dbra(x,y)
#define m68k_dbhi(x,y) if (!CChi) m68k_dbra(x,y)
#define m68k_dbls(x,y) if (!CCls) m68k_dbra(x,y)
#define m68k_dbcc(x,y) if (!CCcc) m68k_dbra(x,y)
#define m68k_dbhs(x,y) if (!CChs) m68k_dbra(x,y)
#define m68k_dbcs(x,y) if (!CCcs) m68k_dbra(x,y)
#define m68k_dblo(x,y) if (!CClo) m68k_dbra(x,y)
#define m68k_dbne(x,y) if (!CCne) m68k_dbra(x,y)
#define m68k_dbeq(x,y) if (!CCeq) m68k_dbra(x,y)
#define m68k_dbvc(x,y) if (!CCvc) m68k_dbra(x,y)
#define m68k_dbvs(x,y) if (!CCvs) m68k_dbra(x,y)
#define m68k_dbpl(x,y) if (!CCpl) m68k_dbra(x,y)
#define m68k_dbmi(x,y) if (!CCmi) m68k_dbra(x,y)
#define m68k_dbge(x,y) if (!CCge) m68k_dbra(x,y)
#define m68k_dblt(x,y) if (!CClt) m68k_dbra(x,y)
#define m68k_dbgt(x,y) if (!CCgt) m68k_dbra(x,y)
#define m68k_dble(x,y) if (!CCle) m68k_dbra(x,y)

uint8_t mem[MEMSIZE];
static uint32_t flag_X,flag_N,flag_Z,flag_V,flag_C;
typedef union { uint8_t b; uint16_t w; uint32_t l; } REGTYPE;
static REGTYPE regs[16];
static uint16_t reg_SR;
enum { D0,D1,D2,D3,D4,D5,D6,D7,A0,A1,A2,A3,A4,A5,A6,A7 };
enum { D0_L,D1_L,D2_L,D3_L,D4_L,D5_L,D6_L,D7_L,A0_L,A1_L,A2_L,A3_L,A4_L,A5_L,A6_L,A7_L,
       D0_W,D1_W,D2_W,D3_W,D4_W,D5_W,D6_W,D7_W,A0_W,A1_W,A2_W,A3_W,A4_W,A5_W,A6_W,A7_W };
enum { T_IMM,T_ABS,T_REG,T_CCR,T_SR,T_IND,T_IDB,T_IIA };

INLINE uint32_t m68k_lb(uint32_t addr) {
#if MEMERR
  if (addr>=MEMSIZE) { printf("lb error [%08X]\n",addr); exit(-1);};
#endif
#if MEMMASK
  addr&=(MEMSIZE-1);
#endif
  return(mem[addr]);
};
INLINE uint32_t m68k_lw(uint32_t addr) {
#if MEMERR
  if (addr>=MEMSIZE) { printf("lw error [%08X]\n",addr); exit(-1);};
#endif
#if MEMMASK
  addr&=(MEMSIZE-1);
#endif
  return m68k_bswap16((*(volatile uint16_t *)&mem[addr]));
};
INLINE uint32_t m68k_ll(uint32_t addr) {
#if MEMERR
  if (addr>=MEMSIZE) { printf("ll error [%08X]\n",addr); exit(-1);};
#endif
#if MEMMASK
  addr&=(MEMSIZE-1);
#endif
  return m68k_bswap32((*(volatile uint32_t *)&mem[addr]));
};
INLINE void m68k_sb(uint32_t addr,uint32_t data) {
#if MEMERR
  if (addr>=MEMSIZE) { printf("sb error [%08X]=%02X\n",addr,data); exit(-1);};
#endif
#if MEMMASK
  addr&=(MEMSIZE-1);
#endif
  mem[addr]=data;
};
INLINE void m68k_sw(uint32_t addr,uint32_t data) {
#if MEMERR
  if (addr>=MEMSIZE) { printf("sw error [%08X]=%04X\n",addr,data); exit(-1);};
#endif
#if MEMMASK
  addr&=(MEMSIZE-1);
#endif
  *(volatile uint16_t *)&mem[addr]=m68k_bswap16(data);
};
INLINE void m68k_sl(uint32_t addr,uint32_t data) {
#if MEMERR
  if (addr>=MEMSIZE) { printf("sl error [%08X]=%08X\n",addr,data); exit(-1);};
#endif
#if MEMMASK
  addr&=(MEMSIZE-1);
#endif
  *(volatile uint32_t *)&mem[addr]=m68k_bswap32(data);
};
INLINE char* m68k_pb(uint32_t addr) {
  return((char*)&mem[addr]);
};
INLINE uint32_t m68k_rlb(int REG) {
  return(regs[REG].b);
};
INLINE uint32_t m68k_rlw(int REG) {
  return(regs[REG].w);
};
INLINE uint32_t m68k_rll(int REG) {
  return(regs[REG].l);
};
INLINE void m68k_rsb(int REG,int DAT) {
  regs[REG].b=DAT;
};
INLINE void m68k_rsw(int REG,int DAT) {
  regs[REG].w=DAT;
};
INLINE void m68k_rsl(int REG,int DAT) {
  regs[REG].l=DAT;
};
INLINE uint32_t m68k_eal(int TYP,int PTR,int REG,int BAS,int MUL) {
  uint32_t tmpl=0;
  if (TYP>=T_IND) tmpl=(PTR+regs[REG].l+GETBASE(BAS,MUL));
  if (TYP==T_ABS) tmpl=PTR;
  return(tmpl);
};

INLINE uint32_t m68k_ell(int TYP,int PTR,int REG,int BAS,int MUL) {
  uint32_t tmp=0;
  if (TYP==T_IDB) regs[REG].l-=4;
  if (TYP>=T_IND) tmp=m68k_ll(PTR+regs[REG].l+GETBASE(BAS,MUL));
  if (TYP==T_REG) tmp=regs[REG].l;
  if (TYP==T_ABS) tmp=m68k_ll(PTR);
  if (TYP==T_IMM) tmp=PTR;
  if (TYP==T_IIA) regs[REG].l+=4;
  return(tmp);
};
INLINE void m68k_esl(int TYP,int PTR,int REG,int BAS,int MUL,uint32_t DAT) {
  if (TYP==T_IDB) regs[REG].l-=4;
  if (TYP>=T_IND) m68k_sl(PTR+regs[REG].l+GETBASE(BAS,MUL),DAT);
  if (TYP==T_REG) regs[REG].l=DAT;
  if (TYP==T_ABS) m68k_sl(PTR,DAT);
  if (TYP==T_IIA) regs[REG].l+=4;
};

INLINE uint32_t m68k_elw(int TYP,int PTR,int REG,int BAS,int MUL) {
  uint32_t tmp=0;
  if (TYP==T_IDB) regs[REG].l-=2;
  if (TYP>=T_IND) tmp=m68k_lw(PTR+regs[REG].l+GETBASE(BAS,MUL));
  if (TYP==T_REG) tmp=regs[REG].w;
  if (TYP==T_ABS) tmp=m68k_lw(PTR);
  if (TYP==T_IMM) tmp=PTR;
  if (TYP==T_IIA) regs[REG].l+=2;
  return(tmp);
};
INLINE void m68k_esw(int TYP,int PTR,int REG,int BAS,int MUL,uint32_t DAT) {
  if (TYP==T_IDB) regs[REG].l-=2;
  if (TYP>=T_IND) m68k_sw(PTR+regs[REG].l+GETBASE(BAS,MUL),DAT);
  if (TYP==T_REG) regs[REG].w=DAT;
  if (TYP==T_ABS) m68k_sw(PTR,DAT);
  if (TYP==T_IIA) regs[REG].l+=2;
};

INLINE uint32_t m68k_elb(int TYP,int PTR,int REG,int BAS,int MUL) {
  uint32_t tmp=0;
  if (TYP==T_IDB) regs[REG].l-=(REG==A7)?2:1;
  if (TYP>=T_IND) tmp=m68k_lb(PTR+regs[REG].l+GETBASE(BAS,MUL));
  if (TYP==T_REG) tmp=regs[REG].b;
  if (TYP==T_ABS) tmp=m68k_lb(PTR);
  if (TYP==T_IMM) tmp=PTR;
  if (TYP==T_IIA) regs[REG].l+=(REG==A7)?2:1;
  return(tmp);
};
INLINE void m68k_esb(int TYP,int PTR,int REG,int BAS,int MUL,uint32_t DAT) {
  if (TYP==T_IDB) regs[REG].l-=(REG==A7)?2:1;
  if (TYP>=T_IND) m68k_sb(PTR+regs[REG].l+GETBASE(BAS,MUL),DAT);
  if (TYP==T_REG) regs[REG].b=DAT;
  if (TYP==T_ABS) m68k_sb(PTR,DAT);
  if (TYP==T_IIA) regs[REG].l+=(REG==A7)?2:1;
};

// ----------------------------------------------------------------------------

INLINE int m68k_getccr() {
  return(
    ((flag_X)?0x10:0) |
    ((flag_N)?0x08:0) |
    ((flag_Z)?0x04:0) |
    ((flag_V)?0x02:0) |
    ((flag_C)?0x01:0));
};
INLINE void m68k_setccr(int ccr) {
  flag_X=(ccr&0x10)?1:0;
  flag_N=(ccr&0x08)?1:0;
  flag_Z=(ccr&0x04)?1:0;
  flag_V=(ccr&0x02)?1:0;
  flag_C=(ccr&0x01)?1:0;
};
INLINE int m68k_getsr() {
  return((reg_SR&0xa700)|m68k_getccr());
};
INLINE void m68k_setsr(int sr) {
  reg_SR=sr;m68k_setccr(sr);
};

// ----------------------------------------------------------------------------

OPCODE INLINE void m68k_add_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC);uint32_t dst=m68k_elb(DIA);
  uint32_t res=dst+src;
  SETADDB_C(src,dst,res);SETADDB_V(src,dst,res);SETB_NZ(res);flag_X=flag_C;
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_add_w(ISRC,IDST) {
  uint32_t src=m68k_elw(SRC);uint32_t dst=m68k_elw(DIA);
  uint32_t res=dst+src;
  SETADDW_C(src,dst,res);SETADDW_V(src,dst,res);SETW_NZ(res);flag_X=flag_C;
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_add_l(ISRC,IDST) {
  uint32_t src=m68k_ell(SRC);uint32_t dst=m68k_ell(DIA);
  uint32_t res=dst+src;
  SETADDL_C(src,dst,res);SETADDL_V(src,dst,res);SETL_NZ(res);flag_X=flag_C;
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_adda_w(ISRC,IDST) {
  m68k_esl(DDB,m68k_ell(DIA)+(int16_t)m68k_elw(SRC));
};
OPCODE INLINE void m68k_adda_l(ISRC,IDST) {
  m68k_esl(DDB,m68k_ell(DIA)+m68k_ell(SRC));
};
OPCODE INLINE void m68k_addx_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC);uint32_t dst=m68k_elb(DIA);
  uint32_t res=dst+src+flag_X;
  SETADDB_C(src,dst,res);SETADDB_V(src,dst,res);SETXB_NZ(res);flag_X=flag_C;
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_addx_w(ISRC,IDST) {
  uint32_t src=m68k_elw(SRC);uint32_t dst=m68k_elw(DIA);
  uint32_t res=dst+src+flag_X;
  SETADDW_C(src,dst,res);SETADDW_V(src,dst,res);SETXW_NZ(res);flag_X=flag_C;
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_addx_l(ISRC,IDST) {
  uint32_t src=m68k_ell(SRC);uint32_t dst=m68k_ell(DIA);
  uint32_t res=dst+src+flag_X;
  SETADDL_C(src,dst,res);SETADDL_V(src,dst,res);SETXL_NZ(res);flag_X=flag_C;
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_and_b(ISRC,IDST) {
  uint32_t res=m68k_elb(SRC)&m68k_elb(DIA);
  CLR_VC;SETB_NZ(res);
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_and_w(ISRC,IDST) {
  uint32_t res=m68k_elw(SRC)&m68k_elw(DIA);
  CLR_VC;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_and_l(ISRC,IDST) {
  uint32_t res=m68k_ell(SRC)&m68k_ell(DIA);
  CLR_VC;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_and_ccr_b(ISRC,IDST) {
  if (dt==T_CCR) m68k_setccr(m68k_elb(SRC)&m68k_getccr());
};
OPCODE INLINE void m68k_and_sr_w(ISRC,IDST) {
  if (dt==T_SR) m68k_setsr(m68k_elw(SRC)&m68k_getsr());
};
OPCODE INLINE void m68k_asl_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elb(DIA);
  uint32_t res=(int8_t)dst<<src;
  flag_X=flag_C=src?((dst>>(8-src))&1):0;flag_V=(int8_t)res!=res;SETB_NZ(res);
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_asl_w(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elw(DIA);
  uint32_t res=(int16_t)dst<<src;
  flag_X=flag_C=src?((dst>>(16-src))&1):0;flag_V=(int16_t)res!=res;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_asl_l(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_ell(DIA);
  uint64_t res=(uint64_t)(int32_t)dst<<src;
  flag_X=flag_C=src?((dst>>(32-src))&1):0;flag_V=(int32_t)res!=res;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_asr_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elb(DIA);
  uint32_t res=(int8_t)dst>>src;
  flag_X=flag_C=src?((dst&(1<<(src-1)))?1:0):0;flag_V=0;SETB_NZ(res);
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_asr_w(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elw(DIA);
  uint32_t res=(int16_t)dst>>src;
  flag_X=flag_C=src?((dst&(1<<(src-1)))?1:0):0;flag_V=0;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_asr_l(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_ell(DIA);
  uint32_t res=(int32_t)dst>>src;
  flag_X=flag_C=src?((dst&(1<<(src-1)))?1:0):0;flag_V=0;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_bchg_b(ISRC,IDST) {
  uint32_t mask=1<<(m68k_elb(SRC)&0x1f);uint32_t dst=m68k_elb(DIA);
  uint32_t res=dst^mask;
  flag_Z=(dst&mask)==0;
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_bchg_l(ISRC,IDST) {
  uint32_t mask=1<<(m68k_ell(SRC)&0x1f);uint32_t dst=m68k_ell(DIA);
  uint32_t res=dst^mask;
  flag_Z=(dst&mask)==0;
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_bclr_b(ISRC,IDST) {
  uint32_t mask=1<<(m68k_elb(SRC)&0x1f);uint32_t dst=m68k_elb(DIA);
  uint32_t res=dst&~mask;
  flag_Z=(dst&mask)==0;
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_bclr_l(ISRC,IDST) {
  uint32_t mask=1<<(m68k_ell(SRC)&0x1f);uint32_t dst=m68k_ell(DIA);
  uint32_t res=dst&~mask;
  flag_Z=(dst&mask)==0;
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_bset_b(ISRC,IDST) {
  uint32_t mask=1<<(m68k_elb(SRC)&0x1f);uint32_t dst=m68k_elb(DIA);
  uint32_t res=dst|mask;
  flag_Z=(dst&mask)==0;
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_bset_l(ISRC,IDST) {
  uint32_t mask=1<<(m68k_ell(SRC)&0x1f);uint32_t dst=m68k_ell(DIA);
  uint32_t res=dst|mask;
  flag_Z=(dst&mask)==0;
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_btst_b(ISRC,IDST) {
  flag_Z=((1<<(m68k_elb(SRC)&0x1f))&m68k_elb(DST))==0;
};
OPCODE INLINE void m68k_btst_l(ISRC,IDST) {
  flag_Z=((1<<(m68k_ell(SRC)&0x1f))&m68k_ell(DST))==0;
};
OPCODE INLINE void m68k_clr_b(IDST) {
  flag_N=flag_V=flag_C=0;flag_Z=1;m68k_esb(DST,0);
};
OPCODE INLINE void m68k_clr_w(IDST) {
  flag_N=flag_V=flag_C=0;flag_Z=1;m68k_esw(DST,0);
};
OPCODE INLINE void m68k_clr_l(IDST) {
  flag_N=flag_V=flag_C=0;flag_Z=1;m68k_esl(DST,0);
};
OPCODE INLINE void m68k_cmp_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC);uint32_t dst=m68k_elb(DST);
  uint32_t res=dst-src;
  SETSUBB_C(src,dst,res);SETSUBB_V(src,dst,res);SETB_NZ(res);
};
OPCODE INLINE void m68k_cmp_w(ISRC,IDST) {
  uint32_t src=m68k_elw(SRC);uint32_t dst=m68k_elw(DST);
  uint32_t res=dst-src;
  SETSUBW_C(src,dst,res);SETSUBW_V(src,dst,res);SETW_NZ(res);
};
OPCODE INLINE void m68k_cmp_l(ISRC,IDST) {
  uint32_t src=m68k_ell(SRC);uint32_t dst=m68k_ell(DST);
  uint32_t res=dst-src;
  SETSUBL_C(src,dst,res);SETSUBL_V(src,dst,res);SETL_NZ(res);
};
OPCODE INLINE void m68k_cmpa_w(ISRC,IDST) {
  uint32_t src=(int16_t)m68k_elw(SRC);uint32_t dst=m68k_ell(DST);
  uint32_t res=dst-src;
  SETSUBL_C(src,dst,res);SETSUBL_V(src,dst,res);SETL_NZ(res);
};
OPCODE INLINE void m68k_cmpa_l(ISRC,IDST) {
  uint32_t src=m68k_ell(SRC);uint32_t dst=m68k_ell(DST);
  uint32_t res=dst-src;
  SETSUBL_C(src,dst,res);SETSUBL_V(src,dst,res);SETL_NZ(res);
};
OPCODE INLINE void m68k_divs_w(ISRC,IDST) {
  int32_t src=(int16_t)m68k_elw(SRC);int32_t dst=(int32_t)m68k_ell(DIA);
  int32_t rquo=dst/src;
  int32_t rrem=dst%src;
  uint32_t res=(rquo&0xffff) | ((rrem&0xffff) << 16);
  if(rquo==(int16_t)rquo) {
    CLR_VC;SETW_NZ(rquo);
  } else {
    flag_V=1;res=dst;
  };
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_divs_l(ISRC,IDST) {
  uint32_t res=(int32_t)m68k_ell(DIA)/(int32_t)m68k_ell(SRC);
  flag_V=flag_C=0;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_divu_w(ISRC,IDST) {
  uint32_t src=m68k_elw(SRC);uint32_t dst=m68k_ell(DIA);
  uint32_t rquo=dst/src;
  uint32_t rrem=dst%src;
  uint32_t res=(rquo&0xffff) | ((rrem&0xffff) << 16);
  if(rquo<0x10000) {
    CLR_VC;SETW_NZ(rquo);
  } else {
    flag_V=1;res=dst;
  };
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_divu_l(ISRC,IDST) {
  uint32_t res=m68k_ell(DIA)/m68k_ell(SRC);
  flag_V=flag_C=0;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_eor_b(ISRC,IDST) {
  uint32_t res=m68k_elb(SRC)^m68k_elb(DIA);
  CLR_VC;SETB_NZ(res);
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_eor_w(ISRC,IDST) {
  uint32_t res=m68k_elw(SRC)^m68k_elw(DIA);
  CLR_VC;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_eor_l(ISRC,IDST) {
  uint32_t res=m68k_ell(SRC)^m68k_ell(DIA);
  CLR_VC;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_eor_ccr_b(ISRC,IDST) {
  if (dt==T_CCR) m68k_setccr(m68k_elb(SRC)^m68k_getccr());
};
OPCODE INLINE void m68k_eor_sr_w(ISRC,IDST) {
  if (dt==T_SR) m68k_setsr(m68k_elw(SRC)^m68k_getsr());
};
OPCODE INLINE void m68k_exg_l(ISRC,IDST) {
  uint32_t src=m68k_ell(SDB);m68k_esl(SIA,m68k_ell(DDB));m68k_esl(DIA,src);
};
OPCODE INLINE void m68k_extb_l(IDST) {
  uint32_t res=(int8_t)m68k_elb(DIA);
  CLR_VC;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_ext_w(IDST) {
  uint32_t res=(int8_t)m68k_elb(DIA);
  CLR_VC;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_ext_l(IDST) {
  uint32_t res=(int16_t)m68k_elw(DIA);
  CLR_VC;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_lea_l(ISRC,IDST) {
  m68k_esl(DST,m68k_eal(SRC));
}
OPCODE INLINE void m68k_link_w(ISRC,IDST) {
  m68k_esl(IDB(A7),m68k_ell(SDB));
  m68k_esl(SIA,m68k_rll(A7));
  m68k_rsl(A7,m68k_rll(A7)+(int16_t)m68k_elw(DST));
};
OPCODE INLINE void m68k_lsl_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elb(DIA);
  uint32_t res=dst<<src;
  flag_X=flag_C=(dst>>(8-src))&1;flag_V=0;SETB_NZ(res);
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_lsl_w(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elw(DIA);
  uint32_t res=dst<<src;
  flag_X=flag_C=(dst>>(16-src))&1;flag_V=0;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_lsl_l(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_ell(DIA);
  uint32_t res=dst<<src;
  flag_X=flag_C=(dst>>(32-src))&1;flag_V=0;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_lsr_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elb(DIA);
  uint32_t res=dst>>src;
  flag_X=flag_C=(dst>>(src-1))&1;flag_V=0;SETB_NZ(res);
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_lsr_w(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elw(DIA);
  uint32_t res=dst>>src;
  flag_X=flag_C=(dst>>(src-1))&1;flag_V=0;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_lsr_l(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_ell(DIA);
  uint32_t res=dst>>src;
  flag_X=flag_C=(dst>>(src-1))&1;flag_V=0;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_move_b(ISRC,IDST) {
  uint32_t res=m68k_elb(SRC);
  CLR_VC;SETB_NZ(res);
  m68k_esb(DST,res);
};
OPCODE INLINE void m68k_move_w(ISRC,IDST) {
  uint32_t res=m68k_elw(SRC);
  CLR_VC;SETW_NZ(res);
  m68k_esw(DST,res);
};
OPCODE INLINE void m68k_move_l(ISRC,IDST) {
  uint32_t res=m68k_ell(SRC);
  CLR_VC;SETL_NZ(res);
  m68k_esl(DST,res);
};
OPCODE INLINE void m68k_movea_w(ISRC,IDST) {
  m68k_esl(DST,(int16_t)m68k_elw(SRC));
};
OPCODE INLINE void m68k_movea_l(ISRC,IDST) {
  m68k_esl(DST,m68k_ell(SRC));
};
OPCODE INLINE void m68k_movem_w(ISRC,IDST) {
  if (dt==T_REG) { // expand into reg
    m68k_esl(DST,(int16_t)m68k_elw(SRC));
  } else {
    m68k_esw(DST,m68k_elw(SRC));
  };
};
OPCODE INLINE void m68k_movem_l(ISRC,IDST) {
  m68k_esl(DST,m68k_ell(SRC));
};
OPCODE INLINE void m68k_movep_w(ISRC,IDST) {
  if (st==T_REG) { //expand into mem
    uint32_t dat=m68k_elw(SRC);m68k_esb(DST,dat>>8);
    dp+=2;m68k_esb(DST,dat);
  } else if (dt==T_REG) { //compress into reg
    uint32_t dat=(m68k_elb(SRC))<<8;
    sp+=2;dat|=m68k_elb(SRC);
    m68k_esw(DST,dat);
  };
};
OPCODE INLINE void m68k_movep_l(ISRC,IDST) {
  if (st==T_REG) { //expand into mem
    uint32_t dat=m68k_ell(SRC);m68k_esb(DST,dat>>24);
    dp+=2;m68k_esb(DST,dat>>16);
    dp+=2;m68k_esb(DST,dat>>8);
    dp+=2;m68k_esb(DST,dat);
  } else if (dt==T_REG) { //compress into reg
    uint32_t dat=(m68k_elb(SRC))<<24;
    sp+=2;dat|=m68k_elb(SRC)<<16;
    sp+=2;dat|=m68k_elb(SRC)<<8;
    sp+=2;dat|=m68k_elb(SRC);
    m68k_esl(DST,dat);
  };
};
OPCODE INLINE void m68k_move_ccr_w(ISRC,IDST) {
  if(st==T_CCR) m68k_esw(DST,m68k_getccr());else if(dt==T_CCR) m68k_setccr(m68k_elw(SRC));
};
OPCODE INLINE void m68k_move_sr_w(ISRC,IDST) {
  if(st==T_SR) m68k_esw(DST,m68k_getsr());else if(dt==T_SR) m68k_setsr(m68k_elw(SRC));
};
OPCODE INLINE void m68k_muls_w(ISRC,IDST) {
  int32_t res=(int16_t)m68k_elw(SRC)*(int16_t)m68k_elw(DIA);
  CLR_VC;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_muls_l(ISRC,IDST) {
  int64_t resq=(int64_t)((int32_t)m68k_ell(SRC))*(int64_t)((int32_t)m68k_ell(DIA));
  flag_V=(int64_t)resq!=(int32_t)resq;flag_C=0;SETL_NZ(resq);
  m68k_esl(DDB,resq);
};
OPCODE INLINE void m68k_mulu_w(ISRC,IDST) {
  uint32_t res=(uint32_t)m68k_elw(SRC)*(uint32_t)m68k_elw(DIA);
  CLR_VC;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_mulu_l(ISRC,IDST) {
  uint64_t resq=(uint64_t)m68k_ell(SRC)*(uint64_t)m68k_ell(DIA);
  flag_V=(resq>0xffffffff)?1:0;flag_C=0;SETL_NZ(resq);
  m68k_esl(DDB,resq);
};
OPCODE INLINE void m68k_not_b(IDST) {
  uint32_t res=m68k_elb(DIA)^0xff;
  CLR_VC;SETB_NZ(res);
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_not_w(IDST) {
  uint32_t res=m68k_elw(DIA)^0xffff;
  CLR_VC;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_not_l(IDST) {
  uint32_t res=m68k_ell(DIA)^0xffffffff;
  CLR_VC;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_neg_b(IDST) {
  uint32_t src=m68k_elb(DIA);uint32_t res=0-src;
  SETSUBB_C(src,0,res);SETSUBB_V(src,0,res);SETB_NZ(res);flag_X=flag_C;
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_neg_w(IDST) {
  uint32_t src=m68k_elw(DIA);uint32_t res=0-src;
  SETSUBW_C(src,0,res);SETSUBW_V(src,0,res);SETW_NZ(res);flag_X=flag_C;
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_neg_l(IDST) {
  uint32_t src=m68k_ell(DIA);uint32_t res=0-src;
  SETSUBL_C(src,0,res);SETSUBL_V(src,0,res);SETL_NZ(res);flag_X=flag_C;
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_negx_b(IDST) {
  uint32_t src=m68k_elb(DIA);uint32_t res=0-src-flag_X;
 SETSUBB_C(src,0,res);SETSUBB_V(src,0,res);SETXB_NZ(res);flag_X=flag_C;
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_negx_w(IDST) {
  uint32_t src=m68k_elw(DIA);uint32_t res=0-src-flag_X;
  SETSUBW_C(src,0,res);SETSUBW_V(src,0,res);SETXW_NZ(res);flag_X=flag_C;
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_negx_l(IDST) {
  uint32_t src=m68k_ell(DIA);uint32_t res=0-src-flag_X;
  SETSUBL_C(src,0,res);SETSUBL_V(src,0,res);SETXL_NZ(res);flag_X=flag_C;
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_or_b(ISRC,IDST) {
  uint32_t res=m68k_elb(SRC)|m68k_elb(DIA);
  CLR_VC;SETB_NZ(res);
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_or_w(ISRC,IDST) {
  uint32_t res=m68k_elw(SRC)|m68k_elw(DIA);
  CLR_VC;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_or_l(ISRC,IDST) {
  uint32_t res=m68k_ell(SRC)|m68k_ell(DIA);
  CLR_VC;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_or_ccr_b(ISRC,IDST) {
  if (dt==T_CCR) m68k_setccr(m68k_elb(SRC)|m68k_getccr());
};
OPCODE INLINE void m68k_or_sr_w(ISRC,IDST) {
  if (dt==T_SR) m68k_setsr(m68k_elw(SRC)|m68k_getsr());
};
OPCODE INLINE void m68k_pea_l(ISRC) {
  m68k_esl(IDB(A7),m68k_eal(SRC));
}
OPCODE INLINE void m68k_rol_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elb(DIA);
  int shft=src&7; uint32_t res=dst<<shft|dst>>(8-shft);
  flag_C=shft?((dst&(0x80>>(shft-1)))?1:0):src?dst&1:0;flag_V=0;SETB_NZ(res);
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_rol_w(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elw(DIA);
  int shft=src&15; uint32_t res=dst<<shft|dst>>(16-shft);
  flag_C=shft?((dst&(0x8000>>(shft-1)))?1:0):src?dst&1:0;flag_V=0;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_rol_l(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_ell(DIA);
  int shft=src&31; uint32_t res=dst<<shft|dst>>(32-shft);
  flag_C=shft?((dst&(0x80000000>>(shft-1)))?1:0):src?dst&1:0;flag_V=0;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_ror_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elb(DIA);
  int shft=src&7; uint32_t res=dst>>shft|dst<<(8-shft);
  flag_C=src?((dst>>((shft-1)&7))&1):0;flag_V=0;SETB_NZ(res);
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_ror_w(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elw(DIA);
  int shft=src&15; uint32_t res=dst>>shft|dst<<(16-shft);
  flag_C=src?((dst>>((shft-1)&15))&1):0;flag_V=0;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_ror_l(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_ell(DIA);
  int shft=src&31; uint32_t res=dst>>shft|dst<<(32-shft);
  flag_C=src?((dst>>((shft-1)&31))&1):0;flag_V=0;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_roxl_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elb(DIA)|(flag_X<<8);
  int shft=src%9; uint32_t res=dst<<shft|dst>>(9-shft);
  flag_C=flag_X=(res&0x100)?1:0;flag_V=0;SETB_NZ(res);
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_roxl_w(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elw(DIA)|(flag_X<<16);
  int shft=src%17; uint32_t res=dst<<shft|dst>>(17-shft);
  flag_C=flag_X=(res&0x10000)?1:0;flag_V=0;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_roxl_l(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint64_t dst=m68k_ell(DIA)|((uint64_t)flag_X<<32);
  int shft=src%33; uint64_t res=dst<<shft|dst>>(33-shft);
  flag_C=flag_X=(res&0x100000000)?1:0;flag_V=0;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_roxr_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elb(DIA)|(flag_X<<8);
  int shft=src%9; uint32_t res=dst>>shft|dst<<(9-shft);
  flag_C=flag_X=(res&0x100)?1:0;flag_V=0;SETB_NZ(res);
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_roxr_w(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint32_t dst=m68k_elw(DIA)|(flag_X<<16);
  int shft=src%17; uint32_t res=dst>>shft|dst<<(17-shft);
  flag_C=flag_X=(res&0x10000)?1:0;flag_V=0;SETW_NZ(res);
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_roxr_l(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC)&63;uint64_t dst=m68k_ell(DIA)|((uint64_t)flag_X<<32);
  int shft=src%33; uint64_t res=dst>>shft|dst<<(33-shft);
  flag_C=flag_X=(res&0x100000000)?1:0;flag_V=0;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_shi_b(IDST) {
  m68k_esb(DST,CChi?0xff:0x00);
};
OPCODE INLINE void m68k_sls_b(IDST) {
  m68k_esb(DST,CCls?0xff:0x00);
};
OPCODE INLINE void m68k_shs_b(IDST) {
  m68k_esb(DST,CChs?0xff:0x00);
};
OPCODE INLINE void m68k_scc_b(IDST) {
  m68k_esb(DST,CCcc?0xff:0x00);
};
OPCODE INLINE void m68k_slo_b(IDST) {
  m68k_esb(DST,CClo?0xff:0x00);
};
OPCODE INLINE void m68k_scs_b(IDST) {
  m68k_esb(DST,CCcs?0xff:0x00);
};
OPCODE INLINE void m68k_sne_b(IDST) {
  m68k_esb(DST,CCne?0xff:0x00);
};
OPCODE INLINE void m68k_seq_b(IDST) {
  m68k_esb(DST,CCeq?0xff:0x00);
};
OPCODE INLINE void m68k_svc_b(IDST) {
  m68k_esb(DST,CCvc?0xff:0x00);
};
OPCODE INLINE void m68k_svs_b(IDST) {
  m68k_esb(DST,CCvs?0xff:0x00);
};
OPCODE INLINE void m68k_spl_b(IDST) {
  m68k_esb(DST,CCpl?0xff:0x00);
};
OPCODE INLINE void m68k_smi_b(IDST) {
  m68k_esb(DST,CCmi?0xff:0x00);
};
OPCODE INLINE void m68k_sge_b(IDST) {
  m68k_esb(DST,CCge?0xff:0x00);
};
OPCODE INLINE void m68k_slt_b(IDST) {
  m68k_esb(DST,CClt?0xff:0x00);
};
OPCODE INLINE void m68k_sgt_b(IDST) {
  m68k_esb(DST,CCgt?0xff:0x00);
};
OPCODE INLINE void m68k_sle_b(IDST) {
  m68k_esb(DST,CCle?0xff:0x00);
};
OPCODE INLINE void m68k_sub_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC);uint32_t dst=m68k_elb(DIA);
  uint32_t res=dst-src;
  SETSUBB_C(src,dst,res);SETSUBB_V(src,dst,res);SETB_NZ(res);flag_X=flag_C;
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_sub_w(ISRC,IDST) {
  uint32_t src=m68k_elw(SRC);uint32_t dst=m68k_elw(DIA);
  uint32_t res=dst-src;
  SETSUBW_C(src,dst,res);SETSUBW_V(src,dst,res);SETW_NZ(res);flag_X=flag_C;
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_sub_l(ISRC,IDST) {
  uint32_t src=m68k_ell(SRC);uint32_t dst=m68k_ell(DIA);
  uint32_t res=dst-src;
  SETSUBL_C(src,dst,res);SETSUBL_V(src,dst,res);SETL_NZ(res);flag_X=flag_C;
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_suba_w(ISRC,IDST) {
  m68k_esl(DDB,m68k_ell(DIA)-(int16_t)m68k_elw(SRC));
};
OPCODE INLINE void m68k_suba_l(ISRC,IDST) {
  m68k_esl(DDB,m68k_ell(DIA)-m68k_ell(SRC));
};
OPCODE INLINE void m68k_subx_b(ISRC,IDST) {
  uint32_t src=m68k_elb(SRC);uint32_t dst=m68k_elb(DIA);
  uint32_t res=dst-src-flag_X;
  SETSUBB_C(src,dst,res);SETSUBB_V(src,dst,res);SETXB_NZ(res);flag_X=flag_C;
  m68k_esb(DDB,res);
};
OPCODE INLINE void m68k_subx_w(ISRC,IDST) {
  uint32_t src=m68k_elw(SRC);uint32_t dst=m68k_elw(DIA);
  uint32_t res=dst-src-flag_X;
  SETSUBW_C(src,dst,res);SETSUBW_V(src,dst,res);SETXW_NZ(res);flag_X=flag_C;
  m68k_esw(DDB,res);
};
OPCODE INLINE void m68k_subx_l(ISRC,IDST) {
  uint32_t src=m68k_ell(SRC);uint32_t dst=m68k_ell(DIA);
  uint32_t res=dst-src-flag_X;
  SETSUBL_C(src,dst,res);SETSUBL_V(src,dst,res);SETXL_NZ(res);flag_X=flag_C;
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_swap_w(IDST) {
  uint32_t res=m68k_ell(DIA);
  res=(((res&0xffff0000)>>16)|((res&0xffff)<<16));CLR_VC;SETL_NZ(res);
  m68k_esl(DDB,res);
};
OPCODE INLINE void m68k_tas_b(IDST) {
  uint32_t res=m68k_elb(DIA);
  CLR_VC;SETB_NZ(res);
  m68k_esb(DDB,res|0x80);
};
OPCODE INLINE void m68k_tst_b(ISRC) {
  uint32_t res=m68k_elb(SRC);CLR_VC;SETB_NZ(res);
};
OPCODE INLINE void m68k_tst_w(ISRC) {
  uint32_t res=m68k_elw(SRC);CLR_VC;SETW_NZ(res);
};
OPCODE INLINE void m68k_tst_l(ISRC) {
  uint32_t res=m68k_ell(SRC);CLR_VC;SETL_NZ(res);
};
OPCODE INLINE void m68k_unlk_w(ISRC) {
  m68k_rsl(A7,m68k_ell(SDB));
  m68k_esl(SIA,m68k_ell(IIA(A7)));
};

