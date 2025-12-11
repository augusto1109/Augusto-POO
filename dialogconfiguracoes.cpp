#include "dialogconfiguracoes.h"
#include "configuracoes.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>

DialogConfiguracoes::DialogConfiguracoes(QWidget *parent)
    : QDialog(parent)
{
    Configuracoes *config = Configuracoes::getInstance();
    setWindowTitle(config->traduzir("configuracoes"));
    setModal(true);
    resize(450, 400);
    
    criarInterface();
    carregarConfiguracoes();
}

void DialogConfiguracoes::criarInterface()
{
    Configuracoes *config = Configuracoes::getInstance();
    
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);
    layoutPrincipal->setSpacing(20);
    layoutPrincipal->setContentsMargins(20, 20, 20, 20);
    
    // Título
    QLabel *titulo = new QLabel(QString("⚙️ %1").arg(config->traduzir("config_sistema")));
    QFont fontTitulo;
    fontTitulo.setPointSize(14);
    fontTitulo.setBold(true);
    titulo->setFont(fontTitulo);
    titulo->setStyleSheet("color: #2c3e50; margin-bottom: 10px;");
    layoutPrincipal->addWidget(titulo);
    
    // ===== GRUPO TEMA =====
    QGroupBox *grupoTemaBox = new QGroupBox(QString("🎨 %1").arg(config->traduzir("tema_interface")));
    grupoTemaBox->setStyleSheet(
        "QGroupBox { "
        "   font-weight: bold; "
        "   border: 2px solid #bdc3c7; "
        "   border-radius: 8px; "
        "   margin-top: 10px; "
        "   padding-top: 15px; "
        "} "
        "QGroupBox::title { "
        "   subcontrol-origin: margin; "
        "   left: 10px; "
        "   padding: 0 5px; "
        "}"
    );
    
    QVBoxLayout *layoutTema = new QVBoxLayout(grupoTemaBox);
    
    grupoTema = new QButtonGroup(this);
    
    radioTemaClaro = new QRadioButton(QString("☀️ %1").arg(config->traduzir("tema_claro")));
    radioTemaClaro->setStyleSheet("font-size: 11pt; padding: 5px;");
    grupoTema->addButton(radioTemaClaro, 0);
    
    radioTemaEscuro = new QRadioButton(QString("🌙 %1").arg(config->traduzir("tema_escuro")));
    radioTemaEscuro->setStyleSheet("font-size: 11pt; padding: 5px;");
    grupoTema->addButton(radioTemaEscuro, 1);
    
    layoutTema->addWidget(radioTemaClaro);
    layoutTema->addWidget(radioTemaEscuro);
    
    layoutPrincipal->addWidget(grupoTemaBox);
    
    // ===== GRUPO FORMATAÇÃO =====
    QGroupBox *grupoFormatacao = new QGroupBox(QString("💰 %1").arg(config->traduzir("formatacao_localizacao")));
    grupoFormatacao->setStyleSheet(
        "QGroupBox { "
        "   font-weight: bold; "
        "   border: 2px solid #bdc3c7; "
        "   border-radius: 8px; "
        "   margin-top: 10px; "
        "   padding-top: 15px; "
        "} "
        "QGroupBox::title { "
        "   subcontrol-origin: margin; "
        "   left: 10px; "
        "   padding: 0 5px; "
        "}"
    );
    
    QFormLayout *layoutFormatacao = new QFormLayout(grupoFormatacao);
    layoutFormatacao->setSpacing(15);
    
    // Moeda
    QLabel *labelMoeda = new QLabel(QString("%1:").arg(config->traduzir("moeda")));
    labelMoeda->setStyleSheet("font-size: 11pt; font-weight: bold; color: #34495e;");
    
    comboMoeda = new QComboBox();
    comboMoeda->addItem("🇧🇷 Real Brasileiro (R$)", "BRL");
    comboMoeda->addItem("🇺🇸 Dólar Americano ($)", "USD");
    comboMoeda->addItem("🇪🇺 Euro (€)", "EUR");
    comboMoeda->addItem("🇬🇧 Libra Esterlina (£)", "GBP");
    comboMoeda->setStyleSheet(
        "QComboBox { "
        "   padding: 8px; "
        "   border: 2px solid #bdc3c7; "
        "   border-radius: 5px; "
        "   font-size: 11pt; "
        "} "
        "QComboBox:hover { "
        "   border: 2px solid #3498db; "
        "}"
    );
    
    layoutFormatacao->addRow(labelMoeda, comboMoeda);
    
    // Idioma
    QLabel *labelIdioma = new QLabel(QString("%1:").arg(config->traduzir("idioma")));
    labelIdioma->setStyleSheet("font-size: 11pt; font-weight: bold; color: #34495e;");
    
    comboIdioma = new QComboBox();
    comboIdioma->addItem("🇧🇷 Português (Brasil)", "PT");
    comboIdioma->addItem("🇺🇸 English (US)", "EN");
    comboIdioma->addItem("🇪🇸 Español", "ES");
    comboIdioma->setStyleSheet(
        "QComboBox { "
        "   padding: 8px; "
        "   border: 2px solid #bdc3c7; "
        "   border-radius: 5px; "
        "   font-size: 11pt; "
        "} "
        "QComboBox:hover { "
        "   border: 2px solid #3498db; "
        "}"
    );
    
    layoutFormatacao->addRow(labelIdioma, comboIdioma);
    
    layoutPrincipal->addWidget(grupoFormatacao);
    
    // Aviso
    QLabel *aviso = new QLabel(QString("⚠️ %1").arg(config->traduzir("aviso_reiniciar")));
    aviso->setStyleSheet("color: #7f8c8d; font-style: italic; font-size: 9pt;");
    aviso->setWordWrap(true);
    layoutPrincipal->addWidget(aviso);
    
    layoutPrincipal->addStretch();
    
    // Botões
    QHBoxLayout *layoutBotoes = new QHBoxLayout();
    layoutBotoes->addStretch();
    
    btnCancelar = new QPushButton(QString("❌ %1").arg(config->traduzir("cancelar")));
    btnCancelar->setMinimumSize(120, 40);
    btnCancelar->setStyleSheet(
        "QPushButton { "
        "   background-color: #95a5a6; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 6px; "
        "   padding: 10px 20px; "
        "   font-size: 11pt; "
        "   font-weight: bold; "
        "} "
        "QPushButton:hover { "
        "   background-color: #7f8c8d; "
        "}"
    );
    connect(btnCancelar, &QPushButton::clicked, this, &QDialog::reject);
    
    btnSalvar = new QPushButton(QString("✅ %1").arg(config->traduzir("salvar")));
    btnSalvar->setMinimumSize(120, 40);
    btnSalvar->setStyleSheet(
        "QPushButton { "
        "   background-color: #27ae60; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 6px; "
        "   padding: 10px 20px; "
        "   font-size: 11pt; "
        "   font-weight: bold; "
        "} "
        "QPushButton:hover { "
        "   background-color: #2ecc71; "
        "}"
    );
    connect(btnSalvar, &QPushButton::clicked, this, &DialogConfiguracoes::onSalvar);
    
    layoutBotoes->addWidget(btnCancelar);
    layoutBotoes->addWidget(btnSalvar);
    
    layoutPrincipal->addLayout(layoutBotoes);
}

