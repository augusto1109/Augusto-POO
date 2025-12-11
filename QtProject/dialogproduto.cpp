#include "dialogproduto.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QLabel>

DialogProduto::DialogProduto(QWidget *parent)
    : QDialog(parent), modoEdicao(false)
{
    setWindowTitle("Cadastro de Produto");
    resize(500, 450);
    criarInterface();
}

void DialogProduto::criarInterface()
{
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);
    
    // Título
    QLabel *titulo = new QLabel("Dados do Produto");
    QFont fontTitulo = titulo->font();
    fontTitulo.setPointSize(14);
    fontTitulo.setBold(true);
    titulo->setFont(fontTitulo);
    
    // Formulário
    QFormLayout *formLayout = new QFormLayout();
    
    lineEditNome = new QLineEdit();
    lineEditNome->setPlaceholderText("Digite o nome do produto");
    
    spinBoxPreco = new QDoubleSpinBox();
    spinBoxPreco->setPrefix("R$ ");
    spinBoxPreco->setMaximum(999999.99);
    spinBoxPreco->setDecimals(2);
    
    spinBoxQuantidade = new QSpinBox();
    spinBoxQuantidade->setMaximum(999999);
    spinBoxQuantidade->setSuffix(" un");
    
    spinBoxEstoqueMinimo = new QSpinBox();
    spinBoxEstoqueMinimo->setMaximum(999999);
    spinBoxEstoqueMinimo->setValue(5); // Valor padrão
    spinBoxEstoqueMinimo->setSuffix(" un");
    spinBoxEstoqueMinimo->setToolTip("Quantidade mínima para alerta de estoque baixo");
    
    lineEditCategoria = new QLineEdit();
    lineEditCategoria->setPlaceholderText("Ex: Eletrônicos, Alimentos...");
    
    textEditDescricao = new QTextEdit();
    textEditDescricao->setPlaceholderText("Descrição detalhada do produto");
    textEditDescricao->setMaximumHeight(120);
    
    formLayout->addRow("* Nome:", lineEditNome);
    formLayout->addRow("* Preço:", spinBoxPreco);
    formLayout->addRow("* Quantidade:", spinBoxQuantidade);
    formLayout->addRow("Estoque Mínimo:", spinBoxEstoqueMinimo);
    formLayout->addRow("Categoria:", lineEditCategoria);
    formLayout->addRow("Descrição:", textEditDescricao);
    
    QLabel *labelObrigatorio = new QLabel("* Campos obrigatórios");
    labelObrigatorio->setStyleSheet("color: gray; font-size: 10pt;");
    
    // Botões
    QHBoxLayout *layoutBotoes = new QHBoxLayout();
    
    btnCancelar = new QPushButton("Cancelar");
    btnSalvar = new QPushButton("Salvar");
    btnSalvar->setDefault(true);
    
    layoutBotoes->addStretch();
    layoutBotoes->addWidget(btnCancelar);
    layoutBotoes->addWidget(btnSalvar);
    
    // Adiciona tudo ao layout principal
    layoutPrincipal->addWidget(titulo);
    layoutPrincipal->addLayout(formLayout);
    layoutPrincipal->addWidget(labelObrigatorio);
    layoutPrincipal->addStretch();
    layoutPrincipal->addLayout(layoutBotoes);
    
    // Conecta sinais
    connect(btnSalvar, &QPushButton::clicked, this, &DialogProduto::onSalvar);
    connect(btnCancelar, &QPushButton::clicked, this, &DialogProduto::onCancelar);
    connect(lineEditNome, &QLineEdit::textChanged, this, &DialogProduto::validarCampos);
    connect(spinBoxPreco, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &DialogProduto::validarCampos);
}

void DialogProduto::validarCampos()
{
    bool valido = !lineEditNome->text().trimmed().isEmpty() && spinBoxPreco->value() > 0;
    btnSalvar->setEnabled(valido);
}
void DialogProduto::setProduto(const Produto &produto)
{
    produtoAtual = produto;
    modoEdicao = true;
    
    lineEditNome->setText(produto.getNome());
    spinBoxPreco->setValue(produto.getPreco());
    spinBoxQuantidade->setValue(produto.getQuantidade());
    spinBoxEstoqueMinimo->setValue(produto.getEstoqueMinimo());
    lineEditCategoria->setText(produto.getCategoria());
    textEditDescricao->setText(produto.getDescricao());
    
    setWindowTitle("Editar Produto");
}

void DialogProduto::onSalvar()
{
    if (lineEditNome->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Atenção", "O nome do produto é obrigatório!");
        lineEditNome->setFocus();
        return;
    }
    
    if (spinBoxPreco->value() <= 0) {
        QMessageBox::warning(this, "Atenção", "O preço deve ser maior que zero!");
        spinBoxPreco->setFocus();
        return;
    }
    produtoAtual.setNome(lineEditNome->text().trimmed());
    produtoAtual.setPreco(spinBoxPreco->value());
    produtoAtual.setQuantidade(spinBoxQuantidade->value());
    produtoAtual.setEstoqueMinimo(spinBoxEstoqueMinimo->value());
    produtoAtual.setCategoria(lineEditCategoria->text().trimmed());
    produtoAtual.setDescricao(textEditDescricao->toPlainText().trimmed());
    
    accept();
    accept();
}

void DialogProduto::onCancelar()
{
    reject();
}

Produto DialogProduto::novoProduto(int proximoId, QWidget *parent)
{
    DialogProduto dialog(parent);
    Produto produto;
    produto.setId(proximoId);
    dialog.produtoAtual = produto;
    
    if (dialog.exec() == QDialog::Accepted) {
        return dialog.produtoAtual;
    }
    
    return Produto(); // Retorna produto inválido
}

bool DialogProduto::editarProduto(Produto &produto, QWidget *parent)
{
    DialogProduto dialog(parent);
    dialog.setProduto(produto);
    
    if (dialog.exec() == QDialog::Accepted) {
        produto = dialog.produtoAtual;
        return true;
    }
    
    return false;
}
