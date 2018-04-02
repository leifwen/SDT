// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#pragma once
//------------------------------------------------------------------------------------------//
class CMyToolBar : public CMFCToolBar{
	public:
				 CMyToolBar(void) : CMFCToolBar(){ ; };
		virtual ~CMyToolBar(void){ ; };
	public:
		virtual void OnUpdateCmdUI		(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler);
		virtual BOOL AllowShowOnList	(void) const;
};
//------------------------------------------------------------------------------------------//

