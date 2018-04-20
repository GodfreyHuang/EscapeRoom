/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "mygame.h"
#include <string>

namespace game_framework
{
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	/////////////////////////////////////////////////////////////////////////////

	CGameStateInit::CGameStateInit(CGame* g) : CGameState(g)
	{
	
	}

	CGameStateInit::~CGameStateInit()
	{
	
	}

	void CGameStateInit::OnInit()
	{

		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(0);	// 一開始的loading進度為0%
		//
		// 開始載入資料
		//
		Background.LoadBitmap("Bitmaps\\map\\clickToStart.bmp");
		CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");

		Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateInit::OnBeginState()
	{
	
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
    
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
		CAudio::Instance()->Play(AUDIO_DING);
	}

	void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)
	{
	
	}


	void CGameStateInit::OnShow()
	{
		//
		// 貼上logo
		//
		Background.ShowBitmap();
		//
		// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
		//
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CGameStateOver::CGameStateOver(CGame* g) : CGameState(g)
	{
	
	}

	void CGameStateOver::OnMove()
	{
		counter--;

		if (counter < 0)
			GotoGameState(GAME_STATE_INIT);
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
	}

	void CGameStateOver::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
		//
		// 開始載入資料
		//
		Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 最終進度為100%
		//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnShow()
	{
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];								// Demo 數字對字串的轉換
		sprintf(str, "Game Over ! (%d)", counter / 30);
		pDC->TextOut(240, 210, str);
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame* g): CGameState(g)
	{
		digit = 0;
		mapNow = 1;
		objNow = 7;
		mapTemp = 15;
		mapMove = false;
		password7 = 1604;
		password9 = 25689;
		passwordInput = 0;
		for (int i = 0; i < MAP_SIZE; i++)
		{
			for (int j = 0; j < OBJ_SIZE; j++)
			{
				hideArr[j] = 0;
				mapArr[i][j] = new CAnimation(1);
			}
		}
		hideArr[0] = 1;
		hideArr[5] = 1;
		hideArr[7] = 1;
		for (int i = 0; i < HIDEOBJ_SIZE; i++)
			hideArr[hideObj[i]] = 1;
	}

	CGameStateRun::~CGameStateRun()
	{
		for (int i = 0; i < MAP_SIZE; i++)
		{
			for (int j = 0; j < OBJ_SIZE; j++)
				delete mapArr[i][j];
		}
	}

	void CGameStateRun::OnBeginState()
	{
		
	}

	void CGameStateRun::OnMove()	// 移動遊戲元素
	{
		for (int i = 0; i < MAP_SIZE; i++)
		{
			for (int j = 0; j < OBJ_SIZE; j++)
			{
				if (mapMove && mapNow == i + 1)
				{
					mapArr[i][j]->Reset();
					for (int k = 0; k < BAGOBJ_SIZE; k++)
					{
						if ((mapNow != 15 && hideArr[j] == 1 && j != 0 && j != 5) || (mapNow == 15 && hideArr[j] == 0 && j == bagObj[k]))
						{
							if (!mapArr[i][j]->IsFinalBitmap())
								mapArr[i][j]->OnMove();	//若不是最後一個圖形，就OnMove到最後一個圖形後停止。
							break;
						}
					}
				}
				else if (mapMove)
				{
					if (!mapArr[i][j]->IsFinalBitmap())
						mapArr[i][j]->OnMove();	//若不是最後一個圖形，就OnMove到最後一個圖形後停止。
				}
			}
		}
	}

