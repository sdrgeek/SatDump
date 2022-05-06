#pragma once

#include "block.h"
#include "file_source.h"
#ifdef BUILD_ZIQ
#include "common/ziq.h"
#endif

namespace dsp
{
    class FileSinkBlock : public Block<complex_t, float>
    {
    private:
        std::mutex rec_mutex;

        BasebandType d_sample_format;
        void work();

        bool should_work = false;

        std::ofstream output_file;

        size_t current_size_out = 0;
        size_t current_size_out_raw = 0;

        int8_t *buffer_s8;
        int16_t *buffer_s16;

#ifdef BUILD_ZIQ
        ziq::ziq_cfg ziqcfg;
        std::shared_ptr<ziq::ziq_writer> ziqWriter;
#endif

    public:
        FileSinkBlock(std::shared_ptr<dsp::stream<complex_t>> input);
        ~FileSinkBlock();

        void set_output_sample_type(BasebandType sample_format)
        {
            d_sample_format = sample_format;
        }

        std::string start_recording(std::string path_without_ext, uint64_t samplerate, int depth = 0) // Depth is only for compressed non-raw formats
        {
            rec_mutex.lock();
            std::string finalt;
            if (d_sample_format == CF_32)
                finalt = path_without_ext + ".f32";
            else if (d_sample_format == IS_16)
                finalt = path_without_ext + ".s16";
            else if (d_sample_format == IS_8)
                finalt = path_without_ext + ".s8";
#ifdef BUILD_ZIQ
            else if (d_sample_format == ZIQ)
                finalt = path_without_ext + ".ziq";
#endif

            current_size_out = 0;
            current_size_out_raw = 0;

            output_file = std::ofstream(finalt);

#ifdef BUILD_ZIQ
            ziqcfg.is_compressed = true;
            ziqcfg.bits_per_sample = depth;
            ziqcfg.samplerate = samplerate;
            ziqcfg.annotation = "";

            ziqWriter = std::make_shared<ziq::ziq_writer>(ziqcfg, output_file);
#endif

            should_work = true;
            rec_mutex.unlock();

            return finalt;
        }

        size_t get_written()
        {
            return current_size_out;
        }

        size_t get_written_raw()
        {
            return current_size_out_raw;
        }

        void stop_recording()
        {
            rec_mutex.lock();
            should_work = false;
            current_size_out = 0;
            current_size_out_raw = 0;
            output_file.close();
            rec_mutex.unlock();
        }
    };
}