void DialogConfiguracoes::carregarConfiguracoes()
{
    Configuracoes *config = Configuracoes::getInstance();
    
    // Carrega tema
    if (config->getTema() == "escuro") {
        radioTemaEscuro->setChecked(true);
    } else {
        radioTemaClaro->setChecked(true);
    }
    
    // Carrega moeda
    int indexMoeda = comboMoeda->findData(config->getMoeda());
    if (indexMoeda >= 0) {
        comboMoeda->setCurrentIndex(indexMoeda);
    }
    
    // Carrega idioma
    int indexIdioma = comboIdioma->findData(config->getIdioma());
    if (indexIdioma >= 0) {
        comboIdioma->setCurrentIndex(indexIdioma);
    }
}

void DialogConfiguracoes::onSalvar()
{
    Configuracoes *config = Configuracoes::getInstance();
    
    // Salva tema
    QString novoTema = radioTemaClaro->isChecked() ? "claro" : "escuro";
    config->setTema(novoTema);
    
    // Salva moeda
    QString novaMoeda = comboMoeda->currentData().toString();
    config->setMoeda(novaMoeda);
    
    // Salva idioma
    QString novoIdioma = comboIdioma->currentData().toString();
    config->setIdioma(novoIdioma);
    
    emit configuracoesAlteradas();
    
    QMessageBox::information(this, config->traduzir("sucesso"), 
        config->traduzir("config_salvas"));
    
    accept();
}
