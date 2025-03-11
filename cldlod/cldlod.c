/* $Id: cldlod.c,v 1.46 1999/03/19 19:51:39 jay Exp $ */
// 64bit fixed by insane/tSCc 19.09.2018
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#if defined ( __WATCOMC__ ) || defined( __GNUC__ ) || defined( MPW )
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#if !defined ( MPW )
#include <sys/types.h>
#endif

#define SKIPSYMBOLS 0
/* Headers for working with COFF files */
#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif
// - coreaddr.h --------------------------------------------------------------
enum memory_map{memory_map_p=0,
memory_map_x,memory_map_y,memory_map_l,memory_map_none,
memory_map_laa,memory_map_lab,memory_map_lba,memory_map_lbb,memory_map_le,
memory_map_li,memory_map_pa,memory_map_pb,memory_map_pe,memory_map_pi,
memory_map_pr,memory_map_xa,memory_map_xb,memory_map_xe,memory_map_xi,
memory_map_xr,memory_map_ya,memory_map_yb,memory_map_ye,memory_map_yi,
memory_map_yr,memory_map_pt,memory_map_pf,memory_map_emi,
memory_map_e0,memory_map_e1,memory_map_e2,memory_map_e3,
memory_map_e4,memory_map_e5,memory_map_e6,memory_map_e7,
memory_map_e8,memory_map_e9,memory_map_e10,memory_map_e11,
memory_map_e12,memory_map_e13,memory_map_e14,memory_map_e15,
memory_map_e16,memory_map_e17,memory_map_e18,memory_map_e19,
memory_map_e20,memory_map_e21,memory_map_e22,memory_map_e23,
memory_map_e24,memory_map_e25,memory_map_e26,memory_map_e27,
memory_map_e28,memory_map_e29,memory_map_e30,memory_map_e31,
memory_map_e32,memory_map_e33,memory_map_e34,memory_map_e35,
memory_map_e36,memory_map_e37,memory_map_e38,memory_map_e39,
memory_map_e40,memory_map_e41,memory_map_e42,memory_map_e43,
memory_map_e44,memory_map_e45,memory_map_e46,memory_map_e47,
memory_map_e48,memory_map_e49,memory_map_e50,memory_map_e51,
memory_map_e52,memory_map_e53,memory_map_e54,memory_map_e55,
memory_map_e56,memory_map_e57,memory_map_e58,memory_map_e59,
memory_map_e60,memory_map_e61,memory_map_e62,memory_map_e63,

memory_map_e64,memory_map_e65,memory_map_e66,memory_map_e67,
memory_map_e68,memory_map_e69,memory_map_e70,memory_map_e71,
memory_map_e72,memory_map_e73,memory_map_e74,memory_map_e75,
memory_map_e76,memory_map_e77,memory_map_e78,memory_map_e79,
memory_map_e80,memory_map_e81,memory_map_e82,memory_map_e83,
memory_map_e84,memory_map_e85,memory_map_e86,memory_map_e87,
memory_map_e88,memory_map_e89,memory_map_e90,memory_map_e91,
memory_map_e92,memory_map_e93,memory_map_e94,memory_map_e95,
memory_map_e96,memory_map_e97,memory_map_e98,memory_map_e99,

memory_map_e100,memory_map_e101,memory_map_e102,memory_map_e103,
memory_map_e104,memory_map_e105,memory_map_e106,memory_map_e107,
memory_map_e108,memory_map_e109,memory_map_e110,memory_map_e111,
memory_map_e112,memory_map_e113,memory_map_e114,memory_map_e115,
memory_map_e116,memory_map_e117,memory_map_e118,memory_map_e119,
memory_map_e120,memory_map_e121,memory_map_e122,memory_map_e123,
memory_map_e124,memory_map_e125,memory_map_e126,memory_map_e127,
memory_map_e128,memory_map_e129,memory_map_e130,memory_map_e131,
memory_map_e132,memory_map_e133,memory_map_e134,memory_map_e135,
memory_map_e136,memory_map_e137,memory_map_e138,memory_map_e139,
memory_map_e140,memory_map_e141,memory_map_e142,memory_map_e143,
memory_map_e144,memory_map_e145,memory_map_e146,memory_map_e147,
memory_map_e148,memory_map_e149,memory_map_e150,memory_map_e151,
memory_map_e152,memory_map_e153,memory_map_e154,memory_map_e155,
memory_map_e156,memory_map_e157,memory_map_e158,memory_map_e159,
memory_map_e160,memory_map_e161,memory_map_e162,memory_map_e163,
memory_map_e164,memory_map_e165,memory_map_e166,memory_map_e167,
memory_map_e168,memory_map_e169,memory_map_e170,memory_map_e171,
memory_map_e172,memory_map_e173,memory_map_e174,memory_map_e175,
memory_map_e176,memory_map_e177,memory_map_e178,memory_map_e179,
memory_map_e180,memory_map_e181,memory_map_e182,memory_map_e183,
memory_map_e184,memory_map_e185,memory_map_e186,memory_map_e187,
memory_map_e188,memory_map_e189,memory_map_e190,memory_map_e191,
memory_map_e192,memory_map_e193,memory_map_e194,memory_map_e195,
memory_map_e196,memory_map_e197,memory_map_e198,memory_map_e199,

memory_map_e200,memory_map_e201,memory_map_e202,memory_map_e203,
memory_map_e204,memory_map_e205,memory_map_e206,memory_map_e207,
memory_map_e208,memory_map_e209,memory_map_e210,memory_map_e211,
memory_map_e212,memory_map_e213,memory_map_e214,memory_map_e215,
memory_map_e216,memory_map_e217,memory_map_e218,memory_map_e219,
memory_map_e220,memory_map_e221,memory_map_e222,memory_map_e223,
memory_map_e224,memory_map_e225,memory_map_e226,memory_map_e227,
memory_map_e228,memory_map_e229,memory_map_e230,memory_map_e231,
memory_map_e232,memory_map_e233,memory_map_e234,memory_map_e235,
memory_map_e236,memory_map_e237,memory_map_e238,memory_map_e239,
memory_map_e240,memory_map_e241,memory_map_e242,memory_map_e243,
memory_map_e244,memory_map_e245,memory_map_e246,memory_map_e247,
memory_map_e248,memory_map_e249,memory_map_e250,memory_map_e251,
memory_map_e252,memory_map_e253,memory_map_e254,memory_map_e255,
memory_map_dm,
memory_map_p8,memory_map_u,memory_map_u8,memory_map_u16,
memory_map_xs,
memory_map_ys,
memory_map_maxp1, /* should stay at last valid memory map plus 1 */
memory_map_error=666666,
memory_map_ic = memory_map_pt
};

union addr_map{
  enum memory_map mape;int32_t l; uint32_t u;
  };
struct core_addr_struct {
   union addr_map w0;
   union addr_map w1;
};

typedef struct core_addr_struct CORE_ADDR;
#define CORE_ADDR_ADDR(ca) ((ca).w0.l)
#define CORE_ADDR_ADDRU(ca) ((ca).w0.u)
#define CORE_ADDR_MAP(ca) ((ca).w1.mape)
// - coreaddr.h --------------------------------------------------------------

// - maout.h -----------------------------------------------------------------

// - maout.h - filehdr.h -----------------------------------------------------
struct filehdr {
	uint32_t f_magic;	/* magic number */
	uint32_t f_nscns;	/* number of sections */
	int32_t  f_timdat;	/* time & date stamp */
	int32_t  f_symptr;	/* file pointer to symbolic header */
	int32_t  f_nsyms;	/* sizeof(symbolic hdr) */
	uint32_t f_opthdr;	/* sizeof(optional hdr) */
	uint32_t f_flags;	/* flags */
	};

