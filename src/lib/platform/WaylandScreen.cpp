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

#include "platform/WaylandScreen.h"

#include "platform/WaylandEventQueueBuffer.h"
#include "platform/WaylandKeyState.h"
#include "barrier/Clipboard.h"
#include "barrier/KeyMap.h"
#include "barrier/XScreen.h"
#include "arch/XArch.h"
#include "arch/Arch.h"
#include "base/Log.h"
#include "base/Stopwatch.h"
#include "base/IEventQueue.h"
#include "base/TMethodEventJob.h"

#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <unistd.h>


#include <libei.h>

//
// WaylandScreen
//
//
WaylandScreen::WaylandScreen(
		bool isPrimary,
		IEventQueue* events) :
	m_isPrimary(isPrimary),
	m_events(events),
	m_keyState(NULL),
    m_ei(NULL),
    m_ei_seat(NULL),
    m_ei_device(NULL),
    PlatformScreen(events)
{
	// Server isn't supported yet
	assert(!isPrimary);

	m_ei = ei_new(NULL);
	ei_log_set_priority(m_ei, EI_LOG_PRIORITY_DEBUG);
	ei_configure_name(m_ei, "barrier");
	auto rc = ei_setup_backend_portal(m_ei);
	if (rc != 0) {
        LOG((CLOG_NOTE "Failed to use libei portal, falling back to socket"));
		rc = ei_setup_backend_socket(m_ei, NULL);
    }

	if (rc != 0) {
		LOG((CLOG_DEBUG "ei init error: %s", strerror(-rc)));
	    throw XArch("Failed to init ei context");
	}

	m_keyState = new WaylandKeyState(events);
	// install event handlers
	m_events->adoptHandler(Event::kSystem, m_events->getSystemTarget(),
							new TMethodEventJob<WaylandScreen>(this,
								&WaylandScreen::handleSystemEvent));

	// install the platform event queue
    m_events->adoptBuffer(new WaylandEventQueueBuffer(m_ei, m_events));

}

WaylandScreen::~WaylandScreen()
{
	m_events->adoptBuffer(NULL);
	m_events->removeHandler(Event::kSystem, m_events->getSystemTarget());

	ei_device_unref(m_ei_device);
	ei_seat_unref(m_ei_seat);
	ei_unref(m_ei);
}

void*
WaylandScreen::getEventTarget() const
{
	printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
	return const_cast<WaylandScreen*>(this);
}

bool
WaylandScreen::getClipboard(ClipboardID id, IClipboard* clipboard) const
{
	printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
	/* FIXME: */
	return false;
}

void
WaylandScreen::getShape(SInt32& x, SInt32& y, SInt32& w, SInt32& h) const
{
	printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
	/* FIXME */
	x = 0;
	y = 0;
	w = 1920;
	h = 1080;
}

void
WaylandScreen::getCursorPos(SInt32& x, SInt32& y) const
{
	printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
	/* FIXME: */

	x = 960;
	y = 540;
}

void
WaylandScreen::reconfigure(UInt32)
{
	// do nothing
}

void
WaylandScreen::warpCursor(SInt32 x, SInt32 y)
{
	/* FIXME */
}

UInt32
WaylandScreen::registerHotKey(KeyID key, KeyModifierMask mask)
{
	/* FIXME */
	return 0;
}

void
WaylandScreen::unregisterHotKey(UInt32 id)
{
	/* FIXME */
}

void
WaylandScreen::fakeInputBegin()
{
	printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
	// FIXME -- not implemented
}

void
WaylandScreen::fakeInputEnd()
{
	printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
	// FIXME -- not implemented
}

SInt32
WaylandScreen::getJumpZoneSize() const
{
	return 1;
}

bool
WaylandScreen::isAnyMouseButtonDown(UInt32& buttonID) const
{
	/* FIXME */
	return false;
}

void
WaylandScreen::getCursorCenter(SInt32& x, SInt32& y) const
{
	/* FIXME */
	x = 0;
	y = 0;
}

