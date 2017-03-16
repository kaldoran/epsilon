#ifndef ESCHER_EXPRESSION_TABLE_CELL_WITH_POINTER_H
#define ESCHER_EXPRESSION_TABLE_CELL_WITH_POINTER_H

#include <escher/expression_table_cell.h>
#include <escher/i18n.h>
#include <escher/pointer_text_view.h>

class ExpressionTableCellWithPointer : public ExpressionTableCell {
public:
  ExpressionTableCellWithPointer(I18n::Message accessoryMessage = (I18n::Message)0, Layout layout = Layout::Horizontal);
  View * accessoryView() const override;
  void setHighlighted(bool highlight) override;
  void setAccessoryMessage(I18n::Message messageBody);
private:
  PointerTextView m_accessoryView;
};

#endif
