#include "FormSwitch.h"
#include "RegisterForm.h"

namespace FarmingApp {

    void FormSwitch::SwitchForm(System::Windows::Forms::Form^ currentForm, System::Windows::Forms::Form^ nextForm)
    {
        currentForm->Hide();        // Ge�erli formu gizle
        nextForm->Show();           // Yeni formu g�ster
        nextForm->BringToFront();   // Yeni formu �n plana getir
    }
}