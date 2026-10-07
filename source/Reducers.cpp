#include "Reducers.h"

std::string modeLabel(ExecMode mode) {
    switch (mode) {
    case ExecMode::NoPolicy: return "no_policy";
    case ExecMode::Seq:      return "seq";
    case ExecMode::Unseq:    return "unseq";
    case ExecMode::Par:      return "par";
    case ExecMode::ParUnseq: return "par_unseq";
    }
    return "unknown";
}
