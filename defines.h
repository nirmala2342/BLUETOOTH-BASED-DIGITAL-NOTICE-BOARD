/*defines.h */

#define SETBIT(WORD,BITPOS) \
        (WORD|=(1<<BITPOS))
#define CLRBIT(WORD,BITPOS) \
        (WORD&=~(1<<BITPOS))
#define CPLBIT(WORD,BITPOS) \
        (WORD^=(1<<BITPOS))
#define WRITEBIT(WORD,BITPOS,BIT) \
        (WORD=((WORD&~(1<<BITPOS))| \
				      (BIT<<BITPOS)))
                  
#define READBIT(WORD,BITPOS) \
        ((WORD>>BITPOS)&1)

#define WRITENIBBLE(WORD,STARTBITPOS,NIBBLE) \
      WORD=((WORD&~(15<<STARTBITPOS))| \
           (NIBBLE<<STARTBITPOS))

#define READNIBBLE(WORD,STARTBITPOS) \
        ((WORD>>STARTBITPOS)&15)

#define WRITEBYTE(WORD,STARTBITPOS,BYTE) \
        (WORD=((WORD&~(255<<STARTBITPOS))| \
             (BYTE<<STARTBITPOS)))

#define READBYTE(WORD,STARTBITPOS) \
        ((WORD>>STARTBITPOS)&255)

#define WRITEHWORD(WORD,STARTBITPOS,HWORD) \
        WORD=((WORD&~(0xFFFF<<STARTBITPOS)) | \
				      (HWORD<<STARTBITPOS))
#define SSETBIT(WORD,BITPOS) (WORD=(1<<BITPOS))
#define SCLRBIT SSETBIT    

				
#define READWRITEBIT2(DWORD,WBIT,SWORD,RBIT) \
        DWORD=((DWORD&~(1<<WBIT))| \
				(((SWORD>>RBIT)&1)<<WBIT))
				
#define READWRITEBIT(WORD,WBIT,RBIT) \
        WORD=((WORD&~(1<<WBIT))| \
				(((WORD>>RBIT)&1)<<WBIT))