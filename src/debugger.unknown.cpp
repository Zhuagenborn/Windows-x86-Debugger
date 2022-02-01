#include "debugger.h"


void Debugger::OnUnknownEvent(const std::uint32_t event_code) {
    continue_status_ = DBG_EXCEPTION_NOT_HANDLED;
    cbUnknownEvent(event_code);
}