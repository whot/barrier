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

#include "platform/WaylandEventQueueBuffer.h"

#include "base/Event.h"
#include "base/IEventQueue.h"
#include "mt/Thread.h"

#include <fcntl.h>
#include <poll.h>
#if HAVE_UNISTD_H
#    include <unistd.h>
#endif

#include <cstdio>

class EventQueueTimer { };

WaylandEventQueueBuffer::WaylandEventQueueBuffer(struct ei *ei, IEventQueue* events) :
    m_events(events),
    m_ei(ei_ref(ei))
{

    // We need a pipe to signal ourselves when addEvent() is called
    int pipefd[2];
    int result = pipe(pipefd);
    assert(result == 0);

    int pipeflags;
    pipeflags = fcntl(pipefd[0], F_GETFL);
    fcntl(pipefd[0], F_SETFL, pipeflags | O_NONBLOCK);
    pipeflags = fcntl(pipefd[1], F_GETFL);
    fcntl(pipefd[1], F_SETFL, pipeflags | O_NONBLOCK);

    m_pipe_r = pipefd[0];
    m_pipe_w = pipefd[1];
}

WaylandEventQueueBuffer::~WaylandEventQueueBuffer()
{
    ei_unref(m_ei);
    close(m_pipe_r);
    close(m_pipe_w);
}

void
WaylandEventQueueBuffer::waitForEvent(double timeout_in_ms)
{
    Thread::testCancel();

    enum {
        EIFD,
        PIPEFD,
        POLLFD_COUNT, // Last element
    };

    struct pollfd pfds[POLLFD_COUNT];
    pfds[EIFD].fd       = ei_get_fd(m_ei);
    pfds[EIFD].events   = POLLIN;
    pfds[PIPEFD].fd     = m_pipe_r;
    pfds[PIPEFD].events = POLLIN;

    int timeout = (timeout_in_ms < 0.0) ? -1 :
                    static_cast<int>(1000.0 * timeout_in_ms);

    int retval = poll(pfds, POLLFD_COUNT, timeout);
    if (retval > 0) {
        if (pfds[EIFD].revents & POLLIN) {
            ei_dispatch(m_ei);
        }
        if (pfds[PIPEFD].revents & POLLIN) {
            char buf[64];
            read(m_pipe_r, buf, sizeof(buf)); // discard
        }
    }
    Thread::testCancel();
}

IEventQueueBuffer::Type
WaylandEventQueueBuffer::getEvent(Event& event, UInt32& dataID)
{
    // FIXME: doc says we should append addEvent to the end of the queue but
    // processing it here means they override any other events. This may
    // cause a bug, fixable by popping everything into a queue.
    if (!m_custom_events.empty()) {
        dataID = m_custom_events.front();
        m_custom_events.pop();
        return kUser;
    }

    struct ei_event *ev = ei_get_event(m_ei);
    event = Event(Event::kSystem,
                  m_events->getSystemTarget(),
                  ev);
    return kSystem;
}

bool
WaylandEventQueueBuffer::addEvent(UInt32 dataID)
{
    m_custom_events.push(dataID);

    /* tickle the pipe so our read thread wakes up */
    write(m_pipe_w, "!", 1);

    return true;
}

bool
WaylandEventQueueBuffer::isEmpty() const
{
    if (!m_custom_events.empty())
        return false;

    struct ei_event *event;
    event = ei_peek_event(m_ei);
    bool retval = event == NULL;
    ei_event_unref(event);
    return retval;
}

EventQueueTimer*
WaylandEventQueueBuffer::newTimer(double, bool) const
{
    return new EventQueueTimer;
}

void
WaylandEventQueueBuffer::deleteTimer(EventQueueTimer* timer) const
{
    delete timer;
}
