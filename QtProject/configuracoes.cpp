#include "configuracoes.h"
#include <QDir>
#include <QDebug>

Configuracoes* Configuracoes::instancia = nullptr;

Configuracoes::Configuracoes()
{
    QDir dir("./BancoDeDados");
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    settings = new QSettings("./BancoDeDados/config.ini", QSettings::IniFormat);
    carregar();
}

Configuracoes::~Configuracoes()
{
    salvar();
    delete settings;
}

Configuracoes* Configuracoes::getInstance()
{
    if (instancia == nullptr) {
        instancia = new Configuracoes();
    }
    return instancia;
}

void Configuracoes::carregar()
{
    tema = settings->value("Interface/tema", "claro").toString();
    moeda = settings->value("Formato/moeda", "BRL").toString();
    idioma = settings->value("Interface/idioma", "PT").toString();
    
    qDebug() << "Configurações carregadas - Tema:" << tema << "Moeda:" << moeda << "Idioma:" << idioma;
}

void Configuracoes::salvar()
{
    settings->setValue("Interface/tema", tema);
    settings->setValue("Formato/moeda", moeda);
    settings->setValue("Interface/idioma", idioma);
    settings->sync();
    
    qDebug() << "Configurações salvas";
}

void Configuracoes::setTema(const QString &t)
{
    tema = t;
    salvar();
}

void Configuracoes::setMoeda(const QString &m)
{
    moeda = m;
    salvar();
}

void Configuracoes::setIdioma(const QString &i)
{
    idioma = i;
    salvar();
}

QString Configuracoes::getSimboloMoeda() const
{
    if (moeda == "BRL") return "R$";
    if (moeda == "USD") return "$";
    if (moeda == "EUR") return "€";
    if (moeda == "GBP") return "£";
    return "R$";
}