/*
 *   Bits for f_flags:
 *
 *	F_RELFLG	relocation info stripped from file
 *	F_EXEC		file is executable  (i.e. no unresolved
 *				externel references)
 *	F_LNNO		line nunbers stripped from file
 *	F_LSYMS		local symbols stripped from file
 *	F_MINMAL	this is a minimal object file (".m") output of fextract
 *	F_UPDATE	this is a fully bound update file, output of ogen
 *	F_SWABD		this file has had its bytes swabbed (in names)
 *	F_AR16WR	this file has the byte ordering of an AR16WR (e.g. 11/70) machine
 *				(it was created there, or was produced by conv)
 *	F_AR32WR	this file has the byte ordering of an AR32WR machine(e.g. vax)
 *	F_AR32W		this file has the byte ordering of an AR32W machine (e.g. 3b,maxi,MC68000)
 *	F_PATCH		file contains "patch" list in optional header
 *	F_NODF		(minimal file only) no decision functions for
 *				replaced functions
 */

#define  F_RELFLG	0000001L
#define  F_EXEC		0000002L
#define  F_LNNO		0000004L
#define  F_LSYMS	0000010L
#define  F_MINMAL	0000020L
#define  F_UPDATE	0000040L
#define  F_SWABD	0000100L
#define  F_AR16WR	0000200L
#define  F_AR32WR	0000400L
#define  F_AR32W	0001000L
#define  F_PATCH	0002000L
#define  F_NODF		0002000L


/* Motorola 56000/96000/56100/56300/56800/56600/SC400 */
#define   M56KMAGIC 709L
#define   M96KMAGIC 710L
#define   M16KMAGIC 711L
#define   M563MAGIC 712L
#define   M568MAGIC 713L
#define   M566MAGIC 714L
#define   SC1MAGIC 715L
#define   M567MAGIC 716L

#define	FILHDR	struct filehdr
#define	FILHSZ	sizeof(FILHDR)

/*
Don't use line continuation (e.g. backslash at end of line)
in the following macro.  It causes problems with the PC compilers
when newline characters are converted.
*/
#define ISCOFF(x) (((x)==M56KMAGIC) || ((x)==M96KMAGIC) || ((x)==M16KMAGIC) || ((x)==M563MAGIC) || ((x)==M568MAGIC) || ((x)==M566MAGIC) || ((x)==SC1MAGIC) || ((x)==M567MAGIC))
// - maout.h - filehdr.h -----------------------------------------------------

// - maout.h - aouthdr.h -----------------------------------------------------
/* Values for the magic field in aouthdr
 */
#define	OMAGIC	0407L
#define	NMAGIC	0410L
#define	ZMAGIC	0413L
#define	LIBMAGIC	0443L
#define	N_BADMAG(x) (((x).magic)!=OMAGIC && ((x).magic)!=NMAGIC && ((x).magic)!=ZMAGIC && ((x).magic)!=LIBMAGIC)

typedef	struct aouthdr {
	int32_t	magic;		/* see above				*/
	int32_t	vstamp;		/* version stamp			*/
	int32_t	tsize;		/* text size in words */
	int32_t	dsize;		/* initialized data "  "		*/
	int32_t	bsize;		/* uninitialized data "   "		*/

	CORE_ADDR entry;		/* entry pt.				*/
	CORE_ADDR text_start;	/* base of text used for this file	*/
	CORE_ADDR data_start;	/* base of data used for this file	*/
	CORE_ADDR text_end;	/* end address of text used for this file	*/
	CORE_ADDR data_end;	/* end address of data used for this file	*/

} AOUTHDR;
#define AOUTHSZ sizeof(AOUTHDR)
// - maout.h - aouthdr.h -----------------------------------------------------

// - maout.h - scnhdr.h ------------------------------------------------------
struct scnhdr {
	char		s_name[8];	/* section name */
	CORE_ADDR	s_paddr;	/* physical address, aliased s_nlib */
	CORE_ADDR	s_vaddr;	/* virtual address */
	int32_t		s_size;		/* section size */
	int32_t		s_scnptr;	/* file ptr to raw data for section */
	int32_t		s_relptr;	/* file ptr to relocation */
	int32_t		s_lnnoptr;	/* file ptr to gp histogram */
	uint32_t	s_nreloc;	/* number of relocation entries */
	uint32_t	s_nlnno;	/* number of gp histogram entries */
	int32_t		s_flags;	/* flags */
	};

/* the number of shared libraries in a .lib section in an absolute output file
 * is put in the s_paddr field of the .lib section header, the following define
 * allows it to be referenced as s_nlib
 */

#define s_nlib	s_paddr
#define	SCNHDR	struct scnhdr
#define	SCNHSZ	sizeof(SCNHDR)


/*
 * Define constants for names of "special" sections
 */

#define	_TEXT	".text"
#define	_DATA	".data"
#define	_BSS	".bss"
#define	_TV	".tv"
#define _INIT ".init"
#define _FINI ".fini"
#define _LIB ".lib"

/*
 * The low 4 bits of s_flags is used as a section "type"
 */

#define STYP_REG	0x00L		/* "regular" section:
						allocated, relocated, loaded */
#define STYP_DSECT	0x01L		/* "dummy" section:
						not allocated, relocated,
						not loaded */
#define STYP_NOLOAD	0x02L		/* "noload" section:
						allocated, relocated,
						 not loaded */
#define STYP_GROUP	0x04L		/* "grouped" section:
						formed of input sections */
#define STYP_PAD	0x08L		/* "padding" section:
						not allocated, not relocated,
						 loaded */
#define STYP_COPY	0x10L		/* "copy" section:
						for decision function used
						by field update;  not
						allocated, not relocated,
						loaded;  reloc & lineno
						entries processed normally */
#define	STYP_TEXT	0x20L		/* section contains text only */
#define STYP_DATA	0x40L		/* section contains data only */
#define STYP_BSS	0x80L		/* section contains bss only */
#define STYP_DEBUG 0x0100L /* DWARF debug section */

/*
 *  In a minimal file or an update file, a new function
 *  (as compared with a replaced function) is indicated by S_NEWFCN
 */

#define S_NEWFCN  0x100L

/*
 * In 3b Update Files (output of ogen), sections which appear in SHARED
 * segments of the Pfile will have the S_SHRSEG flag set by ogen, to inform
 * dufr that updating 1 copy of the proc. will update all process invocations.
 */
#define S_SHRSEG	0x20L

/*
 * Motorola DSP section header flags
 */
#define STYP_BLOCK	0x400L
#define STYP_OVERLAY	0x800L
#define STYP_MACRO	0x1000L
#define STYP_BW		0x2000L
#define STYP_OVERLAYP   0x4000L

/*
 * Flags added for block define pseudo instructions to identify run-time space.
 */
#define STYP_P_RUNSPACE		0x10000L
#define STYP_X_RUNSPACE		0x20000L
#define STYP_Y_RUNSPACE		0x40000L
#define STYP_EMI_RUNSPACE	0x80000L
// - maout.h - scnhdr.h ------------------------------------------------------

// - maout.h - reloc.h -------------------------------------------------------
struct reloc {
	int32_t  r_vaddr;	/* (virtual) address of reference */
	int32_t  r_symndx;	/* index into symbol table */
	uint32_t r_type;	/* relocation type */
	};

/*
 *   relocation types for all products and generics
 */

