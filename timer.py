#!/cygdrive/c/Python27/Python.exe

import sys
from collections import namedtuple
TmrCfg = namedtuple('TmrCfg', ('name', 'tmr', 'argType', 'pwm',
                               'isr', 'slave'))

CPP = True

CAP = 0x20
PWM_MASK = 0x7

if CPP:
    defOut = "inline void"
    defIn = "inline "
    if False:
        op = "\n{\n "
        cl = ";\n}\n"
    else:
        op = "{"
        cl = ";}"
else:
    define = "#define"
    argType = ""
    op = ""
    cl = ""

constants = None
# constants = \
# (\
#  ("auto", "SP_FWD", "1"),
#  ("auto", "SP_REV", "-1"),
#  ("auto", "STEP_WIDTH", "10"),
# )

condConstants = None
# condConstants = \
# (\
#  ("spindleTmr == 8",
#   ( \
#     (None, "DIR_SPIN_PORT", "Dir5_GPIO_Port"), \
#     ("uint32_t", "DIR_SPIN_BIT", "Dir5_Pin"))), \
#  ("spindleTmr == 3",
#   ( \
#     (None, "DIR_SPIN_PORT", "Dir3_GPIO_Port"), \
#     ("uint32_t", "DIR_SPIN_BIT", "Dir3_Pin"))), \
# )

macros = None
# macros = \
# (\
#  (None, "dirSpinFwd", None, None, "DIR_SPIN_PORT->BSRR = spA.dirFwd"),
#  (None, "dirSpinRev", None, None, "DIR_SPIN_PORT->BSRR = spA.dirRev"),
#  (None, "dirZFwd",    None, None, "DIR_SPIN_PORT->BSRR = zAxis.dirFwd"),
#  (None, "dirZRev",    None, None, "DIR_SPIN_PORT->BSRR = zAxis.dirRev"),
#  (None, "dirXFwd",    None, None, "DIR_SPIN_PORT->BSRR = xAxis.dirFwd"),
#  (None, "dirXRev",    None, None, "DIR_SPIN_PORT->BSRR = xAxis.dirRev"),
#  ("uint32_t", "CALC_STEP_WIDTH", "uint32_t", "x", "(cfgFcy * x) / 1000000l"),
# )

regList = \
( \
  ("ClrIE",  None,  "%s->DIER &= ~TIM_IT_UPDATE"),
  ("SetIE",  None,  "%s->DIER |= TIM_IT_UPDATE"),
  ("TstIE",  "is",  "(%s->DIER & TIM_IT_UPDATE) != 0"),
  ("IF",     "is",  "(%s->SR & TIM_FLAG_UPDATE) != 0"),
  ("ClrIF",  None,  "%s->SR = ~TIM_FLAG_UPDATE"),
  ("Start",  None,  "%s->CR1 |= TIM_CR1_CEN"),
  ("Pulse",  None,  "%s->CR1 |= (TIM_CR1_OPM | TIM_CR1_CEN)"),
  ("Stop",   None,  "%s->CR1 &= ~(TIM_CR1_OPM | TIM_CR1_CEN)"),
  ("Scl",    "y",   "%s->PSC = (y)"),
  ("Read",   "i",   "%s->CNT"),
  ("CntClr", None,  "%s->CNT = 0"),
  ("Cnt",    "x",   "%s->CNT = (x)"),
  ("Max",    "x",   "%s->ARR = ((x) - 1)"),
  ("Set",    "x",   "%s->ARR = (x)"),
  ("MaxRead", "i",  "%s->ARR"),
)

pwm1List = \
( \
  ("CCR",       "x",  "%s->CCR1 = (x)"),
  ("PWMMode",   None, "%s->CCMR1 = (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1)"),
  ("PWMEna",    "b",  "%s->CCER |= TIM_CCER_CC1E"),
  ("PWMDis",    None, "%s->CCER &= ~TIM_CCER_CC1E"),
  ("ReadCCR",   "i",  "%s->CCR1"),
  ("ReadCCMR", "is", "%s->CCMR1"),
)

