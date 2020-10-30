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

#include "platform/WaylandKeyState.h"

#include "base/Log.h"
#include "common/stdmap.h"

#include <cstddef>
#include <algorithm>

static const size_t ModifiersFromXDefaultSize = 32;

WaylandKeyState::WaylandKeyState(IEventQueue* events) :
    KeyState(events)
{
    /* FIXME */
}

WaylandKeyState::WaylandKeyState(
    IEventQueue* events, barrier::KeyMap& keyMap) :
    KeyState(events, keyMap)
{
    /* FIXME */
}

WaylandKeyState::~WaylandKeyState()
{
}

bool
WaylandKeyState::fakeCtrlAltDel()
{
    // pass keys through unchanged
    return false;
}

KeyModifierMask
WaylandKeyState::pollActiveModifiers() const
{
    /* FIXME */
    return 0;
}

SInt32
WaylandKeyState::pollActiveGroup() const
{
    printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
    /* FIXME */
    return 0;
}

void
WaylandKeyState::pollPressedKeys(KeyButtonSet& pressedKeys) const
{
    printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
    /* FIXME */
    return;
}

void
WaylandKeyState::getKeyMap(barrier::KeyMap& keyMap)
{
    printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
    /* FIXME */
    return;
}

void
WaylandKeyState::fakeKey(const Keystroke& keystroke)
{
    printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
    /* FIXME */
}
