#include "main.h"
#include "mainframe.h"

IMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
	srand(time(NULL));
	wxInitAllImageHandlers();
	MainFrame* main_frame = new MainFrame("Card Game");
	main_frame->Show(true);
	return true;
}