#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrame* mainFrame = new MainFrame("Matter-Device-Accelerator");
     
    mainFrame->SetClientSize(320, 200);
    mainFrame->Centre();
    mainFrame->Show();
    
    return true;

}