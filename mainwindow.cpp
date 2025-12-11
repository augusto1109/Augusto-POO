#include "mainwindow.h"
#include "dialogproduto.h"
#include "dialogdetalhes.h"
#include "dialogconfiguracoes.h"
#include "estoquedao.h"
#include "configuracoes.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>
#include <QStatusBar>
#include <QKeyEvent>
#include <QShortcut>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QDateTime>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QDir>
#include <QRegularExpression>

MainWindow::MainWindow(int estoqueId, const QString &nomeEstoque, QWidget *parent)
    : QMainWindow(parent), estoqueId(estoqueId), nomeEstoque(nomeEstoque)
{
    setWindowTitle(QString("Estoque: %1").arg(nomeEstoque));
    resize(1200, 700);

    // Inicializa o DAO com diretório específico do estoque
    dao = new ProdutoDAO();
    EstoqueDAO estoqueDAO;
    dao->setDiretorioProdutos(estoqueDAO.getDiretorioProdutos(estoqueId));
    
    // Cria a interface
    criarInterface();
    
    // Cria os menus
    criarMenus();
    
    // Aplica tema das configurações
    aplicarTema();
    
    // Atualiza textos conforme idioma
    atualizarTextos();
    
    // Carrega os produtos
    carregarProdutos();
}

MainWindow::~MainWindow()
{
    delete dao;
}

void MainWindow::criarInterface()
{
    // Widget central
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Layout principal
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(centralWidget);
    layoutPrincipal->setContentsMargins(0, 0, 0, 0);
    layoutPrincipal->setSpacing(0);
    
    // Cria a toolbar
    criarToolbar();
    
    // Tabela de produtos (tela cheia)
    tabelaProdutos = new QTableWidget();
    configurarTabela();
    
    layoutPrincipal->addWidget(tabelaProdutos);
    
    // Conecta sinais
    connect(tabelaProdutos, &QTableWidget::itemSelectionChanged, this, &MainWindow::onProdutoSelecionado);
    connect(tabelaProdutos, &QTableWidget::cellDoubleClicked, this, &MainWindow::onVerDetalhes);
    
    // Cria atalhos de teclado
    criarAtalhos();
}

