/*
 * barrier -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2004 Chris Schoeneman
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "mt/Mutex.h"
#include "base/IEventQueueBuffer.h"

#include <X11/Xlib.h>

#include <libei.h>

#include <queue>

class IEventQueue;

//! Event queue buffer for Wayland
class WaylandEventQueueBuffer : public IEventQueueBuffer {
public:
    WaylandEventQueueBuffer(struct ei* ei, IEventQueue* events);
    virtual ~WaylandEventQueueBuffer();

    // IEventQueueBuffer overrides
    virtual void        init() { }
    virtual void        waitForEvent(double timeout_in_ms);
    virtual Type        getEvent(Event& event, UInt32& dataID);
    virtual bool        addEvent(UInt32 dataID);
    virtual bool        isEmpty() const;
    virtual EventQueueTimer*
                        newTimer(double duration, bool oneShot) const;
    virtual void        deleteTimer(EventQueueTimer*) const;

private:
    struct ei*          m_ei;
    IEventQueue*        m_events;
    std::queue<UInt32>  m_custom_events;
    int                 m_pipe_w, m_pipe_r;
    Mutex               m_mutex;
};
