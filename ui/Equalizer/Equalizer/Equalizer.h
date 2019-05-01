#pragma once

namespace Equalizer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ Equalizer
	/// </summary>
	public ref class Equalizer : public System::Windows::Forms::Form
	{
	public:
		Equalizer(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~Equalizer()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::VScrollBar^ vScrollBar1;
	private: System::Windows::Forms::VScrollBar^ vScrollBar2;
	private: System::Windows::Forms::VScrollBar^ vScrollBar3;
	private: System::Windows::Forms::VScrollBar^ vScrollBar4;
	private: System::Windows::Forms::VScrollBar^ vScrollBar5;
	private: System::Windows::Forms::VScrollBar^ vScrollBar6;
	private: System::Windows::Forms::VScrollBar^ vScrollBar7;
	private: System::Windows::Forms::VScrollBar^ vScrollBar8;
	private: System::Windows::Forms::VScrollBar^ vScrollBar9;
	private: System::Windows::Forms::VScrollBar^ vScrollBar10;
	private: System::Windows::Forms::CheckBox^ checkBox1;
	private: System::Windows::Forms::CheckBox^ checkBox2;
	private: System::Windows::Forms::CheckBox^ checkBox3;
	protected:

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->vScrollBar1 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar2 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar3 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar4 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar5 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar6 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar7 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar8 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar9 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar10 = (gcnew System::Windows::Forms::VScrollBar());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(13, 13);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(134, 38);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Open";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Equalizer::Button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(35, 149);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 67);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Play/\nPause";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Equalizer::Button2_Click);
			// 
			// vScrollBar1
			// 
			this->vScrollBar1->Location = System::Drawing::Point(189, 63);
			this->vScrollBar1->Name = L"vScrollBar1";
			this->vScrollBar1->Size = System::Drawing::Size(34, 263);
			this->vScrollBar1->TabIndex = 2;
			// 
			// vScrollBar2
			// 
			this->vScrollBar2->Location = System::Drawing::Point(248, 63);
			this->vScrollBar2->Name = L"vScrollBar2";
			this->vScrollBar2->Size = System::Drawing::Size(34, 263);
			this->vScrollBar2->TabIndex = 3;
			// 
			// vScrollBar3
			// 
			this->vScrollBar3->Location = System::Drawing::Point(304, 63);
			this->vScrollBar3->Name = L"vScrollBar3";
			this->vScrollBar3->Size = System::Drawing::Size(34, 263);
			this->vScrollBar3->TabIndex = 4;
			// 
			// vScrollBar4
			// 
			this->vScrollBar4->Location = System::Drawing::Point(364, 63);
			this->vScrollBar4->Name = L"vScrollBar4";
			this->vScrollBar4->Size = System::Drawing::Size(34, 263);
			this->vScrollBar4->TabIndex = 5;
			// 
			// vScrollBar5
			// 
			this->vScrollBar5->Location = System::Drawing::Point(422, 63);
			this->vScrollBar5->Name = L"vScrollBar5";
			this->vScrollBar5->Size = System::Drawing::Size(34, 263);
			this->vScrollBar5->TabIndex = 6;
			// 
			// vScrollBar6
			// 
			this->vScrollBar6->Location = System::Drawing::Point(480, 63);
			this->vScrollBar6->Name = L"vScrollBar6";
			this->vScrollBar6->Size = System::Drawing::Size(34, 263);
			this->vScrollBar6->TabIndex = 7;
			// 
			// vScrollBar7
			// 
			this->vScrollBar7->Location = System::Drawing::Point(540, 63);
			this->vScrollBar7->Name = L"vScrollBar7";
			this->vScrollBar7->Size = System::Drawing::Size(34, 263);
			this->vScrollBar7->TabIndex = 8;
			// 
			// vScrollBar8
			// 
			this->vScrollBar8->Location = System::Drawing::Point(601, 63);
			this->vScrollBar8->Name = L"vScrollBar8";
			this->vScrollBar8->Size = System::Drawing::Size(34, 263);
			this->vScrollBar8->TabIndex = 9;
			// 
			// vScrollBar9
			// 
			this->vScrollBar9->Location = System::Drawing::Point(664, 63);
			this->vScrollBar9->Name = L"vScrollBar9";
			this->vScrollBar9->Size = System::Drawing::Size(34, 263);
			this->vScrollBar9->TabIndex = 10;
			// 
			// vScrollBar10
			// 
			this->vScrollBar10->Location = System::Drawing::Point(726, 63);
			this->vScrollBar10->Name = L"vScrollBar10";
			this->vScrollBar10->Size = System::Drawing::Size(34, 263);
			this->vScrollBar10->TabIndex = 11;
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(895, 63);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(113, 24);
			this->checkBox1->TabIndex = 12;
			this->checkBox1->Text = L"Delay";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Equalizer::CheckBox1_CheckedChanged);
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Location = System::Drawing::Point(895, 94);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(113, 24);
			this->checkBox2->TabIndex = 13;
			this->checkBox2->Text = L"Clipping";
			this->checkBox2->UseVisualStyleBackColor = true;
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Location = System::Drawing::Point(895, 125);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(113, 24);
			this->checkBox3->TabIndex = 14;
			this->checkBox3->Text = L"SClipping";
			this->checkBox3->UseVisualStyleBackColor = true;
			// 
			// Equalizer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1427, 350);
			this->Controls->Add(this->checkBox3);
			this->Controls->Add(this->checkBox2);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->vScrollBar10);
			this->Controls->Add(this->vScrollBar9);
			this->Controls->Add(this->vScrollBar8);
			this->Controls->Add(this->vScrollBar7);
			this->Controls->Add(this->vScrollBar6);
			this->Controls->Add(this->vScrollBar5);
			this->Controls->Add(this->vScrollBar4);
			this->Controls->Add(this->vScrollBar3);
			this->Controls->Add(this->vScrollBar2);
			this->Controls->Add(this->vScrollBar1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"Equalizer";
			this->Text = L"Equalizer";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Button1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void Button2_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void CheckBox1_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	}
};
}
