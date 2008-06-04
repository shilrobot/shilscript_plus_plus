#ifndef SS_COMPILEERRORS_H
#define SS_COMPILEERRORS_H

#include "Prereqs.h"
#include "Node.h"

namespace SS {

SS_EXPORT void ReportError(String format, ...);
SS_EXPORT void ReportError(const Node* obj, String format, ...);
SS_EXPORT void ReportWarning(String format, ...);
SS_EXPORT void ReportWarning(const Node* obj, String format, ...);

SS_EXPORT bool AnyErrors();

SS_EXPORT String QuoteString(const String& input);

#define SS_QUOTE(s) (SS::QuoteString(s).c_str())

}

#endif // SS_COMPILEERRORS_H
