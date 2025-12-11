#include "produto.h"

Produto::Produto()
    : id(0), nome(""), preco(0.0), quantidade(0), estoqueMinimo(5), categoria(""), descricao("")
{
}

Produto::Produto(int id, QString nome, double preco, int quantidade, 
                 QString categoria, QString descricao, int estoqueMinimo)
    : id(id), nome(nome), preco(preco), quantidade(quantidade),
      estoqueMinimo(estoqueMinimo), categoria(categoria), descricao(descricao)
{
}

QJsonObject Produto::toJson() const
{
    QJsonObject json;
    json["id"] = id;
    json["nome"] = nome;
    json["preco"] = preco;
    json["quantidade"] = quantidade;
    json["estoqueMinimo"] = estoqueMinimo;
    json["categoria"] = categoria;
    json["descricao"] = descricao;
    return json;
}

Produto Produto::fromJson(const QJsonObject &json)
{
    Produto produto;
    produto.setId(json["id"].toInt());
    produto.setNome(json["nome"].toString());
    produto.setPreco(json["preco"].toDouble());
    produto.setQuantidade(json["quantidade"].toInt());
    produto.setEstoqueMinimo(json["estoqueMinimo"].toInt(5)); // Default 5 se não existir
    produto.setCategoria(json["categoria"].toString());
    produto.setDescricao(json["descricao"].toString());
    return produto;
}

bool Produto::isValid() const
{
    return id > 0 && !nome.isEmpty() && preco >= 0 && quantidade >= 0;
}
