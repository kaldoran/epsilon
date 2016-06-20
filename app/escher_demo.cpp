extern "C" {
#include <assert.h>
#include <stdlib.h>
#include <ion.h>
}
#include <escher.h>

class MyFunCell : public ChildlessView, public Responder {
public:
  MyFunCell();
  void setMessage(const char * message);
  void setEven(bool even);

  void drawRect(KDRect rect) const override;
  void setFocused(bool focused) override;
private:
  const char * m_message;
  bool m_focused;
  bool m_even;
};

MyFunCell::MyFunCell() :
  ChildlessView(),
  m_focused(false),
  m_even(false)
{
  m_message = "NULL";
}

void MyFunCell::drawRect(KDRect rect) const {
  KDColor background = m_even ? KDColorRGB(0xEE, 0xEE, 0xEE) : KDColorRGB(0x77,0x77,0x77);
  KDFillRect(rect, background);
  KDDrawString(m_message, KDPointZero, m_focused);
}

void MyFunCell::setMessage(const char * message) {
  m_message = message;
}

void MyFunCell::setFocused(bool focused) {
  m_focused = focused;
  markAsNeedingRedraw();
}

void MyFunCell::setEven(bool even) {
  m_even = even;
}

class ListController : public ViewController, public TableViewDataSource {
public:
  ListController();

  void setActiveCell(int index);

  View * view() override;
  const char * title() const override;
  bool handleEvent(ion_event_t event) override;

  int numberOfCells() override;
  void willDisplayCellForIndex(View * cell, int index) override;
  KDCoordinate cellHeight() override;
  View * reusableCell(int index) override;
  int reusableCellCount() override;
private:
  constexpr static int k_totalNumberOfModels = 20;
  constexpr static int k_maxNumberOfCells = 10;
  // !!! CAUTION: The order here is important
  // The cells should be initialized *before* the tableview!
  MyFunCell m_cells[k_maxNumberOfCells];
  TableView m_tableView;
  const char ** m_messages;
  int m_activeCell;
  KDCoordinate m_manualScrolling;
};

static const char * sMessages[] = {
  "AAA 0", "BBB 1", "CCC 2", "DDD 3", "EEE 4",
  "FFF 5", "GGG 6", "HHH 7", "III 8", "JJJ 9",
  "KKK10", "LLL11", "MMM12", "NNN13", "OOO14",
  "PPP15", "QQQ16", "RRR17", "SSS18", "TTT19"
};

ListController::ListController() :
  ViewController(),
  m_tableView(TableView(this)),
  m_activeCell(0),
  m_manualScrolling(0)
{
  m_messages = sMessages;
}

View * ListController::view() {
  return &m_tableView;
}

const char * ListController::title() const {
  return "List";
}

void ListController::setActiveCell(int index) {
  if (index < 0 || index >= k_totalNumberOfModels) {
    return;
  }

  m_activeCell = index;
  m_tableView.scrollToRow(index);
  MyFunCell * cell = (MyFunCell *)(m_tableView.cellAtIndex(index));
  cell->setParentResponder(this);
  App::runningApp()->focus(cell);
}

bool ListController::handleEvent(ion_event_t event) {
  switch (event) {
    case DOWN_ARROW:
      setActiveCell(m_activeCell+1);
      return true;
    case UP_ARROW:
      setActiveCell(m_activeCell-1);
      return true;
    case ENTER:
      m_manualScrolling += 10;
      m_tableView.setContentOffset({0, m_manualScrolling});
      return true;
    default:
      return false;
  }
}

int ListController::numberOfCells() {
  return k_totalNumberOfModels;
};

View * ListController::reusableCell(int index) {
  assert(index >= 0);
  assert(index < k_maxNumberOfCells);
  return &m_cells[index];
}

int ListController::reusableCellCount() {
  return k_maxNumberOfCells;
}

void ListController::willDisplayCellForIndex(View * cell, int index) {
  MyFunCell * myCell = (MyFunCell *)cell;
  myCell->setMessage(m_messages[index]);
  myCell->setEven(index%2 == 0);
}

KDCoordinate ListController::cellHeight() {
  return 40;
}

class GraphView : public ChildlessView {
public:
  using ChildlessView::ChildlessView;
  void drawRect(KDRect rect) const override;
};

