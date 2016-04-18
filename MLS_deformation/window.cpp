#include "window.hpp"

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>



//---------------------------------------------------------------

window::window(QWindow* _pParent):
    QWindow(_pParent),
    m_update_pending(false),
    m_animating(false),
    m_context(0),
    m_device(0)
{
    // the window will be used for openGL rendering
    setSurfaceType(QWindow::OpenGLSurface);
}

//---------------------------------------------------------------

window::~window()
{
    delete m_device;
}

//---------------------------------------------------------------

void window::render(QPainter* painter)
{
    Q_UNUSED(painter);
}

//---------------------------------------------------------------

void window::initialize()
{

}

//---------------------------------------------------------------

void window::render()
{
    if (!m_device)
    {
        m_device = new QOpenGLPaintDevice;
    }
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    m_device->setSize(size());
}

//---------------------------------------------------------------

void window::renderLater()
{
    if (!m_update_pending)
    {
        m_update_pending=true;
        QCoreApplication::postEvent(this,new QEvent(QEvent::UpdateRequest));
    }
}


//---------------------------------------------------------------

bool window::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::UpdateRequest :
        m_update_pending = false;
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}


//---------------------------------------------------------------

void window::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

//---------------------------------------------------------------

void window::renderNow()
{
    if (!isExposed())
        return;
    bool needsInitialize = false;

    if (!m_context)
    {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize =true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize)
    {
        initializeOpenGLFunctions();
        initialize();
    }

    render();

    m_context->swapBuffers(this);

    if (m_animating)
    {
        renderLater();
    }
}

//---------------------------------------------------------------

void window::setAnimating(bool animating)
{
    m_animating = animating;

    if(m_animating)
        renderLater();
}

//---------------------------------------------------------------
