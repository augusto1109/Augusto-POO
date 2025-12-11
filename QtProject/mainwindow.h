#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QToolBar>
#include "produto.h"
#include "produtodao.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // DAO
    ProdutoDAO *dao;
    
    // Dados do estoque
    int estoqueId;
    QString nomeEstoque;
    
    // Widgets da interface
    QTableWidget *tabelaProdutos;
    QToolBar *toolbar;
    
    // Ações
    QPushButton *btnVoltar;
    QPushButton *btnAdicionarProduto;
    QPushButton *btnEditarProduto;
    QPushButton *btnExcluirProduto;
    QPushButton *btnDetalhes;
    QPushButton *btnAtualizar;
    
    // Métodos privados
    void criarInterface();
    void configurarTabela();
    void criarToolbar();
    void criarMenus();
    void carregarProdutos();
    void atualizarBotoes();
    void criarAtalhos();
    void exportarCSV();
    void importarCSV();
    void fazerBackup();
    void imprimirInventario();
    void abrirConfiguracoes();
    void aplicarTema();
    void atualizarTextos();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onVoltar();
    void onAdicionarProduto();
    void onEditarProduto();
    void onExcluirProduto();
    void onVerDetalhes();
    void onAtualizarLista();
    void onProdutoSelecionado();

public:
    MainWindow(int estoqueId, const QString &nomeEstoque, QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
