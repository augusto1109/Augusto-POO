#ifndef TELAESTOQUES_H
#define TELAESTOQUES_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include "estoque.h"
#include "estoquedao.h"

class TelaEstoques : public QMainWindow
{
    Q_OBJECT

private:
    EstoqueDAO *estoqueDAO;
    QListWidget *listaEstoques;
    QPushButton *btnNovoEstoque;
    QPushButton *btnExcluirEstoque;
    QLabel *labelInfo;
    
    void criarInterface();
    void carregarEstoques();
    void abrirEstoque(int estoqueId, const QString &nomeEstoque);

private slots:
    void onNovoEstoque();
    void onAbrirEstoque(QListWidgetItem *item);
    void onExcluirEstoque();

public:
    TelaEstoques(QWidget *parent = nullptr);
    ~TelaEstoques();
    
    void atualizarLista();
};

#endif // TELAESTOQUES_H
