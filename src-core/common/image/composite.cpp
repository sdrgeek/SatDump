#include "composite.h"
#include "libs/muparser/muParser.h"
#include "logger.h"
#include "image.h"

namespace image
{
    // Generate a composite from channels and an equation
    template <typename T>
    cimg_library::CImg<T> generate_composite_from_equ(std::vector<cimg_library::CImg<T>> inputChannels, std::vector<int> channelNumbers, std::string equation, nlohmann::json parameters)
    {
        // Equation parsing stuff
        mu::Parser rgbParser;
        double *rgbOut;
        int outValsCnt = 0;

        // Get other parameters such as equalization, etc
        bool equalize = parameters.count("equalize") > 0 ? parameters["equalize"].get<bool>() : false;
        bool normalize = parameters.count("normalize") > 0 ? parameters["normalize"].get<bool>() : false;
        bool white_balance = parameters.count("while_balance") > 0 ? parameters["while_balance"].get<bool>() : false;

        // Compute channel variable names
        std::vector<std::string> channelNames;
        double *channelValues = new double[inputChannels.size()];
        for (int i = 0; i < (int)inputChannels.size(); i++)
        {
            channelValues[i] = 0;
            rgbParser.DefineVar("ch" + std::to_string(channelNumbers[i]), &channelValues[i]);
        }

        // Set expression
        rgbParser.SetExpr(equation);
        rgbParser.Eval(outValsCnt); // Eval once for channel output count

        // Get output width
        int img_width = inputChannels[0].width();
        int img_height = inputChannels[0].height();
        size_t img_fullch = img_width * img_height;

        // Output image
        bool isRgb = outValsCnt == 3;
        cimg_library::CImg<T> rgb_output(img_width, img_height, 1, isRgb ? 3 : 1, 0);

        // Utils
        double R = 0;
        double G = 0;
        double B = 0;

        // Run though the entire image
        for (size_t px = 0; px < img_fullch; px++)
        {
            // Set variables and scale to 1.0
            for (int i = 0; i < (int)inputChannels.size(); i++)
                channelValues[i] = double(inputChannels[i][px]) / double(std::numeric_limits<T>::max());

            // Do the math
            rgbOut = rgbParser.Eval(outValsCnt);

            // Get output and scale back
            R = rgbOut[0] * double(std::numeric_limits<T>::max());
            if (isRgb)
            {
                G = rgbOut[1] * double(std::numeric_limits<T>::max());
                B = rgbOut[2] * double(std::numeric_limits<T>::max());
            }

            // Clamp
            if (R < 0)
                R = 0;

            if (R > std::numeric_limits<T>::max())
                R = std::numeric_limits<T>::max();
            if (isRgb)
            {
                if (G < 0)
                    G = 0;
                if (G > std::numeric_limits<T>::max())
                    G = std::numeric_limits<T>::max();
                if (B < 0)
                    B = 0;
                if (B > std::numeric_limits<T>::max())
                    B = std::numeric_limits<T>::max();
            }

            // Write output
            rgb_output[img_fullch * 0 + px] = R;
            if (isRgb)
            {
                rgb_output[img_fullch * 1 + px] = G;
                rgb_output[img_fullch * 2 + px] = B;
            }
        }

        delete[] channelValues;

        if (equalize)
            rgb_output.equalize(1000);

        if (normalize)
            rgb_output.normalize(0, 65535);

        if (white_balance)
            image::white_balance(rgb_output);

        return rgb_output;
    }

    template cimg_library::CImg<unsigned char> generate_composite_from_equ<unsigned char>(std::vector<cimg_library::CImg<unsigned char>>, std::vector<int>, std::string, nlohmann::json);
    template cimg_library::CImg<unsigned short> generate_composite_from_equ<unsigned short>(std::vector<cimg_library::CImg<unsigned short>>, std::vector<int>, std::string, nlohmann::json);
}