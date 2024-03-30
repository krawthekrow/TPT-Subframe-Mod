#pragma once
#include <array>

#include "SimulationData.h"
#include "Element.h"

#define ELEMENT_NUMBERS_ENUMERATE
#include "ElementNumbers.h"
#undef ELEMENT_NUMBERS_ENUMERATE

std::array<Element, PT_NUM> const &GetElements();

const String FILT_MODES[] = {"SET", "AND", "OR", "SUB", "<<", ">>", "no eff", "XOR", "NOT", "QRTZ scat", "<<<", ">>>"};
const int FILT_NUM_MODES = sizeof(FILT_MODES) / sizeof(FILT_MODES[0]);
