#pragma once

#include "../pdm/protobuf.h"
#include "../../generated/pdm.pb.h"

#if _WIN32
#define DllExport __declspec( dllexport )
#elif __APPLE__
#define DllExport __attribute__((visibility("default")))
#endif

namespace pdm
{
    DllExport eve_launcher::pdm::Attributes GetData(const std::string& applicationName, const std::string& applicationVersion);
}
