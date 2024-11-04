#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/vlbox.h>
#include "Server.h"
#include <string>
#include <wx/listctrl.h>
 #include <wiringPi.h>
 #define KEY1_PIN 23
 #define KEY2_PIN 24

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	wiringPiSetupGpio();
        pinMode(KEY1_PIN, INPUT);
        pinMode(KEY2_PIN, INPUT);
  	 pullUpDnControl(KEY1_PIN, PUD_UP);
        pullUpDnControl(KEY2_PIN, PUD_UP);


    panel->SetBackgroundColour(wxColour(241, 250, 238));

    listCtrl = new wxListCtrl(panel, wxID_ANY,  wxPoint(155, 0), wxSize(165, 240), wxLC_REPORT);
    wxImageList* imageList = new wxImageList(32, 32, true);
    imageList->Add(wxBitmap("Temp.png", wxBITMAP_TYPE_PNG));
    imageList->Add(wxBitmap("Hum.png", wxBITMAP_TYPE_PNG));
     imageList->Add(wxBitmap("Sun.png", wxBITMAP_TYPE_PNG));
    imageList->Add(wxBitmap("Motion.png", wxBITMAP_TYPE_PNG));
   
    wxFont font = listCtrl->GetFont();
    font.SetPointSize(16);   
    listCtrl->SetFont(font);

    listCtrl->AssignImageList(imageList, wxIMAGE_LIST_SMALL);
    listCtrl->InsertColumn(0, "", wxLIST_FORMAT_LEFT, 165);

    wxListItem item1;
    item1.SetId(0);      
    item1.SetImage(0);             
    listCtrl->InsertItem(item1); 

    wxListItem item2;
    item2.SetId(1);    
    item2.SetImage(1);            
    listCtrl->InsertItem(item2); 

    wxListItem item3;
    item3.SetId(2);       
    item3.SetImage(2);               
    listCtrl->InsertItem(item3);

    wxListItem item4;
    item4.SetId(3);       
    item4.SetImage(3);               
    listCtrl->InsertItem(item4);
    
    checkBox = new wxCheckBox(panel, wxID_ANY, "Motion-activated", wxPoint(7, 210));
    checkBox->Bind(wxEVT_CHECKBOX, &MainFrame::OnCheckboxToggle, this);

    wxPNGHandler *handler = new wxPNGHandler;
    wxImage::AddHandler(handler);

    bulbOff = new wxBitmapButton( panel, wxID_ANY, wxBitmap("Off.png", wxBITMAP_TYPE_PNG), wxPoint(27,23), wxDefaultSize, wxNO_BORDER);
    bulbOn = new wxBitmapButton( panel, wxID_ANY, wxBitmap("On.png", wxBITMAP_TYPE_PNG), wxPoint(27,23), wxDefaultSize, wxNO_BORDER);
    bulbOn->Hide();
    bulbOff->SetBackgroundColour(wxColour(241, 250, 238));
    bulbOn->SetBackgroundColour(wxColour(241, 250, 238));
    bulbOff->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    bulbOn->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);

   StartServer();
   #ifdef __aarch64__
 ShowFullScreen(true, wxFULLSCREEN_NOBORDER);
  #endif
}   


void MainFrame::OnButtonClicked(wxCommandEvent& evt)
{
     buttonClicked = true;
}	
void MainFrame::OnTimer(wxTimerEvent& event){
       
      
      if (digitalRead(KEY1_PIN) == LOW) 
      {
         buttonClicked = true;
      }
      if (digitalRead(KEY2_PIN) == LOW) 
      {
           bool newValue = !checkBox->GetValue();
  checkBox->SetValue(newValue);
  wxCommandEvent event(wxEVT_CHECKBOX, checkBox->GetId());
  event.SetInt(newValue); 
  wxPostEvent(checkBox, event); 
      }
    RunServer(); 
}


void MainFrame::StartServer()
{
    initializeServer();
    clientSocket = acceptClient();
    dataFromSensors = new DataFromSensors;
    bulbInfo = new BulbInfo;
    timer = new wxTimer(this);
    Bind(wxEVT_TIMER, &MainFrame::OnTimer, this); 
    timer->Start(500);  
}
void MainFrame::RunServer()
{
  bulbInfo->buttonClicked=buttonClicked;
  bulbInfo->motionAcrivatedMode = motionAcrivatedMode;
  sendBulbInfoToClient(clientSocket, *bulbInfo);
  buttonClicked =false;
  receiveData(clientSocket, *dataFromSensors);
  update(*dataFromSensors);
        
        
}

void MainFrame::OnCheckboxToggle(wxCommandEvent& event)
{
    if (checkBox->IsChecked())
    {
        motionAcrivatedMode = true;
    }
    else
    {
        motionAcrivatedMode = false;
    }
}

void MainFrame::update(DataFromSensors &DataFromSensors){

    if(dataFromSensors->bulb)turnOnBulb();    
    else turnOffBulb();

     char temperatureString[256];
     snprintf(temperatureString, sizeof(temperatureString), "%d", dataFromSensors->temperature);
     listCtrl->SetItem(0, 0, wxString::Format("%s %cC", temperatureString, 0x00B0)); 
 
    
     char humidityString[256];
     snprintf(humidityString, sizeof(humidityString), "%d", dataFromSensors->humidity);
     listCtrl->SetItem(1, 0, wxString::Format("%s %c", humidityString, 0x0025));
     char illuminanceString[256];
     snprintf(illuminanceString, sizeof(illuminanceString), "%d", dataFromSensors->illuminance);
     listCtrl->SetItem(2, 0, wxString::Format("%s lux", illuminanceString));
     
     if(dataFromSensors->motion)
     {
        listCtrl->SetItem(3, 0, "Detected");
     } else { 
        listCtrl->SetItem(3, 0, " - ");
     }

}

void MainFrame::turnOnBulb()
{
    if(!bulbOn->IsShown())
    {
    bulbOff->Hide();
    bulbOn->Show();
    }
}

void MainFrame::turnOffBulb()
{
    if(bulbOn->IsShown())
    {
    bulbOn->Hide();
    bulbOff->Show();
    }
}

bool MainFrame::GetBulbInfo()
{
    if(bulbOn->IsShown())return true;
    else return false;
}
