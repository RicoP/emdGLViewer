#include "glwidgetplugin.h"
#include "glwidget.h"


#include <QtPlugin>

GLWidgetPlugin::GLWidgetPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void GLWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool GLWidgetPlugin::isInitialized() const
{
    return initialized;
}

QWidget *GLWidgetPlugin::createWidget(QWidget *parent)
{
    return new GLWidget(parent);
}

QString GLWidgetPlugin::name() const
{
    return "GLWidget";
}

QString GLWidgetPlugin::group() const
{
    return "Display Widgets [Examples]";
}

QIcon GLWidgetPlugin::icon() const
{
    return QIcon();
}

QString GLWidgetPlugin::toolTip() const
{
    return "";
}

QString GLWidgetPlugin::whatsThis() const
{
    return "";
}

bool GLWidgetPlugin::isContainer() const
{
    return false;
}

QString GLWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"GLWidget\" name=\"analogClock\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>100</width>\n"
           "    <height>100</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"toolTip\" >\n"
           "   <string>The current time</string>\n"
           "  </property>\n"
           "  <property name=\"whatsThis\" >\n"
           "   <string>The analog clock widget displays the current time.</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

QString GLWidgetPlugin::includeFile() const
{
    return "glwidget.h";
}

Q_EXPORT_PLUGIN2(customwidgetplugin, GLWidgetPlugin)
