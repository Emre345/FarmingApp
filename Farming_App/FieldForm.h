#pragma once
#include "User.h"

namespace FarmingApp {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Data::SqlClient;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FieldForm
	/// </summary>
	public ref class FieldForm : public System::Windows::Forms::Form
	{
	public:
		void SetCurrentUser(User^ user) {
			currentUser = user;
		}
		FieldForm()  // Kullan�c�y� al
		{
			InitializeComponent();
			InitializeFieldGrid(); // Harita olu�turma metodunu �a��r
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FieldForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel2;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->tableLayoutPanel2 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->SuspendLayout();
			// 
			// tableLayoutPanel2
			// 
			this->tableLayoutPanel2->ColumnCount = 10;  // 10 kolon ekleniyor
			for (int i = 0; i < 10; i++) {
				this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			}
			this->tableLayoutPanel2->RowCount = 10;  // 10 sat�r ekleniyor
			for (int i = 0; i < 10; i++) {
				this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			}
			this->tableLayoutPanel2->Location = System::Drawing::Point(50, 50);
			this->tableLayoutPanel2->Name = L"tableLayoutPanel2";
			this->tableLayoutPanel2->Size = System::Drawing::Size(500, 500);  // 10x10 alan i�in boyut
			this->tableLayoutPanel2->TabIndex = 0;
			this->tableLayoutPanel2->Padding = System::Windows::Forms::Padding(0);
			this->tableLayoutPanel2->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::None;
			// 
			// FieldForm
			// 
			this->ClientSize = System::Drawing::Size(614, 606);  // Form boyutunu ayarla
			this->Controls->Add(this->tableLayoutPanel2);
			this->Name = L"FieldForm";
			this->Text = L"FieldForm";
			this->ResumeLayout(false);
		}
#pragma endregion

	private:
		SqlConnection^ connection = gcnew SqlConnection("Data Source=MERT;Initial Catalog=farming_system;Integrated Security=True");
		User^ currentUser; // Giri� yapan kullan�c�y� tutacak de�i�ken

		void InitializeFieldGrid() {
			int buttonIndex = 1;  // Numara ba�lat

			// SQL sorgusunu haz�rl�yoruz
			SqlCommand^ command = gcnew SqlCommand("SELECT field_parcel, farmers_id FROM field", connection);
			connection->Open();
			SqlDataReader^ reader = command->ExecuteReader();

			// Veritaban�ndaki verileri okuyoruz
			while (reader->Read()) {
				// field_parcel ve farmers_id de�erlerini al�yoruz
				int fieldParcel = reader->GetInt32(0);
				bool isFarmersIdNull = reader->IsDBNull(1);

				// Panel olu�tur
				Panel^ panel = gcnew Panel();
				panel->Size = System::Drawing::Size(50, 50);  // Panel boyutu 50x50

				// Buton olu�tur
				Button^ button = gcnew Button();
				button->Size = System::Drawing::Size(50, 50);  // 50x50 boyut
				button->Margin = System::Windows::Forms::Padding(0);  // Bo�luklar� s�f�rla
				button->Font = gcnew System::Drawing::Font(L"Dubai", 10, FontStyle::Bold);
				button->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
				button->TextAlign = ContentAlignment::MiddleCenter;

				// Her buton i�in numara ayarla
				button->Text = fieldParcel.ToString();
				button->ForeColor = System::Drawing::Color::White;

				// farmers_id null m� kontrol et
				if (isFarmersIdNull) {
					button->BackColor = System::Drawing::Color::Green;  // Null ise ye�il
				}
				else {
					button->BackColor = System::Drawing::Color::Red;    // Null de�ilse k�rm�z�
				}

				// Buton ve etiketi panelin i�ine ekle
				panel->Controls->Add(button);

				// Buton t�klama olay�n� ekle
				button->Click += gcnew EventHandler(this, &FieldForm::OnFieldButtonClick);

				// Paneli tableLayoutPanel'e ekle
				this->tableLayoutPanel2->Controls->Add(panel, (buttonIndex - 1) % 10, (buttonIndex - 1) / 10);  // Row ve Column hesapla

				buttonIndex++;  // Numara artacak
			}

			// Veritaban� ba�lant�s�n� kapat
			connection->Close();
		}

		void OnFieldButtonClick(Object^ sender, EventArgs^ e) {
			if (currentUser == nullptr) {
				MessageBox::Show("User not set.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}

			// Butonun ba�l� oldu�u paneli buluyoruz
			Button^ clickedButton = dynamic_cast<Button^>(sender);
			if (clickedButton != nullptr) {
				// E�er butonun arka plan rengi k�rm�z�ysa, kullan�c�ya hata mesaj� g�ster
				if (clickedButton->BackColor == System::Drawing::Color::Red) {
					MessageBox::Show("This field is occupied.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					return;  // E�er alan doluysa i�lem yap�lmas�n
				}

				int fieldParcel = Int32::Parse(clickedButton->Text);  // Butonun �zerinde yazan field_parcel numaras�n� al�yoruz

				// farmers_id'yi field tablosunda g�ncelle
				UpdateFarmersIdInField(fieldParcel);

				// Butonlar� tekrar y�kle
				ReloadFieldGrid();
			}
		}


		// Buton t�klama olay�
		void UpdateFarmersIdInField(int fieldParcel) {
			try {
				// SQL ba�lant�s�n� a��yoruz
				connection->Open();

				// SQL komutunu haz�rl�yoruz
				SqlCommand^ updateCommand = gcnew SqlCommand("UPDATE field SET farmers_id = @farmersId WHERE field_parcel = @fieldParcel", connection);

				// Parametreleri ekliyoruz
				updateCommand->Parameters->AddWithValue("@farmersId", currentUser->id);
				updateCommand->Parameters->AddWithValue("@fieldParcel", fieldParcel);

				// Komutu �al��t�r�yoruz
				int rowsAffected = updateCommand->ExecuteNonQuery();

				if (rowsAffected > 0) {
					MessageBox::Show("Field updated successfully!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
				}
				else {
					MessageBox::Show("Field update failed!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
			}
			catch (Exception^ ex) {
				MessageBox::Show("An error occurred: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			finally {
				connection->Close();
			}
		}

		void ReloadFieldGrid() {
			// Mevcut butonlar� temizleyip, veritaban�ndan tekrar alarak butonlar� yeniden y�kleyelim
			this->tableLayoutPanel2->Controls->Clear();
			InitializeFieldGrid();  // Yeni verilerle butonlar� yeniden y�kle
		}
	};
}
