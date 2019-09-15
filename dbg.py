#!/cygdrive/c/Python37/Python.exe

CPP = True

pinList = \
(\
 ("o", "encoder",   "Encoder",   "encoder pulse output"),
 ("o", "syncOut",   "SyncOut",   "sync signal output"),
 ("i", "zFlag",     "ZFlag",     "start encoder"),
 ("o", "xFlag",     "XFlag",     "encoder ready"),
 ("i", "start",     "ZFlag",     "start encoder"),
 ("o", "ready",     "XFlag",     "encoder ready"),
 ("o", "led",       "Led",       "led output"),
 ("i", "a",         "A",         "a encoder input"),
 ("i", "b",         "B",         "b encoder input"),
 ("o", "aTest",     "ATest",     "a test signal"),
 ("o", "bTest",     "BTest",     "b test signal"),
 ("o", "indexTest", "IndexTest", "sync test signal"),
)

dbgList = \
(\
 ("dbgIntC",    0, "toggle on capture input"),
 ("dbgIntP",    1, "toggle on output pulse"),
 ("dbgCycle",   2, "toggle on input cycle"),
 ("dbgCycEnd",  3, "end of internal pulse cycle"),
 ("dbgCapIsr",  4, "length of capture isr"),
 ("dbgIntIsr",  5, "length of internal isr"),
)

dbgPins = (0, 1, 2, 3, 4, 5)

trackSize = 64

DBGTRK = 1
dbgTrk = "2WL0"

dbgTrkList = \
(\
 ("2WL0", "encoder capature"),
 ("2WL1", "interal capature"),
)

if CPP:
    dout = "inline void"
    din = "inline uint16_t"
    empty = " {}"
    emptyIn = " {return(0);}"
    op = "{"
    cl = ";}"
    opIn = "{return("
    clIn = ");}"
else:
    dout = "#define"
    din = "#define"
    empty = ""
    emptyIn = ""
    op = ""
    cl = ""
    opIn = ""
    clIn = ""
    
def fWrite(file, txt):
    file.write(txt.encode())
    
f = open("include/pinDef.h", "wb")
fWrite(f,"#ifdef __STM32F1xx_HAL_H\n")
fWrite(f,"#if !defined(__PINDEF_H)\n")
fWrite(f,"#define __PINDEF_H\n\n")

for (dir, name, pin, comment) in pinList:
    if len(comment) != 0:
        fWrite(f,"/* %s */\n" % (comment))

    if dir == 'o':
        fWrite(f,"#ifdef %s_Pin\n" % (pin))
        fWrite(f,"%s %sSet() %s%s_GPIO_Port->BSRR = %s_Pin%s\n" % \
                (dout, name, op, pin, pin, cl))
        fWrite(f,"%s %sClr() "
                "%s%s_GPIO_Port->BSRR = (%s_Pin << 16)%s\n" % \
                (dout, name, op, pin, pin, cl))
        fWrite(f,"%s %s() %s(%s_GPIO_Port->ODR & %s_Pin) != 0%s\n" % \
                (din, name, opIn, pin, pin, clIn))
        fWrite(f,"#else\n")
        fWrite(f,"%s %sSet()%s\n" % (dout, name, empty))
        fWrite(f,"%s %sClr()%s\n" % (dout, name, empty))
        fWrite(f,"%s %s() %s0%s\n" % \
                (din, name, opIn, clIn))
        fWrite(f,"#endif\n\n")
    elif dir == 'i':
        fWrite(f,"#ifdef %s_Pin\n" % (pin))
        fWrite(f,"%s %s() %s(%s_GPIO_Port->IDR & %s_Pin) != 0%s\n" % \
                (din, name, opIn, pin, pin, clIn))
        fWrite(f,"%s %sIsSet() %s(%s_GPIO_Port->IDR & %s_Pin) != 0%s\n" % \
                (din, name, opIn, pin, pin, clIn))
        fWrite(f,"%s %sIsClr() %s(%s_GPIO_Port->IDR & %s_Pin) == 0%s\n" % \
                (din, name, opIn, pin, pin, clIn))
        fWrite(f,"#else\n")
        fWrite(f,"%s %sSet()%s\n" % (din, name, emptyIn))
        fWrite(f,"%s %sClr()%s\n" % (din, name, emptyIn))
        fWrite(f,"#endif\n\n")
    else:
        print("invalid dir\n");

fWrite(f,"#endif /* __PINDEF_H */\n")
fWrite(f,"#endif /* __STM32F1xx_HAL_H */\n")
f.close()

f = open("include/dbg.h", "wb")
f1 = open("include/dbgPin.h", "wb")

fWrite(f,"#ifdef __STM32F1xx_HAL_H\n")
fWrite(f,"#if !defined(__DBG_H)\n")
fWrite(f,"#define __DBG_H\n\n")

for pin in dbgPins:
    if CPP:
        fWrite(f,"constexpr auto DBG%d = 1;\n" % (pin))
    else:
        fWrite(f,"#define DBG%d 1\n" % (pin))
fWrite(f,"\n")

