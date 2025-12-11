#ifndef DIALOGPRODUTO_H
#define DIALOGPRODUTO_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QTextEdit>
#include <QPushButton>
#include "produto.h"

class DialogProduto : public QDialog
{
    Q_OBJECT

private:
    QLineEdit *lineEditNome;
    QDoubleSpinBox *spinBoxPreco;
    QSpinBox *spinBoxQuantidade;
    QSpinBox *spinBoxEstoqueMinimo;
    QLineEdit *lineEditCategoria;
    QTextEdit *textEditDescricao;
    QPushButton *btnSalvar;
    QPushButton *btnCancelar;
    
    Produto produtoAtual;
    bool modoEdicao;
    
    void criarInterface();
    void validarCampos();

private slots:
    void onSalvar();
    void onCancelar();

public:
    explicit DialogProduto(QWidget *parent = nullptr);
    
    void setProduto(const Produto &produto);
    Produto getProduto() const { return produtoAtual; }
    
    static Produto novoProduto(int proximoId, QWidget *parent = nullptr);
    static bool editarProduto(Produto &produto, QWidget *parent = nullptr);
};

#endif // DIALOGPRODUTO_H
