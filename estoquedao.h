#ifndef ESTOQUEDAO_H
#define ESTOQUEDAO_H

#include <QString>
#include <QList>
#include "estoque.h"

class EstoqueDAO
{
private:
    QString diretorioBase;
    QString diretorioEstoques;
    
    void criarDiretorios();
    QString getCaminhoArquivo(int id) const;

public:
    EstoqueDAO();
    
    // CRUD Operations
    bool salvar(const Estoque &estoque);
    Estoque buscarPorId(int id);
    QList<Estoque> listarTodos();
    bool atualizar(const Estoque &estoque);
    bool excluir(int id);
    
    // Utilidades
    int getProximoId();
    bool existe(int id);
    QString getDiretorioProdutos(int estoqueId) const;
};

#endif // ESTOQUEDAO_H
