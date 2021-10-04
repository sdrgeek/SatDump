#include "project.h"
#include "logger.h"
#include "common/projection/leo_to_equirect.h"
#include "common/projection/proj_file.h"
#include "common/map/map_drawer.h"
#include "resources.h"

#include "common/projection/stereo.h"
#include "common/projection/geos.h"
#include "maidenhead.h"

int project(int argc, char *argv[])
{
    logger->critical("This is EXTREMELY WIP! You have been warned!");

    if (argc < 4)
    {
        logger->error("Usage : project image_height image_width [equirectangular/stereo/geos]. Run that command to get per-projection usage.");
        return 1;
    }

    std::string proj = argv[4];

    if (argc < 6)
    {
        if (proj == "equirectangular")
            logger->error("Usage : project image_height image_width equirectangular output.png image1.png reference1.georef image2.png reference2.georef...");
        else if (proj == "stereo")
            logger->error("Usage : project image_height image_width stereo center_locator projection_scale output.png image.png reference.georef image2.png reference2.georef...");
        else if (proj == "geos")
            logger->error("Usage : project image_height image_width geos satellite_longitude output.png image.png reference.georef image2.png reference2.georef...");
        else
            logger->error("Invalid projection!");
        return 1;
    }

    projection::StereoProjection proj_stereo;
    projection::GEOSProjection proj_geos(30000000, -0);

    std::function<std::pair<int, int>(float, float, int, int)> projectionFunc = [](float lat, float lon, int map_height, int map_width) -> std::pair<int, int>
    {
        int imageLat = map_height - ((90.0f + lat) / 180.0f) * map_height;
        int imageLon = (lon / 360.0f) * map_width + (map_width / 2);
        return {imageLon, imageLat};
    };

    int image_height = std::stoi(argv[1]);
    int image_width = std::stoi(argv[2]);
    std::string output_name = argv[3];

    if (proj == "stereo")
    {
        std::string locator = argv[5];
        float scale = std::stof(argv[6]);
        logger->info("Projecting to Stereographic centered around " + locator);
        proj_stereo.init(mh2lat((char *)locator.c_str()), mh2lon((char *)locator.c_str()));
        // StereoProj
        projectionFunc = [&proj_stereo, scale](float lat, float lon, int map_height, int map_width) -> std::pair<int, int>
        {
            double x, y;
            proj_stereo.forward(lon, lat, x, y);
            x *= map_width / scale;
            y *= map_height / scale;
            return {x + (map_width / 2), map_height - (y + (map_height / 2))};
        };

        argc -= 3;
        argv = &argv[3];
    }
    else if (proj == "geos")
    {
        logger->info("Projecting to Geostationnary point of view at " + std::string(argv[5]) + " longitude.");
        proj_geos.init(30000000, std::stof(argv[5]));
        // Geos
        projectionFunc = [&proj_geos](float lat, float lon, int map_height, int map_width) -> std::pair<int, int>
        {
            double x, y;
            proj_geos.forward(lon, lat, x, y);
            x *= map_width / 2;
            y *= map_height / 2;
            return {x + (map_width / 2), map_height - (y + (map_height / 2))};
        };

        argc -= 2;
        argv = &argv[2];
    }

    std::vector<std::pair<std::string, std::string>> toProject;
    int count = (argc - 3) - ((argc - 3) % 2);
    for (int i = 0; i < count; i += 2)
        toProject.push_back({std::string(argv[i + 4 + 0]), std::string(argv[i + 4 + 1])});

    cimg_library::CImg<unsigned char> projected_image = cimg_library::CImg<unsigned char>(image_width, image_height, 1, 3, 0);

    for (const std::pair<std::string, std::string> &image : toProject)
    {
        logger->info("Projecting " + image.first + "...");
        cimg_library::CImg<unsigned short> src_image(image.first.c_str());
        projection::proj_file::LEO_GeodeticReferenceFile geofile = projection::proj_file::readLEOReferenceFile(image.second);
        projection::LEOScanProjectorSettings settings = leoProjectionRefFile(geofile);
        projection::LEOScanProjector projector(settings);
        logger->info("Reprojecting...");
        projected_image = projection::projectLEOToEquirectangularMapped(src_image, projector, image_width, image_height, src_image.spectrum(), projected_image, projectionFunc);
    }

    unsigned char color[3] = {255, 0, 0};

    map::drawProjectedCapitalsGeoJson(projected_image, {resources::getResourcePath("maps/ne_10m_populated_places_simple.json")}, color, projectionFunc);

    logger->info("Saving...");
    projected_image.save_png(output_name.c_str());

    return 0;
}