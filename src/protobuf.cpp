#include "../include/pdm/protobuf.h"
#include "pdm.h"
#include <google/protobuf/util/time_util.h>
#include "semver.h"

platform::Bitness BitnessToProto(PDM::Bitness bitness)
{
	switch(bitness)
	{
	case PDM::Bitness::BITNESS_32:
		return platform::BITNESS_32;
	case PDM::Bitness::BITNESS_64:
		return platform::BITNESS_64;
    case PDM::Bitness::BITNESS_UNKNOWN:
	default:
        return platform::BITNESS_UNKNOWN;
	}
}

platform::OS::Kind OSKindToProto(PDM::OS osKind)
{
	switch(osKind)
	{
	case PDM::OS::UNKNOWN:
		return platform::OS::Kind::OS_Kind_UNKNOWN;
	case PDM::OS::WINDOWS:
		return platform::OS::Kind::OS_Kind_WINDOWS;
	case PDM::OS::MACOS:
		return platform::OS::Kind::OS_Kind_MACOS;
	case PDM::OS::WINE:
		return platform::OS::Kind::OS_Kind_WINE;
	default:
		throw std::invalid_argument("Invalid osType");
	}
}

void AugmentVersion(const std::string& pdm_version, platform::SemanticVersion* result)
{
    pdm::SemanticVersion version;

    (void)pdm::ParseSemanticVersion(pdm_version, version);

    result->set_major(version.major);
    result->set_minor(version.minor);
    result->set_patch(version.patch);
    result->set_prerelease(version.pre_release);
    result->set_build(version.build);
}

namespace pdm_proto
{
	DllExport platform::Information GetData()
	{
		platform::Information data;

        *data.mutable_timestamp() = google::protobuf::util::TimeUtil::GetCurrentTime();
        data.set_process_bitness(BitnessToProto(PDM::GetProcessBitness()));

        AugmentVersion(PDM::GetPDMVersion(), data.mutable_version());

		auto os = data.mutable_os();
		os->set_type(OSKindToProto(PDM::GetOSType()));
		os->set_name(PDM::GetOSName());
		os->set_bitness(BitnessToProto(PDM::GetOSBitness()));
		os->set_major_version(PDM::GetOSMajorVersion());
		os->set_minor_version(PDM::GetOSMinorVersion());
		os->set_build_number(PDM::GetOSBuildNumber());
		os->set_kernel_version(PDM::GetOSKernelVersion());
		os->set_username(PDM::GetUsername());
		os->set_user_locale(PDM::GetUserLocale());
		os->set_is_remote_session(PDM::IsRemoteSession());

		auto graphicsAPIs = os->mutable_graphics_apis();
		graphicsAPIs->set_metal_supported(PDM::GetMetalSupported());
		auto vulkan = PDM::GetVulkanProperties();
		graphicsAPIs->set_vulkan_supported(vulkan.support == PDM::VulkanSupport::SUPPORTED);
		graphicsAPIs->set_vulkan_highest_supported_version(vulkan.version);
		graphicsAPIs->set_d3d_highest_supported_version(PDM::GetD3DHighestSupport());

		auto wine = os->mutable_wine();
		wine->set_version(PDM::GetWineVersion());
		wine->set_host_os(PDM::GetWineHostOs());

		auto machine = data.mutable_machine();
		machine->set_name(PDM::GetMachineName());
		machine->set_model(PDM::GetHardwareModel());
		machine->set_uuid(PDM::GetMachineUuid());
		machine->set_total_memory(PDM::GetTotalMemory());
		machine->set_monitor_count(PDM::GetMonitorCount());
	
		auto cpu = machine->mutable_cpu();
		auto cpuInfo = PDM::GetCPUInfo();
		cpu->set_bitness(BitnessToProto(cpuInfo.bitness));
		cpu->set_logical_core_count(cpuInfo.logicalCoreCount);
		cpu->set_brand(cpuInfo.brand);
		cpu->set_vendor(cpuInfo.vendor);
		cpu->set_model(cpuInfo.model);
		cpu->set_stepping(cpuInfo.stepping);

		auto vm = machine->mutable_vm();
		vm->set_is_suspected_vm(PDM::IsSuspectedVM());
		vm->set_has_hypervisor_bit(PDM::HasHypervisorBit());
		vm->set_hypervisor_name(PDM::GetHypervisorName());
		vm->set_is_hypervisor_guest_os(PDM::IsHyperVGuestOS());
		vm->set_has_vm_execution_timing(PDM::HasVMExecutionTiming());

		for(const auto& monitorData : PDM::GetMonitorsInfo())
		{
			auto monitor = machine->add_monitors();
			monitor->set_name(monitorData.name);
			monitor->set_horizontal_resolution(monitorData.width);
			monitor->set_vertical_resolution(monitorData.height);
			monitor->set_bits_per_color(monitorData.bitsPerColor);
			monitor->set_refresh_rate(monitorData.refreshRate);
			monitor->set_dpi_scaling_percent(monitorData.dpiScaling);
		}

		for(const auto& gpuData : PDM::GetGPUInfo())
		{
			auto gpu = machine->add_gpus();
			gpu->set_description(gpuData.description);
			gpu->set_vendor_id(gpuData.vendorID);
			gpu->set_device_id(gpuData.deviceID);
			gpu->set_revision(gpuData.revision);
			gpu->set_video_memory(gpuData.memory);

			auto driver = gpu->mutable_driver();
			driver->set_date(gpuData.driverDate);
			driver->set_vendor(gpuData.driverVendor);
			driver->set_version(gpuData.driverVersionString);
		}

		for(const auto& networkAdapterData : PDM::GetNetworkAdapterInfo())
		{
			auto networkAdapter = machine->add_network_adapters();
			networkAdapter->set_name(networkAdapterData.name);
			networkAdapter->set_mac_address(networkAdapterData.macAddress);
			networkAdapter->set_uuid(networkAdapterData.uuid);
		}

		return data;
	}

	// Re-expose
	void exposure_dummy()
	{
		PDM::RetrievePDMData( "", "" );
	}
}