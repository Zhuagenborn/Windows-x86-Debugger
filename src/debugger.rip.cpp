#include "debugger.h"


void Debugger::OnRip(const RIP_INFO& details) {
    continue_status_ = DBG_EXCEPTION_NOT_HANDLED;
    cbRip(details);
}