/*
 * All generics
 *	reloc. already performed to symbol in the same section
 */
#define  R_ABS		0L
#define RELOC  struct reloc
#define RELSZ sizeof(RELOC)
// - maout.h - reloc.h -------------------------------------------------------

// - maout.h - linenum.h -----------------------------------------------------
/*  There is one line number entry for every 
    "breakpointable" source line in a section.
    Line numbers are grouped on a per function
    basis; the first entry in a function grouping
    will have l_lnno = 0 and in place of physical
    address will be the symbol table index of
    the function name.
*/
struct lineno
{
	union
	{
		int32_t	l_symndx ;	/* sym. table index of function name
						iff l_lnno == 0      */
		CORE_ADDR l_paddr ;	/* (physical) address of line number */
	}		l_addr ;
	uint32_t	l_lnno ;	/* line number */
} ;

#define	LINENO	struct lineno
#define	LINESZ	(sizeof(LINENO)) 
// - maout.h - linenum.h -----------------------------------------------------

// - maout.h - syms.h --------------------------------------------------------
/*	Defines for "special" symbols   */

#define _ETEXT	"etext"
#define _EDATA	"edata"
#define _END	"end"

#define _START	"__start"

/*		Storage Classes are defined in storclas.h  */

/*		Number of characters in a symbol name */
#define  SYMNMLEN	8
/*		Number of characters in a file name */
#define  FILNMLEN	16
/*		Number of array dimensions in auxiliary entry */
#define  DIMNUM		4


struct syment
{
	union
	{
		char		_n_name[SYMNMLEN];	/* old COFF version */
		struct
		{
			int32_t	_n_zeroes;	/* new == 0 */
			int32_t	_n_offset;	/* offset into string table */
		} _n_n;
		uint32_t	_n_nptr[2];	/* allows for overlaying, 64bit fix: was pointer */
	} _n;
	union {
		CORE_ADDR _n_address; /* when an address */
		uint32_t _n_val[2]; /* _n_val[0] is least significant */
		}_n_value;	/* value of symbol */
	int32_t			n_scnum;	/* section number */
	uint32_t		n_type;		/* type and derived type */
	int32_t			n_sclass;	/* storage class */
	int32_t			n_numaux;	/* number of aux. entries */
};

#define n_name		_n._n_name
#define n_nptr		_n._n_nptr[1]
#define n_zeroes	_n._n_n._n_zeroes
#define n_offset	_n._n_n._n_offset
#define n_value _n_value._n_address
/*
   Relocatable symbols have a section number of the
   section in which they are defined.  Otherwise, section
   numbers have the following meanings:
*/
        /* undefined symbol */
#define  N_UNDEF	0L
        /* value of symbol is absolute */
#define  N_ABS		-1L
        /* special debugging symbol -- value of symbol is meaningless */
#define  N_DEBUG	-2L

/*
   The fundamental type of a symbol packed into the low 
   4 bits of the word.
*/

#define  _EF	".ef"

#define  T_NULL     0L
#define  T_ARG      1L          /* function argument (only used by compiler) */
#define  T_VOID     1L          /* void */
#define  T_CHAR     2L          /* character */
#define  T_SHORT    3L          /* short integer */
#define  T_INT      4L          /* integer */
#define  T_LONG     5L          /* long integer */
#define  T_FLOAT    6L          /* floating point */
#define  T_DOUBLE   7L          /* double word */
#define  T_STRUCT   8L          /* structure  */
#define  T_UNION    9L          /* union  */
#define  T_ENUM     10L         /* enumeration  */
#define  T_MOE      11L         /* member of enumeration */
#define  T_UCHAR    12L         /* unsigned character */
#define  T_USHORT   13L         /* unsigned short */
#define  T_UINT     14L         /* unsigned integer */
#define  T_ULONG    15L         /* unsigned long */

#define  T_FRAC     0x10000L         /* fractional */
#define  T_UFRAC    0x10001L         /* unsigned fractional */
#define  T_LFRAC    0x10002L         /* long fractional */
#define  T_ULFRAC   0x10003L         /* unsigned long fractional */
#define  T_ACCUM    0x10004L         /* accumulator ext:msb*/
#define  T_LACCUM   0x10005L         /* long accumulator ext:msb:lsb */

/*
 * derived types are:
 */

#define  DT_NON      0L          /* no derived type */
#define  DT_PTR      1L          /* pointer */
#define  DT_FCN      2L          /* function */
#define  DT_ARY      3L          /* array */

/*
 *   type packing constants
 */

#define  N_BTMASK     0x1000fL
#define  N_TMASK      0x30L
#define  N_BTSHFT     4L
#define  N_TSHIFT     2L
#define  N_BTMASK_LOW 0xfL
#define  N_BTMASK_HI  0x10000L
#define  N_BTSHFT_HI  16L

/*
 *   MACROS
 */

	/*   Basic Type of  x   */

#define  BTYPE(x)  ((x) & N_BTMASK)

       /* index for extended basic types - used for table lookup */
#define  BTYPE_INDEX(x)  (((x) & N_BTMASK_LOW) | (((x) & N_BTMASK_HI) >> (N_BTSHFT_HI-N_BTSHFT)))

	/*   Is  x  a  pointer ?   */

#define  ISPTR(x)  (((x) & N_TMASK) == (DT_PTR << N_BTSHFT))

	/*   Is  x  a  function ?  */

#define  ISFCN(x)  (((x) & N_TMASK) == (DT_FCN << N_BTSHFT))

	/*   Is  x  an  array ?   */

#define  ISARY(x)  (((x) & N_TMASK) == (DT_ARY << N_BTSHFT))

	/* Is x a structure, union, or enumeration TAG? */

#define ISTAG(x)  ((x)==C_STRTAG || (x)==C_UNTAG || (x)==C_ENTAG)

#define  INCREF(x) ((((x)&~N_BTMASK)<<N_TSHIFT)|(DT_PTR<<N_BTSHFT)|(x&N_BTMASK))

#define  DECREF(x) (((((x)&~N_BTMASK)>>N_TSHIFT)&~N_BTMASK)|((x)&N_BTMASK))

/*
 *	AUXILIARY ENTRY FORMAT
 */

union auxent
{
	struct
	{
		int32_t		x_tagndx;	/* str, un, or enum tag indx */
		union
		{
			struct
			{
				uint32_t	x_lnno;	/* declaration line number */
				uint32_t	x_size;	/* str, union, array size */
			} x_lnsz;
			int32_t	x_fsize;	/* size of function */
		} x_misc;
		union
		{
			struct			/* if ISFCN, tag, or .bb */
			{
				int32_t	x_lnnoptr;	/* ptr to fcn line # */
				int32_t	x_endndx;	/* entry ndx past block end */
          uint32_t x_type; /* function prologue/epilogue type index */
			} 	x_fcn;
			struct			/* if ISARY, up to 4 dimen. */
			{
				uint32_t	x_dimen[DIMNUM];
			} 	x_ary;
		}		x_fcnary;
		uint32_t  x_tvndx;		/* tv index */
	} 	x_sym;
	struct
	{
		char	x_fname[FILNMLEN]; /* filename here if x_foff==0 */
		uint32_t x_foff; /* if !0 then x_fname in string table */
    uint32_t x_ftype; /* index indicating memory space used by file for stack */
	} 	x_file;
        struct
        {
                int32_t    x_scnlen;          /* section length */
                uint32_t  x_nreloc;  /* number of relocation entries */
                uint32_t  x_nlinno;  /* number of line numbers */
               uint32_t x_soff; /* section name offset in string table (for named sections) */
        }       x_scn;

