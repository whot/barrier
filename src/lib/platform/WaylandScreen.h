/*
 * barrier -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2002 Chris Schoeneman
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

#include "barrier/PlatformScreen.h"
#include "barrier/KeyMap.h"
#include "common/stdset.h"
#include "common/stdvector.h"

class WaylandClipboard;
class WaylandKeyState;

//! Implementation of IPlatformScreen for X11
class WaylandScreen : public PlatformScreen {
public:
    WaylandScreen(bool isPrimary, IEventQueue* events);
    virtual ~WaylandScreen();

    //! @name manipulators
    //@{

    //@}

    // IScreen overrides
    virtual void*       getEventTarget() const;
    virtual bool        getClipboard(ClipboardID id, IClipboard*) const;
    virtual void        getShape(SInt32& x, SInt32& y,
                                 SInt32& width, SInt32& height) const;
    virtual void        getCursorPos(SInt32& x, SInt32& y) const;

    // IPrimaryScreen overrides
    virtual void        reconfigure(UInt32 activeSides);
    virtual void        warpCursor(SInt32 x, SInt32 y);
    virtual UInt32      registerHotKey(KeyID key, KeyModifierMask mask);
    virtual void        unregisterHotKey(UInt32 id);
    virtual void        fakeInputBegin();
    virtual void        fakeInputEnd();
    virtual SInt32      getJumpZoneSize() const;
    virtual bool        isAnyMouseButtonDown(UInt32& buttonID) const;
    virtual void        getCursorCenter(SInt32& x, SInt32& y) const;

    // ISecondaryScreen overrides
    virtual void        fakeMouseButton(ButtonID id, bool press);
    virtual void        fakeMouseMove(SInt32 x, SInt32 y);
    virtual void        fakeMouseRelativeMove(SInt32 dx, SInt32 dy) const;
    virtual void        fakeMouseWheel(SInt32 xDelta, SInt32 yDelta) const;

    // IPlatformScreen overrides
    virtual void        enable();
    virtual void        disable();
    virtual void        enter();
    virtual bool        leave();
    virtual bool        setClipboard(ClipboardID, const IClipboard*);
    virtual void        checkClipboards();
    virtual void        openScreensaver(bool notify);
    virtual void        closeScreensaver();
    virtual void        screensaver(bool activate);
    virtual void        resetOptions();
    virtual void        setOptions(const OptionsList& options);
    virtual void        setSequenceNumber(UInt32);
    virtual bool        isPrimary() const;

protected:
    // IPlatformScreen overrides
    virtual void        handleSystemEvent(const Event&, void*);
    virtual void        updateButtons();
    virtual IKeyState*  getKeyState() const;

private:
    // true if screen is being used as a primary screen, false otherwise
    bool                m_isPrimary;
    IEventQueue*        m_events;

    // keyboard stuff
    WaylandKeyState*    m_keyState;

    struct ei           *m_ei;
    struct ei_seat      *m_ei_seat;
    struct ei_device    *m_ei_device;

};
