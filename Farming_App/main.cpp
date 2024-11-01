#include "LoginForm.h"
#include "RegisterForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Giri� Formu
    FarmingApp::LoginForm loginForm;
    //Application::Run(% loginForm);

    // Kay�t Formu
    FarmingApp::RegisterForm registerForm;
    Application::Run(% registerForm);

    return 0;
}
