#include "pdm.h"

namespace PDM
{
	// Force PDM function export without a DEF file
	void export_pdm_functions_dummy()
	{
		GetPDMVersion();
		GetOSType();
		GetOSName();
		GetOSMajorVersion();
		GetOSMinorVersion();
		GetOSBuildNumber();
		GetOSKernelVersion();
		GetHardwareModel();
		GetMachineName();
		GetUsername();
		GetUserLocale();
		GetMonitorCount();
		GetTotalMemory();
		IsRemoteSession();
		GetTimingCycles();
		GetMachineUuid();
		GetMonitorsInfo();
		GetGPUInfo();
		GetNetworkAdapterInfo();
		GetMetalSupported();
		GetVulkanProperties();
		GetD3DHighestSupport();
		IsWine();
		GetWineVersion();
		GetWineHostOs();
		GetProcessBitness();
		GetOSBitness();
		GetCPUInfo();
		HasVMExecutionTiming();
		HasHypervisorBit();
		GetHypervisorName();
		IsHyperVGuestOS();
		IsSuspectedVM();
	}
}