	struct
	{
		int32_t		x_tvfill;	/* tv fill value */
		uint32_t	x_tvlen;	/* length of .tv */
		uint32_t	x_tvran[2];	/* tv range */
	}	x_tv;	/* info about .tv section (in auxent of symbol .tv)) */
struct syment filler; /* to fill out to size of syment */
};

#define	SYMENT	struct syment
#define	SYMESZ	(sizeof(SYMENT))

#define	AUXENT	union auxent
#define	AUXESZ	(sizeof(AUXENT)) 
// - maout.h - syms.h --------------------------------------------------------

// - maout.h - storclas.h ----------------------------------------------------
/*
 *   STORAGE CLASSES
 */

#define  C_EFCN          -1L    /* physical end of function */
#define  C_NULL          0L
#define  C_AUTO          1L     /* automatic variable */
#define  C_EXT           2L     /* external symbol */
#define  C_STAT          3L     /* static */
#define  C_REG           4L     /* register variable */
#define  C_EXTDEF        5L     /* external definition */
#define  C_LABEL         6L     /* label */
#define  C_ULABEL        7L     /* undefined label */
#define  C_MOS           8L     /* member of structure */
#define  C_ARG           9L     /* function argument */
#define  C_STRTAG        10L    /* structure tag */
#define  C_MOU           11L    /* member of union */
#define  C_UNTAG         12L    /* union tag */
#define  C_TPDEF         13L    /* type definition */
#define C_USTATIC	 14L    /* undefined static */
#define  C_ENTAG         15L    /* enumeration tag */
#define  C_MOE           16L    /* member of enumeration */
#define  C_REGPARM	 17L    /* register parameter */
#define  C_FIELD         18L    /* bit field */
#define C_MEMREG         19L    /* memory locations used as registers */
#define C_OPTIMIZED      20L    /* objects optimized away */

#define  C_BLOCK         100L   /* ".bb" or ".eb" */
#define  C_FCN           101L   /* ".bf" or ".ef" */
#define  C_EOS           102L   /* end of structure */
#define  C_FILE          103L   /* file name */

	/*
	 * The following storage class is a "dummy" used only by STS
	 * for line number entries reformatted as symbol table entries
	 */

#define  C_LINE		 104L
#define  C_ALIAS	 105L   /* duplicate tag */
#define  C_HIDDEN	 106L   /* special storage class for external */

	/*
	 * The following storage classes support Motorola DSP assembly
	 * language symbolic debugging.
	 */

#define  A_FILE		 200L	/* assembly language source file name */
#define	 A_SECT		 201L	/* logical section start/end symbol */
#define	 A_BLOCK	 202L	/* COFF section debug symbol */
#define	 A_MACRO	 203L	/* macro debug symbol */
#define  A_GLOBAL	 210L	/* global assembly language symbol */
#define  A_XDEF		 211L	/* XDEFed assembly language symbol */
#define  A_XREF		 212L	/* XREFed assembly language symbol */
#define  A_SLOCAL	 213L	/* section local label */
#define  A_ULOCAL	 214L	/* underscore local label */
#define  A_MLOCAL	 215L	/* macro local label */
// - maout.h - storclas.h ----------------------------------------------------

/*
 * Format of an maout header
 */
 

struct	exec {	/* maout header */
	int32_t		a_magic;	/* magic number */
	uint32_t a_text;		/* size of text segment */
					/* in bytes		*/
					/* padded out to next	*/
					/* page boundary with	*/
					/* binary zeros.	*/
	uint32_t a_data;		/* size of initialized data */
					/* segment in bytes	*/
					/* padded out to next	*/
					/* page boundary with	*/
					/* binary zeros.	*/
	uint32_t a_bss;		/* Actual size of	*/
					/* uninitialized data	*/
					/* segment in bytes.	*/
	uint32_t a_syms;		/* size of symbol table */
	CORE_ADDR a_entry;	/* entry point */
};

#define	A_MAGIC1	0407L		/* normal */
#define	A_MAGIC0	0401L		/* lpd (UNIX/RT) */
#define	A_MAGIC2	0410L		/* read-only text */
#define	A_MAGIC3	0411L		/* separated I&D */
#define	A_MAGIC4	0405L		/* overlay */
#define	A_MAGIC5	0437L		/* system overlay, separated I&D */
// - maout.h -----------------------------------------------------------------

// - dspext.h ----------------------------------------------------------------
/**
*	DSPEXT - DSP COFF definition extensions
**/

struct opthdr {	/* original (old) linker optional header */
	int32_t modsize;	/* module size */
	int32_t datasize;	/* raw data size */
	int32_t endstr;	/* end expression string */
	int32_t secnt;	/* logical section count */
	int32_t ctrcnt;	/* counter count */
	int32_t relocnt;	/* relocation entry count */
	int32_t lnocnt;	/* line number entry count */
	int32_t bufcnt;	/* buffer count */
	int32_t ovlcnt;	/* overlay count */
	int32_t dumstk;	/* debugger dummy stack size */
};
#define OPTHDR struct opthdr
#define OPTHSZ (sizeof(OPTHDR))

/*
  NOTE:	Currently the linker depends on the fields modsize through
  	ovlcnt to be in the same relative position in both opthdr
	and opthdr2.
*/

struct opthdr2 {	/* linker optional header */
	int32_t modsize;	/* module size */
	int32_t datasize;	/* raw data size */
	int32_t endstr;	/* end expression string */
	int32_t secnt;	/* logical section count */
	int32_t ctrcnt;	/* counter count */
	int32_t relocnt;	/* relocation entry count */
	int32_t lnocnt;	/* line number entry count */
	int32_t bufcnt;	/* buffer count */
	int32_t ovlcnt;	/* overlay count */
	int32_t majver;	/* major version number */
	int32_t minver;	/* minor version number */
	int32_t revno;	/* revision number */
	int32_t optflags;	/* optional header flags */
	int32_t sditot;	/* count of SDIs in module */
};
#define OPTHDR2 struct opthdr2
#define OPTH2SZ (sizeof(OPTHDR2))

union xcnhdr {	/* extended COFF section header */
	union {
		char _s_name[SYMNMLEN];
		struct {
			int32_t	_s_zeroes;
			int32_t	_s_offset;
		} _s_n;
	} _n;
	SCNHDR	_s;
};
#define XCNHDR	union xcnhdr
#define XCNHSZ	sizeof (XCNHDR)

#define scn_name	xcn->_n._s_name
#define scn_zeroes	xcn->_n._s_n._s_zeroes
#define scn_offset	xcn->_n._s_n._s_offset
#define scn_paddr	xcn->_s.s_paddr.w0.u
#define scn_pmap	xcn->_s.s_paddr.w1.mape
#define scn_vaddr	xcn->_s.s_vaddr.w0.u
#define scn_vmap	xcn->_s.s_vaddr.w1.mape
#define scn_size	xcn->_s.s_size
#define scn_scnptr	xcn->_s.s_scnptr
#define scn_relptr	xcn->_s.s_relptr
#define scn_lnnoptr	xcn->_s.s_lnnoptr
#define scn_nreloc	xcn->_s.s_nreloc
#define scn_nlnno	xcn->_s.s_nlnno
#define scn_flags	xcn->_s.s_flags

struct mematt {		/* memory attribute structure */
	enum memory_map mspace; /* memory space */
	enum memory_map mmap;   /* memory mapping */
	int32_t mcntr;             /* counter */
	int32_t mclass;		/* memory class */
};