pwm2List = \
( \
  ("CCR",       "x",  "%s->CCR1 = (x)"),
  ("PWMMode",   None, "%s->CCMR1 = (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1)"),
  ("PWMEna",    "b",  "%s->CCER |= TIM_CCER_CC2E"),
  ("PWMDis",    None, "%s->CCER &= ~TIM_CCER_CC2E"),
  ("ReadCCR",   "i",  "%s->CCR2"),
  ("ReadCCMR", "is", "%s->CCMR1"),
)

pwm3List = \
( \
  ("CCR",       "x",  "%s->CCR3 = (x)"),
  ("PWMMode",   None, "%s->CCMR2 = (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1)"),
  ("PWMEna",    "b",  "%s->CCER |= TIM_CCER_CC3E"),
  ("PWMDis",    None, "%s->CCER &= ~TIM_CCER_CC3E"),
  ("ReadCCR",   "i",  "%s->CCR3"),
  ("ReadCCMR", "is", "%s->CCMR2"),
)

pwm4List = \
( \
  ("CCR",       "x",  "%s->CCR4 = (x)"),
  ("PWMMode",   None, "%s->CCMR2 = (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1)"),
  ("PWMEna",    "b",  "%s->CCER |= TIM_CCER_CC4E"),
  ("PWMDis",    None, "%s->CCER &= ~TIM_CCER_CC4E"),
  ("ReadCCR",   "i",  "%s->CCR4"),
  ("ReadCCMR", "is", "%s->CCMR2"),
)

capList = \
( \
  ("Cap1EnaSet", None, "%s->CCER |= TIM_CCER_CC1E"), \
  ("Cap1SetIE",  None, "%s->DIER |= TIM_DIER_CC1IE"), \
  ("Cap1ClrIE",  None, "%s->DIER &= ~TIM_DIER_CC1IE"), \
  ("Cap1IF",     "i",  "(%s->SR & TIM_SR_CC1IF) != 0"), \
  ("Cap1ClrIF",  None, "%s->SR &= ~TIM_SR_CC1IF"), \
  ("Cap1",       "i",  "%s->CCR1"), \

  ("Cap2EnaSet", None, "%s->CCER |= TIM_CCER_CC2E"), \
  ("Cap2SetIE",  None, "%s->DIER |= TIM_DIER_CC2IE"), \
  ("Cap2ClrIE",  None, "%s->DIER &= ~TIM_DIER_CC2IE"), \
  ("Cap2IF",     "i",  "(%s->SR & TIM_SR_CC1IF) != 0"), \
  ("Cap2ClrIF",  None, "%s->SR &= ~TIM_SR_CC1IF"), \
  ("Cap2",       "i",  "%s->CCR2"), \

  ("OCP1Clr",    None, "%s->SR &= ~TIM_SR_CC1OF"), \
  ("OCP2Clr",    None, "%s->SR &= ~TIM_SR_CC2OF"), \
)

pwmList = (pwm1List, pwm2List, pwm3List, pwm4List)

def makeConstant(cons):
    (type, name, val) = cons
    if CPP and type is not None:
        f.write("constexpr %s %s = %s;\n" % (type, name, val))
    else:
        f.write("#define %s (%s)\n" % (name, val))

def makeMacro(macro, nameLen=None):
    (rtnType, funcName, arg, argType, body) = macro
    if CPP:
        if rtnType is None:
            funcType = "inline void"
        else:
            funcType = "inline %s" % (rtnType)
            body = "return(" + body + ")"
    else:
        funcType = "#define"
    argList = ""
    if arg is not None:
        if CPP:
            argList = arg + " " + argType
        else:
            argList = arg
    call = "%s(%s)" % (funcName, argList)
    if nameLen is not None:
        pass
    else:
        f.write("%s %s %s%s%s\n" % \
                (funcType, call, op, body, cl))

def timerInit(name, tmr, timer):
    body = (" \\\n\t__HAL_RCC_TIM%d_CLK_ENABLE(); \\\n"
            "\t%s->CR1 |= TIM_CR1_DIR; \\\n"
            "\t%s->CR1 &= ~TIM_CR1_CEN")
    body = body.replace("%d", str(tmr))
    makeFunc(timer, "Init" , None, body)
    f.write("\n")