void MainWindow::criarToolbar()
{
    toolbar = new QToolBar("Ferramentas");
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(24, 24));
    toolbar->setStyleSheet(
        "QToolBar { "
        "   background-color: #2c3e50; "
        "   border: none; "
        "   padding: 5px; "
        "   spacing: 8px; "
        "} "
    );
    addToolBar(toolbar);
    
    QFont font;
    font.setPointSize(10);
    font.setBold(false);
    
    // Botão Voltar
    btnVoltar = new QPushButton("  ← Voltar");
    btnVoltar->setMinimumSize(130, 40);
    btnVoltar->setFont(font);
    btnVoltar->setCursor(Qt::PointingHandCursor);
    btnVoltar->setStyleSheet(
        "QPushButton { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "       stop:0 #3498db, stop:1 #2980b9); "
        "   color: white; "
        "   border: 2px solid #2471a3; "
        "   border-radius: 6px; "
        "   padding: 10px 20px; "
        "   text-align: left; "
        "   font-weight: bold; "
        "   font-size: 11pt; "
        "} "
        "QPushButton:hover { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "       stop:0 #5dade2, stop:1 #3498db); "
        "   border: 2px solid #3498db; "
        "} "
        "QPushButton:pressed { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "       stop:0 #2471a3, stop:1 #1f618d); "
        "   border: 2px solid #1a5276; "
        "}"
    );
    
    // Espaçador
    QWidget* spacer1 = new QWidget();
    spacer1->setFixedWidth(20);
    
    // Botão Adicionar
    btnAdicionarProduto = new QPushButton("  ➕ Adicionar");
    btnAdicionarProduto->setMinimumSize(140, 36);
    btnAdicionarProduto->setFont(font);
    btnAdicionarProduto->setCursor(Qt::PointingHandCursor);
    btnAdicionarProduto->setStyleSheet(
        "QPushButton { "
        "   background-color: #27ae60; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 4px; "
        "   padding: 8px 16px; "
        "   text-align: left; "
        "   font-weight: bold; "
        "} "
        "QPushButton:hover { "
        "   background-color: #2ecc71; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #229954; "
        "}"
    );
    
    btnEditarProduto = new QPushButton("  ✏️ Editar");
    btnEditarProduto->setMinimumSize(110, 36);
    btnEditarProduto->setFont(font);
    btnEditarProduto->setCursor(Qt::PointingHandCursor);
    btnEditarProduto->setEnabled(false);
    btnEditarProduto->setStyleSheet(
        "QPushButton { "
        "   background-color: #3498db; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 4px; "
        "   padding: 8px 16px; "
        "   text-align: left; "
        "} "
        "QPushButton:hover:enabled { "
        "   background-color: #5dade2; "
        "} "
        "QPushButton:pressed:enabled { "
        "   background-color: #2980b9; "
        "} "
        "QPushButton:disabled { "
        "   background-color: #95a5a6; "
        "   color: #bdc3c7; "
        "}"
    );
    
    btnExcluirProduto = new QPushButton("  🗑️ Excluir");
    btnExcluirProduto->setMinimumSize(110, 36);
    btnExcluirProduto->setFont(font);
    btnExcluirProduto->setCursor(Qt::PointingHandCursor);
    btnExcluirProduto->setEnabled(false);
    btnExcluirProduto->setStyleSheet(
        "QPushButton { "
        "   background-color: #e74c3c; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 4px; "
        "   padding: 8px 16px; "
        "   text-align: left; "
        "} "
        "QPushButton:hover:enabled { "
        "   background-color: #ec7063; "
        "} "
        "QPushButton:pressed:enabled { "
        "   background-color: #c0392b; "
        "} "
        "QPushButton:disabled { "
        "   background-color: #95a5a6; "
        "   color: #bdc3c7; "
        "}"
    );
    
    btnDetalhes = new QPushButton("  👁️ Detalhes");
    btnDetalhes->setMinimumSize(120, 36);
    btnDetalhes->setFont(font);
    btnDetalhes->setCursor(Qt::PointingHandCursor);
    btnDetalhes->setEnabled(false);
    btnDetalhes->setStyleSheet(
        "QPushButton { "
        "   background-color: #9b59b6; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 4px; "
        "   padding: 8px 16px; "
        "   text-align: left; "
        "} "
        "QPushButton:hover:enabled { "
        "   background-color: #af7ac5; "
        "} "
        "QPushButton:pressed:enabled { "
        "   background-color: #7d3c98; "
        "} "
        "QPushButton:disabled { "
        "   background-color: #95a5a6; "
        "   color: #bdc3c7; "
        "}"
    );
    
    // Espaçador expansível
    QWidget* expandingSpacer = new QWidget();
    expandingSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    
    btnAtualizar = new QPushButton("  🔄 Atualizar");
    btnAtualizar->setMinimumSize(120, 36);
    btnAtualizar->setFont(font);
    btnAtualizar->setCursor(Qt::PointingHandCursor);
    btnAtualizar->setStyleSheet(
        "QPushButton { "
        "   background-color: #34495e; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 4px; "
        "   padding: 8px 16px; "
        "   text-align: left; "
        "} "
        "QPushButton:hover { "
        "   background-color: #4a5f7f; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #2c3e50; "
        "}"
    );
    
    toolbar->addWidget(btnVoltar);
    toolbar->addWidget(spacer1);
    toolbar->addWidget(btnAdicionarProduto);
    toolbar->addSeparator();
    toolbar->addWidget(btnEditarProduto);
    toolbar->addWidget(btnExcluirProduto);
    toolbar->addWidget(btnDetalhes);
    toolbar->addWidget(expandingSpacer);
    toolbar->addWidget(btnAtualizar);
    
    // Conecta os botões
    connect(btnVoltar, &QPushButton::clicked, this, &MainWindow::onVoltar);
    connect(btnAdicionarProduto, &QPushButton::clicked, this, &MainWindow::onAdicionarProduto);
    connect(btnEditarProduto, &QPushButton::clicked, this, &MainWindow::onEditarProduto);
    connect(btnExcluirProduto, &QPushButton::clicked, this, &MainWindow::onExcluirProduto);
    connect(btnDetalhes, &QPushButton::clicked, this, &MainWindow::onVerDetalhes);
    connect(btnAtualizar, &QPushButton::clicked, this, &MainWindow::onAtualizarLista);
}

