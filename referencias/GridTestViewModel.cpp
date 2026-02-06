#include "GridTestViewModel.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace DualComponents::TestHelpers {

// Helper: String format doubles
std::string formatMoney(double val) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(2) << val;
  return ss.str();
}

// Helper: Current Date String
std::string currentDate() {
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
#if defined(_MSC_VER)
  localtime_s(&tstruct, &now);
#else
  tstruct = *localtime(&now);
#endif
  strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
  return std::string(buf);
}

GridTestViewModel::GridTestViewModel() {
  // Init properties via macro-generated members (which are typically _grid,
  // _editQty etc see DECLARE_CONTROL_BINDING)
  // DECLARE_CONTROL_BINDING(GRD_SIMPLE, Grid) ->
  // std::shared_ptr<DcGridViewModel> _grid; The macro name 'GRD_SIMPLE' maps to
  // 'grid', so member is '_grid'.

  _grid = DcGridViewModel::create();

  _editQty = std::make_shared<DcNumberFieldViewModel>();
  _editArticle = std::make_shared<DcComboBoxViewModel>();
  _editPrice = std::make_shared<DcNumberFieldViewModel>();
  _editDelivered = std::make_shared<DcCheckViewModel>();
  _editDate = std::make_shared<DcDateViewModel>();

  _cmdSave = std::make_shared<DcCommandViewModel>();
  _cmdCancel = std::make_shared<DcCommandViewModel>();
  _lblTotal = std::make_shared<DcControlViewModel>();

  setup();
}

GridTestViewModel::~GridTestViewModel() {}

std::shared_ptr<GridTestViewModel> GridTestViewModel::create() {
  return std::make_shared<GridTestViewModel>();
}

void GridTestViewModel::setup() {
  setupGrid();
  configureEditors();
  setupEditingLogic();

  // Initial Data
  auto item1 = std::make_shared<InvoiceItem>();
  item1->cantidad = 1;
  item1->descripcion = "CPU";
  item1->precio = 350.0;
  item1->entregado = true;
  item1->fechaEntrega = currentDate();
  _items.push_back(item1);

  auto item2 = std::make_shared<InvoiceItem>();
  item2->cantidad = 2;
  item2->descripcion = "RAM 8GB";
  item2->precio = 45.0;
  item2->entregado = false;
  item2->fechaEntrega = currentDate();
  _items.push_back(item2);

  refreshRows();
}

void GridTestViewModel::setupGrid() {
  std::vector<DcGridColumn> cols;
  cols.push_back(DcGridColumn("#", 3, DcTextAlign::Center));
  cols.push_back(
      DcGridColumn("Cant", 5, DcTextAlign::Center, DcColumnType::Number));
  cols.push_back(DcGridColumn("Descripción", 20, DcTextAlign::Left,
                              DcColumnType::ComboBox));
  cols.push_back(
      DcGridColumn("Precio", 10, DcTextAlign::Right, DcColumnType::Number));
  cols.push_back(
      DcGridColumn("Entr?", 6, DcTextAlign::Center, DcColumnType::CheckBox));
  cols.push_back(
      DcGridColumn("F. Entrega", 14, DcTextAlign::Center, DcColumnType::Date));
  cols.push_back(DcGridColumn("Subtotal", 10, DcTextAlign::Right));
  cols.push_back(
      DcGridColumn("Op", 8, DcTextAlign::Center, DcColumnType::Actions));

  _grid->setColumns(cols);
}

void GridTestViewModel::configureEditors() {
  // Qty
  _editQty->setDecimalPlaces(0);

  // Article
  _articlesDb = {
      {"1", "CPU"}, {"2", "RAM"}, {"3", "SSD"}, {"4", "GPU"}, {"5", "Case"}};
  std::vector<DcComboBoxItem> items;
  for (auto &p : _articlesDb)
    items.push_back({p.first, p.second});
  _editArticle->setItems(items);
  _editArticle->setAllowNew(true);

  // Price
  _editPrice->setDecimalPlaces(2);

  // Date ? (Defaults ok)
}

