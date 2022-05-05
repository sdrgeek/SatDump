#include "core/plugin.h"
#include "logger.h"
#include "rtlsdr_sdr.h"

class RtlSdrSDRSupport : public satdump::Plugin
{
public:
    std::string getID()
    {
        return "rtlsdr_sdr_support";
    }

    void init()
    {
        satdump::eventBus->register_handler<dsp::RegisterDSPSampleSourcesEvent>(registerSources);
    }

    static void registerSources(const dsp::RegisterDSPSampleSourcesEvent &evt)
    {
        evt.dsp_sources_registry.insert({RtlSdrSource::getID(), {RtlSdrSource::getInstance, RtlSdrSource::getAvailableSources}});
    }
};

PLUGIN_LOADER(RtlSdrSDRSupport)