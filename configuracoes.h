#ifndef CONFIGURACOES_H
#define CONFIGURACOES_H

#include <QString>
#include <QSettings>

class Configuracoes
{
private:
    QSettings *settings;
    
    QString tema;
    QString moeda;
    QString idioma;
    
    static Configuracoes* instancia;
    
    Configuracoes();
    
public:
    ~Configuracoes();
    
    static Configuracoes* getInstance();
    
    // Getters
    QString getTema() const { return tema; }
    QString getMoeda() const { return moeda; }
    QString getIdioma() const { return idioma; }
    QString getSimboloMoeda() const;
    
    // Setters
    void setTema(const QString &tema);
    void setMoeda(const QString &moeda);
    void setIdioma(const QString &idioma);
    
    // Métodos
    void carregar();
    void salvar();
    QString traduzir(const QString &chave) const;
};

#endif // CONFIGURACOES_H