union auxlnk {	/* original (old) linker auxiliary symbol structure */
	struct {
		int32_t secno;
		int32_t rsecno;
		int32_t mem;
		int32_t flags;
		union {
			struct {
				int32_t bufcnt;
				int32_t buftyp;
				int32_t buflim;
			} buf;
			struct {
				int32_t ovlcnt;
				int32_t ovlmem;
				int32_t ovlstr;
			} ovl;
		} bufovl;
	} aux;
	SYMENT filler;	/* to fill out to size of SYMENT */
};
#define AUXLNK union auxlnk
#define AUXLSZ (sizeof(AUXLNK))

/*
  NOTE:	Currently the linker depends on the fields secno and
  	rsecno to be in the same relative position in both auxlnk
	and auxlnk1.
*/

union auxlnk1 {	/* linker first auxiliary symbol structure */
	struct {
		int32_t secno;
		int32_t rsecno;
		int32_t flags;
		struct mematt mem;
	} aux;
	SYMENT filler;	/* to fill out to size of SYMENT */
};
#define AUXLNK1 union auxlnk1
#define AUXL1SZ (sizeof(AUXLNK1))

union auxlnk2 {	/* linker second auxiliary symbol structure */
	union {
		struct {
			int32_t bufcnt;
			int32_t buftyp;
			int32_t buflim;
		} buf;
		struct {
			struct mematt ovlmem;
			int32_t ovlcnt;
			int32_t ovlstr;
			uint32_t ovloff;
		} ovl;
	} bufovl;
	SYMENT filler;	/* to fill out to size of SYMENT */
};
#define AUXLNK2 union auxlnk2
#define AUXL2SZ (sizeof(AUXLNK2))

#define F_CC	0x00010000L	/* object file produced via C compiler */
#define F_SDI   0x00020000L	/* object file contains span-dep. instr. */

#define C_SECT  128L		/* special class for nested section symbols */
#define C_SDI	129L		/* special class for SDI symbol record */

#define T_MOD	1L		/* special module file type (maps to T_ARG) */

#define _TXT	".txt"		/* .txt section name */
#define _DAT	".dat"		/* .dat section name */
#define _CMT	".cmt"		/* .cmt comment symbol name */
#define _SDI	".sdi"		/* .sdi span-dependent instruction name */
#define _BS	".bs"		/* .bs begin section symbol name */
#define _ES	".es"		/* .es end section symbol name */
#define _TX	".tx"		/* .tx section debug name */
#define _DA	".da"		/* .da section debug name */
#define _XR	".xr"		/* .xr xref symbol name */
#define _BM	".bm"		/* .bm begin macro symbol name */
#define _EM	".em"		/* .em end macro symbol name */
// - dspext.h ----------------------------------------------------------------

union dbl {     /* double-precision floating point/integer union */
        double dval;
        struct {
#if !BIG_ENDIAN
                uint32_t lval, hval;
#else
                uint32_t hval, lval;
#endif
        } l;
};

struct comment {	/* comment structure */
	int32_t c_scno;
	int32_t c_text;
	struct comment *c_next;
};

/* function definitions */
static void cld_to_lod();
static void read_headers();
static void collect_comments();
static void read_strings();
static void start_record();
static void read_sections();
static void dump_comments();
static void dump_data();
static void dump_deb_symbols (void);
static int freads (char *ptr, int size, int nitems,FILE *stream);
static void dump_se_d (SYMENT *se);

#if !BIG_ENDIAN
static void swapw (char *ptr, int size, int nitems);
#endif

#if defined ( __WATCOMC__ ) || defined( __GNUC__ ) || defined( MPW )
static void eprintf( FILE*, char*, ... );
static void error( char*, ... );
#else
static void eprintf();
static void error();
#endif

int32_t symentsize = SYMESZ;
/*  Global variables  */
FILHDR	file_header;	/* File header structure */
AOUTHDR	opt_header;	/* Optional header structure */
OPTHDR	link_header;	/* Linker header structure */
int	absolute;	/* Absolute file flag */

int32_t num_sections;	/* Number of sections */
int32_t section_seek;	/* Used to seek to first section */

int32_t symptr;		/* File pointer to symbol table entries */
int32_t num_symbols;	/* Number of symbols */

int  data_width;        /* width of data for printing */
int  addr_width;        /* width of address for printing */

char *str_tab;		/* Pointer to start of string char. array */
int32_t str_length;	/* Length in bytes of string array */

FILE *ifile = NULL;	/* file pointer for input file */
char *ifn = NULL;	/* pointer to input file name */

/* init is to non valid memory space */
int  space = 777; /* 0=p, 1=x, 2=y, 3=l, 4=N */

struct comment *com_head = NULL;	/* head of comment chain */
struct comment *cur_cmt = NULL;		/* current comment in chain */

static void
onintr (int signum)			/* clean up from signal */
{
    void exit ();

    exit (1);
}

void
main (argc, argv)
int argc;
char *argv[];
{
    void exit ();

    /* set up for signals, save program name, check for command line options */
    signal (SIGINT, onintr);

    /* check for correct command-line */
    if ( argc != 2 )
    {
	fprintf( stderr, "Version 6.3 usage: cldlod cldfile > lodfile\n" );
	exit ( -1 );
    }


    if ( ( ifile = fopen( argv[1], "rb" ) ) == NULL )
	error( "cannot open input file %s", argv[1] );

    cld_to_lod();
    
    fclose( ifile );
    exit (0);
}


static void
cld_to_lod()
{
    read_headers ();

    /* blow out the _START record */
    if ( symptr != 0  && num_symbols != 0 )	/* no symbols */
    {
	read_strings ();
	start_record();
	collect_comments ();
    }

    read_sections ();
#if (SKIPSYMBOLS==0)
    if ( symptr != 0  && num_symbols != 0 )	/* no symbols */
	dump_deb_symbols ();
#endif
    /* blow out the _END record */
    eprintf( stdout, "\n_END %01.*X\n", addr_width,
	    CORE_ADDR_ADDR (opt_header.entry) );
}


/* blow out the first .cmt symbol with:
        n_sclass == C_NULL
	n_type   == T_NULL
*/
static void
start_record()
{
    SYMENT se;
    int32_t i = 0, sym_id = -709;

    
    if ( fseek( ifile, symptr, 0 ) != 0 )
	error ("cannot seek to symbol table");

    while ( i < num_symbols )
    {
	if ( freads( (char *)&se, sizeof (SYMENT), 1, ifile ) != 1 )
	    error ("cannot read symbol table entry %d", i);
#if !BIG_ENDIAN
	if ( se.n_zeroes )
		swapw( se.n_name, sizeof (int32_t), 2 );
#endif
	if ( strcmp( se.n_name, ".cmt" ) == 0 &&
	     se.n_scnum == N_ABS &&
	     se.n_sclass == C_NULL &&
	     se.n_type == T_NULL )
	{
	    sym_id = CORE_ADDR_ADDR(se.n_value);
	    break;
	}
	
	i++;
    }
    
    /* */
    eprintf( stdout, "_START " );

    if ( sym_id >= 0 && str_length != 0 )
    {
	char *str_ptr = str_tab;
	char *cmt_ptr;
	int  len;
	int32_t offset = (int32_t) sizeof( str_length );
	
	do
	{
	    if ( offset == sym_id )
		break;
	    else
	    {
		len = strlen( str_ptr );
		offset += len + 1;
		str_ptr += len + 1;
	    }
	} while ( str_ptr < ( str_tab + str_length ) );

	cmt_ptr = strchr (str_ptr, ';');
	if (!cmt_ptr)
		eprintf( stdout, "%s\n\n", str_ptr );
	else
	{
		*cmt_ptr = 0;
		eprintf( stdout, "%s\n", str_ptr );
		*cmt_ptr = ';';
		eprintf( stdout, "%s\n", cmt_ptr );
	}
    }
    else
	eprintf( stdout, "\n\n" );
}