void MainWindow::configurarTabela()
{
    tabelaProdutos->setColumnCount(6);
    tabelaProdutos->setHorizontalHeaderLabels({"ID", "Nome", "Preço", "Quantidade", "Categoria", "Descrição"});
    
    tabelaProdutos->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabelaProdutos->setSelectionMode(QAbstractItemView::ExtendedSelection); // Permite seleção múltipla
    tabelaProdutos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabelaProdutos->setAlternatingRowColors(true);
    tabelaProdutos->verticalHeader()->setVisible(false);
    
    // Ajusta largura das colunas
    tabelaProdutos->horizontalHeader()->setStretchLastSection(true);
    tabelaProdutos->setColumnWidth(0, 60);
    tabelaProdutos->setColumnWidth(1, 250);
    tabelaProdutos->setColumnWidth(2, 120);
    tabelaProdutos->setColumnWidth(3, 120);
    tabelaProdutos->setColumnWidth(4, 150);
    
    // Estilo da tabela
    tabelaProdutos->setStyleSheet(
        "QTableWidget { "
        "   border: none; "
        "   gridline-color: #dcdde1; "
        "   font-size: 11pt; "
        "} "
        "QTableWidget::item { "
        "   padding: 8px; "
        "} "
        "QTableWidget::item:selected { "
        "   background-color: #3498db; "
        "   color: white; "
        "} "
        "QHeaderView::section { "
        "   background-color: #34495e; "
        "   color: white; "
        "   padding: 10px; "
        "   border: none; "
        "   font-weight: bold; "
        "   font-size: 11pt; "
        "}"
    );
}

void MainWindow::carregarProdutos()
{
    tabelaProdutos->setRowCount(0);
    
    Configuracoes *config = Configuracoes::getInstance();
    QString simboloMoeda = config->getSimboloMoeda();
    
    QList<Produto> produtos = dao->listarTodos();
    
    for (const Produto &produto : produtos) {
        int row = tabelaProdutos->rowCount();
        tabelaProdutos->insertRow(row);
        
        tabelaProdutos->setItem(row, 0, new QTableWidgetItem(QString::number(produto.getId())));
        tabelaProdutos->setItem(row, 1, new QTableWidgetItem(produto.getNome()));
        tabelaProdutos->setItem(row, 2, new QTableWidgetItem(QString("%1 %2").arg(simboloMoeda).arg(produto.getPreco(), 0, 'f', 2)));
        
        QTableWidgetItem *itemQtd = new QTableWidgetItem(QString::number(produto.getQuantidade()));
        if (produto.getQuantidade() == 0) {
            itemQtd->setForeground(Qt::red);
        } else if (produto.getQuantidade() <= produto.getEstoqueMinimo()) {
            itemQtd->setForeground(QColor(255, 140, 0)); // Orange
        }
        tabelaProdutos->setItem(row, 3, itemQtd);
        
        tabelaProdutos->setItem(row, 4, new QTableWidgetItem(produto.getCategoria()));
        
        QString descricaoResumida = produto.getDescricao();
        if (descricaoResumida.length() > 50) {
            descricaoResumida = descricaoResumida.left(50) + "...";
        }
        tabelaProdutos->setItem(row, 5, new QTableWidgetItem(descricaoResumida));
    }
    
    statusBar()->showMessage(QString("Total de produtos: %1").arg(produtos.size()));
    atualizarBotoes();
}

void MainWindow::atualizarBotoes()
{
    bool temSelecao = tabelaProdutos->currentRow() >= 0;
    btnEditarProduto->setEnabled(temSelecao);
    btnExcluirProduto->setEnabled(temSelecao);
    btnDetalhes->setEnabled(temSelecao);
}

void MainWindow::onAdicionarProduto()
{
    Produto produto = DialogProduto::novoProduto(dao->getProximoId(), this);
    
    if (produto.isValid()) {
        if (dao->salvar(produto)) {
            QMessageBox::information(this, "Sucesso", "Produto adicionado com sucesso!");
            carregarProdutos();
        } else {
            QMessageBox::critical(this, "Erro", "Erro ao salvar o produto!");
        }
    }
}

void MainWindow::onEditarProduto()
{
    int row = tabelaProdutos->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Atenção", "Selecione um produto para editar!");
        return;
    }
    
    int produtoId = tabelaProdutos->item(row, 0)->text().toInt();
    Produto produto = dao->buscarPorId(produtoId);
    
    if (produto.isValid()) {
        if (DialogProduto::editarProduto(produto, this)) {
            if (dao->atualizar(produto)) {
                QMessageBox::information(this, "Sucesso", "Produto atualizado com sucesso!");
                carregarProdutos();
            } else {
                QMessageBox::critical(this, "Erro", "Erro ao atualizar o produto!");
            }
        }
    }
}

