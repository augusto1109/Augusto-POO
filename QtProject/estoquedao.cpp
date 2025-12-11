#include "estoquedao.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

EstoqueDAO::EstoqueDAO()
{
    diretorioBase = "./BancoDeDados";
    diretorioEstoques = diretorioBase + "/Estoques";
    criarDiretorios();
}

void EstoqueDAO::criarDiretorios()
{
    QDir dir;
    if (!dir.exists(diretorioBase)) {
        dir.mkpath(diretorioBase);
        qDebug() << "Diretório criado:" << diretorioBase;
    }
    if (!dir.exists(diretorioEstoques)) {
        dir.mkpath(diretorioEstoques);
        qDebug() << "Diretório criado:" << diretorioEstoques;
    }
}

QString EstoqueDAO::getCaminhoArquivo(int id) const
{
    return QString("%1/%2.json").arg(diretorioEstoques).arg(id);
}

QString EstoqueDAO::getDiretorioProdutos(int estoqueId) const
{
    return QString("%1/Estoque_%2").arg(diretorioBase).arg(estoqueId);
}

bool EstoqueDAO::salvar(const Estoque &estoque)
{
    if (!estoque.isValid()) {
        qDebug() << "Estoque inválido!";
        return false;
    }

    QString nomeArquivo = getCaminhoArquivo(estoque.getId());
    QFile arquivo(nomeArquivo);
    
    if (!arquivo.open(QIODevice::WriteOnly)) {
        qDebug() << "Erro ao abrir arquivo para escrita:" << nomeArquivo;
        return false;
    }

    QJsonDocument doc(estoque.toJson());
    arquivo.write(doc.toJson(QJsonDocument::Indented));
    arquivo.close();
    
    // Cria o diretório de produtos para este estoque
    QDir dir;
    QString dirProdutos = getDiretorioProdutos(estoque.getId());
    if (!dir.exists(dirProdutos)) {
        dir.mkpath(dirProdutos);
        qDebug() << "Diretório de produtos criado:" << dirProdutos;
    }
    
    qDebug() << "Estoque salvo:" << nomeArquivo;
    return true;
}

Estoque EstoqueDAO::buscarPorId(int id)
{
    QString nomeArquivo = getCaminhoArquivo(id);
    QFile arquivo(nomeArquivo);
    
    if (!arquivo.open(QIODevice::ReadOnly)) {
        qDebug() << "Arquivo não encontrado:" << nomeArquivo;
        return Estoque();
    }

    QByteArray dados = arquivo.readAll();
    arquivo.close();

    QJsonDocument doc = QJsonDocument::fromJson(dados);
    return Estoque::fromJson(doc.object());
}

QList<Estoque> EstoqueDAO::listarTodos()
{
    QList<Estoque> estoques;
    QDir dir(diretorioEstoques);
    
    QStringList filtros;
    filtros << "*.json";
    QFileInfoList arquivos = dir.entryInfoList(filtros, QDir::Files);

    for (const QFileInfo &fileInfo : arquivos) {
        QFile arquivo(fileInfo.absoluteFilePath());
        if (arquivo.open(QIODevice::ReadOnly)) {
            QByteArray dados = arquivo.readAll();
            arquivo.close();

            QJsonDocument doc = QJsonDocument::fromJson(dados);
            Estoque estoque = Estoque::fromJson(doc.object());
            
            if (estoque.isValid()) {
                estoques.append(estoque);
            }
        }
    }
    
    qDebug() << "Total de estoques carregados:" << estoques.size();
    return estoques;
}

bool EstoqueDAO::atualizar(const Estoque &estoque)
{
    return salvar(estoque);
}

bool EstoqueDAO::excluir(int id)
{
    QString nomeArquivo = getCaminhoArquivo(id);
    QFile arquivo(nomeArquivo);
    
    if (!arquivo.exists()) {
        qDebug() << "Arquivo não existe:" << nomeArquivo;
        return false;
    }

    bool removido = arquivo.remove();
    if (removido) {
        qDebug() << "Estoque excluído:" << nomeArquivo;
        
        // Remove também o diretório de produtos (opcional)
        QString dirProdutos = getDiretorioProdutos(id);
        QDir dir(dirProdutos);
        if (dir.exists()) {
            dir.removeRecursively();
            qDebug() << "Diretório de produtos removido:" << dirProdutos;
        }
    } else {
        qDebug() << "Erro ao excluir:" << nomeArquivo;
    }
    
    return removido;
}

int EstoqueDAO::getProximoId()
{
    QDir dir(diretorioEstoques);
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

bool EstoqueDAO::existe(int id)
{
    return QFile::exists(getCaminhoArquivo(id));
}
