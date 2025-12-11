#ifndef PRODUTO_H
#define PRODUTO_H

#include <QString>
#include <QJsonObject>

class Produto
{
private:
    int id;
    QString nome;
    double preco;
    int quantidade;
    int estoqueMinimo;
    QString categoria;
    QString descricao;

public:
    // Construtores
    Produto();
    Produto(int id, QString nome, double preco, int quantidade, 
            QString categoria = "", QString descricao = "", int estoqueMinimo = 5);

    // Getters
    int getId() const { return id; }
    QString getNome() const { return nome; }
    double getPreco() const { return preco; }
    int getQuantidade() const { return quantidade; }
    int getEstoqueMinimo() const { return estoqueMinimo; }
    QString getCategoria() const { return categoria; }
    QString getDescricao() const { return descricao; }

    // Setters
    void setId(int id) { this->id = id; }
    void setNome(const QString &nome) { this->nome = nome; }
    void setPreco(double preco) { this->preco = preco; }
    void setQuantidade(int quantidade) { this->quantidade = quantidade; }
    void setEstoqueMinimo(int estoqueMinimo) { this->estoqueMinimo = estoqueMinimo; }
    void setCategoria(const QString &categoria) { this->categoria = categoria; }
    void setDescricao(const QString &descricao) { this->descricao = descricao; }

    // Conversão JSON
    QJsonObject toJson() const;
    static Produto fromJson(const QJsonObject &json);
    
    // Validação
    bool isValid() const;
};

#endif // PRODUTO_H
