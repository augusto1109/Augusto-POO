#ifndef DIALOGCONFIGURACOES_H
#define DIALOGCONFIGURACOES_H

#include <QDialog>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <QButtonGroup>

class DialogConfiguracoes : public QDialog
{
    Q_OBJECT

private:
    QComboBox *comboMoeda;
    QComboBox *comboIdioma;
    QRadioButton *radioTemaClaro;
    QRadioButton *radioTemaEscuro;
    QButtonGroup *grupoTema;
    QPushButton *btnSalvar;
    QPushButton *btnCancelar;
    
    void criarInterface();
    void carregarConfiguracoes();
    
private slots:
    void onSalvar();

public:
    DialogConfiguracoes(QWidget *parent = nullptr);
    
signals:
    void configuracoesAlteradas();
};

#endif // DIALOGCONFIGURACOES_H