void MainWindow::onExcluirProduto()
{
    QList<QTableWidgetItem*> itensSelecionados = tabelaProdutos->selectedItems();
    if (itensSelecionados.isEmpty()) {
        QMessageBox::warning(this, "Atenção", "Selecione um ou mais produtos para excluir!");
        return;
    }
    
    // Obtém linhas únicas selecionadas
    QSet<int> linhasSelecionadas;
    for (QTableWidgetItem *item : itensSelecionados) {
        linhasSelecionadas.insert(item->row());
    }
    
    int quantidadeProdutos = linhasSelecionadas.size();
    QString mensagem;
    
    if (quantidadeProdutos == 1) {
        int row = *linhasSelecionadas.begin();
        QString nomeProduto = tabelaProdutos->item(row, 1)->text();
        mensagem = QString("Deseja realmente excluir o produto '%1'?").arg(nomeProduto);
    } else {
        mensagem = QString("Deseja realmente excluir %1 produtos selecionados?").arg(quantidadeProdutos);
    }
    
    QMessageBox::StandardButton resposta = QMessageBox::question(this, 
        "Confirmar Exclusão", 
        mensagem,
        QMessageBox::Yes | QMessageBox::No);
    
    if (resposta == QMessageBox::Yes) {
        int excluidos = 0;
        int erros = 0;
        
        // Coleta os IDs antes de excluir
        QList<int> ids;
        for (int row : linhasSelecionadas) {
            ids.append(tabelaProdutos->item(row, 0)->text().toInt());
        }
        
        // Exclui cada produto
        for (int produtoId : ids) {
            if (dao->excluir(produtoId)) {
                excluidos++;
            } else {
                erros++;
            }
        }
        
        // Recarrega a lista
        carregarProdutos();
        
        // Mostra resultado
        if (erros == 0) {
            QMessageBox::information(this, "Sucesso", 
                QString("%1 produto(s) excluído(s) com sucesso!").arg(excluidos));
        } else {
            QMessageBox::warning(this, "Atenção", 
                QString("%1 produto(s) excluído(s), %2 erro(s)!").arg(excluidos).arg(erros));
        }
    }
}

void MainWindow::onVerDetalhes()
{
    int row = tabelaProdutos->currentRow();
    if (row < 0) {
        return;
    }
    
    int produtoId = tabelaProdutos->item(row, 0)->text().toInt();
    Produto produto = dao->buscarPorId(produtoId);
    
    if (produto.isValid()) {
        DialogDetalhes::mostrarDetalhes(produto, this);
    }
}

void MainWindow::onAtualizarLista()
{
    carregarProdutos();
    statusBar()->showMessage("Lista atualizada!", 2000);
}

void MainWindow::onProdutoSelecionado()
{
    atualizarBotoes();
}

void MainWindow::onVoltar()
{
    close(); // Fecha a janela de produtos e volta para a tela de estoques
}

void MainWindow::criarMenus()
{
    Configuracoes *config = Configuracoes::getInstance();
    
    // Menu Arquivo
    QMenu *menuArquivo = menuBar()->addMenu(config->traduzir("arquivo"));
    
    QAction *actionExportar = menuArquivo->addAction(config->traduzir("exportar_csv"));
    connect(actionExportar, &QAction::triggered, this, &MainWindow::exportarCSV);
    
    QAction *actionImportar = menuArquivo->addAction(config->traduzir("importar_csv"));
    connect(actionImportar, &QAction::triggered, this, &MainWindow::importarCSV);
    
    menuArquivo->addSeparator();
    
    QAction *actionBackup = menuArquivo->addAction(config->traduzir("backup"));
    connect(actionBackup, &QAction::triggered, this, &MainWindow::fazerBackup);
    
    menuArquivo->addSeparator();
    
    QAction *actionSair = menuArquivo->addAction(config->traduzir("sair"));
    actionSair->setShortcut(QKeySequence("Ctrl+Q"));
    connect(actionSair, &QAction::triggered, this, &MainWindow::onVoltar);
    
    // Menu Relatórios
    QMenu *menuRelatorios = menuBar()->addMenu(config->traduzir("relatorios"));
    
    QAction *actionInventario = menuRelatorios->addAction(config->traduzir("imprimir"));
    actionInventario->setShortcut(QKeySequence("Ctrl+P"));
    connect(actionInventario, &QAction::triggered, this, &MainWindow::imprimirInventario);
    
    // Menu Configurações
    QMenu *menuConfiguracoes = menuBar()->addMenu(config->traduzir("configuracoes"));
    
    QAction *actionConfiguracoes = menuConfiguracoes->addAction(config->traduzir("preferencias"));
    actionConfiguracoes->setShortcut(QKeySequence("Ctrl+,"));
    connect(actionConfiguracoes, &QAction::triggered, this, &MainWindow::abrirConfiguracoes);
}