def timerStart(name, timer):
    body = (" \\\n\t%s->EGR = TIM_EGR_UG; \\\n"
            "\t%s->CR1 |= TIM_CR1_CEN")
    makeFunc(timer, "Start" , None, body)
    f.write("\n")

def timerBDTR(name, tmr, timer):
    if tmr == 1 or tmr == 8:
        body = "%s->BDTR |= TIM_BDTR_MOE"
        makeFunc(timer, "BDTR" , None, body)
        f.write("\n")

def makeFuncList(timer, lst):
    maxLen = 0
    extra = len(name) + 2
    argLen = len(argType) + 2
    for (funcName, arg, body) in lst:
        l = len(funcName) + extra
        if arg == "x" or arg == "y":
            l += argLen
        if l > maxLen:
            maxLen = l
    # print("maxLen %d" % (maxLen))
    for (funcName, arg, body) in lst:
        makeFunc(timer, funcName, arg, body, nameLen=maxLen)
    f.write("\n")

def makeFunc(timer, funcName, arg, body, nameLen=None):
    argList = ""
    if CPP:
        if arg == "i":
            define = "inline " + argType
        elif arg == "is":
            define = "inline uint16_t"
        else:
            define = "inline void"
            if arg == "x":
                argList = "%s %s" % (argType, arg)
            elif arg == "y":
                argList = "uint16_t %s" % (arg)
            elif arg == "b":
                if timer == "TIM1" or timer == "TIM8":
                    body = name + "BDTR(); " + body
    else:
        define = "#define"
        if arg == "x" or arg =="y":
            argList = arg
    if body is not None:
        body = body.replace("%s", timer)
    else:
        body = ""
    if arg is not None and arg.startswith("i"):
        body = "(" + body + ")"
        if CPP:
            body = "return" + body
    call = "%s%s(%s)" % (name, funcName, argList)
    if nameLen is not None:
        call = call.ljust(nameLen)
        if CPP:
            define = define.ljust(15)
        body = op + body + cl
        if (len(body) + nameLen + 15) >= 80:
            body = "\\\n\t" + body
        f.write("%s %s %s\n" % (define, call, body))
    else:
        f.write("%s %s %s%s%s\n" % \
                (define, call, op, body, cl))

