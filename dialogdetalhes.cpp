#include "dialogdetalhes.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QFont>

DialogDetalhes::DialogDetalhes(const Produto &produto, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Detalhes do Produto");
    setMinimumSize(550, 500);
    resize(550, 500);
    
    // Define cor de fundo do dialog
    setStyleSheet("QDialog { background-color: #f5f6fa; }");
    
    criarInterface(produto);
}

void DialogDetalhes::criarInterface(const Produto &produto)
{
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);
    layoutPrincipal->setSpacing(20);
    layoutPrincipal->setContentsMargins(25, 25, 25, 25);
    
    // ========== CABEÇALHO COM NOME DO PRODUTO ==========
    QWidget *headerWidget = new QWidget();
    headerWidget->setStyleSheet(
        "QWidget { "
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3498db, stop:1 #2980b9); "
        "   border-radius: 10px; "
        "}"
    );
    QVBoxLayout *headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(20, 15, 20, 15);
    
    QLabel *labelNome = new QLabel(produto.getNome());
    QFont fontNome;
    fontNome.setPointSize(20);
    fontNome.setBold(true);
    labelNome->setFont(fontNome);
    labelNome->setWordWrap(true);
    labelNome->setStyleSheet("color: white;");
    labelNome->setAlignment(Qt::AlignCenter);
    
    headerLayout->addWidget(labelNome);
    
    // ========== CARD DE INFORMAÇÕES PRINCIPAIS ==========
    QWidget *cardInfo = new QWidget();
    cardInfo->setStyleSheet(
        "QWidget { "
        "   background-color: white; "
        "   border-radius: 10px; "
        "}"
    );
    QVBoxLayout *cardLayout = new QVBoxLayout(cardInfo);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(15);
    
    QFont fontInfo;
    fontInfo.setPointSize(11);
    
    // Grid para informações em 2 colunas
    QGridLayout *gridInfo = new QGridLayout();
    gridInfo->setSpacing(15);
    
    // ID
    QLabel *labelIdTitle = new QLabel("ID:");
    labelIdTitle->setFont(fontInfo);
    labelIdTitle->setStyleSheet("color: #7f8c8d; font-weight: bold;");
    QLabel *labelIdValue = new QLabel(QString::number(produto.getId()));
    labelIdValue->setFont(fontInfo);
    labelIdValue->setStyleSheet("color: #2c3e50;");
    
    // Preço
    QLabel *labelPrecoTitle = new QLabel("Preço:");
    labelPrecoTitle->setFont(fontInfo);
    labelPrecoTitle->setStyleSheet("color: #7f8c8d; font-weight: bold;");
    QLabel *labelPrecoValue = new QLabel(QString("R$ %1").arg(produto.getPreco(), 0, 'f', 2));
    QFont fontPreco = fontInfo;
    fontPreco.setPointSize(16);
    fontPreco.setBold(true);
    labelPrecoValue->setFont(fontPreco);
    labelPrecoValue->setStyleSheet("color: #27ae60;");
    
    // Quantidade
    QLabel *labelQtdTitle = new QLabel("Quantidade:");
    labelQtdTitle->setFont(fontInfo);
    labelQtdTitle->setStyleSheet("color: #7f8c8d; font-weight: bold;");
    QLabel *labelQtdValue = new QLabel(QString("%1 un").arg(produto.getQuantidade()));
    QFont fontQtd = fontInfo;
    fontQtd.setPointSize(14);
    fontQtd.setBold(true);
    labelQtdValue->setFont(fontQtd);
    labelQtdValue->setStyleSheet("color: #2c3e50;");
    
    // Estoque Mínimo
    QLabel *labelEstMinimoTitle = new QLabel("Estoque Mínimo:");
    labelEstMinimoTitle->setFont(fontInfo);
    labelEstMinimoTitle->setStyleSheet("color: #7f8c8d; font-weight: bold;");
    QLabel *labelEstMinimoValue = new QLabel(QString("%1 un").arg(produto.getEstoqueMinimo()));
    labelEstMinimoValue->setFont(fontInfo);
    labelEstMinimoValue->setStyleSheet("color: #2c3e50;");
    
    gridInfo->addWidget(labelIdTitle, 0, 0);
    gridInfo->addWidget(labelIdValue, 0, 1);
    gridInfo->addWidget(labelPrecoTitle, 1, 0);
    gridInfo->addWidget(labelPrecoValue, 1, 1);
    gridInfo->addWidget(labelQtdTitle, 2, 0);
    gridInfo->addWidget(labelQtdValue, 2, 1);
    gridInfo->addWidget(labelEstMinimoTitle, 3, 0);
    gridInfo->addWidget(labelEstMinimoValue, 3, 1);
    
    cardLayout->addLayout(gridInfo);
    
    // Linha separadora
    QFrame *linha = new QFrame();
    linha->setFrameShape(QFrame::HLine);
    linha->setStyleSheet("background-color: #ecf0f1;");
    linha->setFixedHeight(2);
    cardLayout->addWidget(linha);
    
    // Status com badge
    QHBoxLayout *statusLayout = new QHBoxLayout();
    statusLayout->setSpacing(10);
    
    QLabel *labelStatusTitle = new QLabel("Status:");
    labelStatusTitle->setFont(fontInfo);
    labelStatusTitle->setStyleSheet("color: #7f8c8d; font-weight: bold;");
    
    QString statusText;
    QString statusColor;
    QString statusBg;
    
    if (produto.getQuantidade() == 0) {
        statusText = "ESGOTADO";
        statusColor = "white";
        statusBg = "#e74c3c";
    } else if (produto.getQuantidade() <= produto.getEstoqueMinimo()) {
        statusText = "ESTOQUE BAIXO";
        statusColor = "white";
        statusBg = "#f39c12";
    } else {
        statusText = "DISPONÍVEL";
        statusColor = "white";
        statusBg = "#27ae60";
    }
    
    QLabel *labelStatusBadge = new QLabel(statusText);
    labelStatusBadge->setFont(fontInfo);
    labelStatusBadge->setStyleSheet(
        QString("background-color: %1; color: %2; padding: 6px 15px; border-radius: 15px; font-weight: bold;")
        .arg(statusBg, statusColor)
    );
    
    statusLayout->addWidget(labelStatusTitle);
    statusLayout->addWidget(labelStatusBadge);
    statusLayout->addStretch();
    
    cardLayout->addLayout(statusLayout);
    
    // ========== CATEGORIA (SE EXISTIR) ==========
    if (!produto.getCategoria().isEmpty()) {
        QWidget *cardCategoria = new QWidget();
        cardCategoria->setStyleSheet("background-color: white; border-radius: 10px;");
        QVBoxLayout *catLayout = new QVBoxLayout(cardCategoria);
        catLayout->setContentsMargins(20, 15, 20, 15);
        
        QLabel *catTitle = new QLabel("📁 Categoria");
        QFont fontCatTitle = fontInfo;
        fontCatTitle.setBold(true);
        catTitle->setFont(fontCatTitle);
        catTitle->setStyleSheet("color: #34495e;");
        
        QLabel *catValue = new QLabel(produto.getCategoria());
        catValue->setFont(fontInfo);
        catValue->setStyleSheet(
            "background-color: #3498db; "
            "color: white; "
            "padding: 10px 20px; "
            "border-radius: 8px; "
            "font-weight: bold;"
        );
        catValue->setAlignment(Qt::AlignCenter);
        
        catLayout->addWidget(catTitle);
        catLayout->addWidget(catValue);
        
        layoutPrincipal->addWidget(cardCategoria);
    }
    
    // ========== DESCRIÇÃO (SE EXISTIR) ==========
    if (!produto.getDescricao().isEmpty()) {
        QWidget *cardDesc = new QWidget();
        cardDesc->setStyleSheet("background-color: white; border-radius: 10px;");
        QVBoxLayout *descLayout = new QVBoxLayout(cardDesc);
        descLayout->setContentsMargins(20, 15, 20, 15);
        
        QLabel *descTitle = new QLabel("📝 Descrição");
        QFont fontDescTitle = fontInfo;
        fontDescTitle.setBold(true);
        descTitle->setFont(fontDescTitle);
        descTitle->setStyleSheet("color: #34495e;");
        
        QLabel *descValue = new QLabel(produto.getDescricao());
        descValue->setFont(fontInfo);
        descValue->setWordWrap(true);
        descValue->setStyleSheet("color: #2c3e50; padding: 10px; line-height: 1.6;");
        
        descLayout->addWidget(descTitle);
        descLayout->addWidget(descValue);
        
        layoutPrincipal->addWidget(cardDesc);
    }
    
    // ========== BOTÃO FECHAR ==========
    QPushButton *btnFechar = new QPushButton("Fechar");
    btnFechar->setMinimumHeight(40);
    btnFechar->setMinimumWidth(150);
    btnFechar->setDefault(true);
    btnFechar->setCursor(Qt::PointingHandCursor);
    QFont fontBtn = fontInfo;
    fontBtn.setBold(true);
    btnFechar->setFont(fontBtn);
    btnFechar->setStyleSheet(
        "QPushButton { "
        "   background-color: #34495e; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 8px; "
        "   padding: 10px 30px; "
        "   font-size: 11pt; "
        "} "
        "QPushButton:hover { "
        "   background-color: #475a6e; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #2c3e50; "
        "}"
    );
    connect(btnFechar, &QPushButton::clicked, this, &QDialog::accept);
    
    QHBoxLayout *layoutBotao = new QHBoxLayout();
    layoutBotao->addStretch();
    layoutBotao->addWidget(btnFechar);
    layoutBotao->addStretch();
    
    // ========== ADICIONA TUDO AO LAYOUT PRINCIPAL ==========
    layoutPrincipal->addWidget(headerWidget);
    layoutPrincipal->addWidget(cardInfo);
    layoutPrincipal->addStretch();
    layoutPrincipal->addLayout(layoutBotao);
}

void DialogDetalhes::mostrarDetalhes(const Produto &produto, QWidget *parent)
{
    DialogDetalhes dialog(produto, parent);
    dialog.exec();
}