void MainWindow::exportarCSV()
{
    QString nomeArquivo = QFileDialog::getSaveFileName(this,
        "Exportar Estoque para CSV",
        QString("estoque_%1_%2.csv").arg(nomeEstoque.replace(" ", "_")).arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")),
        "CSV Files (*.csv)");
    
    if (nomeArquivo.isEmpty()) {
        return;
    }
    
    QFile arquivo(nomeArquivo);
    if (!arquivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível criar o arquivo!");
        return;
    }
    
    QTextStream saida(&arquivo);
    saida.setEncoding(QStringConverter::Utf8);
    
    // Cabeçalho
    saida << "ID;Nome;Preco;Quantidade;EstoqueMinimo;Categoria;Descricao\n";
    
    // Dados
    QList<Produto> produtos = dao->listarTodos();
    for (const Produto &p : produtos) {
        saida << p.getId() << ";"
              << p.getNome() << ";"
              << QString::number(p.getPreco(), 'f', 2) << ";"
              << p.getQuantidade() << ";"
              << p.getEstoqueMinimo() << ";"
              << p.getCategoria() << ";"
              << p.getDescricao().replace("\n", " ") << "\n";
    }
    
    arquivo.close();
    
    QMessageBox::information(this, "Sucesso", 
        QString("Estoque exportado com sucesso!\n%1 produtos exportados.").arg(produtos.size()));
}

void MainWindow::importarCSV()
{
    QString nomeArquivo = QFileDialog::getOpenFileName(this,
        "Importar Produtos de CSV",
        "",
        "CSV Files (*.csv);;All Files (*.*)");
    
    if (nomeArquivo.isEmpty()) {
        return;
    }
    
    QFile arquivo(nomeArquivo);
    if (!arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível abrir o arquivo!");
        return;
    }
    
    QTextStream entrada(&arquivo);
    entrada.setEncoding(QStringConverter::Utf8);
    
    // Lê a primeira linha para detectar o delimitador
    QString primeiraLinha;
    if (!entrada.atEnd()) {
        primeiraLinha = entrada.readLine();
    }
    
    // Detecta o delimitador (ponto-e-vírgula ou vírgula)
    QChar delimitador = ';';
    if (primeiraLinha.count(',') > primeiraLinha.count(';')) {
        delimitador = ',';
    }
    
    int importados = 0;
    int erros = 0;
    QStringList errosDetalhados;
    int linha = 1;
    
    while (!entrada.atEnd()) {
        linha++;
        QString linhaTexto = entrada.readLine().trimmed();
        
        // Ignora linhas vazias
        if (linhaTexto.isEmpty()) {
            continue;
        }
        
        QStringList campos = linhaTexto.split(delimitador);
        
        // Valida número mínimo de campos (pode ter ou não ID)
        if (campos.size() < 4) {
            errosDetalhados.append(QString("Linha %1: Número insuficiente de campos (%2)").arg(linha).arg(campos.size()));
            erros++;
            continue;
        }
        
        try {
            Produto produto;
            
            // Se tiver 7 campos, assume formato: ID;Nome;Preco;Quantidade;EstoqueMinimo;Categoria;Descricao
            // Se tiver 6 campos, assume formato: Nome;Preco;Quantidade;EstoqueMinimo;Categoria;Descricao
            // Se tiver 4-5 campos, assume formato mínimo: Nome;Preco;Quantidade;Categoria
            
            int idx = 0;
            
            // Se primeiro campo é numérico e temos 7 campos, pula o ID
            if (campos.size() >= 7 && campos[0].trimmed().toInt() > 0) {
                idx = 1; // Pula o ID
            }
            
            QString nome = campos[idx++].trimmed();
            if (nome.isEmpty()) {
                errosDetalhados.append(QString("Linha %1: Nome vazio").arg(linha));
                erros++;
                continue;
            }
            produto.setNome(nome);
            
            // Preço
            QString precoStr = campos[idx++].trimmed().replace(",", ".");
            bool precoOk;
            double preco = precoStr.toDouble(&precoOk);
            if (!precoOk || preco < 0) {
                errosDetalhados.append(QString("Linha %1: Preço inválido (%2)").arg(linha).arg(campos[idx-1]));
                erros++;
                continue;
            }
            produto.setPreco(preco);
            
            // Quantidade
            bool qtdOk;
            int quantidade = campos[idx++].trimmed().toInt(&qtdOk);
            if (!qtdOk || quantidade < 0) {
                errosDetalhados.append(QString("Linha %1: Quantidade inválida (%2)").arg(linha).arg(campos[idx-1]));
                erros++;
                continue;
            }
            produto.setQuantidade(quantidade);
            
            // Estoque Mínimo (opcional, padrão 5)
            int estoqueMinimo = 5;
            if (idx < campos.size()) {
                bool estMinOk;
                int estMin = campos[idx].trimmed().toInt(&estMinOk);
                if (estMinOk && estMin >= 0) {
                    estoqueMinimo = estMin;
                    idx++;
                } else if (!campos[idx].trimmed().isEmpty() && !campos[idx].contains(QRegularExpression("[a-zA-Z]"))) {
                    // Se não é vazio e não contém letras, mas falhou conversão, pula
                    idx++;
                }
            }
            produto.setEstoqueMinimo(estoqueMinimo);
            
            // Categoria (opcional)
            QString categoria = "";
            if (idx < campos.size()) {
                categoria = campos[idx++].trimmed();
            }
            produto.setCategoria(categoria);
            
            // Descrição (opcional)
            QString descricao = "";
            if (idx < campos.size()) {
                descricao = campos[idx++].trimmed();
            }
            produto.setDescricao(descricao);
            
            if (dao->salvar(produto)) {
                importados++;
            } else {
                errosDetalhados.append(QString("Linha %1: Erro ao salvar no banco").arg(linha));
                erros++;
            }
            
        } catch (...) {
            errosDetalhados.append(QString("Linha %1: Erro inesperado").arg(linha));
            erros++;
        }
    }
    
    arquivo.close();
    carregarProdutos();
    
    QString mensagem = QString("Importação concluída!\n%1 produto(s) importado(s)").arg(importados);
    if (erros > 0) {
        mensagem += QString("\n%1 erro(s)").arg(erros);
        
        // Mostra detalhes dos erros (máximo 10)
        if (!errosDetalhados.isEmpty()) {
            mensagem += "\n\nPrimeiros erros:";
            for (int i = 0; i < qMin(10, errosDetalhados.size()); i++) {
                mensagem += "\n• " + errosDetalhados[i];
            }
            if (errosDetalhados.size() > 10) {
                mensagem += QString("\n... e mais %1 erro(s)").arg(errosDetalhados.size() - 10);
            }
        }
    }
    
    QMessageBox::information(this, "Importação", mensagem);
}

