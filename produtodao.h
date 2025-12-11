#ifndef PRODUTODAO_H
#define PRODUTODAO_H

#include <QString>
#include <QList>
#include <QDir>
#include "produto.h"

class ProdutoDAO
{
private:
    QString diretorioBase;
    QString diretorioProdutos;
    
    void criarDiretorios();
    QString getCaminhoArquivo(int id) const;

public:
    ProdutoDAO();
    
    // Configuração
    void setDiretorioProdutos(const QString &diretorio);
    
    // CRUD Operations
    bool salvar(const Produto &produto);
    Produto buscarPorId(int id);
    QList<Produto> listarTodos();
    bool atualizar(const Produto &produto);
    bool excluir(int id);
    
    // Utilidades
    int getProximoId();
    bool existe(int id);
    int contarProdutos();
};

#endif // PRODUTODAO_H
