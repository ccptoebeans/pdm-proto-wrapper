#pragma once

#include <ostream>

namespace pdm_proto
{
  bool GetEVEPublicData(std::ostream* stream);
  bool GetEVELauncherData(std::ostream* stream);
}