void MainWindow::fazerBackup()
{
    QString pastaDestino = QFileDialog::getExistingDirectory(this,
        "Selecionar Pasta para Backup",
        QDir::homePath());
    
    if (pastaDestino.isEmpty()) {
        return;
    }
    
    QString dataHora = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString pastaBackup = QString("%1/Backup_%2_%3").arg(pastaDestino, nomeEstoque.replace(" ", "_"), dataHora);
    
    QDir dir;
    if (!dir.mkpath(pastaBackup)) {
        QMessageBox::critical(this, "Erro", "Não foi possível criar a pasta de backup!");
        return;
    }
    
    // Copia todos os arquivos JSON do estoque
    EstoqueDAO estoqueDAO;
    QString diretorioProdutos = estoqueDAO.getDiretorioProdutos(estoqueId);
    QDir dirProdutos(diretorioProdutos);
    
    QStringList arquivos = dirProdutos.entryList(QStringList() << "*.json", QDir::Files);
    
    int copiados = 0;
    for (const QString &nomeArquivo : arquivos) {
        QString origem = dirProdutos.absoluteFilePath(nomeArquivo);
        QString destino = pastaBackup + "/" + nomeArquivo;
        
        if (QFile::copy(origem, destino)) {
            copiados++;
        }
    }
    
    QMessageBox::information(this, "Backup Concluído",
        QString("Backup realizado com sucesso!\n%1 arquivo(s) copiado(s)\nLocal: %2").arg(copiados).arg(pastaBackup));
}

