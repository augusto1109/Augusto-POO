#ifndef ESTOQUE_H
#define ESTOQUE_H

#include <QString>
#include <QJsonObject>
#include <QDateTime>

class Estoque
{
private:
    int id;
    QString nome;
    QString descricao;
    QDateTime dataCriacao;

public:
    Estoque();
    Estoque(int id, QString nome, QString descricao = "");

    // Getters
    int getId() const { return id; }
    QString getNome() const { return nome; }
    QString getDescricao() const { return descricao; }
    QDateTime getDataCriacao() const { return dataCriacao; }

    // Setters
    void setId(int id) { this->id = id; }
    void setNome(const QString &nome) { this->nome = nome; }
    void setDescricao(const QString &descricao) { this->descricao = descricao; }
    void setDataCriacao(const QDateTime &data) { this->dataCriacao = data; }

    // Conversão JSON
    QJsonObject toJson() const;
    static Estoque fromJson(const QJsonObject &json);
    
    // Validação
    bool isValid() const;
};

#endif // ESTOQUE_H
