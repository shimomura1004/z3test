#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlProperty>
#include <QQuickWindow>
#include <QQuickItem>
#include <QMetaObject>
#include <QMouseEvent>
#include <QList>
#include <QDebug>

#include <z3++.h>

using namespace z3;

bool isClickable(QQuickItem *item) {
    bool visible = item->property("visible").toBool();
    bool enabled = item->property("enabled").toBool();
    bool hasMethod = item->metaObject()->indexOfMethod("clicked(QQuickMouseEvent*)") >= 0;
    return visible && enabled && hasMethod;
}

void findClickableItems(QQuickItem *item, QList<QRectF> &clickables) {
    if (isClickable(item)) {
        clickables.push_back(item->mapRectToScene(item->boundingRect()));
    }

    Q_FOREACH(QQuickItem *child, item->childItems()) {
        if (child->property("visible").toBool()) {
            findClickableItems(child, clickables);
        }
    }
}

QList<QPoint> getClicakablePoint(const QList<QRectF> &clickables) {
    QList<QPoint> result;

    for (int i=0; i < clickables.length() - 1; i++) {
        QRectF target = clickables[i];

        context c;
        expr x = c.int_const("x");
        expr y = c.int_const("y");
        solver s(c);

        s.add( (int(target.x()) <= x) && (x <= int(target.x() + target.width())) );
        s.add( (int(target.y()) <= y) && (y <= int(target.y() + target.height())) );

        for (int j=i+1; j < clickables.length(); j++) {
            QRectF item = clickables[j];
            s.add( !( (int(item.x()) <= x) && (x <= int(item.x() + item.width())) &&
                      (int(item.y()) <= y) && (y <= int(item.y() + item.height())) ) );
        }

        switch(s.check()) {
        case unsat: {
            qDebug() << "unsat" << target;
            break;
        }
        case sat: {
            qDebug() << "sat" << target;

            model m = s.get_model();

            int xValue, yValue;
            Z3_get_numeral_int(c, m.eval(x), &xValue);
            Z3_get_numeral_int(c, m.eval(y), &yValue);
            result.push_back(QPoint(xValue, yValue));

            break;
        }
        case unknown: {
            qDebug() << "unknown" << target;
            break;
        }
        }
    }

    // add last mouse area here
    QPoint pt(int(clickables.last().x()), int(clickables.last().y()));
    result.push_back(pt);

    return result;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QQuickWindow *window = qobject_cast<QQuickWindow*>(engine.rootObjects()[0]);
    QList<QPoint> points;
    Q_FOREACH(QObject *obj, window->children()) {
        QQuickItem *item = qobject_cast<QQuickItem*>(obj);

        QList<QRectF> clickables;
        findClickableItems(item, clickables);
        qDebug() << clickables;
        points = getClicakablePoint(clickables);
    }

//    Q_FOREACH(QPoint pt, points) {
//        QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress, pt, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
//        QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease, pt, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

//        QApplication::postEvent(item, pressEvent);
//        QApplication::processEvents();
//        QApplication::postEvent(item, releaseEvent);
//        QApplication::processEvents();

//    }

    return app.exec();
}