void GridTestViewModel::setupEditingLogic() {
  // 1. Activation
  auto startEdit = [this](int index) {
    InvoiceItem *item = nullptr;
    if (index >= 0 && index < _items.size()) {
      item = _items[index].get();
    }

    // Lock Grid
    _grid->setIsInputLocked(true);
    currentEditingItem.reset();

    if (item) {
      isNewItem = false;
      _editQty->setValue(item->cantidad);
      _editArticle->selectItemByText(item->descripcion);
      _editPrice->setValue((double)item->precio);
      _editDelivered->setValue(item->entregado);
    } else {
      isNewItem = true;
      _editQty->setValue(1);
      _editArticle->selectItemByText("");
      _editPrice->setValue(0.0);
      _editDelivered->setValue(false);
    }
  };

  _grid->setOnRowActivated([this, startEdit](int index) { startEdit(index); });
  _grid->setOnAddRequested([this, startEdit]() { startEdit(-1); });

  // 2. Save
  _cmdSave->setOnExecuted([this]() {
    int qty = (int)_editQty->getValue();
    std::string desc = _editArticle->getText();
    double price = _editPrice->getValue();
    bool delivered = _editDelivered->getValue();

    if (qty <= 0)
      return;
    if (desc.empty())
      return;

    if (isNewItem) {
      auto newItem = std::make_shared<InvoiceItem>();
      newItem->cantidad = qty;
      newItem->descripcion = desc;
      newItem->precio = price;
      newItem->entregado = delivered;
      newItem->fechaEntrega = currentDate();
      _items.push_back(newItem);
    } else {
      int idx = _grid->getSelectedIndex();
      if (idx >= 0 && idx < _items.size()) {
        auto item = _items[idx];
        item->cantidad = qty;
        item->descripcion = desc;
        item->precio = price;
        item->entregado = delivered;
      }
    }

    _grid->setIsInputLocked(false);
    refreshRows();
  });

  // 3. Cancel
  _cmdCancel->setOnExecuted([this]() { _grid->setIsInputLocked(false); });
}

void GridTestViewModel::refreshRows() {
  std::vector<DcGridRow> rows;
  double total = 0;
  int idx = 1;

  for (auto &item : _items) {
    DcGridRow r;
    r.tag = item.get();
    r.cells.push_back(std::to_string(idx++));
    r.cells.push_back(std::to_string(item->cantidad));
    r.cells.push_back(item->descripcion);
    r.cells.push_back(formatMoney(item->precio));
    r.cells.push_back(item->entregado ? "[x]" : "[ ]");
    r.cells.push_back(item->fechaEntrega);
    r.cells.push_back(formatMoney(item->getSubtotal()));
    r.cells.push_back("");

    rows.push_back(r);
    total += item->getSubtotal();
  }

  // Ghost
  rows.push_back(DcGridRow::CreateGhost(8));

  _grid->setRows(rows);
  updateTotal();
}

void GridTestViewModel::updateTotal() {
  double total = 0;
  for (auto &i : _items)
    total += i->getSubtotal();
  _lblTotal->setLabelText("Total: $" + formatMoney(total));
}

// =========================================================
// Accessor Implementations (Macro-based)
// =========================================================

// Note: 'grid' is the expanded macro name for GRD_SIMPLE defined in header
// IMPLEMENT_CONTROL_INTERNAL(Namespace, ClassName, MacroName/Name, ControlType)

IMPLEMENT_CONTROL_INTERNAL(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                           GRD_SIMPLE, Grid)

IMPLEMENT_CONTROL_INTERNAL(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                           EDT_QTY, NumberField)
IMPLEMENT_CONTROL_INTERNAL(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                           EDT_ARTICLE, ComboBox)
IMPLEMENT_CONTROL_INTERNAL(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                           EDT_PRICE, NumberField)
IMPLEMENT_CONTROL_INTERNAL(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                           EDT_DELIVERED, Check)
IMPLEMENT_CONTROL_INTERNAL(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                           EDT_DATE, Date)

IMPLEMENT_CONTROL_INTERNAL(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                           LBL_TOTAL, Control)

IMPLEMENT_CONTROL_INTERNAL(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                           CMD_SAVE, Command)
IMPLEMENT_CONTROL_INTERNAL(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                           CMD_CANCEL, Command)

} // namespace

