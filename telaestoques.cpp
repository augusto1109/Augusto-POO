#include "telaestoques.h"
#include "mainwindow.h"
#include "configuracoes.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QFont>
#include <QStatusBar>

TelaEstoques::TelaEstoques(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Gerenciador de Estoques - POO2");
    resize(800, 600);
    
    estoqueDAO = new EstoqueDAO();
    criarInterface();
    carregarEstoques();
}

TelaEstoques::~TelaEstoques()
{
    delete estoqueDAO;
}

void TelaEstoques::criarInterface()
{
    Configuracoes *config = Configuracoes::getInstance();
    
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(centralWidget);
    
    // Título
    QLabel *titulo = new QLabel(config->traduzir("meus_estoques"));
    QFont fontTitulo = titulo->font();
    fontTitulo.setPointSize(18);
    fontTitulo.setBold(true);
    titulo->setFont(fontTitulo);
    titulo->setAlignment(Qt::AlignCenter);
    
    // Informação
    labelInfo = new QLabel(config->traduzir("info_estoques"));
    labelInfo->setAlignment(Qt::AlignCenter);
    labelInfo->setStyleSheet("color: gray; margin: 10px;");
    
    // Lista de estoques
    listaEstoques = new QListWidget();
    listaEstoques->setIconSize(QSize(48, 48));
    listaEstoques->setSpacing(5);
    QFont fontLista = listaEstoques->font();
    fontLista.setPointSize(12);
    listaEstoques->setFont(fontLista);
    
    // Botões
    QHBoxLayout *layoutBotoes = new QHBoxLayout();
    
    btnNovoEstoque = new QPushButton(QString("➕ %1").arg(config->traduzir("novo_estoque")));
    btnNovoEstoque->setMinimumHeight(40);
    QFont fontBotao = btnNovoEstoque->font();
    fontBotao.setPointSize(11);
    btnNovoEstoque->setFont(fontBotao);
    
    btnExcluirEstoque = new QPushButton(QString("🗑️ %1").arg(config->traduzir("excluir_estoque")));
    btnExcluirEstoque->setMinimumHeight(40);
    btnExcluirEstoque->setFont(fontBotao);
    btnExcluirEstoque->setEnabled(false);
    
    layoutBotoes->addWidget(btnNovoEstoque);
    layoutBotoes->addWidget(btnExcluirEstoque);
    
    // Adiciona ao layout principal
    layoutPrincipal->addWidget(titulo);
    layoutPrincipal->addWidget(labelInfo);
    layoutPrincipal->addWidget(listaEstoques);
    layoutPrincipal->addLayout(layoutBotoes);
    
    // Conecta sinais
    connect(btnNovoEstoque, &QPushButton::clicked, this, &TelaEstoques::onNovoEstoque);
    connect(btnExcluirEstoque, &QPushButton::clicked, this, &TelaEstoques::onExcluirEstoque);
    connect(listaEstoques, &QListWidget::itemDoubleClicked, this, &TelaEstoques::onAbrirEstoque);
    connect(listaEstoques, &QListWidget::itemClicked, [this]() {
        btnExcluirEstoque->setEnabled(true);
    });
}

void TelaEstoques::carregarEstoques()
{
    listaEstoques->clear();
    QList<Estoque> estoques = estoqueDAO->listarTodos();
    
    for (const Estoque &estoque : estoques) {
        QListWidgetItem *item = new QListWidgetItem(listaEstoques);
        
        QString texto = QString("📦 %1").arg(estoque.getNome());
        if (!estoque.getDescricao().isEmpty()) {
            texto += QString("\n   %1").arg(estoque.getDescricao());
        }
        texto += QString("\n   Criado em: %1").arg(estoque.getDataCriacao().toString("dd/MM/yyyy HH:mm"));
        
        item->setText(texto);
        item->setData(Qt::UserRole, estoque.getId());
    }
    
    Configuracoes *config = Configuracoes::getInstance();
    
    if (estoques.isEmpty()) {
        labelInfo->setText(config->traduzir("nenhum_estoque"));
    } else {
        labelInfo->setText(QString(config->traduzir("estoques_disponiveis")).arg(estoques.size()));
    }
    
    statusBar()->showMessage(QString(config->traduzir("total_estoques")).arg(estoques.size()));
}

void TelaEstoques::onNovoEstoque()
{
    Configuracoes *config = Configuracoes::getInstance();
    
    bool ok;
    QString nome = QInputDialog::getText(this, config->traduzir("novo_estoque"), 
                                         config->traduzir("nome_estoque"), 
                                         QLineEdit::Normal, "", &ok);
    
    if (ok && !nome.trimmed().isEmpty()) {
        QString descricao = QInputDialog::getText(this, config->traduzir("novo_estoque"), 
                                                  config->traduzir("descricao_opcional"), 
                                                  QLineEdit::Normal, "", &ok);
        
        Estoque estoque;
        estoque.setId(estoqueDAO->getProximoId());
        estoque.setNome(nome.trimmed());
        if (ok) {
            estoque.setDescricao(descricao.trimmed());
        }
        
        if (estoqueDAO->salvar(estoque)) {
            QMessageBox::information(this, config->traduzir("sucesso"), config->traduzir("estoque_criado"));
            carregarEstoques();
        } else {
            QMessageBox::critical(this, config->traduzir("erro"), config->traduzir("erro_criar_estoque"));
        }
    }
}

void TelaEstoques::onAbrirEstoque(QListWidgetItem *item)
{
    int estoqueId = item->data(Qt::UserRole).toInt();
    Estoque estoque = estoqueDAO->buscarPorId(estoqueId);
    
    if (estoque.isValid()) {
        abrirEstoque(estoqueId, estoque.getNome());
    }
}

void TelaEstoques::abrirEstoque(int estoqueId, const QString &nomeEstoque)
{
    MainWindow *janelaProdutos = new MainWindow(estoqueId, nomeEstoque);
    janelaProdutos->setAttribute(Qt::WA_DeleteOnClose);
    janelaProdutos->show();
    
    // Conecta sinal de fechamento para atualizar a lista
    connect(janelaProdutos, &QMainWindow::destroyed, this, &TelaEstoques::atualizarLista);
}

void TelaEstoques::onExcluirEstoque()
{
    Configuracoes *config = Configuracoes::getInstance();
    
    QListWidgetItem *item = listaEstoques->currentItem();
    if (!item) {
        QMessageBox::warning(this, config->traduzir("atencao"), config->traduzir("selecione_estoque_excluir"));
        return;
    }
    
    int estoqueId = item->data(Qt::UserRole).toInt();
    Estoque estoque = estoqueDAO->buscarPorId(estoqueId);
    
    QMessageBox::StandardButton resposta = QMessageBox::question(this,
        config->traduzir("confirmar_exclusao"),
        QString(config->traduzir("msg_confirmar_exclusao")).arg(estoque.getNome()),
        QMessageBox::Yes | QMessageBox::No);
    
    if (resposta == QMessageBox::Yes) {
        if (estoqueDAO->excluir(estoqueId)) {
            QMessageBox::information(this, config->traduzir("sucesso"), config->traduzir("estoque_excluido"));
            carregarEstoques();
            btnExcluirEstoque->setEnabled(false);
        } else {
            QMessageBox::critical(this, config->traduzir("erro"), config->traduzir("erro_excluir_estoque"));
        }
    }
}

void TelaEstoques::atualizarLista()
{
    carregarEstoques();
}
