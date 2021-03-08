#pragma once

#include "eve_public/app/platform.pb.h"

namespace platform = eve_public::app::platform;

namespace pdm_proto
{
    DllExport platform::Information GetData();
}