// =========================================================
// C-Bridge Function Implementations
// =========================================================

extern "C" {

IMPLEMENT_VIEWMODEL_LIFECYCLE(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL)

IMPLEMENT_CONTROL_BRIDGE(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                         GRD_SIMPLE)
IMPLEMENT_CONTROL_BRIDGE(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                         EDT_QTY)
IMPLEMENT_CONTROL_BRIDGE(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                         EDT_ARTICLE)
IMPLEMENT_CONTROL_BRIDGE(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                         EDT_PRICE)
IMPLEMENT_CONTROL_BRIDGE(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                         EDT_DELIVERED)
IMPLEMENT_CONTROL_BRIDGE(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                         EDT_DATE)
IMPLEMENT_CONTROL_BRIDGE(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                         LBL_TOTAL)
IMPLEMENT_CONTROL_BRIDGE(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                         CMD_SAVE)
IMPLEMENT_CONTROL_BRIDGE(DualComponents::TestHelpers, GRID_TEST_VIEW_MODEL,
                         CMD_CANCEL)

} // extern "C"

// Helper: Current Date String
std::string currentDate() {
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
#if defined(_MSC_VER)
  localtime_s(&tstruct, &now);
#else
  tstruct = *localtime(&now);
#endif
  strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
  return std::string(buf);
}
/*
GridTestViewModel::GridTestViewModel() {
  _grid = DcGridViewModel::create();

  _editQty = std::make_shared<DcNumberFieldViewModel>();
  _editArticle = std::make_shared<DcComboBoxViewModel>();
  _editPrice = std::make_shared<DcNumberFieldViewModel>();
  _editDelivered = std::make_shared<DcCheckViewModel>();
  _editDate = std::make_shared<DcDateViewModel>();

  _cmdSave = std::make_shared<DcCommandViewModel>();
  _cmdCancel = std::make_shared<DcCommandViewModel>();
  _lblTotal = std::make_shared<DcControlViewModel>();

  setup();
}

GridTestViewModel::~GridTestViewModel() {}

std::shared_ptr<GridTestViewModel> GridTestViewModel::create() {
  return std::make_shared<GridTestViewModel>();
}

void GridTestViewModel::setup() {
  setupGrid();
  configureEditors();
  setupEditingLogic();

  // Initial Data
  auto item1 = std::make_shared<InvoiceItem>();
  item1->cantidad = 1;
  item1->descripcion = "CPU";
  item1->precio = 350.0;
  item1->entregado = true;
  item1->fechaEntrega = currentDate();
  _items.push_back(item1);

  auto item2 = std::make_shared<InvoiceItem>();
  item2->cantidad = 2;
  item2->descripcion = "RAM 8GB";
  item2->precio = 45.0;
  item2->entregado = false;
  item2->fechaEntrega = currentDate();
  _items.push_back(item2);

  refreshRows();
}

void GridTestViewModel::setupGrid() {
  std::vector<DcGridColumn> cols;
  cols.push_back(DcGridColumn("#", 3, DcTextAlign::Center));
  cols.push_back(
      DcGridColumn("Cant", 5, DcTextAlign::Center, DcColumnType::Number));
  cols.push_back(DcGridColumn("Descripción", 20, DcTextAlign::Left,
                              DcColumnType::ComboBox));
  cols.push_back(
      DcGridColumn("Precio", 10, DcTextAlign::Right, DcColumnType::Number));
  cols.push_back(
      DcGridColumn("Entr?", 6, DcTextAlign::Center, DcColumnType::CheckBox));
  cols.push_back(
      DcGridColumn("F. Entrega", 14, DcTextAlign::Center, DcColumnType::Date));
  cols.push_back(DcGridColumn("Subtotal", 10, DcTextAlign::Right));
  cols.push_back(
      DcGridColumn("Op", 8, DcTextAlign::Center, DcColumnType::Actions));

  _grid->setColumns(cols);
}

void GridTestViewModel::configureEditors() {
  // Qty
  _editQty->setDecimalPlaces(0);

  // Article
  _articlesDb = {
      {"1", "CPU"}, {"2", "RAM"}, {"3", "SSD"}, {"4", "GPU"}, {"5", "Case"}};
  std::vector<DcComboBoxItem> items;
  for (auto &p : _articlesDb)
    items.push_back({p.first, p.second});
  _editArticle->setItems(items);
  _editArticle->setAllowNew(true);

  // Price
  _editPrice->setDecimalPlaces(2);

  // Date ? (Defaults ok)
}

void GridTestViewModel::setupEditingLogic() {
  // 1. Activation (Double Click / Enter)
  // In C# Logic: this mapped to "OnRowActivated" or "OnAddRequested"
  // Here we can use a lambda in grid

  auto startEdit = [this](int index, void *tag) {
    InvoiceItem *item = static_cast<InvoiceItem *>(tag);

    // Lock Grid
    _grid->setIsInputLocked(true);
    currentEditingItem.reset(); // clear weak-ish ref logic if needed, but here
                                // we just store ptr?
    // Wait, we need to know if we are editing NEW or EXISTING.
    // If tag is null, it's ghost row -> NEW.

    if (item) {
      isNewItem = false;
      // Populating editors
      _editQty->setValue(item->cantidad);
      _editArticle->selectItemByText(item->descripcion);
      _editPrice->setValue((int64_t)(item->precio * 100)); // int cents
      _editDelivered->setValue(item->entregado);
      // Date handling: simplified for demo, string? Or timestamp?
      // Bridge uses timestamp. Core SetValue needs double timestamp.
      // Assuming current date for simplicity or parsing logic if needed.
      // _editDate->setDate(...);
    } else {
      isNewItem = true;
      _editQty->setValue(1);
      _editArticle->selectItemByText("");
      _editPrice->setValue(0);
      _editDelivered->setValue(false);
    }
  };

  _grid->setOnRowActivated(startEdit);
  _grid->setOnAddRequested([this, startEdit]() { startEdit(-1, nullptr); });

  // 2. Save
  _cmdSave->setOnExecute([this]() {
    int qty = (int)_editQty->getValue();
    std::string desc = _editArticle->getText();
    double price = _editPrice->getValue() / 100.0;
    bool delivered = _editDelivered->getValue();

    if (qty <= 0)
      return; // Validation
    if (desc.empty())
      return;

    if (isNewItem) {
      auto newItem = std::make_shared<InvoiceItem>();
      newItem->cantidad = qty;
      newItem->descripcion = desc;
      newItem->precio = price;
      newItem->entregado = delivered;
      newItem->fechaEntrega = currentDate();
      _items.push_back(newItem);
    } else {
      // Find and update?
      // Since we rely on index activation, we assume selection didn't change
      // (locked).
      int idx = _grid->getSelectedIndex();
      if (idx >= 0 && idx < _items.size()) {
        auto item = _items[idx];
        item->cantidad = qty;
        item->descripcion = desc;
        item->precio = price;
        item->entregado = delivered;
      }
    }

    _grid->setIsInputLocked(false);
    refreshRows();
  });

  // 3. Cancel
  _cmdCancel->setOnExecute([this]() { _grid->setIsInputLocked(false); });
}

void GridTestViewModel::refreshRows() {
  std::vector<DcGridRow> rows;
  double total = 0;
  int idx = 1;

  for (auto &item : _items) {
    DcGridRow r;
    r.tag = item.get();
    r.cells.push_back(std::to_string(idx++));
    r.cells.push_back(std::to_string(item->cantidad));
    r.cells.push_back(item->descripcion);
    r.cells.push_back(formatMoney(item->precio));
    r.cells.push_back(item->entregado ? "[x]" : "[ ]");
    r.cells.push_back(item->fechaEntrega);
    r.cells.push_back(formatMoney(item->getSubtotal()));
    r.cells.push_back(""); // Actions placeholder

    rows.push_back(r);
    total += item->getSubtotal();
  }

  // Ghost
  rows.push_back(DcGridRow::CreateGhost(8));

  _grid->setRows(rows);
  updateTotal();
}

void GridTestViewModel::updateTotal() {
  double total = 0;
  for (auto &i : _items)
    total += i->getSubtotal();
  _lblTotal->setLabelText("Total: $" + formatMoney(total));
}
}
*/
