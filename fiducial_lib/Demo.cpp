// Copyright (c) 2013 by Wayne C. Gramlich.  All rights reserved.

#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

#include "CV.hpp"
#include "File.hpp"
#include "Fiducials.hpp"
#include "Map.hpp"
#include "String.hpp"

/// @brief Run the demo code.
/// @param arguments_size is the number of arguments
/// @param arguments is the vector of command line arguments.
/// @returns 0 for success.
///
/// *main*() will run the demonstration code.

int main(int arguments_size, char *arguments[]) {
    struct timeval start_time_value_struct;    
    struct timeval end_time_value_struct;    
    struct timeval difference_time_value_struct;    
    Time_Value start_time_value = &start_time_value_struct;
    Time_Value end_time_value = &end_time_value_struct;
    Time_Value difference_time_value = &difference_time_value_struct;

    assert (gettimeofday(start_time_value, (struct timezone *)0) == 0);

    bool image_log = (bool)0;
    std::vector<String> image_file_names;
    String lens_calibrate_file_name = (String)0;
    String log_file_name = (String)0;
    //File__format(stdout, "Hello\n");
    if (arguments_size <= 1) {
        File__format(stderr, "Usage: Demo lens.txt *.pnm\n");
    } else {
        for (unsigned int index = 1; index < arguments_size; index++) {
            String argument = arguments[index];
            unsigned int size = String__size(argument);
            if (String__equal(argument, "--image_log")) {
                image_log = (bool)1;
            } else if (size > 4 && String__equal(argument + size - 4, ".txt")) {
                lens_calibrate_file_name = argument;
            } else if (size > 4 && String__equal(argument + size - 4, ".log")) {
                log_file_name = argument;
            } else if (size > 4 && String__equal(argument + size - 4, ".pnm")) {
                image_file_names.push_back(argument);
            } else if (size > 4 && String__equal(argument + size - 4, ".chk")) {
                // Do nothing:
            } else {
                File__format(stderr, "Unrecoginized file '%s'\n", argument);
            }
        }
    }

    unsigned int size = image_file_names.size();
    if (size > 0) {
        String image_file_name0 = image_file_names[0];
        CV_Image image = (CV_Image)0;
        image = CV_Image__pnm_read(image_file_name0);
        assert (image != (CV_Image)0);

        // Load up *fiducials_create*:
        Fiducials_Create fiducials_create = Fiducials_Create__one_and_only();
        fiducials_create->fiducials_path = ".";
        fiducials_create->lens_calibrate_file_name = lens_calibrate_file_name;
        fiducials_create->announce_object = (Memory)0;
        fiducials_create->arc_announce_routine = Fiducials__arc_announce;
        fiducials_create->location_announce_routine =
          Fiducials__location_announce;
        fiducials_create->tag_announce_routine = Fiducials__tag_announce;
        fiducials_create->log_file_name = log_file_name;
        fiducials_create->map_base_name = "Demo_Map";
        fiducials_create->tag_heights_file_name = "Tag_Heights.xml";
        
        Fiducials fiducials = Fiducials__create(image, fiducials_create);
        fiducials->map->image_log = image_log;

        for (unsigned int index = 0; index < size; index++) {
            String image_file_name = image_file_names[index];
            image = CV_Image__pnm_read(image_file_name);
            Fiducials__image_set(fiducials, image);
            Fiducials__process(fiducials);
        }

        assert (gettimeofday(end_time_value, (struct timezone *)0) == 0);

        double start_time = (double)start_time_value->tv_usec / 1000000.0;
        double end_time =
          (double)(end_time_value->tv_sec - start_time_value->tv_sec) +
          (double)end_time_value->tv_usec / 1000000.0;
        double time = end_time - start_time;
        double frames_per_second = (double)size / time;

        File__format(stderr, "%d frames / %f sec = %f Frame/sec\n",
          size, time, frames_per_second);

        if (size == 1) {
            Fiducials__image_show(fiducials, (bool)1);
        } else {
            Map map = fiducials->map;
            Map__save(map);
            File__format(stderr,
              "Outputing %d locations\n", fiducials->locations_path.size());
            Map__svg_write(map, "Demo", fiducials->locations_path);
        }

        // Release all the storage associated with *fiducials*:

        // Some debugging code to make sure that we are reading/writing
        // map.xml files correctly:
        //fiducials =
        //  Fiducials__create(image, lens_calibrate_file_name, (void *)0,
        //  Fiducials__location_announce, Map__tag_announce, log_file_name,
        //  "Demo.xml");
        //fiducials->map->is_saved = (bool)0;
        //fiducials->map->file_name = "Demo2.xml";
        //Fiducials__free(fiducials);
    }

    return 0;
}

