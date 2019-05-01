#include "Equalizer.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ arg) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Equalizer::Equalizer equalizer;
	Application::Run(% equalizer);

	return 0;
}