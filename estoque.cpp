#include "estoque.h"

Estoque::Estoque()
    : id(0), nome(""), descricao(""), dataCriacao(QDateTime::currentDateTime())
{
}

Estoque::Estoque(int id, QString nome, QString descricao)
    : id(id), nome(nome), descricao(descricao), dataCriacao(QDateTime::currentDateTime())
{
}

QJsonObject Estoque::toJson() const
{
    QJsonObject json;
    json["id"] = id;
    json["nome"] = nome;
    json["descricao"] = descricao;
    json["dataCriacao"] = dataCriacao.toString(Qt::ISODate);
    return json;
}

Estoque Estoque::fromJson(const QJsonObject &json)
{
    Estoque estoque;
    estoque.setId(json["id"].toInt());
    estoque.setNome(json["nome"].toString());
    estoque.setDescricao(json["descricao"].toString());
    
    QString dataStr = json["dataCriacao"].toString();
    if (!dataStr.isEmpty()) {
        estoque.setDataCriacao(QDateTime::fromString(dataStr, Qt::ISODate));
    }
    
    return estoque;
}

bool Estoque::isValid() const
{
    return id > 0 && !nome.isEmpty();
}