for pin in dbgPins:
    fWrite(f,"#ifdef Dbg%d_Pin\n" % (pin))
    fWrite(f,"%s dbg%dIni()%s\n" % (dout, pin, empty))
    fWrite(f,"%s dbg%dSet() %sDbg%d_GPIO_Port->BSRR = Dbg%d_Pin%s\n" % \
            (dout, pin, op, pin, pin, cl))
    fWrite(f,"%s dbg%dClr() %sDbg%d_GPIO_Port->BSRR = (Dbg%d_Pin << 16)%s\n" %\
            (dout, pin, op, pin, pin, cl))
    fWrite(f,"#else\n")
    fWrite(f,"%s dbg%dIni()%s\n" % (dout, pin, empty))
    fWrite(f,"%s dbg%dSet()%s\n" % (dout, pin, empty))
    fWrite(f,"%s dbg%dClr()%s\n" % (dout, pin, empty))
    fWrite(f,"#endif\n\n")

for (name, pin, comment) in dbgList:
    pin = int(pin)
    fWrite(f,"/* %s */\n" % (comment))
    if pin >= 0:
        fWrite(f,"#ifdef Dbg%d_Pin\n" % (pin))
        fWrite(f,"%s %sSet() %sDbg%d_GPIO_Port->BSRR = Dbg%d_Pin%s\n" % \
                (dout, name, op, pin, pin, cl))
        fWrite(f,"%s %sClr() "
                "%sDbg%d_GPIO_Port->BSRR = (Dbg%d_Pin << 16)%s\n" % \
                (dout, name, op, pin, pin, cl))
        fWrite(f,"#else\n")
        fWrite(f,"%s %sSet()%s\n" % (dout, name, empty))
        fWrite(f,"%s %sClr()%s\n" % (dout, name, empty))
        fWrite(f,"#endif\n\n")

        fWrite(f1,"#ifdef Dbg%d_Pin\n" % (pin))
        fWrite(f1," PIN(%s, Dbg%d),\n" % (name, pin))
        fWrite(f1,"#endif\n")
    else:
        fWrite(f,"%s %sSet()%s\n" % (dout, name, empty))
        fWrite(f,"%s %sClr()%s\n\n" % (dout, name, empty))
fWrite(f,"#endif /* __DBG_H */\n")
fWrite(f,"#endif /* __STM32F1xx_HAL_H */\n")
f.close()
f1.close()

dbgTrkCode = \
(\
 ("1W", ("val1",), ("uint16_t",), \
  "{ \\\n" \
  " int16_t *p = (int16_t *) &trkbuf[trkidx]; \\\n" \
  " *p = val1; \\\n" \
  " trkidx += 1; \\\n" \
  " trkidx &= (TRKBUFSIZE - 1); \\\n" \
  "}\n"), \

 ("1L", ("val1",) , ("uint32_t",), \
  "{ \\\n" \
  " int32_t *p = (int32_t *) &trkbuf[trkidx]; \\\n" \
  " *p = val1; \\\n" \
  " trkidx += 2; \\\n" \
  " trkidx &= (TRKBUFSIZE - 1); \\\n" \
  "}\n"), \

 ("2L", ("val1", "val2"), ("uint32_t", "uint32_t"), \
  "{ \\\n" \
  " int32_t *p = (int32_t *) &trkbuf[trkidx]; \\\n" \
  " *p++ = val1; \\\n" \
  " *p = val2; \\\n" \
  " trkidx += 4; \\\n" \
  " trkidx &= (TRKBUFSIZE - 1); \\\n" \
  "}\n"), \

 ("2WL", ("val1", "val2", "val3",), ("uint16_t", "uint16_t", "uint32_t"), \
  "{ \\\n" \
  " int16_t *p = (int16_t *) &trkbuf[trkidx]; \\\n" \
  " *p++ = val1; \\\n" \
  " *p++ = val2; \\\n" \
  " *((int32_t *) p) = val3; \\\n" \
  " trkidx += 4; \\\n" \
  " trkidx &= (TRKBUFSIZE - 1); \\\n" \
  "}\n"), \
)

f = open("include/dbgtrk.h", "wb")

code = dbgTrk[:-1]

if CPP:
    d0 = "constexpr auto"
    d1 = "inline void"
    eq = " ="
    empty = " {}"
    eol = ";"
else:
    d0 = "#define"
    d1 = "#define"
    eq = ""
    empty = ""
    eol = ""

fWrite(f,"#define DBGTRK %d\n\n" % (DBGTRK))

#fWrite(f,"#if DBGTRK\n\n")

fWrite(f,"EXT boolean dbgTrk;\n")
fWrite(f,"#define TRKBUFSIZE (4*%d)\n" % trackSize)
fWrite(f,"EXT int16_t trkidx;\n")
fWrite(f,"EXT int16_t trkbuf[TRKBUFSIZE];\n\n")

for (label, comment) in dbgTrkList:
    define = "%s DBGTRK%s%s %d%s" % (d0, label, eq, label == dbgTrk, eol)
    fWrite(f,"%s/* %s */\n" % (define.ljust(32), comment))

fWrite(f,"\n")

for (label, arg, argType, macro) in dbgTrkCode:
    if dbgTrk != 0:
        defined = label == code
    else:
        defined = False
    fWrite(f,"%s DBGTRK%s%s %d%s\n" % (d0, label, eq, defined, eol))
    fWrite(f,"%s dbgTrk%s(" % (d1, label))
    first = True
    for i in range(len(arg)):
        if first:
            first = False
        else:
            fWrite(f,",\n\t")
        if CPP:
            fWrite(f,"%s " % (argType[i]))
        fWrite(f,arg[i])
        if CPP:
            fWrite(f,"  __attribute__((unused))");
    fWrite(f,")")
    if defined:
        fWrite(f," \\\n%s\n" % (macro))
    else:
        fWrite(f,"%s\n\n" % (empty))
                
#fWrite(f,"#endif /* DBGTRK */\n")
f.close()
