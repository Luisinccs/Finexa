#pragma once

#include "CommonMacros.hpp"
#include "DcCheckViewModel.hpp"
#include "DcComboBoxViewModel.hpp"
#include "DcCommandViewModel.hpp"
#include "DcControlViewModel.hpp"
#include "DcDateViewModel.hpp"
#include "DcGridViewModel.hpp"
#include "DcNumberFieldViewModel.hpp"
#include "DcViewModelToolkit.hpp"
#include <memory>
#include <string>
#include <vector>

// Definition of class and namespace
#define GRID_TEST_VIEW_MODEL GridTestViewModel

// Member Macros
#define GRD_SIMPLE grid
#define EDT_QTY editQty
#define EDT_ARTICLE editArticle
#define EDT_PRICE editPrice
#define EDT_DELIVERED editDelivered
#define EDT_DATE editDate
#define LBL_TOTAL lblTotal
#define CMD_SAVE cmdSave
#define CMD_CANCEL cmdCancel

using namespace DualComponents::ViewModels;

namespace DualComponents::TestHelpers {

struct InvoiceItem {
  int cantidad;
  std::string descripcion;
  double precio;
  bool entregado;
  std::string fechaEntrega; // YYYY-MM-DD for simplicity in C++ demo

  double getSubtotal() const { return cantidad * precio; }
};

class GridTestViewModel {
public:
  GridTestViewModel();
  virtual ~GridTestViewModel();
  static std::shared_ptr<GridTestViewModel> create();

  void setup();

  // --- Declarations ---
  DECLARE_CONTROL_BINDING(GRD_SIMPLE, Grid)

  DECLARE_CONTROL_BINDING(EDT_QTY, NumberField)
  DECLARE_CONTROL_BINDING(EDT_ARTICLE, ComboBox)
  DECLARE_CONTROL_BINDING(EDT_PRICE, NumberField)
  DECLARE_CONTROL_BINDING(EDT_DELIVERED, Check)
  DECLARE_CONTROL_BINDING(EDT_DATE, Date)

  DECLARE_CONTROL_BINDING(LBL_TOTAL, Control)

  DECLARE_CONTROL_BINDING(CMD_SAVE, Command)
  DECLARE_CONTROL_BINDING(CMD_CANCEL, Command)

private:
  // Data
  std::vector<std::shared_ptr<InvoiceItem>> _items; // Keep alive tags
  std::vector<std::pair<std::string, std::string>> _articlesDb; // Key, Text

  // Helpers
  std::shared_ptr<InvoiceItem> currentEditingItem;
  bool isNewItem = false;

  void setupGrid();
  void configureEditors();
  void setupEditingLogic();
  void refreshRows();
  void updateTotal();
};

using GridTestViewModelPtr = std::shared_ptr<GridTestViewModel>;

} // namespace DualComponents::TestHelpers

// =========================================================
// C-Bridge Functions
// =========================================================

extern "C" {

DECLARE_VIEWMODEL_LIFECYCLE(GRID_TEST_VIEW_MODEL)

DECLARE_CONTROL_BRIDGE(GRD_SIMPLE, GRID_TEST_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(EDT_QTY, GRID_TEST_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(EDT_ARTICLE, GRID_TEST_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(EDT_PRICE, GRID_TEST_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(EDT_DELIVERED, GRID_TEST_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(EDT_DATE, GRID_TEST_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(LBL_TOTAL, GRID_TEST_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(CMD_SAVE, GRID_TEST_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(CMD_CANCEL, GRID_TEST_VIEW_MODEL)

} // extern "C"