	void CGameStateRun::OnInit()	// 遊戲的初值及圖形設定
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(33);	    // 接個前一個狀態的進度，此處進度視為33%
		//
		// 開始載入資料
		//
		Background.LoadBitmap("Bitmaps\\map\\map0.bmp");
		char test[100];
		string filePath;
		const string rootMap = "Bitmaps\\map\\map";
		const string rootObj = "Bitmaps\\obj\\obj";
		for (int i = 0; i < MAP_SIZE; i++)
		{
			filePath = rootMap + to_string(i + 1) + ".bmp";
			strcpy(test, filePath.c_str());
			mapArr[i][0]->AddBitmap(test, RGB(255, 255, 255));
			mapArr[i][0]->AddBitmap("Bitmaps\\hide.bmp", RGB(255, 255, 255));
			for (int j = 1; j < OBJ_SIZE; j++)
			{
				filePath = rootObj + to_string(j) + ".bmp";
				strcpy(test, filePath.c_str());
				if (mapObj[i][j] == 1)
					mapArr[i][j]->AddBitmap(test, RGB(255, 255, 255));
				mapArr[i][j]->AddBitmap("Bitmaps\\hide.bmp", RGB(255, 255, 255));
			}
		}
		//
		// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT  = 0x25; // keyboard左箭頭
		const char KEY_UP    = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN  = 0x28; // keyboard下箭頭
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT  = 0x25; // keyboard左箭頭
		const char KEY_UP    = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN  = 0x28; // keyboard下箭頭
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		for (int i = 0; i < MAP_SIZE; i++)
		{
			for (int j = 1; j < OBJ_SIZE; j++)
			{
				if (mapNow == 12 && j >= 61 && j <= 62 && mapObj[i][j] == 1 && hideArr[j] == 0 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
				{
					if (objNow != 0)
						mapArr[i][57]->Reset();
					else
					{
						mapArr[i][j + 2]->Reset();
						mapNow *= 100;
						return;
					}
				}
				if (mapNow == 14 && j == 61 && mapObj[i][j] == 1 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
				{
					mapNow = 12;
					hideArr[61] = 0;
					hideArr[62] = 1;
					mapMove = true;
					return;
				}
				if (mapNow == 14 && j == 62 && mapObj[i][j] == 1 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
				{
					if (hideArr[20] != 1)
						mapArr[i][56]->Reset();
					else
					{
						mapNow = 12;
						hideArr[61] = 1;
						hideArr[62] = 0;
						mapMove = true;
						return;
					}
				}
				if (mapNow == i + 1 && j == 17 && mapObj[i][j] == 1 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
				{
					if (objNow != 10)
						mapArr[i][55]->Reset();
					else
					{
						if (!mapArr[i][56]->IsFinalBitmap())
							mapArr[i][56]->OnMove();
						if (!mapArr[i][61]->IsFinalBitmap())
							mapArr[i][61]->OnMove();
						if (!mapArr[i][62]->IsFinalBitmap())
							mapArr[i][62]->OnMove();
						mapArr[i][58]->Reset();
						mapNow = 16;
						return;
					}
				}
				if (mapNow == i + 1 && j == 15 && mapObj[i][j] == 1 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
				{
					if (!mapArr[i][40]->IsFinalBitmap())
						mapArr[i][40]->OnMove();
					if (!mapArr[i][42]->IsFinalBitmap())
						mapArr[i][42]->OnMove();
					if (!mapArr[i][43]->IsFinalBitmap())
						mapArr[i][43]->OnMove();
					if (!mapArr[i][44]->IsFinalBitmap())
						mapArr[i][44]->OnMove();
					if (!mapArr[i][45]->IsFinalBitmap())
						mapArr[i][45]->OnMove();
					if (!mapArr[i][52]->IsFinalBitmap())
						mapArr[i][52]->OnMove();
					for (int k = 30; k < 40; k++)
					{
						if (!mapArr[i][k]->IsFinalBitmap())
							mapArr[i][k]->OnMove();
						number[k - 30] = false;
					}
					if (hideArr[12] == 1 && hideArr[13] == 1 && hideArr[16] == 1 && hideArr[46] == 0)
					{
						mapArr[i][52]->Reset();
						hideArr[46] = 1;
					}
					else if(hideArr[46] == 0)
						mapArr[i][44]->Reset();
				}
				if (mapNow == i + 1 && j == 19 && mapObj[i][j] == 1 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
				{
					for (int k = 30; k < 40; k++)
					{
						mapArr[i][k]->Reset();
						number[k - 30] = true;
					}
					if (!mapArr[i][40]->IsFinalBitmap())
						mapArr[i][40]->OnMove();
					if (!mapArr[i][42]->IsFinalBitmap())
						mapArr[i][42]->OnMove();
					if (!mapArr[i][43]->IsFinalBitmap())
						mapArr[i][43]->OnMove();
					if (!mapArr[i][44]->IsFinalBitmap())
						mapArr[i][44]->OnMove();
					if (!mapArr[i][45]->IsFinalBitmap())
						mapArr[i][45]->OnMove();
					if (!mapArr[i][52]->IsFinalBitmap())
						mapArr[i][52]->OnMove();
					passwordInput = 0;
					digit = 0;
				}
				if (mapNow == i + 1 && j >= 30 && j <= 39 && number[j - 30] && mapObj[i][j] == 1 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
				{
					digit++;
					number[j - 30] = false;
					passwordInput = passwordInput * 10 + j - 30;
					if (!mapArr[i][j]->IsFinalBitmap())
						mapArr[i][j]->OnMove();
					if ((mapNow == 7 && passwordInput == password7) || (mapNow == 7 && digit == 4 && passwordInput != password7) || (mapNow == 9 && passwordInput == password9) || (mapNow == 9 && digit == 5 && passwordInput != password9))
					{
						for (int k = 30; k < 40; k++)
						{
							if (!mapArr[i][k]->IsFinalBitmap())
								mapArr[i][k]->OnMove();
						}
					}
					if (mapNow == 7 && passwordInput == password7 && hideArr[10] == 0)
					{
						hideArr[10] = 1;
						mapArr[i][42]->Reset();
					}
					else if (mapNow == 9 && passwordInput == password9 && hideArr[11] == 0)
					{
						hideArr[11] = 1;
						mapArr[i][43]->Reset();
					}
					else if ((mapNow == 7 && digit == 4 && passwordInput != password7) || (mapNow == 9 && digit == 5 && passwordInput != password9))
						mapArr[i][40]->Reset();
				}
				if (mapNow == i + 1 && j >= 26 && j <= 29 && mapObj[i][j] == 1 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
				{
					mapArr[i][j + 21]->Reset();
				}
				if (mapNow == i + 1 && j == 7 && mapObj[i][j] == 1 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
				{
					if (objNow == 8 || objNow == 11 || objNow == 13)
					{
						if (!mapArr[i][j]->IsFinalBitmap())
							mapArr[i][j]->OnMove();
						mapArr[i][54]->Reset();
					}
				}
				if (mapNow == i + 1 && j >= 1 && j <= 4 && mapObj[i][j] == 1 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
				{
					if (mapNow == 6 && objNow != 3 && j == 1)
					{
						if (!mapArr[i][43]->IsFinalBitmap())
							mapArr[i][43]->OnMove();
						mapArr[i][41]->Reset();
					}
					else if (mapNow == 6 && objNow != 5 && j == 4)
					{
						if (!mapArr[1][41]->IsFinalBitmap())
							mapArr[i][41]->OnMove();
						mapArr[i][43]->Reset();
					}
					else if (mapNow == 8 && objNow != 4 && j == 3)
						mapArr[i][42]->Reset();
					else
					{
						mapNow = mapChange[i][j - 1];
						mapMove = true;
						return;
					}
				}
				else if (mapNow == i + 1 && j == 5 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
				{
					Swap(mapTemp, mapNow);
					mapMove = true;
					return;
				}
				for (int k = 0; k < BAGOBJ_SIZE; k++)
				{
					if (mapNow == i + 1 && mapNow == 15 && j == bagObj[k] && hideArr[j] == 1 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
					{
						Swap(mapTemp, mapNow);
						objNow = k;
						mapMove = true;
						return;
					}
					else if (mapNow == i + 1 && mapNow != 15 && j == bagObj[k] && hideArr[j] == 0 && point.x > mapArr[i][j]->Left() && point.x < mapArr[i][j]->Left() + mapArr[i][j]->Width() && point.y > mapArr[i][j]->Top() && point.y < mapArr[i][j]->Top() + mapArr[i][j]->Height())
					{
						hideArr[j] = 1;
						if (hideArr[20] == 0 && hideArr[21] == 1 && hideArr[22] == 1 && hideArr[23] == 1 && ((mapNow == 6) || (mapNow == 7) || (mapNow == 10)))
						{
							hideArr[20] = 1;
							mapArr[i][45]->Reset();
							if (!mapArr[i][40]->IsFinalBitmap())
								mapArr[i][40]->OnMove();
							if (!mapArr[i][42]->IsFinalBitmap())
								mapArr[i][42]->OnMove();
							if (!mapArr[i][43]->IsFinalBitmap())
								mapArr[i][43]->OnMove();
							if (!mapArr[i][44]->IsFinalBitmap())
								mapArr[i][44]->OnMove();
							if (!mapArr[i][52]->IsFinalBitmap())
								mapArr[i][52]->OnMove();
						}
						mapArr[i][j]->OnMove();	//若不是最後一個圖形，就OnMove到最後一個圖形後停止。
						return;
					}
				}
			}
		}
		if (mapNow == 4)
		{
			if (objNow != 12)
				mapArr[3][53]->Reset();
			else
			{
				mapArr[3][51]->Reset();
				if (hideArr[9] == 0)
				{
					hideArr[9] = 1;
					mapArr[3][41]->Reset();
				}
			}
		}
		else if (mapNow == 16)
		{
			if (!mapArr[13][58]->IsFinalBitmap())
				mapArr[13][58]->OnMove();
			mapArr[13][59]->Reset();
			mapNow = 17;
		}
		else if (mapNow == 17)
		{
			if (!mapArr[13][59]->IsFinalBitmap())
				mapArr[13][59]->OnMove();
			mapArr[13][60]->Reset();
			mapNow = 18;
		}
		else if (mapNow == 18)
		{
			if (!mapArr[13][60]->IsFinalBitmap())
				mapArr[13][60]->OnMove();
			mapArr[13][61]->Reset();
			mapArr[13][62]->Reset();
			mapNow = 14;
		}
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		mapMove = false;
		if (!mapArr[3][51]->IsFinalBitmap())
			mapArr[3][51]->OnMove();
		if (mapNow == 8)
		{
			if (!mapArr[7][54]->IsFinalBitmap())
				mapArr[7][54]->OnMove();
			mapArr[7][7]->Reset();
		}
		for (int i = 0; i < MAP_SIZE; i++)
		{
			for (int j = 1; j < OBJ_SIZE; j++)
			{
				if (mapNow == i + 1 && j >= 26 && j <= 29 && mapObj[i][j] == 1)
				{
					if (!mapArr[i][j + 21]->IsFinalBitmap())
						mapArr[i][j + 21]->OnMove();
				}
			}
		}
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
	
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
 
	}

	void CGameStateRun::OnShow()
	{
		//
		//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
		//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
		//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
		//
		Background.ShowBitmap();
		for (int i = MAP_SIZE - 1; i >= 0; i--)
		{
			for (int j = 0; j < OBJ_SIZE; j++)
			{
				mapArr[i][j]->SetTopLeft(mapX[i][j], mapY[i][j]);
				mapArr[i][j]->OnShow();
			}
		}
	}
}