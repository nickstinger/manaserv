/*
 *  The Mana World
 *  Copyright 2004 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana World is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana World; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  $Id$
 */

#include <cassert>

#include "resourcemanager.h"
#include "game-server/map.hpp"
#include "game-server/mapcomposite.hpp"
#include "game-server/mapmanager.hpp"
#include "game-server/mapreader.hpp"
#include "utils/logger.h"
#include "utils/xml.hpp"

MapManager::MapManager(std::string const &mapReferenceFile)
{
    ResourceManager *resman = ResourceManager::getInstance();
    int size;
    char *data = (char *)resman->loadFile(mapReferenceFile, size);

    if (!data) {
        LOG_ERROR("Map Manager: Could not find " << mapReferenceFile << "!");
        free(data);
        return;
    }

    xmlDocPtr doc = xmlParseMemory(data, size);
    free(data);

    if (!doc)
    {
        LOG_ERROR("Map Manager: Error while parsing map database ("
                  << mapReferenceFile << ")!");
        return;
    }

    xmlNodePtr node = xmlDocGetRootElement(doc);
    if (!node || !xmlStrEqual(node->name, BAD_CAST "maps"))
    {
        LOG_ERROR("Map Manager: " << mapReferenceFile
                  << " is not a valid database file!");
        xmlFreeDoc(doc);
        return;
    }

    LOG_INFO("Loading map reference...");
    for (node = node->xmlChildrenNode; node != NULL; node = node->next)
    {
        if (!xmlStrEqual(node->name, BAD_CAST "map")) {
            continue;
        }

        int id = XML::getProperty(node, "id", 0);
        std::string name = XML::getProperty(node, "name", std::string());
        if (id != 0 && !name.empty())
        {
            maps[id] = new MapComposite(id, name);
        }
    }

    xmlFreeDoc(doc);
}

MapManager::~MapManager()
{
    for (Maps::iterator i = maps.begin(), i_end = maps.end(); i != i_end; ++i)
    {
        delete i->second;
    }
}

MapComposite *MapManager::getMap(int mapId)
{
    Maps::iterator i = maps.find(mapId);
    assert(i != maps.end());
    return i->second;
}

void MapManager::raiseActive(int mapId)
{
    Maps::iterator i = maps.find(mapId);
    assert(i != maps.end());
    MapComposite *composite = i->second;
    if (composite->isActive())
    {
        return;
    }

    std::string const &file = composite->getName();
    Map *map = MapReader::readMap("maps/" + file);
    if (!map)
    {
        LOG_ERROR("Unable to load map \"" << file << "\" (id "
                  << mapId << ")");
        return;
    }

    composite->setMap(map);
    LOG_INFO("Activated map \"" << file << "\" (id " << mapId << ")");
    // will need to load extra map related resources here also
    extern void testingMap(MapComposite *);
    testingMap(composite);
}