static void
read_headers ()
{
    if ( freads( (char *)&file_header, sizeof (FILHDR), 1, ifile ) != 1 )
	error ("cannot read file header");

    /* Save the global values */
    num_sections = file_header.f_nscns;
    num_symbols = file_header.f_nsyms;
    symptr = file_header.f_symptr;
    absolute = !!(file_header.f_flags & F_RELFLG);

    /* check the MAGIC number */
    if ( file_header.f_magic == M56KMAGIC )
    {
	data_width = 6;
	addr_width = 4;
    }
    else if ( file_header.f_magic == M96KMAGIC )
    {
	data_width = addr_width = 8;
    }
    else if ( file_header.f_magic == M16KMAGIC )
    {
	data_width = addr_width = 4;
    }
    else if ( file_header.f_magic == M563MAGIC )
    {
	data_width = addr_width = 6;
    }
    else if ( file_header.f_magic == M566MAGIC )
    {
	data_width = 6;
        addr_width = 4;
    }
    else if ( file_header.f_magic == M568MAGIC )
    {
	data_width = addr_width = 4;
    }
    else if ( file_header.f_magic == SC1MAGIC )
    {
	data_width = 4;
	addr_width = 8;
    }
    else if ( file_header.f_magic == M567MAGIC )
    {
	data_width = 4;
	addr_width = 8;
    }
    else
    {
	error( "Header has a bad magic number" );
    }
    

    /* optional header present */
    if ( file_header.f_opthdr )
    {
	if ( absolute )
	{
	    if ( freads( (char *)&opt_header,
			(int)file_header.f_opthdr, 1, ifile ) != 1 )
		error( "cannot read optional file header" );
	}
	else
	{
	    if ( freads( (char *)&link_header,
			(int)file_header.f_opthdr, 1, ifile ) != 1 )
		error( "cannot read linker file header" );
	}
    }

    /* File offset for first section headers */
    section_seek = sizeof(FILHDR) + file_header.f_opthdr;
}


static void
read_strings ()
{
    int32_t strings;

    strings = symptr + (num_symbols * SYMESZ);
    if ( fseek( ifile, strings, 0 ) != 0 )
	error ( "cannot seek to string table length" );
    if ( freads( (char *)&str_length, 4, 1, ifile ) != 1 && !feof( ifile ))
	error ( "cannot read string table length" );
    if ( feof( ifile ))
	str_length = 0L;
    else if ( str_length )
    {
	str_length -= 4;
	str_tab = (char *)malloc ((size_t)str_length);
	if ( !str_tab )
	    error( "cannot allocate string table" );
	if ( fseek( ifile, strings + 4, 0 ) != 0 )
	    error( "cannot seek to string table" );
	if (fread (str_tab, (int)str_length, 1, ifile) != 1)
	    error( "cannot read string table" );
    }
}

static void
collect_comments ()
{
    SYMENT se;
    AUXENT ae;
    int    i, j, k;

    if (fseek (ifile, symptr, 0) != 0)
	error ("cannot seek to symbol table");

    i = 0;
    while (i < num_symbols)
    {
	struct comment *cmnt;

	if (freads ((char *)&se, sizeof (SYMENT), 1, ifile) != 1)
	    error ("cannot read symbol table entry %d", i);

#if !BIG_ENDIAN
	if ( se.n_zeroes )
		swapw( se.n_name, sizeof (int32_t), 2 );
#endif
	if ( strcmp( se.n_name, ".cmt" ) == 0 &&
	     se.n_scnum != N_ABS &&
	     se.n_sclass == C_NULL &&
	     se.n_type == T_NULL )
	{
	    struct comment *cp =
		(struct comment *)malloc (sizeof (struct comment));
	    if ( !cp )
	        error ( "cannot allocate comment record" );
	    cp->c_scno = se.n_scnum;
	    cp->c_text = CORE_ADDR_ADDR(se.n_value);
	    cp->c_next = NULL;
	    if ( !com_head )
	        com_head = cmnt = cp;
	    else
	        cmnt = cmnt->c_next = cp;
	}

	k = i++;

	for (j = 0; j < se.n_numaux; j++)
	{
	    if (freads ((char *)&ae, sizeof (AUXENT), 1, ifile) != 1)
		error ("cannot read auxiliary entry %d for symbol entry %d", j, k);
	    i++;
	}
    }
    cur_cmt = com_head;		/* initialize current comment */
}

static void
read_sections()
{
    int i;
    XCNHDR sh;		/* Section header structure */

    for (i = 0; i < num_sections; i++)
    {
	if (fseek (ifile, section_seek, 0) != 0)
	    error ("cannot seek to section headers");
	if (freads ((char *)&sh, sizeof (XCNHDR), 1, ifile) != 1)
	    error ("cannot read section headers");
#if !BIG_ENDIAN
	if (sh._n._s_n._s_zeroes)
	    swapw (sh._n._s_name, sizeof (int32_t), 2);
#endif
	section_seek += sizeof (XCNHDR);

	dump_comments (i + 1);

	dump_data (&sh);
    }
}


static void
dump_comments (scnum)
int scnum;
{
    while ( cur_cmt && cur_cmt->c_scno <= scnum )
    {
        if ( cur_cmt->c_scno <= 0 )	/* no section mapping */
	    continue;

        eprintf( stdout, "_COMMENT\n" );

	if ( cur_cmt->c_text < sizeof ( str_length ) ||
	     cur_cmt->c_text > str_length )
	    error ( "invalid string table offset for comment" );

	eprintf( stdout, "%s\n",
	    &str_tab[ cur_cmt->c_text - sizeof( str_length ) ]);

	cur_cmt = cur_cmt->c_next;
    }
}


char *
get_secname (sh)
XCNHDR *sh;
{
    char *secname;

    if ( sh->_n._s_n._s_zeroes )
	secname = sh->_n._s_name;
    else
    {
	if (sh->_n._s_n._s_offset < sizeof (str_length) ||
	    sh->_n._s_n._s_offset > str_length)
	    error ("invalid string table offset for section header name");
	secname = &str_tab[sh->_n._s_n._s_offset - sizeof (str_length)];
    }

    return (secname);
}


