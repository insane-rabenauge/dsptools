#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// from the Falcon XBIOS Sources

uint8_t binbuff[96*1024];
uint8_t *codebuf;
uint8_t *curptr;
uint8_t *bufend;
uint32_t fisize;
uint32_t fosize;
uint32_t binindex=0;
FILE *fo,*fi;
long size_loc_index;
long size_count;

#define NUMTOKENS       6
#define DSPDATA         1
#define DSPBLOCK        2
#define DSPSYMBOL       3
#define DSPEND          5

char *tokens[] = {
   "START",
   "DATA",
   "BLOCKDATA",
   "SYMBOL",
   "COMMENT",
   "END"
};

int token_lengths[] = {
   5,
   4,
   9,
   6,
   7,
   3
};

int is_token() {
  if(*curptr == '_') {
    curptr++;           /* Only bump pointer if we have a token */
    return(1);
  } else {
    return(0);
  };
}

int iseol() {
  return ((*curptr==0x0D)||(*curptr==0x0A));
};

void newline() {
  while(curptr <= bufend && (!iseol())) curptr++;
  while(curptr <= bufend && ( iseol())) curptr++;
}

long make_int() {
  int val;
  int temp;
  int mult;
  int i;
  mult = 4096;
  val = 0;
  for(i = 0; i < 4;i++) {
    if(*curptr >='0' && *curptr <= '9')
      temp = *curptr - '0';
    else if(*curptr >= 'A' && *curptr <= 'F')
      temp = (*curptr - 'A')+10;
    else
      temp = (*curptr - 'a')+10;
    curptr++;
    val += temp * mult;
    mult /= 16;
  }
  return(val);
}

void stuff_header(char memtype) {
  long block_loc;

  /********************************************************/
  /*  First put the block type into the buffer             */
  /********************************************************/
  binbuff[binindex++] = 0;
  binbuff[binindex++] = 0;
  if(memtype == 'P') {
     binbuff[binindex++] = 0;
  } else if(memtype == 'X') {
     binbuff[binindex++] = 1;
  } else {
     binbuff[binindex++] = 2;
  }

  /**********************************************************/
  /* Next store the block location                           */
  /**********************************************************/

  binbuff[binindex++] = 0;
  block_loc = (long)make_int();
  binbuff[binindex++] = (char)(block_loc >> 8);
  binbuff[binindex++] = (char)(block_loc);

  /***********************************************************/
  /* Now setup to store the block size.  Create place holder */
  /* in buffer, init size_loc_index to binindex, and reset   */
  /* size_count                                       */
  /**********************************************************/
  
  size_count = 0L;
  size_loc_index = binindex;
  binindex += 3;      
  newline();
}

int strcmp1(char bufstr[],char tablestr[],int len) {
  int i;
  int identical;
  identical = 1;
  for(i = 0;i < len;i++)
    if(bufstr[i] != tablestr[i] && bufstr[i] != (char)((int)tablestr[i] + 32)) {
      identical = 0;
      break;
    }
  return(identical);
}

int get_token() {
  int i;
  for(i = 0;i < NUMTOKENS;i++) {
    if(strcmp1(curptr,tokens[i],token_lengths[i]))
      break;
  }
  return(i);
}

void put_dspword(char a,char b,char c) {
  binbuff[binindex++] = a;
  binbuff[binindex++] = b;
  binbuff[binindex++] = c;
  size_count++;                 /* counter for size of block    */
}

void make_dspword() {
  int i,j;
  char ch[3];
  char mult;
  char temp;
  char val;
  for(i = 0; i < 3;i++)         /* For the next 3 bytes - 24 bits */
  {
    val = 0;
    mult = 16;
    for(j = 0;j < 2; j++)               /* 2 characters to a byte         */
    {
      if(*curptr >='0' && *curptr <= '9')
        temp = (*curptr - '0');
      else if(*curptr >= 'A' && *curptr <= 'F')
        temp = (*curptr - 'A')+10;
      else
        temp = (*curptr - 'a')+10;
      curptr++;
      val += temp * mult;
      mult /= 16;
    }
    ch[i] = val;
  }
  put_dspword(ch[0],ch[1],ch[2]);   
}

void convert_line() {
  while(!iseol()) {
    while(*curptr == ' ')
      curptr++;
    if(!iseol())        /* If not end of line then, */
      make_dspword();           /* it must be a data word   */
   }
}

void move_to_endoftoken(int index) {
  curptr += token_lengths[index];
}

void do_convert() {
  char memtype;         /* X, Y, or P memory                       */
  move_to_endoftoken(DSPDATA);
  while(*curptr == ' ') /* Get rid of spaces between "DATA" and    */
    curptr++;           /* Memory type "X,Y,L, or P"               */
  memtype = *curptr++;          /* Get memory type                         */
  curptr++;                     /* skip next blank space                   */
  stuff_header(memtype);        /* Setup Block's header info               */
  while(!is_token())            /* Convert all lines of this section to bin */
  {
    if(iseol())
      newline();
    else
      convert_line();
  }
  /****************************************************************/
  /*  Now that we've completed the block and know its size, put   */
  /*  it into the blocks header that's at the beginning of the    */
  /*  block.                                                       */

  binbuff[size_loc_index++] = 0;
  binbuff[size_loc_index++] = (char)(size_count >> 8);
  binbuff[size_loc_index++] = (char)(size_count);
}

uint32_t convert_file() {
  int dsptoken;
  curptr = codebuf;
  bufend = curptr+fisize;
  binindex = 0L;
  while(!is_token())            /* Look for first token, prime pump */
    newline();
  while((dsptoken = get_token()) != DSPEND)  {
    switch(dsptoken) {
      case  DSPDATA:
        do_convert();
        break;
#if 0	
      case  DSPSYMBOL:
        while(get_token()!= DSPEND) {
	  printf("%i\n",get_token());
	  while(!is_token()) newline();
	}
	break;
#endif	
      default:
        newline();         /* Blow away token line */
        while(!is_token()) /* Find next token */
          newline();
        break;
    }
  }
  return(binindex/3);
};

int main(int argc, char **argv) {

  if (argc==2) { //stdin, file out
    fi=stdin;
    if ((fo = fopen(argv[1], "wb")) == NULL) {
      fprintf(stderr,"error writing to output file\n");
      return -1;
    };
  } else if (argc==3) { //file in, file out
    if ((fi = fopen(argv[1], "rb")) == NULL) {
      fprintf(stderr,"error reading from input file\n");
      return -1;
    };
    if ((fo = fopen(argv[2], "wb")) == NULL) {
      fprintf(stderr,"error writing to output file\n");
      return -1;
    };
  } else {
    fprintf(stderr, "LOD to P56 converter - insane/tSCc 2019-2024\n");
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  lod2p56 [LOD file] [P56 file]\n");
    fprintf(stderr, "  lod2p56 [P56 file] < [LOD FILE]\n");
    fprintf(stderr, "  cldlod [CLD file] | lod2p56 [P56 file]\n");
    return 0;
  };
  fseek(fi,0L,SEEK_END);
  fisize=ftell(fi);
  rewind(fi);
  codebuf=(uint8_t*)calloc(fisize,sizeof(uint8_t));
  fread(codebuf,1,fisize,fi);

  fosize = convert_file();
  if (fosize >= 0) {
    fwrite(binbuff, 3, fosize, fo);
  } else {
    fprintf(stderr, "error during conversion\n");
  }
  fclose(fi);
  fclose(fo);
  free(codebuf);
  return 0;
}

