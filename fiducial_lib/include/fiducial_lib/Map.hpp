// Copyright (c) 2013-2014 by Wayne C. Gramlich.  All rights reserved.

#if !defined(MAP_H_INCLUDED)
#define MAP_H_INCLUDED 1

#include <map>
#include <vector>

#include "File.hpp"
#include "Location.hpp"

/// @brief *Map* is the representation of a fiducial marker map.
typedef struct Map__Struct *Map;

#include "Arc.hpp"
#include "Fiducials.hpp"

class CameraTag;
class Tag;
class TagHeight;

/// @brief A *Map__Struct* represents the fiducial location map.
struct Map__Struct {
    /// @brief Routine to call to announce change to arc.
    Fiducials_Arc_Announce_Routine arc_announce_routine;

    /// @brief All of the *Arc*'s (i.e. measured intertag distances) in the map.
    std::vector<Arc*> all_arcs;

    /// @brief All of the tags (i.e. fiducials) in the map.
    std::vector<Tag*> all_tags;

    /// @brief Opaque object passed into announce routines.
    void *announce_object;

    /// @brief An lookup *Arc* table.
    std::map<std::pair<unsigned int, unsigned int>, Arc*> arcs_;

    /// @brief Number of map changes:
    unsigned int changes_count;

    /// @brief Base name of map file name.
    String_Const file_base;

    /// @brief Directory/folder of map file.
    String_Const file_path;

    /// @brief True if images that change map need to be recorded.
    bool image_log;

    /// @brief True if map has changed since last update.
    bool is_changed;

    /// @brief True if changed map has been saved.
    bool is_saved;

    /// @brief List of pending *Arc*'s for map tree extraction.
    std::vector<Arc*> pending_arcs;

    /// @brief Routine that is called each time a tag is changed.
    Fiducials_Tag_Announce_Routine tag_announce_routine;

    /// @brief List of all known tag heights:
    std::vector<TagHeight*> tag_heights;

    /// @brief Table of all *tags* indexed by *Tag* *id*.
    std::map<unsigned int, Tag*> tags_;

    /// @brief a te
    Arc *temporary_arc;

    /// @brief Increment *visit* each time a map update is propogated.
    unsigned int visit;
};

// *Map* routines:

extern void Map__arc_announce(
  Map map, Arc *arc, CV_Image image, unsigned int sequence_number);
extern void Map__arc_append(Map map, Arc *arc);
extern Arc * Map__arc_lookup(Map map, Tag *from, Tag *to);
extern unsigned int Map__arc_update(Map map, CameraTag * camera_from,
  CameraTag * camera_to, CV_Image image, unsigned int sequence_number);
extern bool Map__equals(Map map1, Map map2);
extern Map Map__create(String_Const map_path, String_Const map_base,
  void *announce_object, Fiducials_Arc_Announce_Routine arc_announce_routine,
  Fiducials_Tag_Announce_Routine tag_announce_routine,
  String_Const tag_heights_file_name, String_Const from);
extern void Map__free(Map map);
extern TagHeight * Map__tag_height_lookup(Map map, unsigned int id);
extern void Map__image_log(Map map, CV_Image image, unsigned int sequence_number);
extern void Map__restore(Map map, File in_file);
extern void Map__save(Map map);
extern void Map__sort(Map map);
extern void Map__svg_write(
  Map map, String_Const svg_base_name, std::vector<Location*> &locations);
extern void Map__tag_heights_xml_read(
  Map map, String_Const tag_heights_file_name);
extern void Map__tag_announce(
  Map map, Tag *tag, bool visible, CV_Image image,
  unsigned int sequence_number);
extern Tag * Map__tag_lookup(Map map, unsigned int tag_id);
extern void Map__update(Map map, CV_Image image, unsigned int sequence_number);
extern void Map__write(Map map, File out_file);

#endif // !defined(MAP_H_INCLUDED)