void GraphView::drawRect(KDRect rect) const {
  KDFillRect(rect, KDColorWhite);
  KDCoordinate x_grid_step = m_frame.width/10;
  KDCoordinate y_grid_step = m_frame.height/10;
  KDColor gridColor = KDColorGray(0xEE);
  for (KDCoordinate x=rect.x; x<rect.width; x += x_grid_step) {
    KDRect verticalGridRect;
    verticalGridRect.x = x;
    verticalGridRect.y = rect.y;
    verticalGridRect.width = 1;
    verticalGridRect.height = rect.height;
    KDFillRect(verticalGridRect, gridColor);
  }
  for (KDCoordinate y=rect.y; y<rect.height; y += y_grid_step) {
    KDRect horizontalGridRect;
    horizontalGridRect.x = rect.x;
    horizontalGridRect.y = y;
    horizontalGridRect.width = rect.width;
    horizontalGridRect.height = 1;
    KDFillRect(horizontalGridRect, gridColor);
  }

  for (int i=rect.x; i<rect.width; i++) {
    KDPoint p;
    p.x = i;
    p.y = (i*i)/rect.height;
    KDSetPixel(p, KDColorRGB(0x7F, 0, 0));
  }

};

class DemoViewController : public ViewController {
public:
  DemoViewController(KDColor c);
  View * view() override;
  const char * title() const override;
  void setFocused(bool focused) override;
private:
#if 0
  //SolidColorView m_view;
  static constexpr int k_bufferSize = 100;
  char buffer[k_bufferSize];
  TextField m_view;
#else
  GraphView m_view;
#endif
};

DemoViewController::DemoViewController(KDColor c) :
  ViewController(),
  //m_view(TextField(buffer, k_bufferSize))
  //m_view(SolidColorView(c))
  m_view(GraphView())
{
  //m_view.setParentResponder(this);
}

View * DemoViewController::view() {
  return &m_view;
}

const char * DemoViewController::title() const {
  return "HELLO";
}

void DemoViewController::setFocused(bool focused) {
  /*
  if (focused) {
    App::runningApp()->focus(&m_view);
  }
  */
}

class MyTestApp : public App {
public:
  MyTestApp();
protected:
  ViewController * rootViewController() override;
private:
  DemoViewController m_demoViewController;
  ListController m_listViewController;
  TabViewController m_tabViewController;
};

MyTestApp::MyTestApp() :
  App(),
  m_demoViewController(DemoViewController(KDColorWhite)),
  m_listViewController(ListController()),
  m_tabViewController(&m_demoViewController, &m_listViewController)
{
}

ViewController * MyTestApp::rootViewController() {
  return &m_tabViewController;
}

void ion_app() {

  //KDDrawString("Hello", {0,0}, 0);

  //KDFillRect({0,0,100,100}, 0x55);
  //KDFillRect({100,100,100,100}, 0x99);

  /*
  KDCoordinate i = 0;
  char message[4] = {'F', 'O', 'O', 0};
  while (1) {
    KDPoint p = {i, i};
    KDDrawString(message, p, false);
    ion_event_t event = ion_get_event();
    if (event < 0x100) {
      message[1] = event;
    }
    i+= 1;
  }
  */

  MyTestApp myApp = MyTestApp();

  myApp.run();

  /*
  int i = 0;

  while(true) {
#if ESCHER_VIEW_LOGGING
    std::cout << window << std::endl;
#endif
    tabVC.setActiveTab(i);
    ion_event_t event = ion_get_event();
    tabVC.handleKeyEvent(event);
    i = (i+1)%2;
  }
  */



  /*
  static GraphApp app = GraphApp();
  app.run();
  */
  /*
  KDRect wholeScreen = {0, 0, 100, 100};
  KDColor a = 0x55;
  View * window = new SolidColorView(wholeScreen, a);
  KDRect textRect = {0, 0, 50, 50};
  MyTextView * textView = new MyTextView(textRect);
  window->addSubview(textView);

  //window->draw();

  while (1) {
    ion_sleep();
    textRect.y = textRect.y+1;
    textView->setFrame(textRect);
  }

  delete textView;
  delete window;
  */
}
