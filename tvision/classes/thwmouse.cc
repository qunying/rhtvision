// Definitions are located at the event.h header

#define Uses_TEvent
#define Uses_TEventQueue
#include <tv.h>

/*****************************************************************************
  Data members initialization
*****************************************************************************/

Boolean THWMouse::handlerInstalled=False;
Boolean THWMouse::noMouse         =False;
uchar   THWMouse::buttonCount     =0;
char    THWMouse::visible         =0;
char    THWMouse::forced          =0;
MouseEventType THWMouse::forcedME;

/*****************************************************************************
  Function pointer members initialization
*****************************************************************************/

void (*THWMouse::Show)()                               =defaultShow;
void (*THWMouse::Hide)()                               =defaultHide;
void (*THWMouse::setRange)(ushort, ushort)             =defaultSetRange;
void (*THWMouse::GetEvent)(MouseEventType&)            =defaultGetEvent;
void (*THWMouse::registerHandler)(unsigned, void (*)())=defaultRegisterHandler;
void (*THWMouse::Suspend)()                            =defaultSuspend;
void (*THWMouse::Resume)()                             =defaultResume;
void (*THWMouse::drawMouse)(int x, int y)              =defaultDrawMouse;

/*****************************************************************************
  Default behaviors for the members
*****************************************************************************/

void THWMouse::defaultShow()
{
 visible=1;
}

void THWMouse::defaultHide()
{
 visible=0;
}

void THWMouse::defaultSuspend() {}
void THWMouse::defaultResume() {}
void THWMouse::defaultSetRange(ushort /*rx*/, ushort /*ry*/) {}
void THWMouse::defaultDrawMouse(int /*x*/, int /*y*/) {}

void THWMouse::defaultRegisterHandler(unsigned, void (*)()) {}

void THWMouse::defaultGetEvent(MouseEventType &me)
{
 me.where.x=TEventQueue::curMouse.where.x;
 me.where.y=TEventQueue::curMouse.where.y;
 me.buttons=TEventQueue::curMouse.buttons;
 me.doubleClick=False;
}

/*****************************************************************************
  Real members
*****************************************************************************/

THWMouse::THWMouse()
{
 resume();
}

THWMouse::~THWMouse()
{
 suspend();
}

void THWMouse::show()
{
 if (!present() || visible) return;
 Show();
}

void THWMouse::hide()
{
 if (!present() || !visible) return;
 Hide();
}

void THWMouse::suspend()
{
 if (!present())
    return;
 if (visible)
    Hide();
 buttonCount=0;
 Suspend();
}

void THWMouse::resume()
{
 if (present())
    return;
 Resume();
 if (!visible)
    Show();
}

void THWMouse::forceEvent(int x, int y, int buttons)
{
 forced=1;
 forcedME.where.x=x;
 forcedME.where.y=y;
 forcedME.doubleClick=False;
 forcedME.buttons=buttons;
}

void THWMouse::getEvent(MouseEventType& me)
{
 if (forced)
   {
    forced=0;
    me=forcedME;
    TEventQueue::curMouse=me;
    drawMouse(forcedME.where.x,forcedME.where.y);
   }
 else if (handlerInstalled)
   {
    me=TEventQueue::curMouse;
   }
 else
   {
    GetEvent(me);
    TEventQueue::curMouse=me;
   }
}