void
WaylandScreen::fakeMouseButton(ButtonID button, bool press)
{
	printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
	ei_device_pointer_button(m_ei_device, button, press);
}

void
WaylandScreen::fakeMouseMove(SInt32 x, SInt32 y)
{
	printf("::::::::: %s:%d:%s() - %d/%d\n", __FILE__, __LINE__, __func__, x, y);
	ei_device_pointer_motion_absolute(m_ei_device, x, y);
}

void
WaylandScreen::fakeMouseRelativeMove(SInt32 dx, SInt32 dy) const
{
	printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
	ei_device_pointer_motion(m_ei_device, dx, dy);
}

void
WaylandScreen::fakeMouseWheel(SInt32 xDelta, SInt32 yDelta) const
{
	printf("::::::::: %s:%d:%s() - \n", __FILE__, __LINE__, __func__);
	/* FIXME */
}

void
WaylandScreen::enable()
{
	/* FIXME */
}

void
WaylandScreen::disable()
{
	/* FIXME */
}

void
WaylandScreen::enter()
{
	/* FIXME */
}

bool
WaylandScreen::leave()
{
	/* FIXME */
	return true;
}

bool
WaylandScreen::setClipboard(ClipboardID id, const IClipboard* clipboard)
{
	/* FIXME */
	return false;
}

void
WaylandScreen::checkClipboards()
{
	// do nothing, we're always up to date
}

void
WaylandScreen::openScreensaver(bool notify)
{
	/* FIXME */
}

void
WaylandScreen::closeScreensaver()
{
	/* FIXME */
}

void
WaylandScreen::screensaver(bool activate)
{
	/* FIXME */
}

void
WaylandScreen::resetOptions()
{
	/* FIXME */
}

void
WaylandScreen::setOptions(const OptionsList& options)
{
	/* FIXME */
}

void
WaylandScreen::setSequenceNumber(UInt32 seqNum)
{
	/* FIXME */
}

bool
WaylandScreen::isPrimary() const
{
	return m_isPrimary;
}

void
WaylandScreen::handleSystemEvent(const Event& sysevent, void* data)
{
    struct ei_event *event = static_cast<struct ei_event*>(sysevent.getData());
    if (!event)
        return;

    switch (ei_event_get_type(event)) {
        case EI_EVENT_SEAT_ADDED:
            if (!m_ei_seat) {
                m_ei_seat = ei_seat_ref(ei_event_get_seat(event));
                LOG((CLOG_DEBUG "using seat %s", ei_seat_get_name(m_ei_seat)));

                auto device = ei_device_new(m_ei_seat);
                ei_device_configure_capability(device, EI_DEVICE_CAP_POINTER);
                ei_device_configure_capability(device, EI_DEVICE_CAP_POINTER_ABSOLUTE);
                ei_device_configure_capability(device, EI_DEVICE_CAP_KEYBOARD);
                SInt32 x, y, w, h;

                getShape(x, y, w, h);
                ei_device_pointer_configure_range(device, w, h);
                ei_device_add(device);
                ei_device_unref(device);
            }
            break;
        case EI_EVENT_DEVICE_REMOVED:
        case EI_EVENT_SEAT_REMOVED:
        case EI_EVENT_DISCONNECT:
        case EI_EVENT_DEVICE_SUSPENDED:
            throw XArch("Oops, EIS didn't like us");
        case EI_EVENT_DEVICE_ADDED:
            break;
        case EI_EVENT_DEVICE_RESUMED:
            LOG((CLOG_DEBUG "device %s is available", ei_device_get_name(ei_event_get_device(event))));
            m_ei_device = ei_device_ref(ei_event_get_device(event));
            break;
    }
    ei_event_unref(event);
}

void
WaylandScreen::updateButtons()
{
	/* FIXME */
}

IKeyState*
WaylandScreen::getKeyState() const
{
	return m_keyState;
}
