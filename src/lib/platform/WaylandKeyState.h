/*
 * barrier -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2003 Chris Schoeneman
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

#include "barrier/KeyState.h"
#include "common/stdmap.h"
#include "common/stdvector.h"

class IEventQueue;

//! Wayland key state
/*!
A key state for Wayland
*/
class WaylandKeyState : public KeyState {
public:
    WaylandKeyState(IEventQueue* events);
    WaylandKeyState(IEventQueue* events, barrier::KeyMap& keyMap);
    ~WaylandKeyState();

    // IKeyState overrides
    virtual bool        fakeCtrlAltDel();
    virtual KeyModifierMask
                        pollActiveModifiers() const;
    virtual SInt32        pollActiveGroup() const;
    virtual void        pollPressedKeys(KeyButtonSet& pressedKeys) const;

protected:
    // KeyState overrides
    virtual void        getKeyMap(barrier::KeyMap& keyMap);
    virtual void        fakeKey(const Keystroke& keystroke);
};
