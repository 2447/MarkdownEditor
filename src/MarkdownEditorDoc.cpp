
// MarkdownEditorDoc.cpp : CMarkdownEditorDoc ���ʵ��
//

#include "stdafx.h"
#include "./Util.h"
#include <memory>
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MarkdownEditor.h"
#endif

#include "MarkdownEditorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMarkdownEditorDoc

IMPLEMENT_DYNCREATE(CMarkdownEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMarkdownEditorDoc, CDocument)

END_MESSAGE_MAP()


// CMarkdownEditorDoc ����/����

CMarkdownEditorDoc::CMarkdownEditorDoc()
{
	// TODO: �ڴ����һ���Թ������
	resetData();
}

CMarkdownEditorDoc::~CMarkdownEditorDoc()
{
}

BOOL CMarkdownEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	resetData();
	this->UpdateAllViews(NULL, LPARAM_Update);
	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	return TRUE;
}




// CMarkdownEditorDoc ���л�

void CMarkdownEditorDoc::Serialize(CArchive& ar)
{
	string strFile = ar.GetFile()->GetFilePath();
	_strPath = Util::GetFilePath(strFile,true);

	if (ar.IsStoring())
	{
		ar.WriteString(Util::ANSIToUTF8(_strText.c_str()).c_str());
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		
		_strText = Util::ReadStringFile(*ar.GetFile()).c_str();
		Util::ReplaceAllStr(_strText,"\r\n", "\n");
		Util::ReplaceAllStr(_strText,"\n", "\r\n");
		this->UpdateAllViews(NULL,LPARAM_Update);
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CMarkdownEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CMarkdownEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CMarkdownEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMarkdownEditorDoc ���

#ifdef _DEBUG
void CMarkdownEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMarkdownEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
const CString PREFIX_MODIFIED = "* ";
void setModified(CMarkdownEditorDoc*pDoc, bool modified) {
	pDoc->SetModifiedFlag(modified);
	//string path = pDoc->GetTitle();
	CString title = pDoc->GetTitle();
	if (modified) {
		CString strTitle;
		AfxGetMainWnd()->GetWindowText(strTitle);
		if (modified && strTitle.Find(PREFIX_MODIFIED) !=0)
			AfxGetMainWnd()->SetWindowText("* " + strTitle);
	}
}
// CMarkdownEditorDoc ����
//�����ı�����
void CMarkdownEditorDoc::UpdateText(const string& text, CView* pSender, bool bMoveToEnd){
	_strText = text;
	int lParam = LPARAM_Update;
	if(bMoveToEnd)
		lParam |= LPARAM_MoveEnd;
	this->UpdateAllViews(pSender, lParam);
	setModified(this, true);
	//this->SetModifiedFlag();
}

void CMarkdownEditorDoc::resetData(void)
{
	_strText = "";
	_strPath = "";
}