void MainWindow::imprimirInventario()
{
    QList<Produto> produtos = dao->listarTodos();
    
    if (produtos.isEmpty()) {
        QMessageBox::information(this, "Aviso", "Não há produtos para imprimir!");
        return;
    }
    
    // Cria documento HTML para impressão
    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; }"
                   "h1 { color: #2c3e50; text-align: center; }"
                   "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                   "th { background-color: #34495e; color: white; padding: 10px; text-align: left; }"
                   "td { border: 1px solid #ddd; padding: 8px; }"
                   "tr:nth-child(even) { background-color: #f2f2f2; }"
                   ".baixo { background-color: #f39c12; color: white; font-weight: bold; }"
                   ".esgotado { background-color: #e74c3c; color: white; font-weight: bold; }"
                   ".ok { background-color: #27ae60; color: white; font-weight: bold; }"
                   ".footer { margin-top: 30px; text-align: center; font-size: 10pt; color: #7f8c8d; }"
                   "</style></head><body>";
    
    html += QString("<h1>Relatório de Inventário - %1</h1>").arg(nomeEstoque);
    html += QString("<p><strong>Data:</strong> %1</p>").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));
    html += QString("<p><strong>Total de Produtos:</strong> %1</p>").arg(produtos.size());
    
    html += "<table>";
    html += "<tr><th>ID</th><th>Nome</th><th>Preço</th><th>Quantidade</th><th>Est. Mínimo</th><th>Categoria</th><th>Status</th></tr>";
    
    double valorTotal = 0.0;
    int totalEsgotados = 0;
    int totalBaixo = 0;
    
    for (const Produto &p : produtos) {
        QString statusClass, statusTexto;
        
        if (p.getQuantidade() == 0) {
            statusClass = "esgotado";
            statusTexto = "ESGOTADO";
            totalEsgotados++;
        } else if (p.getQuantidade() <= p.getEstoqueMinimo()) {
            statusClass = "baixo";
            statusTexto = "BAIXO";
            totalBaixo++;
        } else {
            statusClass = "ok";
            statusTexto = "OK";
        }
        
        valorTotal += p.getPreco() * p.getQuantidade();
        
        html += QString("<tr>"
                       "<td>%1</td>"
                       "<td>%2</td>"
                       "<td>R$ %3</td>"
                       "<td>%4</td>"
                       "<td>%5</td>"
                       "<td>%6</td>"
                       "<td class='%7'>%8</td>"
                       "</tr>")
                .arg(p.getId())
                .arg(p.getNome())
                .arg(p.getPreco(), 0, 'f', 2)
                .arg(p.getQuantidade())
                .arg(p.getEstoqueMinimo())
                .arg(p.getCategoria())
                .arg(statusClass)
                .arg(statusTexto);
    }
    
    html += "</table>";
    
    html += QString("<div class='footer'>"
                   "<p><strong>Resumo:</strong> Valor Total do Estoque: R$ %1 | "
                   "Produtos Esgotados: %2 | Estoque Baixo: %3</p>"
                   "<p>Relatório gerado pelo Sistema de Gerenciamento de Estoque</p>"
                   "</div>")
           .arg(valorTotal, 0, 'f', 2)
           .arg(totalEsgotados)
           .arg(totalBaixo);
    
    html += "</body></html>";
    
    // Cria o diálogo de impressão
    QPrinter impressora(QPrinter::HighResolution);
    QPrintDialog *dialogo = new QPrintDialog(&impressora, this);
    dialogo->setWindowTitle("Imprimir Inventário");
    
    if (dialogo->exec() == QDialog::Accepted) {
        QTextDocument documento;
        documento.setHtml(html);
        documento.print(&impressora);
        
        QMessageBox::information(this, "Sucesso", "Relatório enviado para impressão!");
    }
    
    delete dialogo;
}

void MainWindow::criarAtalhos()
{
    // Ctrl+N - Novo produto
    QShortcut *shortcutNovo = new QShortcut(QKeySequence("Ctrl+N"), this);
    connect(shortcutNovo, &QShortcut::activated, this, &MainWindow::onAdicionarProduto);
    
    // F5 - Atualizar lista
    QShortcut *shortcutAtualizar = new QShortcut(QKeySequence("F5"), this);
    connect(shortcutAtualizar, &QShortcut::activated, this, &MainWindow::onAtualizarLista);
    
    // Ctrl+E - Editar produto selecionado
    QShortcut *shortcutEditar = new QShortcut(QKeySequence("Ctrl+E"), this);
    connect(shortcutEditar, &QShortcut::activated, this, &MainWindow::onEditarProduto);
    
    // Delete - Excluir produto selecionado
    QShortcut *shortcutExcluir = new QShortcut(QKeySequence::Delete, this);
    connect(shortcutExcluir, &QShortcut::activated, this, &MainWindow::onExcluirProduto);
    
    // Enter - Ver detalhes do produto selecionado
    QShortcut *shortcutDetalhes = new QShortcut(QKeySequence("Return"), this);
    connect(shortcutDetalhes, &QShortcut::activated, this, &MainWindow::onVerDetalhes);
    
    // Esc - Voltar
    QShortcut *shortcutVoltar = new QShortcut(QKeySequence("Esc"), this);
    connect(shortcutVoltar, &QShortcut::activated, this, &MainWindow::onVoltar);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Permite navegação com setas na tabela
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
        QMainWindow::keyPressEvent(event);
        return;
    }
    
    QMainWindow::keyPressEvent(event);
}

