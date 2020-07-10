//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------

USEFORM("TFrmMain.cpp", FrmMain);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
    Application->Initialize();
    Application->MainFormOnTaskBar = true;
    Application->ShowHint = true;
		Application->CreateForm(__classid(TFrmMain), &FrmMain);
     Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception(_T("Unknown exception"));
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