static void
dump_data (sh)
XCNHDR *sh;
{
    char *secname, *get_secname ();
    int32_t *raw_data;
    int  j;
    char emi_name_buf[8];

    if ( sh->_s.s_scnptr && sh->_s.s_size )
    {
	int memtype = CORE_ADDR_MAP( sh->_s.s_paddr );
	int address  = CORE_ADDR_ADDR( sh->_s.s_paddr );
	char *mem_field;
	
	secname = get_secname( sh );
	
	/* determine the memory field (optional counter ok) */
	switch ( memtype )
	{
	case memory_map_p:
	    mem_field = "P";
	    break;
	    
	case memory_map_pa:
	    mem_field = "PA";
	    break;
	    
	case memory_map_pb:
	    mem_field = "PB";
	    break;
	    
	case memory_map_pe:
	    mem_field = "PE";
	    break;
	    
	case memory_map_pi:
	    mem_field = "PI";
	    break;
	    
	case memory_map_pr:
	    mem_field = "PR";
	    break;
	    
	case memory_map_y:
	    mem_field = "Y";
	    break;
	    
	case memory_map_ya:
	    mem_field = "YA";
	    break;
	    
	case memory_map_yb:
	    mem_field = "YB";
	    break;
	    
	case memory_map_ye:
	    mem_field = "YE";
	    break;
	    
	case memory_map_yi:
	    mem_field = "YI";
	    break;
	    
	case memory_map_yr:
	    mem_field = "YR";
	    break;
	    
	case memory_map_x:
	    mem_field = "X";
	    break;
	    
	case memory_map_xa:
	    mem_field = "XA";
	    break;
	    
	case memory_map_xb:
	    mem_field = "XB";
	    break;
	    
	case memory_map_xe:
	    mem_field = "XE";
	    break;
	    
	case memory_map_xi:
	    mem_field = "XI";
	    break;
	    
	case memory_map_xr:
	    mem_field = "XR";
	    break;
	    
	case memory_map_l:
	    mem_field = "L";
	    break;
	    
	case memory_map_laa:
	    mem_field = "LAA";
	    break;
	    
	case memory_map_lab:
	    mem_field = "LAB";
	    break;
	    
	case memory_map_lba:
	    mem_field = "LBA";
	    break;
	    
	case memory_map_lbb:
	    mem_field = "LBB";
	    break;
	    
	case memory_map_le:
	    mem_field = "LE";
	    break;
	    
	case memory_map_li:
	    mem_field = "LI";
	    break;
	    
	case memory_map_emi:
	    mem_field = "EM";
	    break;
	    
	case memory_map_dm:
	    mem_field = "DM";
	    break;
	    
	default:

	    if ((file_header.f_magic == M566MAGIC) &&
		(memtype == memory_map_e1) && 
		(sh->_s.s_flags & STYP_OVERLAY) &&
		(!(sh->_s.s_flags & STYP_OVERLAYP)) 
		)
	    {
		/* substitute when x or y overlay memory destination */
		memtype = memory_map_e0;
	    }
	    
	    if (memtype>= memory_map_e0 && memtype<= memory_map_e255)
	{
	    sprintf(emi_name_buf,"E%d",memtype-memory_map_e0);
	    mem_field = emi_name_buf;
	}
	    else
	{
	    mem_field = "<ERROR>";
	}
	    break;
	}
	

	raw_data = (int32_t *)malloc((size_t)(sh->_s.s_size * sizeof (int32_t)));
	if (!raw_data)
	    error ("cannot allocate raw data for section %s", secname);

	if (fseek (ifile, sh->_s.s_scnptr, 0) != 0)
	    error ("cannot seek to raw data in section %s", secname);

	if (freads ((char *)raw_data, (int)sh->_s.s_size,
		    sizeof (int32_t), ifile) != sizeof (int32_t))
	    error ("cannot read raw data in section %s",  secname);


	/* check for block data */
	if ( sh->_s.s_flags & STYP_BLOCK )
	{
	    if ( mem_field[0] == 'L' )
	    {
		eprintf( stdout, "_BLOCKDATA Y %01.*X %01.*X %01.*X\n",
			addr_width, address,
			addr_width, CORE_ADDR_ADDR( sh->_s.s_vaddr ),
			data_width, *raw_data++ );

		eprintf( stdout, "_BLOCKDATA X %01.*X %01.*X %01.*X\n",
			addr_width, address,
			addr_width, CORE_ADDR_ADDR( sh->_s.s_vaddr ),
			data_width, *raw_data++ );
	    }
	    else
	    {
		eprintf( stdout, "_BLOCKDATA %s %01.*X %01.*X %01.*X\n",
			mem_field,
			addr_width, address,
			addr_width, CORE_ADDR_ADDR( sh->_s.s_vaddr ),
			data_width, *raw_data++ );
	    }
	}
	
	else
	{
	    eprintf (stdout, "_DATA %s %01.*X\n", mem_field,
		     addr_width, address );
	    
	    j = 0;
	    while ( j < sh->_s.s_size )
	    {
		if ( mem_field[0] == 'L' )
		{
		    eprintf (stdout, "%01.*lX %01.*lX ",
			     data_width, *(raw_data+1),
			     data_width, *raw_data);
		    raw_data += 2;
		    j += 2;
		}
		else
		{
		    eprintf (stdout, "%01.*lX ", data_width, *raw_data++);
		    j++;
		}
		

		if ( j % 8 == 0 && j < sh->_s.s_size )
		    eprintf (stdout, "\n");
	    }
	    eprintf (stdout, "\n");
	}
    }
}


static void
dump_deb_symbols ()
{
    SYMENT se;
    AUXENT ae;
    int    i, j, k;


    if (fseek (ifile, symptr, 0) != 0)
	error ("cannot seek to symbol table");

    i = 0;
      while (i < num_symbols)
    {
	if (freads ((char *)&se, sizeof (SYMENT), 1, ifile) != 1)
	    error ("cannot read symbol table entry %d", i);

	dump_se_d (&se); /* ek */
	k = i++;

	for (j = 0; j < se.n_numaux; j++)
	{
	    if (freads ((char *)&ae, sizeof (AUXENT), 1, ifile) != 1)
		error ("cannot read auxiliary entry %d for symbol entry %d", j, k);
	    i++;
	}
    }
}

static char *map_chars[]={
"P",
"X","Y","L","N",
"LAA","LAB","LBA","LBB","LE",
"LI","PA","PB","PE","PI",
"PR","XA","XB","XE","XI",
"XR","YA","YB","YE","YI",
"YR","PT","PF","EM",
"E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9 ",
"E10", "E11", "E12", "E13", "E14", "E15", "E16", "E17", "E18", "E19", 
"E20", "E21", "E22", "E23", "E24", "E25", "E26", "E27", "E28", "E29", 
"E30", "E31", "E32", "E33", "E34", "E35", "E36", "E37", "E38", "E39", 
"E40", "E41", "E42", "E43", "E44", "E45", "E46", "E47", "E48", "E49", 
"E50", "E51", "E52", "E53", "E54", "E55", "E56", "E57", "E58", "E59", 
"E60", "E61", "E62", "E63", "E64", "E65", "E66", "E67", 
"E68", "E69", "E70", "E71", "E72", "E73", "E74", "E75", 
"E76", "E77", "E78", "E79", "E80", "E81", "E82", "E83", 
"E84", "E85", "E86", "E87", "E88", "E89", "E90", "E91", 
"E92", "E93", "E94", "E95", "E96", "E97", "E98", "E99", 
"E100", "E101", "E102", "E103", "E104", "E105", "E106", "E107", 
"E108", "E109", "E110", "E111", "E112", "E113", "E114", "E115", 
"E116", "E117", "E118", "E119", "E120", "E121", "E122", "E123", 
"E124", "E125", "E126", "E127", "E128", "E129", "E130", "E131", 
"E132", "E133", "E134", "E135", "E136", "E137", "E138", "E139", 
"E140", "E141", "E142", "E143", "E144", "E145", "E146", "E147", 
"E148", "E149", "E150", "E151", "E152", "E153", "E154", "E155", 
"E156", "E157", "E158", "E159", "E160", "E161", "E162", "E163", 
"E164", "E165", "E166", "E167", "E168", "E169", "E170", "E171", 
"E172", "E173", "E174", "E175", "E176", "E177", "E178", "E179", 
"E180", "E181", "E182", "E183", "E184", "E185", "E186", "E187", 
"E188", "E189", "E190", "E191", "E192", "E193", "E194", "E195", 
"E196", "E197", "E198", "E199", "E200", "E201", "E202", "E203", 
"E204", "E205", "E206", "E207", "E208", "E209", "E210", "E211", 
"E212", "E213", "E214", "E215", "E216", "E217", "E218", "E219", 
"E220", "E221", "E222", "E223", "E224", "E225", "E226", "E227", 
"E228", "E229", "E230", "E231", "E232", "E233", "E234", "E235", 
"E236", "E237", "E238", "E239", "E240", "E241", "E242", "E243", 
"E244", "E245", "E246", "E247", "E248", "E249", "E250", "E251", 
"E252", "E253", "E254", "E255"
    };
