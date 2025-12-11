#include <QApplication>
#include "telaestoques.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TelaEstoques telaInicial;
    telaInicial.show();
    return app.exec();
}
