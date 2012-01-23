/* Copyright (c) 2011-2012 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/time.h>

#include "Debug.h"
#include "DLogEvent.h"
#include "DLogEventInt.h"
#include "DLogEventLE2.h"

/**
 * Event Loop boiler plate functionality.
 */

namespace DLog {
namespace RPC {

EventSocket::EventSocket(EventLoop& loop)
    : priv(new EventSocketLE2Priv(loop, *this))
{
}

EventSocket::~EventSocket()
{
}

bool
EventSocket::bind(int fd)
{
    return priv->bind(fd);
}

bool
EventSocket::connect(const char* ip, uint16_t port)
{
    return priv->connect(ip, port);
}

int
EventSocket::write(const void* buf, int length)
{
    return priv->write(buf, length);
}

void
EventSocket::setReadWatermark(int length)
{
    priv->setReadWatermark(length);
}

size_t
EventSocket::getLength()
{
    return priv->getLength();
}

int
EventSocket::read(void* buf, int length)
{
    return priv->read(buf, length);
}

int
EventSocket::discard(int length)
{
    return priv->discard(length);
}

void
EventSocket::lock()
{
    priv->lock();
}

void
EventSocket::unlock()
{
    priv->unlock();
}

EventListener::EventListener(EventLoop& loop)
    : priv(new EventListenerLE2Priv(loop, *this))
{
    // TODO(ali): EventLoopImpl should construct the object.
}

EventListener::~EventListener()
{
}

bool
EventListener::bind(uint16_t port)
{
    return priv->bind(port);
}

/**
 * Constructor.
 * \param loop EventLoop instance to bind to
 * \param s Signal to listen for
 */
EventSignal::EventSignal(EventLoop &loop, int s)
    : signal(s),
      priv(new EventSignalLE2Priv(loop, *this))
{
    // TODO(ali): EventLoopImpl should construct the object.
}

EventSignal::~EventSignal()
{
}

/**
 * Add the signal event.
 */
void
EventSignal::add()
{
    priv->add();
}

/**
 * Add the signal event with a specified timeout. If the signal is not
 * fired within the timeout the event will be called anyways.
 */
void
EventSignal::add(time_t timeout)
{
    priv->add(timeout);
}

/**
 * Remove the signal event from the event loop.
 */
void
EventSignal::remove()
{
    priv->remove();
}

/**
 * Is a signal event pending?
 */
bool
EventSignal::isPending()
{
    return priv->isPending();
}

/**
 * Returns the signal that this event is bound to.
 */
int
EventSignal::getSignal()
{
    return signal;
}

/**
 * Constructor.
 * \param loop EventLoop instance to bind to.
 */
EventTimer::EventTimer(EventLoop& loop)
    : period(0),
      persist(false),
      priv(new EventTimerLE2Priv(loop, *this))
{
    // TODO(ali): EventLoopImpl should construct the object.
}

EventTimer::~EventTimer()
{
}

/**
 * Add a periodic timer with the specified interval.
 */
void
EventTimer::addPeriodic(time_t seconds)
{
    period = seconds;
    persist = true;
    priv->add(seconds);
}

/**
 * Add the timer with the specified timeout in seconds.
 */
void
EventTimer::add(time_t seconds)
{
    period = 0;
    persist = false;
    priv->add(seconds);
}

/**
 * Remove the timer event from the event loop.
 */
void
EventTimer::remove()
{
    priv->remove();
}

/**
 * Is there a timer-event pending?
 */
bool
EventTimer::isPending()
{
    return priv->isPending();
}

/**
 * Is the timer persistent or one-shot?
 */
bool
EventTimer::isPersistent()
{
    return persist;
}

/**
 * Returns the period of the EventTimer, which is only valid for a persistent
 * timer.
 */
time_t
EventTimer::getPeriod()
{
    return period;
}

/**
 * EventLoop factory. In the future this function will take in a string 
 * describing the eventloop implementation to use.
 */
EventLoop*
EventLoop::makeEventLoop()
{
    return static_cast<EventLoop*>(new EventLoopLE2Impl());
}

} // namespace
} // namespace

