#include "../include/pdm/protobuf.h"
#include "pdm.h"
#include <google/protobuf/util/time_util.h>

namespace pdmproto = eve_launcher::pdm;

constexpr pdmproto::Bitness BitnessToProto(PDM::Bitness bitness)
{
	switch(bitness)
	{
	case PDM::Bitness::BITNESS_UNKNOWN:
		return pdmproto::BITNESS_UNKNOWN;
	case PDM::Bitness::BITNESS_32:
		return pdmproto::BITNESS_32;
	case PDM::Bitness::BITNESS_64:
		return pdmproto::BITNESS_64;
	default:
		throw std::invalid_argument("Invalid bitness");
	}
}

constexpr pdmproto::OS::Kind OSKindToProto(PDM::OS osKind)
{
	switch(osKind)
	{
	case PDM::OS::UNKNOWN:
		return pdmproto::OS::Kind::OS_Kind_UNKNOWN;
	case PDM::OS::WINDOWS:
		return pdmproto::OS::Kind::OS_Kind_WINDOWS;
	case PDM::OS::MACOS:
		return pdmproto::OS::Kind::OS_Kind_MACOS;
	case PDM::OS::WINE:
		return pdmproto::OS::Kind::OS_Kind_WINE;
	default:
		throw std::invalid_argument("Invalid osType");
	}
}

constexpr pdmproto::OS_GraphicsAPIs_VulkanSupport VulkanSupportToProto(PDM::VulkanSupport support)
{
	switch(support)
	{
	case PDM::VulkanSupport::UNKNOWN:
		return pdmproto::OS_GraphicsAPIs_VulkanSupport::OS_GraphicsAPIs_VulkanSupport_UNKNOWN;
	case PDM::VulkanSupport::SUPPORTED:
		return pdmproto::OS_GraphicsAPIs_VulkanSupport::OS_GraphicsAPIs_VulkanSupport_SUPPORTED;
	case PDM::VulkanSupport::UNSUPPORTED:
		return pdmproto::OS_GraphicsAPIs_VulkanSupport::OS_GraphicsAPIs_VulkanSupport_UNSUPPORTED;
	default:
		throw std::invalid_argument("Invalid vulkanSupport");
	}
}

namespace PDMProtobuf
{
	pdmproto::Attributes GetData(const std::string& applicationName, const std::string& applicationVersion)
	{
		pdmproto::Attributes data;

		auto application = data.mutable_application();
		application->set_name(applicationName.c_str());
		application->set_version(applicationVersion.c_str());

		auto process = data.mutable_process();
		process->set_version(PDM::GetPDMVersion());
		*process->mutable_timestamp() = google::protobuf::util::TimeUtil::GetCurrentTime();
		process->set_bitness(BitnessToProto(PDM::GetProcessBitness()));

		auto os = data.mutable_os();
		os->set_kind(OSKindToProto(PDM::GetOSType()));
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
		graphicsAPIs->set_vulkan_supported(VulkanSupportToProto(PDM::GetVulkanProperties().support));
		graphicsAPIs->set_vulkan_highest_support(PDM::GetVulkanProperties().version);
		graphicsAPIs->set_d3d_highest_support(PDM::GetD3DHighestSupport());

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

		for(auto monitorData : PDM::GetMonitorsInfo())
		{
			auto monitor = machine->add_monitors();
			monitor->set_name(monitorData.name);
			monitor->set_horizontal_resolution(monitorData.width);
			monitor->set_vertical_resolution(monitorData.height);
			monitor->set_bits_per_color(monitorData.bitsPerColor);
			monitor->set_refresh_rate(monitorData.refreshRate);
			monitor->set_dpi_scaling_percent(monitorData.dpiScaling);
		}

		for(auto gpuData : PDM::GetGPUInfo())
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

		for(auto networkAdapterData : PDM::GetNetworkAdapterInfo())
		{
			auto networkAdapter = machine->add_network_adapters();
			networkAdapter->set_name(networkAdapterData.name);
			networkAdapter->set_mac_address(networkAdapterData.macAddress);
			networkAdapter->set_uuid(networkAdapterData.uuid);
		}

		return data;
	}
}