static void
dump_se_d (se) /* for debug symbol table */
SYMENT *se;
{
    int  old_space;
#if 0
    char *type, *sclass;
#endif
    char *name;
    char sym_type = 'I';
	

    if (se->n_zeroes)
    {
#if !BIG_ENDIAN
	swapw (se->n_name, sizeof (int32_t), 2);
#endif
	name = se->n_name;
    }
    else
    {
	if (se->n_offset < sizeof (str_length) ||
	    se->n_offset > str_length)
	    error ("invalid string table offset for symbol table entry %d name", se - symptr);
	name = &str_tab[se->n_offset - sizeof (str_length)];
    }

    if ( name[0] == '.' )
    {
	return;
    }


    if (ISFCN (se->n_type)); /* C functions - value type is that returned */
    else 
    {
	switch ( se->n_sclass )
	{
	case C_EFCN:
	case C_NULL:
	case C_AUTO:
	case C_EXT:
	case C_STAT:
	case C_REG:
	case C_EXTDEF:
	case C_LABEL:
	case C_ULABEL:
	case C_MOS:
	case C_ARG:
	case C_STRTAG:
	case C_MOU:
	case C_UNTAG:
	case C_TPDEF:
	case C_USTATIC:
	case C_ENTAG:
	case C_MOE:
	case C_REGPARM:
	case C_FIELD:
	case C_BLOCK:
	case C_FCN:
	case C_EOS:
	case C_FILE:
	case C_LINE:
	case C_ALIAS:
	case C_HIDDEN:
	case C_MEMREG:
	case C_OPTIMIZED:
	    break; /* use 'I' type for all the C types */
	default:
	    switch ( BTYPE( se->n_type ) )
	    {
	    case T_FLOAT:
	    case T_DOUBLE:
		sym_type = 'F';
		break;
#if 0
	    case T_STRUCT:
	    case T_UNION:
	    case T_ENUM:
	    case T_MOE:
	    case T_UCHAR:
	    case T_UINT:
	    case T_ULONG:
	    case T_NULL:
	    case T_CHAR:
	    case T_SHORT:
	    case T_INT:
	    case T_LONG:
	    default:
#endif
	    }
	}
    }

    /* ignore overhead symbols */
    if (strcmp (name, "etext") == 0 &&
	se->n_scnum == N_ABS &&
	se->n_type == T_NULL &&
	se->n_sclass == C_EXT)
	    return;
    if (strcmp (name, "end") == 0 &&
	se->n_scnum == N_ABS &&
	se->n_type == T_NULL &&
	se->n_sclass == C_EXT)
	    return;

    old_space = space;
    if (se->n_scnum > 0 || ISPTR (se->n_type))
	    space = CORE_ADDR_MAP ( se->n_value );
    else
	    space = memory_map_none;

    if (space < (sizeof (map_chars)/sizeof (char *))){
	if ( old_space != space ) eprintf( stdout, "_SYMBOL %s\n",map_chars[space] );
	/* print symbol name and value */
	eprintf (stdout, "%-19s  %c ", name, sym_type);
	if (sym_type == 'F') {
		union dbl val;
		val.l.hval = se->_n_value._n_val[1];
		val.l.lval = se->_n_value._n_val[0];
		eprintf (stdout, "%-.6E\n", val.dval);
	} else
		eprintf (stdout, "%01.*lX\n",
			 data_width, CORE_ADDR_ADDR (se->n_value));
	}
    else space = old_space; /* restore last written space */

}


/**
*
* name		freads - swap bytes and read
*
* synopsis	freads (ptr, size, nitems, stream)
*		char *ptr;		pointer to buffer
*		int size;		size of buffer
*		int nitems;		number of items to read
*		FILE *stream;		file pointer
*
* description	Treats ptr as reference to union array; if necessary,
*		swaps bytes to maintain base format byte ordering
*		(big endian).  Calls fread to do I/O.
*
**/
static int
freads (ptr, size, nitems, stream)
char *ptr;
int size, nitems;
FILE *stream;
{
    int rc;

    rc = fread (ptr, size, nitems, stream);
#if !BIG_ENDIAN
    swapw (ptr, size, nitems);
#endif
    return (rc);
}


#if !BIG_ENDIAN


union wrd
{   /* word union for byte swapping */
    uint32_t l;
    unsigned char b[4];
};

/**
*
* name		swapw - swap bytes in words
*
* synopsis	swapw (ptr, size, nitems)
*		char *ptr;		pointer to buffer
*		int size;		size of buffer
*		int nitems;		number of items to write
*
* description	Treats ptr as reference to union array; if necessary,
*		swaps bytes to maintain base format byte ordering
*		(big endian).
*
**/
static void
swapw (ptr, size, nitems)
char *ptr;
int size, nitems;
{
    union wrd *w;
    union wrd *end = (union wrd *)ptr +	((size * nitems) / sizeof (union wrd));
    unsigned i;

    for (w = (union wrd *)ptr; w < end; w++)
    {
	i = w->b[0];
	w->b[0] = w->b[3];
	w->b[3] = i;
	i = w->b[1];
	w->b[1] = w->b[2];
	w->b[2] = i;
    }
}
#endif


#if defined (LINT)
#ifdef va_dcl
#undef va_dcl
#define va_dcl char *va_alist;
#endif
#endif

/* VARARGS */

/* call fprintf, check for errors */
static void
#if defined ( __WATCOMC__ ) || defined( __GNUC__ ) || defined( MPW )
eprintf (FILE *fp, char *fmt, ...)
#else
eprintf( va_alist )
va_dcl
#endif
{
    void exit ();
    va_list ap;
#if defined ( __WATCOMC__ ) || defined( __GNUC__ ) || defined( MPW )
    va_start (ap, fmt);
#else
    FILE *fp;
    char *fmt;
    va_start (ap);
    fp = va_arg (ap, FILE *);
    fmt = va_arg (ap, char *);
#endif
    if (vfprintf (fp, fmt, ap) < 0)
	error ("cannot write to output file");
    va_end (ap);
}


/* VARARGS */
static void
#if defined ( __WATCOMC__ ) || defined( __GNUC__ ) || defined( MPW )
error (char *fmt, ...)		/* display error on stderr, exit nonzero */
#else
error (va_alist)		/* display error on stderr, exit nonzero */
va_dcl
#endif
{
    void exit ();
    va_list ap;
#if !LINT
    int err = errno;
#endif
#if defined ( __WATCOMC__ ) || defined( __GNUC__ ) || defined( MPW )
    va_start (ap, fmt);
#else
    char *fmt;
    va_start (ap);
    fmt = va_arg (ap, char *);
#endif
    fprintf  (stderr, "cldlod: ");
    vfprintf (stderr, fmt, ap);
    fprintf  (stderr, "\n");
    va_end (ap);
#if !LINT
    if (err)
    {
	errno = err;
	perror ( "cldlod" );
    }
#endif
    exit (1);
}