void MainWindow::abrirConfiguracoes()
{
    DialogConfiguracoes dialog(this);
    connect(&dialog, &DialogConfiguracoes::configuracoesAlteradas, this, [this]() {
        aplicarTema();
        
        // Recria os menus para atualizar o idioma
        menuBar()->clear();
        criarMenus();
        
        atualizarTextos();
    });
    dialog.exec();
}

void MainWindow::atualizarTextos()
{
    Configuracoes *config = Configuracoes::getInstance();
    
    // Atualiza título da janela
    setWindowTitle(QString("%1: %2").arg(config->traduzir("estoque")).arg(nomeEstoque));
    
    // Atualiza textos dos botões
    btnVoltar->setText(QString("  ← %1").arg(config->traduzir("voltar")));
    btnAdicionarProduto->setText(QString("  ➕ %1").arg(config->traduzir("adicionar")));
    btnEditarProduto->setText(QString("  ✏️ %1").arg(config->traduzir("editar")));
    btnExcluirProduto->setText(QString("  🗑️ %1").arg(config->traduzir("excluir")));
    btnDetalhes->setText(QString("  📄 %1").arg(config->traduzir("detalhes")));
    btnAtualizar->setText(QString("  🔄 %1").arg(config->traduzir("atualizar")));
    
    // Atualiza cabeçalhos da tabela
    QStringList cabecalhos;
    cabecalhos << "ID" 
               << config->traduzir("nome")
               << config->traduzir("preco")
               << config->traduzir("quantidade")
               << config->traduzir("categoria")
               << config->traduzir("descricao");
    tabelaProdutos->setHorizontalHeaderLabels(cabecalhos);
}

void MainWindow::aplicarTema()
{
    Configuracoes *config = Configuracoes::getInstance();
    
    if (config->getTema() == "escuro") {
        // Tema escuro - fundo preto
        setStyleSheet(
            "QMainWindow { background-color: #000000; }"
            "QTableWidget { "
            "   background-color: #1a1a1a; "
            "   color: #ffffff; "
            "   gridline-color: #444444; "
            "   selection-background-color: #3498db; "
            "}"
            "QTableWidget::item { color: #ffffff; }"
            "QHeaderView::section { "
            "   background-color: #0d0d0d; "
            "   color: #ffffff; "
            "   padding: 8px; "
            "   border: 1px solid #444444; "
            "   font-weight: bold; "
            "}"
            "QMenuBar { "
            "   background-color: #0d0d0d; "
            "   color: #ffffff; "
            "}"
            "QMenuBar::item:selected { "
            "   background-color: #1a1a1a; "
            "}"
            "QMenu { "
            "   background-color: #1a1a1a; "
            "   color: #ffffff; "
            "   border: 1px solid #444444; "
            "}"
            "QMenu::item:selected { "
            "   background-color: #3498db; "
            "}"
        );
        
        toolbar->setStyleSheet(
            "QToolBar { "
            "   background-color: #0d0d0d; "
            "   border: none; "
            "   padding: 5px; "
            "   spacing: 8px; "
            "}"
        );
    } else {
        // Tema claro - fundo branco
        setStyleSheet(
            "QMainWindow { background-color: #ffffff; }"
            "QTableWidget { "
            "   background-color: #ffffff; "
            "   color: #000000; "
            "   gridline-color: #dcdcdc; "
            "   selection-background-color: #3498db; "
            "}"
            "QTableWidget::item { color: #000000; }"
            "QHeaderView::section { "
            "   background-color: #f5f5f5; "
            "   color: #000000; "
            "   padding: 8px; "
            "   border: 1px solid #dcdcdc; "
            "   font-weight: bold; "
            "}"
            "QMenuBar { "
            "   background-color: #f5f5f5; "
            "   color: #000000; "
            "}"
            "QMenuBar::item:selected { "
            "   background-color: #e0e0e0; "
            "}"
            "QMenu { "
            "   background-color: #ffffff; "
            "   color: #000000; "
            "   border: 1px solid #dcdcdc; "
            "}"
            "QMenu::item:selected { "
            "   background-color: #3498db; "
            "   color: #ffffff; "
            "}"
        );
        
        toolbar->setStyleSheet(
            "QToolBar { "
            "   background-color: #2c3e50; "
            "   border: none; "
            "   padding: 5px; "
            "   spacing: 8px; "
            "}"
        );
    }
    
    // Recarrega a lista para aplicar cores
    carregarProdutos();
}
