#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QtGui/QWindow>
#include <QOpenGLFunctions_3_3_Core>



QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE


class window : public QWindow, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit window( QWindow* _pParent = 0);
    ~window();
    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_update_pending;
    bool m_animating;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
};

#endif // WINDOW_HPP
