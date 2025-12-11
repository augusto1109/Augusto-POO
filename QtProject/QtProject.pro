QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    produto.cpp \
    produtodao.cpp \
    estoque.cpp \
    estoquedao.cpp \
    telaestoques.cpp \
    dialogproduto.cpp \
    dialogdetalhes.cpp \
    configuracoes.cpp \
    dialogconfiguracoes.cpp

HEADERS += \
    mainwindow.h \
    produto.h \
    produtodao.h \
    estoque.h \
    estoquedao.h \
    telaestoques.h \
    dialogproduto.h \
    dialogdetalhes.h \
    configuracoes.h \
    dialogconfiguracoes.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
