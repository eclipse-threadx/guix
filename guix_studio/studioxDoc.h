// studioxDoc.h : interface of the CstudioxDoc class
//


#pragma once


class CstudioxDoc : public CDocument
{
protected: // create from serialization only
	CstudioxDoc();
	DECLARE_DYNCREATE(CstudioxDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CstudioxDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


