/*
 *  The Mana World Server
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

#ifndef _TMWSERV_CHATCHANNELHANDLER_H_
#define _TMWSERV_CHATCHANNELHANDLER_H_

#include <map>

#include "chatchannel.h"

class ChatChannelManager {

public:

    /**
     * Constructor
     */
    ChatChannelManager();

    /**
     * Destructor
     */
    ~ChatChannelManager();

    /**
     * Add a public channel.
     *
     * @return the number of the channel registered.
     * 0 if the registering was unsuccessful.
     */
    short registerPublicChannel(const std::string& channelName);

    /**
     * Add a private channel.
     *
     * @return the number of the channel registered.
     * 0 if the registering was unsuccessful.
     */
    short registerPrivateChannel(const std::string& channelName);

    /**
     * Remove a channel.
     */
    bool removeChannel(const short channelId);

    /**
     * get the id of a channel from its name.
     *
     * @return the id of the channel
     * 0 if it was unsuccessful.
     */
    short getChannelId(const std::string& channelName);

    /**
     * get the name of a channel from its id.
     *
     * @return the name of the channel
     */
    const std::string getChannelName(const short channelId);

    /**
     * get the announcement string of a channel from its id.
     *
     * @return the announcement string of the channel
     */
    const std::string
    ChatChannelManager::getChannelAnnouncement(const short channelId);

    /**
     * get the password of a channel from its id.
     *
     * @return the password of the channel
     */
    const std::string
    ChatChannelManager::getChannelPassword(const short channelId);

    /**
     * get the ChatChannel object from its id.
     *
     * @return the ChatChannel object
     */
    const ChatChannel
    ChatChannelManager::getChannel(const short channelId);

    /**
     * Add a user in a channel
     */
    bool addUserInChannel(tmwserv::BeingPtr beingPtr, const short channelId);

    /**
     * Remove a user from a channel.
     */
    bool removeUserFromChannel(tmwserv::BeingPtr beingPtr, const short channelId);

    /**
     * Remove a user from every channels.
     * Used at logout.
     */
    void removeUserFromEveryChannels(tmwserv::BeingPtr beingPtr);

private:

    /**
    * The list keeping all the chat channels.
    *
    * The channel id must be unique.
    */
    std::map<short, ChatChannel> mChatChannels;

};

#endif