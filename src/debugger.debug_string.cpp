#include "debugger.h"


void Debugger::OnOutputString(const OUTPUT_DEBUG_STRING_INFO& details) {
    continue_status_ = DBG_EXCEPTION_NOT_HANDLED;
    cbOutputString(details);
}