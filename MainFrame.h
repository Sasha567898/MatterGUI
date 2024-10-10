#pragma once 
#include <wx/wx.h>
#include <thread>
#include "Server.h"
#include <wx/listctrl.h>


class MainFrame : public wxFrame
{
    public:
        MainFrame(const wxString& title);
    private:
    
        void update(DataFromSensors &DataFromSensors);
        void turnOnBulb();
        void turnOffBulb();
        bool GetBulbInfo();
        void OnCheckboxToggle(wxCommandEvent& event);
        void OnTimer(wxTimerEvent& event);
        void OnButtonClicked(wxCommandEvent& evt);  
        void StartServer();
        void RunServer();

        wxPanel *panel = new wxPanel(this);
        wxListCtrl* listCtrl;
        DataFromSensors *dataFromSensors;
        BulbInfo *bulbInfo;
        bool buttonClicked = false;
        bool motionAcrivatedMode = false;
        wxBitmapButton *bulbOn;
        wxBitmapButton *bulbOff;
        wxCheckBox * checkBox;
        wxTimer* timer;
        int clientSocket;
   
};