QString Configuracoes::traduzir(const QString &chave) const
{
    // Dicionário de traduções
    static QMap<QString, QMap<QString, QString>> traducoes = {
        // Botões e ações
        {"voltar", {{"PT", "Voltar"}, {"EN", "Back"}, {"ES", "Volver"}}},
        {"adicionar", {{"PT", "Adicionar"}, {"EN", "Add"}, {"ES", "Agregar"}}},
        {"editar", {{"PT", "Editar"}, {"EN", "Edit"}, {"ES", "Editar"}}},
        {"excluir", {{"PT", "Excluir"}, {"EN", "Delete"}, {"ES", "Eliminar"}}},
        {"detalhes", {{"PT", "Detalhes"}, {"EN", "Details"}, {"ES", "Detalles"}}},
        {"atualizar", {{"PT", "Atualizar"}, {"EN", "Refresh"}, {"ES", "Actualizar"}}},
        {"salvar", {{"PT", "Salvar"}, {"EN", "Save"}, {"ES", "Guardar"}}},
        {"cancelar", {{"PT", "Cancelar"}, {"EN", "Cancel"}, {"ES", "Cancelar"}}},
        
        // Campos
        {"nome", {{"PT", "Nome"}, {"EN", "Name"}, {"ES", "Nombre"}}},
        {"preco", {{"PT", "Preço"}, {"EN", "Price"}, {"ES", "Precio"}}},
        {"quantidade", {{"PT", "Quantidade"}, {"EN", "Quantity"}, {"ES", "Cantidad"}}},
        {"estoque_minimo", {{"PT", "Estoque Mínimo"}, {"EN", "Minimum Stock"}, {"ES", "Stock Mínimo"}}},
        {"categoria", {{"PT", "Categoria"}, {"EN", "Category"}, {"ES", "Categoría"}}},
        {"descricao", {{"PT", "Descrição"}, {"EN", "Description"}, {"ES", "Descripción"}}},
        {"estoque", {{"PT", "Estoque"}, {"EN", "Stock"}, {"ES", "Inventario"}}},
        
        // Status
        {"esgotado", {{"PT", "ESGOTADO"}, {"EN", "OUT OF STOCK"}, {"ES", "AGOTADO"}}},
        {"estoque_baixo", {{"PT", "ESTOQUE BAIXO"}, {"EN", "LOW STOCK"}, {"ES", "STOCK BAJO"}}},
        {"disponivel", {{"PT", "DISPONÍVEL"}, {"EN", "AVAILABLE"}, {"ES", "DISPONIBLE"}}},
        
        // Menus
        {"arquivo", {{"PT", "Arquivo"}, {"EN", "File"}, {"ES", "Archivo"}}},
        {"exportar_csv", {{"PT", "Exportar para CSV"}, {"EN", "Export to CSV"}, {"ES", "Exportar a CSV"}}},
        {"importar_csv", {{"PT", "Importar de CSV"}, {"EN", "Import from CSV"}, {"ES", "Importar desde CSV"}}},
        {"backup", {{"PT", "Fazer Backup"}, {"EN", "Make Backup"}, {"ES", "Hacer Backup"}}},
        {"sair", {{"PT", "Sair"}, {"EN", "Exit"}, {"ES", "Salir"}}},
        {"relatorios", {{"PT", "Relatórios"}, {"EN", "Reports"}, {"ES", "Informes"}}},
        {"imprimir_inventario", {{"PT", "Imprimir"}, {"EN", "Print"}, {"ES", "Imprimir"}}},
        {"configuracoes", {{"PT", "Configurações"}, {"EN", "Settings"}, {"ES", "Configuración"}}},
        {"preferencias", {{"PT", "Preferências"}, {"EN", "Preferences"}, {"ES", "Preferencias"}}},
        
        // Configurações
        {"tema", {{"PT", "Tema"}, {"EN", "Theme"}, {"ES", "Tema"}}},
        {"tema_claro", {{"PT", "Tema Claro"}, {"EN", "Light Theme"}, {"ES", "Tema Claro"}}},
        {"tema_escuro", {{"PT", "Tema Escuro"}, {"EN", "Dark Theme"}, {"ES", "Tema Oscuro"}}},
        {"tema_interface", {{"PT", "Tema da Interface"}, {"EN", "Interface Theme"}, {"ES", "Tema de la Interfaz"}}},
        {"moeda", {{"PT", "Moeda"}, {"EN", "Currency"}, {"ES", "Moneda"}}},
        {"idioma", {{"PT", "Idioma"}, {"EN", "Language"}, {"ES", "Idioma"}}},
        {"formatacao_localizacao", {{"PT", "Formatação e Localização"}, {"EN", "Formatting and Localization"}, {"ES", "Formato y Localización"}}},
        {"config_sistema", {{"PT", "Configurações do Sistema"}, {"EN", "System Settings"}, {"ES", "Configuración del Sistema"}}},
        {"aviso_reiniciar", {{"PT", "Algumas alterações podem requerer reiniciar o programa."}, {"EN", "Some changes may require restarting the program."}, {"ES", "Algunos cambios pueden requerir reiniciar el programa."}}},
        {"sucesso", {{"PT", "Sucesso"}, {"EN", "Success"}, {"ES", "Éxito"}}},
        {"config_salvas", {{"PT", "Configurações salvas com sucesso!\n\nAlgumas alterações serão aplicadas ao reabrir as janelas."}, {"EN", "Settings saved successfully!\n\nSome changes will be applied when reopening windows."}, {"ES", "¡Configuración guardada exitosamente!\n\nAlgunos cambios se aplicarán al reabrir las ventanas."}}},
        
        // Tela de Estoques
        {"meus_estoques", {{"PT", "Meus Estoques"}, {"EN", "My Stocks"}, {"ES", "Mis Inventarios"}}},
        {"info_estoques", {{"PT", "Clique em um estoque para abrir ou crie um novo"}, {"EN", "Click on a stock to open or create a new one"}, {"ES", "Haga clic en un inventario para abrir o crear uno nuevo"}}},
        {"novo_estoque", {{"PT", "Novo Estoque"}, {"EN", "New Stock"}, {"ES", "Nuevo Inventario"}}},
        {"excluir_estoque", {{"PT", "Excluir Estoque"}, {"EN", "Delete Stock"}, {"ES", "Eliminar Inventario"}}},
        {"nenhum_estoque", {{"PT", "Nenhum estoque criado. Clique em 'Novo Estoque' para começar!"}, {"EN", "No stock created. Click on 'New Stock' to start!"}, {"ES", "No hay inventario creado. ¡Haga clic en 'Nuevo Inventario' para comenzar!"}}},
        {"estoques_disponiveis", {{"PT", "%1 estoque(s) disponível(is). Clique duas vezes para abrir."}, {"EN", "%1 stock(s) available. Double click to open."}, {"ES", "%1 inventario(s) disponible(s). Haga doble clic para abrir."}}},
        {"total_estoques", {{"PT", "Total de estoques: %1"}, {"EN", "Total stocks: %1"}, {"ES", "Total de inventarios: %1"}}},
        {"nome_estoque", {{"PT", "Nome do estoque:"}, {"EN", "Stock name:"}, {"ES", "Nombre del inventario:"}}},
        {"descricao_opcional", {{"PT", "Descrição (opcional):"}, {"EN", "Description (optional):"}, {"ES", "Descripción (opcional):"}}},
        {"estoque_criado", {{"PT", "Estoque criado com sucesso!"}, {"EN", "Stock created successfully!"}, {"ES", "¡Inventario creado exitosamente!"}}},
        {"erro", {{"PT", "Erro"}, {"EN", "Error"}, {"ES", "Error"}}},
        {"erro_criar_estoque", {{"PT", "Erro ao criar estoque!"}, {"EN", "Error creating stock!"}, {"ES", "¡Error al crear inventario!"}}},
        {"atencao", {{"PT", "Atenção"}, {"EN", "Warning"}, {"ES", "Atención"}}},
        {"selecione_estoque_excluir", {{"PT", "Selecione um estoque para excluir!"}, {"EN", "Select a stock to delete!"}, {"ES", "¡Seleccione un inventario para eliminar!"}}},
        {"confirmar_exclusao", {{"PT", "Confirmar Exclusão"}, {"EN", "Confirm Deletion"}, {"ES", "Confirmar Eliminación"}}},
        {"msg_confirmar_exclusao", {{"PT", "Deseja realmente excluir o estoque '%1'?\n\nATENÇÃO: Todos os produtos deste estoque também serão excluídos!"}, {"EN", "Do you really want to delete the stock '%1'?\n\nWARNING: All products in this stock will also be deleted!"}, {"ES", "¿Realmente desea eliminar el inventario '%1'?\n\n¡ATENCIÓN: Todos los productos de este inventario también serán eliminados!"}}},
        {"estoque_excluido", {{"PT", "Estoque excluído com sucesso!"}, {"EN", "Stock deleted successfully!"}, {"ES", "¡Inventario eliminado exitosamente!"}}},
        {"erro_excluir_estoque", {{"PT", "Erro ao excluir estoque!"}, {"EN", "Error deleting stock!"}, {"ES", "¡Error al eliminar inventario!"}}}
    };
    
    if (traducoes.contains(chave) && traducoes[chave].contains(idioma)) {
        return traducoes[chave][idioma];
    }
    
    // Retorna a chave se não encontrar tradução
    return chave;
}
