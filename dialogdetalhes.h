#ifndef DIALOGDETALHES_H
#define DIALOGDETALHES_H

#include <QDialog>
#include <QLabel>
#include "produto.h"

class DialogDetalhes : public QDialog
{
    Q_OBJECT

private:
    void criarInterface(const Produto &produto);

public:
    explicit DialogDetalhes(const Produto &produto, QWidget *parent = nullptr);
    
    static void mostrarDetalhes(const Produto &produto, QWidget *parent = nullptr);
};

#endif // DIALOGDETALHES_H