def main(cfg, path):
    global f
    global name, tmr, argType, pwm, isr, slave

    f = open("../" + path + "/inc/timers.h", "wb")
    f.write("#ifdef __STM32F1xx_HAL_H\n")
    f.write("#if !defined(__TIMERS_H)\n")
    f.write("#define __TIMERS_H\n\n")

    setConfig(cfg)

    maxLen = 0
    for tmp in cfg:
        parm = tmp[0]
        if len(parm) > maxLen:
            maxLen = len(parm)
    f.write("/*\n")
    for tmp in cfg:
        parm = tmp[0]
        val = tmp[1]
        f.write("%s %s\n" % (parm.ljust(maxLen), str(globals()[parm])))
    f.write("*/\n\n")

    for tmp in cfg:
        val = tmp[1]
        if len(tmp) > 2:
            symbol = tmp[2]
            f.write("#define %s%s\n" % (symbol, val))
    f.write("\n")

    triggers()

    if constants is not None:
        for c in constants:
            makeConstant(c)
        f.write("\n")

    if condConstants is not None:
        for (test, constantList) in condConstants:
            cond = eval(test)
            if cond:
                for c in constantList:
                    makeConstant(c)
            f.write("\n")

    if macros is not None:
        for m in macros:
            makeMacro(m)
        f.write("\n")

    for t in timers:
        name = t.name
        tmr = t.tmr
        argType = t.argType
        pwm = t.pwm & PWM_MASK
        isr = t.isr
        slave = t.slave
        if tmr == 0:
            continue

        timer = "TIM%d" % tmr

        f.write("/* %s timer %d" % (name, tmr))
        if pwm != 0:
            f.write(" pwm %d" % (pwm))
        f.write(" */\n\n")

        f.write("#define %s %s\n" % (name.upper().replace("TMR", "_TIMER"), tmr))
        f.write("#define %s %s\n\n" % (name.upper().replace("TMR", "_TMR"), timer))

        if isr != None and len(isr) != 0:
            f.write("#define %sISR(x) %s_IRQHandler(x)\n\n" % (name, isr))

        timerInit(name, tmr, timer)
        # timerStart(name, timer)
        timerBDTR(name, tmr, timer)

        makeFuncList(timer, regList)
        if pwm != 0:
            f.write("/* pwm %d */\n\n" % (pwm))
            f.write("#define %s_PWM %d\n\n" % \
                    (name.upper().replace("TMR", "_TMR"), pwm))
            makeFuncList(timer, pwmList[pwm - 1])

        if (t.pwm & CAP) != 0:
            makeFuncList(timer, capList)

        if slave:
            # print("slave %d spindle %d" % (tmr, spindleTmr))
            for (slv, trig) in slaveTrig:
                # print("slvx %d" % (slv))
                if tmr == slv:
                    for (i, trigTimer) in enumerate(trig):
                        # print("i %d trigTimer %d" % (i, trigTimer))
                        if spindleTmr == trigTimer:
                            body = timer + "->SMCR = ("
                            if (i & 2) != 0:
                                body += "TIM_SMCR_TS_1 | "
                            if (i & 1) != 0:
                                body += "TIM_SMCR_TS_0 | "
                            body += "\\\n"
                            body += "\tTIM_SMCR_SMS_2 | TIM_SMCR_SMS_1)"
                            f.write("/* timer %d trigger %d */\n\n" % (spindleTmr, i))
                            makeFunc(timer, "SlvEna", None, body)
                            body = timer + "->SMCR = 0"
                            makeFunc(timer, "SlvDis", None, body)
                            f.write("\n")
                            break

    f.write("#endif /* __TIMERS_H */\n")
    f.write("#endif /* __STM32F4xx_HAL_H */\n")
    f.close()
    
sync = \
( \
  ("board",     "sync"),
  ("proc",      "STM32F103"),
  ("encTmr",    2, "ENC_TMR"),
  ("intTmr",    3, "INT_TMR"),
  ("intTmrPwm", 0, "INT_TMR_PWM"),
  ("cmpTmr",    4, "CMP_TMR"),
  ("encTmrIsr", "TIM2"),
  ("intTmrIsr", "TIM3"),
  ("cmpTmrIsr", "TIM4"),
)

def triggers():
    global slaveTrig
    if proc == "STM32F407" or proc == "STM32F446":
        slaveTrig = \
        (\
        (2, (1, 8, 3, 4)),
        (3, (1, 2, 5, 4)),
        (4, (1, 2, 3, 8)),
        (5, (2, 3, 4, 8)),
        (1, (5, 2, 3, 4)),
        (8, (1, 2, 4, 5)),
        )
    elif proc == "STM32F401":
        slaveTrig = \
        (\
        (2, (1, 0, 3, 4)),
        (3, (1, 2, 5, 4)),
        (4, (1, 2, 3, 0)),
        (5, (2, 3, 4, 0)),
        (1, (5, 2, 3, 4)),
        )

def setConfig(cfg):
    global timers
    for tmp in cfg:
        parm = tmp[0]
        val = tmp[1]
        globals()[parm] = val

    timers = \
    ( \
      TmrCfg("encTmr", encTmr, "uint16_t", 0, "TIM2", None),
      TmrCfg("intTmr", intTmr, "uint16_t", 0, "TIM3", None),
      TmrCfg("cmpTmr", cmpTmr, "uint16_t", CAP, "TIM4", None),
    )

cfg = sync
path = "Sync"
# n = 1
# while True:
#     if n >= len(sys.argv):
#         break
#     val = sys.argv[n]
#     if val == "disc407":
#         cfg = disc407
#         path = "LatheCPP"
#     elif val == "core407":
#         cfg = core407
#         path = "LatheD"
#     elif val == "nuc446":
#         cfg = nuc446
#         path = "LatheN"
#     elif val == "nuc401":
#         cfg = nuc401
#         path = "LatheX0"
#     n += 1

main(cfg, path)
