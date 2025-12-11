#include "produtodao.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

ProdutoDAO::ProdutoDAO()
{
    diretorioBase = "./BancoDeDados";
    diretorioProdutos = diretorioBase + "/Produtos";
    criarDiretorios();
}

void ProdutoDAO::criarDiretorios()
{
    QDir dir;
    if (!dir.exists(diretorioBase)) {
        dir.mkpath(diretorioBase);
        qDebug() << "Diretório criado:" << diretorioBase;
    }
    if (!dir.exists(diretorioProdutos)) {
        dir.mkpath(diretorioProdutos);
        qDebug() << "Diretório criado:" << diretorioProdutos;
    }
}

void ProdutoDAO::setDiretorioProdutos(const QString &diretorio)
{
    diretorioProdutos = diretorio;
    criarDiretorios();
}

QString ProdutoDAO::getCaminhoArquivo(int id) const
{
    return QString("%1/%2.json").arg(diretorioProdutos).arg(id);
}

bool ProdutoDAO::salvar(const Produto &produto)
{
    Produto produtoParaSalvar = produto;
    
    // Se o produto não tem ID válido, gera um novo
    if (produtoParaSalvar.getId() <= 0) {
        int novoId = getProximoId();
        produtoParaSalvar.setId(novoId);
        qDebug() << "Gerando novo ID para produto:" << novoId;
    }
    
    if (!produtoParaSalvar.isValid()) {
        qDebug() << "Produto inválido!";
        return false;
    }

    QString nomeArquivo = getCaminhoArquivo(produtoParaSalvar.getId());
    QFile arquivo(nomeArquivo);
    
    if (!arquivo.open(QIODevice::WriteOnly)) {
        qDebug() << "Erro ao abrir arquivo para escrita:" << nomeArquivo;
        return false;
    }

    QJsonDocument doc(produtoParaSalvar.toJson());
    arquivo.write(doc.toJson(QJsonDocument::Indented));
    arquivo.close();
    
    qDebug() << "Produto salvo:" << nomeArquivo;
    return true;
}

Produto ProdutoDAO::buscarPorId(int id)
{
    QString nomeArquivo = getCaminhoArquivo(id);
    QFile arquivo(nomeArquivo);
    
    if (!arquivo.open(QIODevice::ReadOnly)) {
        qDebug() << "Arquivo não encontrado:" << nomeArquivo;
        return Produto();
    }

    QByteArray dados = arquivo.readAll();
    arquivo.close();

    QJsonDocument doc = QJsonDocument::fromJson(dados);
    return Produto::fromJson(doc.object());
}

QList<Produto> ProdutoDAO::listarTodos()
{
    QList<Produto> produtos;
    QDir dir(diretorioProdutos);
    
    QStringList filtros;
    filtros << "*.json";
    QFileInfoList arquivos = dir.entryInfoList(filtros, QDir::Files);

    for (const QFileInfo &fileInfo : arquivos) {
        QFile arquivo(fileInfo.absoluteFilePath());
        if (arquivo.open(QIODevice::ReadOnly)) {
            QByteArray dados = arquivo.readAll();
            arquivo.close();

            QJsonDocument doc = QJsonDocument::fromJson(dados);
            Produto produto = Produto::fromJson(doc.object());
            
            if (produto.isValid()) {
                produtos.append(produto);
            }
        }
    }
    
    qDebug() << "Total de produtos carregados:" << produtos.size();
    return produtos;
}

bool ProdutoDAO::atualizar(const Produto &produto)
{
    // Atualizar é igual a salvar (sobrescreve o arquivo)
    return salvar(produto);
}

bool ProdutoDAO::excluir(int id)
{
    QString nomeArquivo = getCaminhoArquivo(id);
    QFile arquivo(nomeArquivo);
    
    if (!arquivo.exists()) {
        qDebug() << "Arquivo não existe:" << nomeArquivo;
        return false;
    }

    bool removido = arquivo.remove();
    if (removido) {
        qDebug() << "Produto excluído:" << nomeArquivo;
    } else {
        qDebug() << "Erro ao excluir:" << nomeArquivo;
    }
    
    return removido;
}

int ProdutoDAO::getProximoId()
{
    QDir dir(diretorioProdutos);
    QStringList filtros;
    filtros << "*.json";
    QFileInfoList arquivos = dir.entryInfoList(filtros, QDir::Files);

    int maxId = 0;
    for (const QFileInfo &fileInfo : arquivos) {
        QString baseName = fileInfo.baseName();
        int id = baseName.toInt();
        if (id > maxId) {
            maxId = id;
        }
    }
    
    return maxId + 1;
}

bool ProdutoDAO::existe(int id)
{
    return QFile::exists(getCaminhoArquivo(id));
}

int ProdutoDAO::contarProdutos()
{
    QDir dir(diretorioProdutos);
    QStringList filtros;
    filtros << "*.json";
    return dir.entryInfoList(filtros, QDir::Files).size();
}

