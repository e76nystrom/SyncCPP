#!/cygdrive/c/Python27/Python.exe

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
    
f = open("include/pinDef.h", "wb")
f.write("#ifdef __STM32F1xx_HAL_H\n")
f.write("#if !defined(__PINDEF_H)\n")
f.write("#define __PINDEF_H\n\n")

for (dir, name, pin, comment) in pinList:
    if len(comment) != 0:
        f.write("/* %s */\n" % (comment))

    if dir == 'o':
        f.write("#ifdef %s_Pin\n" % (pin))
        f.write("%s %sSet() %s%s_GPIO_Port->BSRR = %s_Pin%s\n" % \
                (dout, name, op, pin, pin, cl))
        f.write("%s %sClr() "
                "%s%s_GPIO_Port->BSRR = (%s_Pin << 16)%s\n" % \
                (dout, name, op, pin, pin, cl))
        f.write("%s %s() %s(%s_GPIO_Port->BSRR & %s_Pin) != 0%s\n" % \
                (din, name, opIn, pin, pin, clIn))
        f.write("#else\n")
        f.write("%s %sSet()%s\n" % (dout, name, empty))
        f.write("%s %sClr()%s\n" % (dout, name, empty))
        f.write("%s %s() %s0%s\n" % \
                (din, name, opIn, clIn))
        f.write("#endif\n\n")
    elif dir == 'i':
        f.write("#ifdef %s_Pin\n" % (pin))
        f.write("%s %s() %s(%s_GPIO_Port->BSRR & %s_Pin) != 0%s\n" % \
                (din, name, opIn, pin, pin, clIn))
        f.write("%s %sSet() %s(%s_GPIO_Port->BSRR = %s_Pin) != 0%s\n" % \
                (din, name, opIn, pin, pin, clIn))
        f.write("%s %sClr() %s(%s_GPIO_Port->BSRR = %s_Pin) == 0%s\n" % \
                (din, name, opIn, pin, pin, clIn))
        f.write("#else\n")
        f.write("%s %sSet()%s\n" % (din, name, emptyIn))
        f.write("%s %sClr()%s\n" % (din, name, emptyIn))
        f.write("#endif\n\n")
    else:
        print("invalid dir\n");

f.write("#endif /* __PINDEF_H */\n")
f.write("#endif /* __STM32F1xx_HAL_H */\n")
f.close()

f = open("include/dbg.h", "wb")
f1 = open("include/dbgPin.h", "wb")

f.write("#ifdef __STM32F1xx_HAL_H\n")
f.write("#if !defined(__DBG_H)\n")
f.write("#define __DBG_H\n\n")

for pin in dbgPins:
    if CPP:
        f.write("constexpr auto DBG%d = 1;\n" % (pin))
    else:
        f.write("#define DBG%d 1\n" % (pin))
f.write("\n")

for pin in dbgPins:
    f.write("#ifdef Dbg%d_Pin\n" % (pin))
    f.write("%s dbg%dIni()%s\n" % (dout, pin, empty))
    f.write("%s dbg%dSet() %sDbg%d_GPIO_Port->BSRR = Dbg%d_Pin%s\n" % \
            (dout, pin, op, pin, pin, cl))
    f.write("%s dbg%dClr() %sDbg%d_GPIO_Port->BSRR = (Dbg%d_Pin << 16)%s\n" %\
            (dout, pin, op, pin, pin, cl))
    f.write("#else\n")
    f.write("%s dbg%dIni()%s\n" % (dout, pin, empty))
    f.write("%s dbg%dSet()%s\n" % (dout, pin, empty))
    f.write("%s dbg%dClr()%s\n" % (dout, pin, empty))
    f.write("#endif\n\n")

for (name, pin, comment) in dbgList:
    pin = int(pin)
    f.write("/* %s */\n" % (comment))
    if pin >= 0:
        f.write("#ifdef Dbg%d_Pin\n" % (pin))
        f.write("%s %sSet() %sDbg%d_GPIO_Port->BSRR = Dbg%d_Pin%s\n" % \
                (dout, name, op, pin, pin, cl))
        f.write("%s %sClr() "
                "%sDbg%d_GPIO_Port->BSRR = (Dbg%d_Pin << 16)%s\n" % \
                (dout, name, op, pin, pin, cl))
        f.write("#else\n")
        f.write("%s %sSet()%s\n" % (dout, name, empty))
        f.write("%s %sClr()%s\n" % (dout, name, empty))
        f.write("#endif\n\n")

        f1.write("#ifdef Dbg%d_Pin\n" % (pin))
        f1.write(" PIN(%s, Dbg%d),\n" % (name, pin))
        f1.write("#endif\n")
    else:
        f.write("%s %sSet()%s\n" % (dout, name, empty))
        f.write("%s %sClr()%s\n\n" % (dout, name, empty))
f.write("#endif /* __DBG_H */\n")
f.write("#endif /* __STM32F1xx_HAL_H */\n")
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

f.write("#define DBGTRK 1\n\n")

f.write("#if DBGTRK\n\n")

f.write("EXT boolean dbgTrk;\n")
f.write("#define TRKBUFSIZE (4*%d)\n" % trackSize)
f.write("EXT int16_t trkidx;\n")
f.write("EXT int16_t trkbuf[TRKBUFSIZE];\n\n")

for (label, comment) in dbgTrkList:
    define = "%s DBGTRK%s%s %d%s" % (d0, label, eq, label == dbgTrk, eol)
    f.write("%s/* %s */\n" % (define.ljust(32), comment))

f.write("\n")

for (label, arg, argType, macro) in dbgTrkCode:
    defined = label == code
    f.write("%s DBGTRK%s%s %d%s\n" % (d0, label, eq, defined, eol))
    f.write("%s dbgTrk%s(" % (d1, label))
    first = True
    for i in range(len(arg)):
        if first:
            first = False
        else:
            f.write(",\n\t")
        if CPP:
            f.write("%s " % (argType[i]))
        f.write(arg[i])
        if CPP:
            f.write("  __attribute__((unused))");
    f.write(")")
    if defined:
        f.write(" \\\n%s\n" % (macro))
    else:
        f.write("%s\n\n" % (empty))
                
f.write("#endif /* DBGTRK */\n")
f.close()
