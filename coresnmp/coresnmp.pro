#Configuracion para Libreria
TEMPLATE = lib
QT += core network
CONFIG += dll
TARGETDIR = lib

#Comprueba si se compila como aplicacion
TEST=YES
!isEmpty($$TEST)
{
	#Configuracion para Aplicacion de Test
	TEMPLATE = app
	CONFIG += console
	CONFIG -= app_bundle
	CONFIG -= dll
	TARGETDIR = bin
	include(test/test.pri)
}
 
#Define la Plataforma de compilacion
win32 { platform = win32 }
unix { platform = unix }

#CODB COre Data Base
TARGET = coresnmp
INCLUDEPATH += cfg include src

#Indica la compilacion con Librerias especificas
#debug:DEFINES +=

release:DESTDIR = $$TARGETDIR
release:OBJECTS_DIR = build/release/$${platform}-obj
release:MOC_DIR = build/release/$${platform}-moc
release:RCC_DIR = build/release/$${platform}-rcc
release:UI_DIR = build/release/$${platform}-ui

debug:DESTDIR = $$TARGETDIR
debug:OBJECTS_DIR = build/debug/$${platform}-obj
debug:MOC_DIR = build/debug/$${platform}-moc
debug:RCC_DIR = build/debug/$${platform}-rcc
debug:UI_DIR = build/debug/$${platform}-ui

#Seccion de Includes
include(src/src.pri)

#Seccion de Includes de la aplicacion
#HEADERS += \
#    include/coresnmp.h

#LIBS += -L/usr/local/lib -L/usr/lib -luuid -lnetsnmp `net-snmp-config --agent-libs`

